#pragma once

#include <cstdint>
#include <vector>

#include "jitregtypes.h"

namespace dsp56k
{
	class JitBlock;

	class JitStackHelper
	{
	public:
		using Reg = asmjit::x86::Reg;

		JitStackHelper(JitBlock& _block);
		~JitStackHelper();

		void push(const JitRegGP& _reg);
		void push(const JitReg128& _reg);

		void pop(const JitRegGP& _reg);
		void pop(const JitReg128& _reg);
		void pop(const Reg& _reg);
		void pop();

		void popAll();

		void pushNonVolatiles();
		
		void call(const void* _funcAsPtr) const;
		
		static bool isFuncArg(const JitRegGP& _gp);
		static bool isNonVolatile(const JitRegGP& _gp);
		static bool isNonVolatile(const JitReg128& _xm);
		void setUsed(const Reg& _reg);
		void setUsed(const JitRegGP& _reg);
		void setUsed(const JitReg128& _reg);

		bool isUsed(const Reg& _reg) const;
	private:
		JitBlock& m_block;
		uint32_t m_pushedBytes = 0;
		std::vector<Reg> m_pushedRegs;
		std::vector<Reg> m_usedRegs;
	};
}
