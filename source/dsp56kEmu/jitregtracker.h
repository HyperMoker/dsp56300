#pragma once

#include <list>

#include "jitdspregpool.h"
#include "types.h"
#include "jitregtypes.h"
#include "jithelper.h"

#include "asmjit/x86/x86operand.h"

namespace dsp56k
{
	class JitScopedReg;
	class JitBlock;

	class JitRegpool
	{
	public:
		JitRegpool(std::initializer_list<JitReg> _availableRegs);

		void put(JitScopedReg* _scopedReg, bool _weak);
		JitReg get(JitScopedReg* _scopedReg, bool _weak);
		bool empty() const;
		size_t available() const;
		bool isInUse(const JitReg& _gp) const;

		size_t capacity() const { return m_capacity; }

		void reset(std::initializer_list<JitReg> _availableRegs);

	private:
		const size_t m_capacity;
		std::vector<JitReg> m_availableRegs;
		std::list<JitScopedReg*> m_weakRegs;
	};

	class JitScopedReg
	{
	public:
		JitScopedReg() = delete;
		JitScopedReg(const JitScopedReg&) = delete;
		JitScopedReg(JitScopedReg&& _other) noexcept : m_reg(_other.m_reg), m_block(_other.m_block), m_pool(_other.m_pool), m_acquired(_other.m_acquired), m_weak(_other.m_weak)
		{
			_other.m_acquired = false;
		}
		JitScopedReg(JitBlock& _block, JitRegpool& _pool, const bool _acquire = true, const bool _weak = false) : m_block(_block), m_pool(_pool), m_weak(_weak)
		{
			if(_acquire) 
				acquire();
		}
		~JitScopedReg()
		{
			release();
		}

		JitScopedReg& operator = (const JitScopedReg&) = delete;

		JitScopedReg& operator = (JitScopedReg&& _other) noexcept;

		void acquire();

		void release();

		bool isValid() const { return m_acquired; }
		bool isWeak() const { return m_weak; }

		const JitReg& get() const { assert(isValid()); return m_reg; }

		JitBlock& block() const { return m_block; }

	private:
		JitReg m_reg;
		JitBlock& m_block;
		JitRegpool& m_pool;
		bool m_acquired = false;
		bool m_weak = false;
	};

	class RegGP : public JitScopedReg
	{
	public:
		RegGP(JitBlock& _block, bool _acquire = true, bool _weak = false);
		RegGP(RegGP&& _other) noexcept : JitScopedReg(std::move(_other)) {}

		const JitReg64& get() const { return JitScopedReg::get().as<JitReg64>(); }
		operator const JitReg64& () const { return get(); }
		RegGP& operator = (RegGP&& other) noexcept
		{
			JitScopedReg::operator=(std::move(other));
			return *this;
		}
	};
	
	class RegXMM : public JitScopedReg
	{
	public:
		RegXMM(JitBlock& _block, bool _acquire = true, bool _weak = false);

		const JitReg128& get() const { return JitScopedReg::get().as<JitReg128>(); }
		operator const JitReg128& () const { return get(); }
	};

	class DSPReg
	{
	public:
		DSPReg(JitBlock& _block, PoolReg _reg, bool _read = true, bool _write = true, bool _acquire = true);
		DSPReg(DSPReg&& _other) noexcept;
		~DSPReg();

		JitRegGP get() const { return m_reg; }
		operator JitRegGP() const { return get(); }

		JitReg32 r32() const { return dsp56k::r32(get()); }
		JitReg64 r64() const { return dsp56k::r64(get()); }

		void write();

		operator asmjit::Imm () const = delete;

		void acquire();
		void release();

		bool acquired() const { return m_acquired; }
		bool read() const { return m_read; }
		bool write() const { return m_write; }

		PoolReg dspReg() const { return m_dspReg; }

		JitBlock& block() { return m_block; }

		DSPReg& operator = (const DSPReg& _other) = delete;
		DSPReg& operator = (DSPReg&& _other) noexcept;

	private:
		JitBlock& m_block;
		bool m_read;
		bool m_write;
		bool m_acquired;
		bool m_locked;
		PoolReg m_dspReg;
		JitRegGP m_reg;
	};

	class DSPRegTemp
	{
	public:
		DSPRegTemp(JitBlock& _block, bool _acquire);

		DSPRegTemp(const DSPRegTemp& _existing) = delete;
		DSPRegTemp(DSPRegTemp&& _existing) noexcept : m_block(_existing.m_block)
		{
			*this = std::move(_existing);
		}

		~DSPRegTemp();

		JitReg64 get() const { return r64(m_reg); }
		operator JitReg64() const { return get(); }

		void acquire();
		void release();

		bool acquired() const { return m_dspReg != PoolReg::DspRegInvalid; }

		JitBlock& block() const { return m_block; }

