#pragma once

#include "jitblock.h"
#include "jitdspregs.h"
#include "jitemitter.h"
#include "jitregtracker.h"
#include "jittypes.h"

#include "opcodes.h"
#include "opcodetypes.h"
#include "registers.h"
#include "types.h"
#include "utils.h"
#include "opcodeanalysis.h"

namespace dsp56k
{
	class Jit;
	class Opcodes;

	class JitOps
	{
	public:
		enum ResultFlags
		{
			None				= 0,

			WritePMem			= 0x01,
			WriteToLC			= 0x02,
			WriteToLA			= 0x04
		};

		JitOps(JitBlock& _block, bool _fastInterrupt = false);

		void emit(TWord _pc);
		void emit(TWord _pc, TWord _op, TWord _opB = 0);
		void emit(Instruction _inst, TWord _op);
		void emit(Instruction _instMove, Instruction _instAlu, TWord _op);

		void emitOpProlog();
		void emitOpEpilog();

		void op_Abs(TWord op);
		void op_ADC(TWord op)			{ errNotImplemented(op); }
		void op_Add_SD(TWord op);
		void op_Add_xx(TWord op);
		void op_Add_xxxx(TWord op);
		void op_Addl(TWord op);
		void op_Addr(TWord op);
		void op_And_SD(TWord op);
		void op_And_xx(TWord op);
		void op_And_xxxx(TWord op);
		void op_Andi(TWord op);
		void op_Asl_D(TWord op);
		void op_Asl_ii(TWord op);
		void op_Asl_S1S2D(TWord op);
		void op_Asr_D(TWord op);
		void op_Asr_ii(TWord op);
		void op_Asr_S1S2D(TWord op);
		void op_Bcc_xxxx(TWord op);
		void op_Bcc_xxx(TWord op);
		void op_Bcc_Rn(TWord op);
		void op_Bchg_ea(TWord op);
		void op_Bchg_aa(TWord op);
		void op_Bchg_pp(TWord op);
		void op_Bchg_qq(TWord op);
		void op_Bchg_D(TWord op);
		void op_Bclr_ea(TWord op);
		void op_Bclr_aa(TWord op);
		void op_Bclr_pp(TWord op);
		void op_Bclr_qq(TWord op);
		void op_Bclr_D(TWord op);
		void op_Bra_xxxx(TWord op);
		void op_Bra_xxx(TWord op);
		void op_Bra_Rn(TWord op);
		void op_Brclr_ea(TWord op);
		void op_Brclr_aa(TWord op);
		void op_Brclr_pp(TWord op);
		void op_Brclr_qq(TWord op);
		void op_Brclr_S(TWord op);
		void op_BRKcc(TWord op)			{ errNotImplemented(op); }
		void op_Brset_ea(TWord op);
		void op_Brset_aa(TWord op);
		void op_Brset_pp(TWord op);
		void op_Brset_qq(TWord op);
		void op_Brset_S(TWord op);
		void op_BScc_xxxx(TWord op);
		void op_BScc_xxx(TWord op);
		void op_BScc_Rn(TWord op);
		void op_Bsclr_ea(TWord op);
		void op_Bsclr_aa(TWord op);
		void op_Bsclr_pp(TWord op);
		void op_Bsclr_qq(TWord op);
		void op_Bsclr_S(TWord op);
		void op_Bset_ea(TWord op);
		void op_Bset_aa(TWord op);
		void op_Bset_pp(TWord op);
		void op_Bset_qq(TWord op);
		void op_Bset_D(TWord op);
		void op_Bsr_xxxx(TWord op);
		void op_Bsr_xxx(TWord op);
		void op_Bsr_Rn(TWord op);
		void op_Bsset_ea(TWord op);
		void op_Bsset_aa(TWord op);
		void op_Bsset_pp(TWord op);
		void op_Bsset_qq(TWord op);
		void op_Bsset_S(TWord op);
		void op_Btst_ea(TWord op);
		void op_Btst_aa(TWord op);
		void op_Btst_pp(TWord op);
		void op_Btst_qq(TWord op);
		void op_Btst_D(TWord op);
		void op_Clb(TWord op)				{ errNotImplemented(op); }
		void op_Clr(TWord op);
		void op_Cmp_S1S2(TWord op);
		void op_Cmp_xxS2(TWord op);
		void op_Cmp_xxxxS2(TWord op);
		void op_Cmpm_S1S2(TWord op);
		void op_Cmpu_S1S2(TWord op)			{ errNotImplemented(op); }
		void op_Debug(TWord op);
		void op_Debugcc(TWord op);
		void op_Dec(TWord op);
		void op_Div(TWord op);
		void op_Rep_Div(TWord _op, TWord _iterationCount);
		void op_Dmac(TWord op);
		void op_Do_ea(TWord op);
		void op_Do_aa(TWord op);
		void op_Do_xxx(TWord op);
		void op_Do_S(TWord op);
		void op_DoForever(TWord op)			{ errNotImplemented(op); }
		void op_Dor_ea(TWord op);
		void op_Dor_aa(TWord op)			{ errNotImplemented(op); }
		void op_Dor_xxx(TWord op);
		void op_Dor_S(TWord op);
		void op_DorForever(TWord op)		{ errNotImplemented(op); }
		void op_Enddo(TWord op);
		void op_Eor_SD(TWord op)			{ errNotImplemented(op); }
		void op_Eor_xx(TWord op)			{ errNotImplemented(op); }
		void op_Eor_xxxx(TWord op)			{ errNotImplemented(op); }
		void op_Extract_S1S2(TWord op)		{ errNotImplemented(op); }
		void op_Extract_CoS2(TWord op)		{ errNotImplemented(op); }
		void op_Extractu_S1S2(TWord op);
		void op_Extractu_CoS2(TWord op);
		template<bool BackupCCR> void op_Ifcc(TWord op);
		void op_Illegal(TWord op)			{ errNotImplemented(op); }
		void op_Inc(TWord op);
		void op_Insert_S1S2(TWord op)		{ errNotImplemented(op); }
		void op_Insert_CoS2(TWord op)		{ errNotImplemented(op); }
		void op_Jcc_xxx(TWord op);
		void op_Jcc_ea(TWord op);
		void op_Jclr_ea(TWord op);
		void op_Jclr_aa(TWord op);
		void op_Jclr_pp(TWord op);
		void op_Jclr_qq(TWord op);
		void op_Jclr_S(TWord op);
		void op_Jmp_ea(TWord op);
		void op_Jmp_xxx(TWord op);
		void op_Jscc_xxx(TWord op);
		void op_Jscc_ea(TWord op);
		void op_Jsclr_ea(TWord op);
		void op_Jsclr_aa(TWord op);
		void op_Jsclr_pp(TWord op);
		void op_Jsclr_qq(TWord op);
		void op_Jsclr_S(TWord op);
		void op_Jset_ea(TWord op);
		void op_Jset_aa(TWord op);
		void op_Jset_pp(TWord op);
		void op_Jset_qq(TWord op);
		void op_Jset_S(TWord op);
		void op_Jsr_ea(TWord op);
		void op_Jsr_xxx(TWord op);
		void op_Jsset_ea(TWord op);
		void op_Jsset_aa(TWord op);
		void op_Jsset_pp(TWord op);
		void op_Jsset_qq(TWord op);
		void op_Jsset_S(TWord op);
		void op_Lra_Rn(TWord op)		{ errNotImplemented(op); }
		void op_Lra_xxxx(TWord op);
		void op_Lsl_D(TWord op);
		void op_Lsl_ii(TWord op);
		void op_Lsl_SD(TWord op)		{ errNotImplemented(op); }
		void op_Lsr_D(TWord op);
		void op_Lsr_ii(TWord op);
		void op_Lsr_SD(TWord op)		{ errNotImplemented(op); }
		void op_Lua_ea(TWord _op);
		void op_Lua_Rn(TWord op);
		void op_Mac_S1S2(TWord op)		{ alu_multiply(op); }
		template<Instruction Inst, bool Accumulate, bool Round> void op_Mac_S(TWord op);
		void op_Maci_xxxx(TWord op)		{ errNotImplemented(op); }
		void op_Macr_S1S2(TWord op)		{ alu_multiply(op); }
		void op_Macri_xxxx(TWord op)	{ errNotImplemented(op); }
		void op_Max(TWord op)			{ errNotImplemented(op); }
		void op_Maxm(TWord op)			{ errNotImplemented(op); }
		void op_Merge(TWord op)			{ errNotImplemented(op); }
		void op_Move_Nop(TWord op);
		void op_Move_xx(TWord op);
		void op_Mover(TWord op);
		void op_Move_ea(TWord op);
		void op_Movex_ea(TWord op);
		void op_Movex_aa(TWord op);
		void op_Movex_Rnxxxx(TWord op);
		void op_Movex_Rnxxx(TWord op);
		void op_Movexr_ea(TWord op);
		void op_Movexr_A(TWord op);
		void op_Movey_ea(TWord op);
		void op_Movey_aa(TWord op);
		void op_Movey_Rnxxxx(TWord op);
		void op_Movey_Rnxxx(TWord op);
		void op_Moveyr_ea(TWord op);
		void op_Moveyr_A(TWord op);
		void op_Movel_ea(TWord op);
		void op_Movel_aa(TWord op);
		void op_Movexy(TWord op);
		void op_Movec_ea(TWord op);
		void op_Movec_aa(TWord op);
		void op_Movec_S1D2(TWord op);
		void op_Movec_xx(TWord op);
		void op_Movem_ea(TWord op);
		void op_Movem_aa(TWord op)				{ errNotImplemented(op); }
		void op_Movep_ppea(TWord op);
		void op_Movep_Xqqea(TWord op);
		void op_Movep_Yqqea(TWord op);
		void op_Movep_eapp(TWord op)			{ errNotImplemented(op); }
		void op_Movep_eaqq(TWord op)			{ errNotImplemented(op); }
		void op_Movep_Spp(TWord op);
		void op_Movep_SXqq(TWord op);
		void op_Movep_SYqq(TWord op);
		void op_Mpy_S1S2D(TWord op)				{ alu_multiply(op); }
		template<Instruction Inst, bool Accumulate> void op_Mpy_su(TWord op);
		void op_Mpyi(TWord op);
		void op_Mpyr_S1S2D(TWord op)			{ alu_multiply(op); }
		void op_Mpyri(TWord op)					{ errNotImplemented(op); }
		void op_Neg(TWord op);
		void op_Nop(TWord op);
		void op_Norm(TWord op)					{ errNotImplemented(op); }
		void op_Normf(TWord op)					{ errNotImplemented(op); }
		void op_Not(TWord op);
		void op_Or_SD(TWord op);
		void op_Or_xx(TWord op)					{ errNotImplemented(op); }
		void op_Or_xxxx(TWord op)				{ errNotImplemented(op); }
		void op_Ori(TWord op);
		void op_Pflush(TWord op)				{ errNotImplemented(op); }
		void op_Pflushun(TWord op);
		void op_Pfree(TWord op);
		void op_Plock(TWord op);
		void op_Plockr(TWord op)				{ errNotImplemented(op); }
		void op_Punlock(TWord op)				{ errNotImplemented(op); }
		void op_Punlockr(TWord op)				{ errNotImplemented(op); }
		void op_Rep_ea(TWord op)				{ errNotImplemented(op); }
		void op_Rep_aa(TWord op)				{ errNotImplemented(op); }
		void op_Rep_xxx(TWord op);
		void op_Rep_S(TWord op);
		void op_Reset(TWord op);
		void op_Rnd(TWord op);
		void op_Rol(TWord op);
		void op_Ror(TWord op)					{ errNotImplemented(op); }
		void op_Rti(TWord op);
		void op_Rts(TWord op);
		void op_Sbc(TWord op)					{ errNotImplemented(op); }
		void op_Stop(TWord op);
		void op_Sub_SD(TWord op);
		void op_Sub_xx(TWord op);
		void op_Sub_xxxx(TWord op);
		void op_Subl(TWord op);
		void op_Subr(TWord op);
		void op_Tcc_S1D1(TWord op);
		void op_Tcc_S1D1S2D2(TWord op);
		void op_Tcc_S2D2(TWord op);
		void op_Tfr(TWord op);
		void op_Trap(TWord op)					{ errNotImplemented(op); }
		void op_Trapcc(TWord op)				{ errNotImplemented(op); }
		void op_Tst(TWord op);
		void op_Vsl(TWord op)					{ errNotImplemented(op); }
		void op_Wait(TWord op);

