#pragma once

#include <vector>
#include <cstddef>

#include "opcodefields.h"
#include "opcodeinfo.h"
#include "opcodetypes.h"
#include "types.h"
#include "utils.h"

namespace dsp56k
{
	enum class RegisterMask : uint64_t;

	const FieldInfo& getFieldInfo(Instruction _i, Field _f);

	template<Instruction I, Field F>
	constexpr FieldInfo static getFieldInfoCE()
	{
		return initField(g_opcodes[I].m_opcode, g_fieldParseConfigs[F].ch, g_fieldParseConfigs[F].count);
	}

	inline bool hasField(const Instruction _inst, const Field _field)
	{
		return getFieldInfo(_inst, _field).len > 0;
	}

	template<Instruction I, Field F> constexpr bool hasField()
	{
		return getFieldInfoCE<I,F>().len > 0;
	}

	template<Instruction I, Field A, Field B> constexpr bool hasFields()
	{
		return hasField<I,A>() && hasField<I,B>();
	}

	template<Instruction I, Field A, Field B> constexpr bool hasAnyField()
	{
		return hasField<I,A>() || hasField<I,B>();
	}

	template<Instruction I, Field A, Field B, Field C> constexpr bool hasFields()
	{
		return hasField<I,A>() && hasField<I,B>() && hasField<I,C>();
	}

	template<Instruction I, Field A, Field B, Field C> constexpr bool hasAnyField()
	{
		return hasField<I,A>() || hasField<I,B>() || hasField<I,C>();
	}

	static constexpr TWord getFieldValue(const FieldInfo& _fi, TWord _memoryValue)
	{
		assert(_fi.len > 0 && "field not known");
		return (_memoryValue >> _fi.bit) & _fi.mask;
	}

	template<Instruction I, Field F> TWord getFieldValue(const TWord _memValue)
	{
		constexpr auto fi = getFieldInfoCE<I,F>();
		static_assert(fi.len > 0, "field not known");
		return getFieldValue(fi, _memValue);
	}

	template<Instruction I, Field MSB, Field LSB> TWord getFieldValue(const TWord _memValue)
	{
		constexpr auto fa = getFieldInfoCE<I,MSB>();
		static_assert(fa.len > 0, "field MSB not known");
		constexpr auto fb = getFieldInfoCE<I,LSB>();
		static_assert(fb.len > 0, "field LSB not known");
		return getFieldValue<I,MSB>(_memValue) << fb.len | getFieldValue<I,LSB>(_memValue);
	}

	static TWord getFieldValue(Instruction _instruction, const Field _field, TWord _memoryValue)
	{
		const auto& fi = getFieldInfo(_instruction, _field);
		return getFieldValue(fi, _memoryValue);
	}

	static TWord getFieldValue(Instruction _instruction, const Field _msb, const Field _lsb, TWord _memoryValue)
	{
		const auto& fa = getFieldInfo(_instruction, _msb);
		const auto& fb = getFieldInfo(_instruction, _lsb);

		return getFieldValue(fa, _memoryValue) << fb.len | getFieldValue(fb, _memoryValue);
	}

	static bool matchFieldRange(Instruction _instruction, Field _field, TWord _mem, TWord min, TWord max)
	{
		const auto& fi = getFieldInfo(_instruction, _field);
		if(!fi.len)
			return true;

		const auto v = getFieldValue(fi, _mem);
		return v >= min && v <= max;
	}

	static bool matchdddddd(const Instruction inst, const TWord v)
	{
		switch (inst)
		{
		case Lua_ea:
		case Lua_Rn:
		case Lra_xxxx:
		case Lra_Rn:
			break;
		default:
			if(v < 4)		// undocumented: There are no valid register if dddddd < 4, but the Motorola disasm says these are x0,x1,y0,y1, just as the values 4-7
				return false;
		}
		if(v >= 0b101000 && v <= 0b101111 && v != 0b101010)	// For 101EEE, only 010 is valid for EEE (EP register)
			return false;
		if(v >= 0b110010 && v <= 0b110111)					// For 110VVV, only VBA (000) and SC (001) exist
			return false;
		return true;			
	}