		DSPRegTemp& operator = (const DSPRegTemp& _other) = delete;
		DSPRegTemp& operator = (DSPRegTemp&& _other) noexcept
		{
			m_reg = _other.m_reg;
			m_dspReg = _other.m_dspReg;

			_other.m_reg.reset();
			_other.m_dspReg = PoolReg::DspRegInvalid;

			return *this;
		}

	private:
		JitBlock& m_block;
		PoolReg m_dspReg = PoolReg::DspRegInvalid;
		JitRegGP m_reg;
	};

	class AluReg
	{
	public:
		AluReg(JitBlock& _block, TWord _aluIndex, bool readOnly = false, bool writeOnly = false);
		~AluReg();
		JitReg64 get() const;
		operator JitReg64 () { return get(); }
		void release();

	private:
		JitBlock& m_block;
		RegGP m_reg;
		const bool m_readOnly;
		const bool m_writeOnly;
		const TWord m_aluIndex;
	};


	class AluRef
	{
	public:
		AluRef(JitBlock& _block, TWord _aluIndex, bool _read = true, bool _write = true);
		~AluRef();
		JitReg64 get();
		operator JitReg64 () { return get(); }

	private:
		JitBlock& m_block;
		JitReg64 m_reg;
		const bool m_read;
		const bool m_write;
		const TWord m_aluIndex;
		bool m_lockedByUs = false;
	};

	class PushGP
	{
	public:
		PushGP(JitBlock& _block, const JitReg64& _reg);
		~PushGP();

		const JitReg64& get() const { return m_reg; }
		operator const JitReg64& () const { return m_reg; }

	protected:
		JitBlock& m_block;
	private:
		const JitReg64 m_reg;
		const bool m_pushed;
	};

	class FuncArg : public PushGP
	{
	public:
		FuncArg(JitBlock& _block, const uint32_t& _argIndex);
		~FuncArg();

	private:
		const uint32_t m_funcArgIndex;
	};

#ifdef HAVE_X86_64
	class ShiftReg : public PushGP
	{
	public:
		explicit ShiftReg(JitBlock& _block);
		~ShiftReg();
	};
#else
	using ShiftReg = RegGP;
#endif

	class ShiftTemp
	{
	public:
		ShiftTemp(JitBlock& _block, bool _acquire) : m_block(_block)
		{
			if(_acquire)
				acquire();
		}

		~ShiftTemp()
		{
			release();
		}

		ShiftTemp(const ShiftTemp&) = delete;
		ShiftTemp(ShiftTemp&& _source) noexcept : m_block(_source.m_block), m_reg(std::move(_source.m_reg))
		{
		}

		ShiftTemp& operator = (const ShiftTemp&) = delete;
		ShiftTemp& operator = (ShiftTemp&& _source) noexcept
		{
			m_reg = std::move(_source.m_reg);
			return *this;
		}

		void acquire();
		void release();
		bool isValid() const { return m_reg != nullptr; }

		JitReg64 get() const
		{
			return isValid() ? m_reg->get() : JitReg64();
		}

	private:
		JitBlock& m_block;
		std::unique_ptr<ShiftReg> m_reg;
	};

	template<typename T>
	class PushRegs
	{
	public:
		PushRegs(JitBlock& _block) : m_block(_block) {}
		~PushRegs();

		void push(const T& _reg);

	protected:
		JitBlock& m_block;
	private:
		std::list<T> m_pushedRegs;
	};

	class PushXMMRegs : PushRegs<JitReg128>
	{
	public:
		PushXMMRegs(JitBlock& _block);
	};

	class PushGPRegs : PushRegs<JitReg64>
	{
	public:
		PushGPRegs(JitBlock& _block);

	private:
		void push(const JitRegGP& _gp);
	};

	class PushBeforeFunctionCall
	{
	public:
		PushBeforeFunctionCall(JitBlock& _block);

		PushXMMRegs m_xmm;
		PushGPRegs m_gp;
	};

	class RegScratch
	{
	public:
		explicit RegScratch(JitBlock& _block, bool _acquire = true);
		RegScratch(RegScratch&& _other) noexcept : m_block(_other.m_block), m_acquired(_other.m_acquired)
		{
			_other.m_acquired = false;
		}
		explicit RegScratch(const RegScratch&) = delete;

		~RegScratch();

		void acquire();
		void release();

		JitBlock& block() const { return m_block; }

		RegScratch& operator = (RegScratch&& _other) noexcept
		{
			if(_other.isValid())
			{
				_other.release();
				acquire();
			}
			return *this;
		}
		RegScratch& operator = (const RegScratch&) = delete;

		JitReg64 get() const { assert(isValid()); return isValid() ? regReturnVal : JitReg64(); }
		JitReg64 reg() const { return get(); }
		operator JitReg64 () const { return get(); }

		bool isValid() const
		{
			return m_acquired;
		}

#ifdef HAVE_X86_64
		auto r8() const { return get().r8(); }
#endif

	private:
		JitBlock& m_block;
		bool m_acquired = false;
	};
}