		// helpers
		void signextend56to64(const JitReg64& _reg) const;
		void signextend48to64(const JitReg64& _reg) const;
		void signextend48to56(const JitReg64& _reg) const;
		void signextend24to56(const JitReg64& _reg) const;
		void signextend24to64(const JitReg64& _reg) const;
		void signextend24To32(const JitReg32& _reg) const;

		void updateAddressRegister(const JitReg64& _r, TWord _mmm, TWord _rrr, bool _writeR = true, bool _returnPostR = false);
		void updateAddressRegister(const JitReg32& _r, const JitReg32& _n, const JitReg32& _m, uint32_t _rrr);
		void updateAddressRegisterConst(const JitReg32& _r, const int _n, const JitReg32& _m, uint32_t _rrr);
		void updateAddressRegisterModulo(const JitReg32& _r, const JitReg32& _n, const JitReg32& _m, const JitReg32& _mMask) const;
		void updateAddressRegisterMultipleWrapModulo(const JitReg32& _r, const JitReg32& _n, const JitReg32& _m);
		static void updateAddressRegisterBitreverse(const JitReg32& _r, const JitReg32& _n, const JitReg32& _m);

		void signed24To56(const JitReg64& _r) const;
		constexpr static uint64_t signed24To56(const TWord _src)
		{
			return static_cast<uint64_t>((static_cast<int64_t>(_src) << 40ull) >> 8ull) >> 8ull;
		}