	static bool matchdddddd(Instruction _instruction, Field _field, TWord _word)
	{
		return matchdddddd(_instruction, getFieldValue(_instruction, _field, _word));
	}

	static bool hasField(const OpcodeInfo& _oi, const Field _field)
	{
		return getFieldInfo(_oi.m_instruction, _field).len > 0;
	}

	static bool isNonParallelOpcode(const OpcodeInfo& _oi)
	{
		return !hasField(_oi, Field_MoveOperation) && !hasField(_oi, Field_AluOperation);
	}

	static bool match(const OpcodeInfo& _oi, const TWord _word)
	{
		const auto m = _oi.m_mask1 | _oi.m_mask0;
		if((_word & m) != _oi.m_mask1)
			return false;

		if(!matchFieldRange(_oi.m_instruction, Field_bbbbb, _word, 0, 23))
			return false;

		if(hasField(_oi, Field_JJJ))
		{
			const auto v = getFieldValue(_oi.m_instruction, Field_JJJ, _word);

			if(!OpcodeInfo::isParallelOpcode(_word))
			{
				// There are two variants for JJJ, one does not allow 001, 010 and 011 (56 bit regs), only 24 bit are allowed. For Non-parallel instructions, the only instruction using it is Tcc and there it does not allow all values
				if(v > 0 && v < 4)
					return false;
			}
			else
			{
				switch (_oi.m_instruction)
				{
				case Tfr:
				case Cmp_S1S2:
				case Cmpm_S1S2:
					// There are two variants for JJJ, one does not allow 001, 010 and 011 (56 bit regs), only 24 bit are allowed.
					if(v > 0 && v < 4)
						return false;
					break;
				default:;
				}
			}
		}

		if(hasField(_oi, Field_DDDD))
		{
			const auto v = getFieldValue(_oi.m_instruction, Field_DDDD, _word);
			if(v < 4)
				return false;
		}

		if(hasField(_oi, Field_dddddd))
		{
			if(!matchdddddd(_oi.m_instruction, Field_dddddd, _word))
				return false;
		}

		if(hasField(_oi, Field_ddddd))
		{
			if(!matchdddddd(_oi.m_instruction, Field_ddddd, _word))
				return false;
		}
		
		if(hasField(_oi, Field_dd) && hasField(_oi, Field_ddd))
		{
			if(!matchdddddd(_oi.m_instruction, getFieldValue(_oi.m_instruction, Field_dd, Field_ddd, _word)))
				return false;
		}
		
		if(hasField(_oi, Field_eeeee))
		{
			if(!matchdddddd(_oi.m_instruction, Field_eeeee, _word))
				return false;
		}
		
		if(hasField(_oi, Field_eeeeee))
		{
			if(!matchdddddd(_oi.m_instruction, Field_eeeeee, _word))
				return false;
		}
		
		if(hasField(_oi, Field_DDDDD))
		{
			const auto v = getFieldValue(_oi.m_instruction, Field_DDDDD, _word);
			switch(v)
			{
			case 0x0:		// M0
			case 0x1:		// M1
			case 0x2:		// M2
			case 0x3:		// M3
			case 0x4:		// M4
			case 0x5:		// M5
			case 0x6:		// M6
			case 0x7:		// M7
			case 0b01010:	// EP
			case 0b10000:	// VBA
			case 0b10001:	// SC
			case 0b11000:	// SZ
			case 0b11001:	// SR
			case 0b11010:	// OMR
			case 0b11011:	// SP
			case 0b11100:	// SSH
			case 0b11101:	// SSL
			case 0b11110:	// LA
			case 0b11111:	// LC
				break;
			default:
				return false;
			}
		}

		if(!hasField(_oi, Field_MMM))
			return true;

		const auto mmm = getFieldValue(_oi.m_instruction, Field_MMM, _word);
		if(mmm != 0x6)
			return true;

		const auto rrr = getFieldValue(_oi.m_instruction, Field_RRR, _word);
		if(rrr == 0 && (_oi.m_extensionWordType & EffectiveAddress) != 0)
			return true;
		if(rrr == 4 && (_oi.m_extensionWordType & ImmediateData) != 0)
		{
			// If peripheral is target, the source can be an immediate extension word, but if peripheral is source, we cannot write to immediate data
			if(!hasField(_oi, Field_W) || getFieldValue(_oi.m_instruction, Field_W, _word) == 1)
				return true;				
		}
		return false;
	}