		void callDSPFunc(void(* _func)(DSP*, TWord)) const;
		void callDSPFunc(void(* _func)(DSP*, TWord), TWord _arg) const;
		void callDSPFunc(void(* _func)(DSP*, TWord), const JitRegGP& _arg) const;

		void setDspProcessingMode(uint32_t _mode);
		
		// Check Condition
		void checkCondition(const TWord _cc, const std::function<void()>& _true, const std::function<void()>& _false, bool _hasFalseFunc, bool _updateDirtyCCR)
		{
			If(m_block, [&](const asmjit::Label& _toFalse)
			{
				const RegGP r(m_block);
#ifdef HAVE_ARM64
				m_asm.mov(r, asmjit::a64::xzr);
				decode_cccc(r, _cc);
				m_asm.test(r.get());
#else
				decode_cccc(r, _cc);
				m_asm.test(r.get().r8());
#endif
				m_block.dspRegPool().releaseAll();
				m_asm.jz(_toFalse);
			}, _true, _false, _hasFalseFunc, _updateDirtyCCR);
		}
		template <Instruction Inst> void checkCondition(const TWord _op, const std::function<void()>& _true, const std::function<void()>& _false, bool _hasFalseFunc, bool _updateDirtyCCR)
		{
			const TWord cccc = getFieldValue<Inst,Field_CCCC>(_op);

			checkCondition(cccc, _true, _false, _hasFalseFunc, _updateDirtyCCR);
		}