	class Opcodes
	{
	public:
		Opcodes();

		const OpcodeInfo* findNonParallelOpcodeInfo(TWord _opcode) const;
		const OpcodeInfo* findParallelMoveOpcodeInfo(TWord _opcode) const;
		const OpcodeInfo* findParallelAluOpcodeInfo(TWord _opcode) const;

		static bool isParallelOpcode(const TWord _opcode)
		{
			return OpcodeInfo::isParallelOpcode(_opcode);
		}

		static bool isNonParallelOpcode(const TWord _opcode)
		{
			return !isParallelOpcode(_opcode);
		}

		static const OpcodeInfo& getOpcodeInfoAt(size_t _index);

		uint32_t getOpcodeLength(TWord _op) const;
		static uint32_t getOpcodeLength(TWord _op, Instruction _instA, Instruction _instB);
		bool writesToPMemory(TWord _op) const;
		uint32_t getInstructionTypes(TWord _op, Instruction& _a, Instruction& _b) const;
		bool getRegisters(RegisterMask& _written, RegisterMask& _read, TWord _opA) const;
		static bool getRegisters(RegisterMask& _written, RegisterMask& _read, TWord _opA, Instruction _instA, Instruction _instB);
		static uint32_t getFlags(Instruction _instA, Instruction _instB);
		bool getMemoryAddress(TWord& _addr, EMemArea& _area, TWord opA, TWord opB) const;
	private:
		static const OpcodeInfo* findOpcodeInfo(TWord _opcode, const std::vector<const OpcodeInfo*>& _opcodes);
		
		std::vector<const OpcodeInfo*> m_opcodesNonParallel;
		std::vector<const OpcodeInfo*> m_opcodesMove;
		std::vector<const OpcodeInfo*> m_opcodesAlu;
	};

	// _____________________________________________
	// higher level field extraction

	template <Instruction Inst> TWord getBit(TWord op)
	{
		return getFieldValue<Inst, Field_bbbbb>(op);
	}

	template<Instruction I> EMemArea getFieldValueMemArea(const TWord _op)
	{
		return static_cast<EMemArea>(getFieldValue<I,Field_S>(_op) + MemArea_X);
	}
	
	template <Instruction Inst, std::enable_if_t<hasField<Inst, Field_aaaaaaaaaaaa>()>* = nullptr> TWord getEffectiveAddress(const TWord op)
	{
		return getFieldValue<Inst, Field_aaaaaaaaaaaa>(op);
	}

	template <Instruction Inst, std::enable_if_t<!hasAnyField<Inst, Field_a, Field_RRR>() && hasField<Inst, Field_aaaaaa>()>* = nullptr> TWord getEffectiveAddress(const TWord op)
	{
		return getFieldValue<Inst, Field_aaaaaa>(op);
	}

	template <Instruction Inst, std::enable_if_t<hasFields<Inst, Field_aaaa, Field_aaaaa>()>* = nullptr> int getRelativeAddressOffset(const TWord op)
	{
		const TWord a = getFieldValue<Inst,Field_aaaa, Field_aaaaa>(op);
		return signextend<int,9>( a );
	}
}