		template <Instruction Inst> void checkCondition(const TWord _op, const std::function<void()>& _true, bool _updateDirtyCCR = true)
		{
			checkCondition<Inst>(_op, _true, [] {}, false, _updateDirtyCCR);
		}

		// extension word access
		template<Instruction Inst> int pcRelativeAddressExt()
		{
			static_assert(g_opcodes[Inst].m_extensionWordType & PCRelativeAddressExt, "opcode does not have a PC-relative address extension word");
			return signextend<int,24>(getOpWordB());
		}

		template<Instruction Inst> TWord absAddressExt()
		{
			static_assert(g_opcodes[Inst].m_extensionWordType & AbsoluteAddressExt, "opcode does not have an absolute address extension word");
			return getOpWordB();
		}

		// stack
		void	pushPCSR();
		void	popPCSR();
		void	popPC();

		// DSP memory access
		enum EffectiveAddressType
		{
			Immediate,
			Peripherals,
			Memory,
			Dynamic
		};

		template <Instruction Inst, typename std::enable_if<hasFields<Inst, Field_MMM, Field_RRR>()>::type* = nullptr> EffectiveAddressType effectiveAddress(const JitReg64& _dst, TWord _op);
		template <Instruction Inst, typename std::enable_if<hasFields<Inst, Field_MMM, Field_RRR>()>::type* = nullptr> EffectiveAddressType effectiveAddressType(TWord _op) const;

		template <Instruction Inst, typename std::enable_if<!hasField<Inst,Field_s>() && hasFields<Inst, Field_MMM, Field_RRR, Field_S>()>::type* = nullptr> void readMem(const JitReg64& _dst, TWord _op);
		template <Instruction Inst, typename std::enable_if<hasFields<Inst, Field_MMM, Field_RRR>()>::type* = nullptr> EffectiveAddressType readMem(const JitReg64& _dst, TWord _op, EMemArea _area);
		template <Instruction Inst, typename std::enable_if<!hasAnyField<Inst, Field_MMM, Field_RRR>() && hasFields<Inst, Field_qqqqqq, Field_S>()>::type* = nullptr> void readMem(const JitReg64& _dst, TWord op) const;
		template <Instruction Inst, typename std::enable_if<!hasAnyField<Inst, Field_MMM, Field_RRR>() && hasFields<Inst, Field_pppppp, Field_S>()>::type* = nullptr> void readMem(const JitReg64& _dst, TWord op) const;
		template <Instruction Inst, typename std::enable_if<!hasField<Inst, Field_s>() && hasFields<Inst, Field_aaaaaa, Field_S>()>::type* = nullptr> void readMem(const JitReg64& _dst, TWord op) const;
		template <Instruction Inst, typename std::enable_if<!hasAnyField<Inst, Field_S, Field_s>() && hasField<Inst, Field_aaaaaa>()>::type* = nullptr> void readMem(const JitReg64& _dst, TWord op, EMemArea _area) const;

		template <Instruction Inst, typename std::enable_if<!hasFields<Inst,Field_s, Field_S>() && hasFields<Inst, Field_MMM, Field_RRR>()>::type* = nullptr> EffectiveAddressType writeMem(TWord _op, EMemArea _area, const JitReg64& _src);
		template <Instruction Inst, typename std::enable_if<!hasField<Inst, Field_s>() && hasFields<Inst, Field_MMM, Field_RRR, Field_S>()>::type* = nullptr> void writeMem(TWord _op, const JitReg64& _src);
		template <Instruction Inst, typename std::enable_if<!hasAnyField<Inst, Field_MMM, Field_RRR>() && hasFields<Inst, Field_qqqqqq, Field_S>()>::type* = nullptr> void writeMem(TWord op, const JitReg64& _src);
		template <Instruction Inst, typename std::enable_if<!hasAnyField<Inst, Field_MMM, Field_RRR>() && hasFields<Inst, Field_pppppp, Field_S>()>::type* = nullptr> void writeMem(TWord op, const JitReg64& _src);
		template <Instruction Inst, typename std::enable_if<!hasAnyField<Inst, Field_S, Field_s>() && hasField<Inst, Field_aaaaaa>()>::type* = nullptr> void writeMem(TWord op, EMemArea _area, const JitReg64& _src) const;

		void readMemOrPeriph(const JitReg64& _dst, EMemArea _area, const JitReg64& _offset, Instruction _inst);
		void writeMemOrPeriph(EMemArea _area, const JitReg64& _offset, const JitReg64& _value);

		template <Instruction Inst, typename std::enable_if<hasFields<Inst, Field_bbbbb, Field_S>()>::type* = nullptr> void bitTestMemory(TWord _op, ExpectedBitValue _bitValue, asmjit::Label _skip);
		template <Instruction Inst, typename std::enable_if<hasField<Inst, Field_bbbbb>()>::type* = nullptr> void bitTest(TWord op, const JitRegGP& _value, ExpectedBitValue _bitValue, asmjit::Label _skip) const;

		// DSP register access
		void getMR(const JitReg64& _dst) const;
		void getCCR(RegGP& _dst);
		void getCOM(const JitReg64& _dst) const;
		void getEOM(const JitReg64& _dst) const;

		void setMR(const JitReg64& _src) const;
		void setCCR(const JitReg64& _src);
		void setCOM(const JitReg64& _src) const;
		void setEOM(const JitReg64& _src) const;

		void getSR(const JitReg32& _dst);
		JitRegGP getSR(JitDspRegs::AccessType _accessType);
		void setSR(const JitReg32& _src);

		void getXY0(const JitRegGP& _dst, uint32_t _aluIndex) const;
		void setXY0(uint32_t _xy, const JitRegGP& _src);
		void getXY1(const JitRegGP& _dst, uint32_t _aluIndex) const;
		void setXY1(uint32_t _xy, const JitRegGP& _src);

		void getX0(const JitRegGP& _dst) const { return getXY0(_dst, 0); }
		void getY0(const JitRegGP& _dst) const { return getXY0(_dst, 1); }
		void getX1(const JitRegGP& _dst) const { return getXY1(_dst, 0); }
		void getY1(const JitRegGP& _dst) const { return getXY1(_dst, 1); }

		void getALU0(const JitRegGP& _dst, uint32_t _aluIndex) const;
		void getALU1(const JitRegGP& _dst, uint32_t _aluIndex) const;
		void getALU2signed(const JitRegGP& _dst, uint32_t _aluIndex) const;

		void setALU0(uint32_t _aluIndex, const JitRegGP& _src);
		void setALU1(uint32_t _aluIndex, const JitReg32& _src);
		void setALU2(uint32_t _aluIndex, const JitReg32& _src);

		void getSSH(const JitReg32& _dst) const;
		void setSSH(const JitReg32& _src) const;
		void getSSL(const JitReg32& _dst) const;
		void setSSL(const JitReg32& _src) const;
		void setSSHSSL(const JitReg32& _ssh, const JitReg32& _ssl);

		void decSP() const;
		void incSP() const;

		void transferAluTo24(const JitRegGP& _dst, int _alu);
		void transfer24ToAlu(int _alu, const JitRegGP& _src);
		void transferSaturation(const JitRegGP& _dst);

		// CCR
		void ccr_update_ifZero(CCRBit _bit);
		void ccr_update_ifNotZero(CCRBit _bit);
		void ccr_update_ifGreater(CCRBit _bit);
		void ccr_update_ifGreaterEqual(CCRBit _bit);
		void ccr_update_ifLess(CCRBit _bit);
		void ccr_update_ifLessEqual(CCRBit _bit);
		void ccr_update_ifCarry(CCRBit _bit);
		void ccr_update_ifNotCarry(CCRBit _bit);
#ifndef HAVE_ARM64
		void ccr_update_ifParity(CCRBit _bit);
		void ccr_update_ifNotParity(CCRBit _bit);
#endif
		void ccr_update_ifAbove(CCRBit _bit);
		void ccr_update_ifBelow(CCRBit _bit);

		void ccr_update(const JitRegGP& _value, CCRBit _bit);

#ifdef HAVE_ARM64
		void ccr_update(CCRBit _bit, asmjit::arm::CondCode _armConditionCode);
#endif
		
		void ccr_u_update(const JitReg64& _alu);
		void ccr_e_update(const JitReg64& _alu);
		void ccr_n_update_by55(const JitReg64& _alu);
		void ccr_n_update_by47(const JitReg64& _alu);
		void ccr_n_update_by23(const JitReg64& _alu);
		void ccr_s_update(const JitReg64& _alu);
		void ccr_l_update_by_v();
		void ccr_v_update(const JitReg64& _nonMaskedResult);

		void ccr_clear(CCRMask _mask);
		void ccr_set(CCRMask _mask);
		void ccr_dirty(TWord _aluIndex, const JitReg64& _alu, CCRMask _dirtyBits = static_cast<CCRMask>(CCR_E | CCR_U));
		void ccr_clearDirty(CCRMask _mask);
		void updateDirtyCCR();
		void updateDirtyCCR(CCRMask _whatToUpdate);
		void updateDirtyCCR(const JitReg64& _alu, CCRMask _dirtyBits);

		void ccr_getBitValue(const JitRegGP& _dst, CCRBit _bit);
		void sr_getBitValue(const JitRegGP& _dst, SRBit _bit) const;

		void XYto56(const JitReg64& _dst, int _xy) const;
		void XY0to56(const JitReg64& _dst, int _xy) const;
		void XY1to56(const JitReg64& _dst, int _xy) const;

		// decode
		void decode_cccc(const JitRegGP& _dst, TWord cccc);
		void decode_dddddd_read(const JitReg32& _dst, TWord _dddddd);
		void decode_dddddd_write(TWord _dddddd, const JitReg32& _src, bool _sourceIs8Bit = false);
		void decode_ddddd_pcr_read(const JitReg32& _dst, TWord _ddddd);
		void decode_ddddd_pcr_write(TWord _ddddd, const JitReg32& _src);
		void decode_ee_read(const JitRegGP& _dst, TWord _ee);
		void decode_ee_write(TWord _ee, const JitRegGP& _value);
		void decode_EE_read(RegGP& dst, TWord _ee);
		void decode_EE_write(const JitReg64& _src, TWord _ee);
		void decode_ff_read(const JitRegGP& _dst, TWord _ff);
		void decode_ff_write(TWord _ff, const JitRegGP& _value);
		void decode_JJJ_read_56(const JitReg64& _dst, TWord JJJ, bool _b) const;
		void decode_JJ_read(const JitReg64& _dst, TWord jj) const;
		void decode_RRR_read(const JitRegGP& _dst, TWord _mmmrrr, int _shortDisplacement = 0);
		void decode_qq_read(const JitRegGP& _dst, TWord _qq);
		void decode_QQ_read(const JitRegGP& _dst, TWord _qq);
		void decode_QQQQ_read(const JitRegGP& _s1, const JitRegGP& _s2, TWord _qqqq) const;
		void decode_sss_read(const JitReg64& _dst, TWord _sss) const;
		void decode_LLL_read(TWord _lll, const JitReg32& x, const JitReg32& y);
		void decode_LLL_write(TWord _lll, const JitReg32& x, const JitReg32& y);
		void decode_XMove_MMRRR(const JitReg64& _dst, TWord _mm, TWord _rrr);

		TWord getOpWordA() const { return m_opWordA; }
		TWord getOpWordB();
		void getOpWordB(const JitRegGP& _dst);

		// ALU
		void unsignedImmediateToAlu(const JitReg64& _r, const asmjit::Imm& _i) const;

		void alu_abs(const JitRegGP& _r);
		
		void alu_add(TWord _ab, RegGP& _v);
		void alu_add(TWord _ab, const asmjit::Imm& _v);

		void alu_sub(TWord _ab, RegGP& _v);
		void alu_sub(TWord _ab, const asmjit::Imm& _v);

		void alu_and(TWord ab, RegGP& _v);

		void alu_asl(TWord _abSrc, TWord _abDst, const ShiftReg& _v);
		void alu_asr(TWord _abSrc, TWord _abDst, const ShiftReg& _v);

		void alu_bclr(const JitReg64& _dst, TWord _bit);
		void alu_bset(const JitReg64& _dst, TWord _bit);
		void alu_bchg(const JitReg64& _dst, TWord _bit);

		void alu_cmp(TWord ab, const JitReg64& _v, bool magnitude, bool updateCarry = true);

		void alu_lsl(TWord ab, int _shiftAmount);
		void alu_lsr(TWord ab, int _shiftAmount);
		void alu_mpy(TWord ab, RegGP& _s1, RegGP& _s2, bool _negate, bool _accumulate, bool _s1Unsigned, bool _s2Unsigned, bool _round);
		void alu_multiply(TWord op);
		void alu_or(TWord ab, const JitRegGP& _v);
		void alu_rnd(TWord ab);
		void alu_rnd(TWord ab, const JitReg64& d);
		
		template<Instruction Inst> void bitmod_ea(TWord _op, void(JitOps::*_bitmodFunc)(const JitReg64&, TWord));
		template<Instruction Inst> void bitmod_aa(TWord _op, void(JitOps::*_bitmodFunc)(const JitReg64&, TWord));
		template<Instruction Inst> void bitmod_ppqq(TWord _op, void(JitOps::*_bitmodFunc)(const JitReg64&, TWord));
		template<Instruction Inst> void bitmod_D(TWord _op, void(JitOps::*_bitmodFunc)(const JitReg64&, TWord));

		// move
		template<Instruction Inst> void move_ddddd_MMMRRR(TWord _op, EMemArea _area);
		template<Instruction Inst> void move_ddddd_absAddr(TWord _op, EMemArea _area);
		template<Instruction Inst> void move_Rnxxxx(TWord op, EMemArea _area);
		template<Instruction Inst> void move_Rnxxx(TWord op, EMemArea _area);
		template<Instruction Inst> void move_L(TWord op);
		template<Instruction Inst> void movep_qqea(TWord op, EMemArea _area);
		template<Instruction Inst> void movep_sqq(TWord op, EMemArea _area);

		// loops
		void do_exec(RegGP& _lc, TWord _addr);
		void do_end(const RegGP& _temp);
		void do_end();
		void rep_exec(TWord _lc);
		void rep_exec(RegGP& _lc, TWord _lcImmediateOperand);

		// -------------- bra variants
		template<BraMode Bmode> void braOrBsr(int offset);
		template<BraMode Bmode> void braOrBsr(const JitReg32& _offset);

		template<Instruction Inst, BraMode Bmode, typename TOff> void braIfCC(TWord op, const TOff& offset);

		template<Instruction Inst, BraMode Bmode, ExpectedBitValue BitValue> void braIfBitTestMem(TWord op);
		template<Instruction Inst, BraMode Bmode, ExpectedBitValue BitValue> void braIfBitTestDDDDDD(TWord op);

		// -------------- jmp variants
		template<JumpMode Jsr> void jumpOrJSR(TWord ea);
		template<JumpMode Jsr> void jumpOrJSR(const JitReg32& ea);

		template<Instruction Inst, JumpMode Jsr, typename TAbsAddr> void jumpIfCC(TWord op, const TAbsAddr& ea);
		
		template<Instruction Inst, JumpMode Jsr, ExpectedBitValue BitValue> void jumpIfBitTestMem(TWord _op);
		template<Instruction Inst, JumpMode Jsr, ExpectedBitValue BitValue> void jumpIfBitTestDDDDDD(TWord op);

		void jmp(const JitReg32& _absAddr);
		void jsr(const JitReg32& _absAddr);

		void jmp(TWord _absAddr);
		void jsr(TWord _absAddr);

		TWord getOpSize() const { return m_opSize; }
		Instruction getInstruction() const { return m_instruction; }

		bool checkResultFlag(const ResultFlags _flag) const { return (m_resultFlags & _flag) != 0; }
		uint32_t getResultFlags() const { return m_resultFlags; }

		RegisterMask getWrittenRegs() const { return m_writtenRegs; }
		RegisterMask getReadRegs() const { return m_readRegs; }

	private:
		enum RepMode
		{
			RepNone,
			RepDynamic,
			RepFirst,
			RepLoop,
			RepLast
		};

		void errNotImplemented(TWord op);

		JitBlock& m_block;
		const Opcodes& m_opcodes;
		JitDspRegs& m_dspRegs;
		JitEmitter& m_asm;

		CCRMask& m_ccrDirty;
		bool m_ccr_update_clear = true;

		TWord m_pcCurrentOp = 0;
		TWord m_opWordA = 0;
		TWord m_opWordB = 0;

		TWord m_opSize = 0;
		Instruction m_instruction = InstructionCount;

		uint32_t m_resultFlags = None;
		RepMode m_repMode = RepNone;
		RegisterMask m_writtenRegs = RegisterMask::None;
		RegisterMask m_readRegs = RegisterMask::None;
		bool m_fastInterrupt = false;
	};
}
