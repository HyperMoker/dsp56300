#pragma once

#include <map>

#include "jitdspregs.h"
#include "jitregtracker.h"
#include "opcodetypes.h"
#include "registers.h"
#include "types.h"

namespace dsp56k
{
	class Jit;
	class Opcodes;

	class JitOps
	{
	public:
		JitOps(JitBlock& _block, bool _useSRCache = true);

		void emit(TWord pc, TWord op);
		void emit(Instruction _inst, TWord _op);
		void emit(Instruction _instMove, Instruction _instAlu, TWord _op);

		void emitOpProlog();
		void emitOpEpilog();

		void op_Abs(TWord op);
		void op_ADC(TWord op);
		void op_Add_SD(TWord op);
		void op_Add_xx(TWord op);
		void op_Add_xxxx(TWord op);
		void op_Addl(TWord op);
		void op_Addr(TWord op);
		void op_And_SD(TWord op){}
		void op_And_xx(TWord op){}
		void op_And_xxxx(TWord op){}
		void op_Andi(TWord op){}
		void op_Asl_D(TWord op){}
		void op_Asl_ii(TWord op){}
		void op_Asl_S1S2D(TWord op){}
		void op_Asr_D(TWord op){}
		void op_Asr_ii(TWord op){}
		void op_Asr_S1S2D(TWord op){}
		void op_Bcc_xxxx(TWord op){}
		void op_Bcc_xxx(TWord op){}
		void op_Bcc_Rn(TWord op){}
		void op_Bchg_ea(TWord op){}
		void op_Bchg_aa(TWord op){}
		void op_Bchg_pp(TWord op){}
		void op_Bchg_qq(TWord op){}
		void op_Bchg_D(TWord op){}
		void op_Bclr_ea(TWord op){}
		void op_Bclr_aa(TWord op){}
		void op_Bclr_pp(TWord op){}
		void op_Bclr_qq(TWord op){}
		void op_Bclr_D(TWord op){}
		void op_Bra_xxxx(TWord op){}
		void op_Bra_xxx(TWord op){}
		void op_Bra_Rn(TWord op){}
		void op_Brclr_ea(TWord op){}
		void op_Brclr_aa(TWord op){}
		void op_Brclr_pp(TWord op){}
		void op_Brclr_qq(TWord op){}
		void op_Brclr_S(TWord op){}
		void op_BRKcc(TWord op){}
		void op_Brset_ea(TWord op){}
		void op_Brset_aa(TWord op){}
		void op_Brset_pp(TWord op){}
		void op_Brset_qq(TWord op){}
		void op_Brset_S(TWord op){}
		void op_BScc_xxxx(TWord op){}
		void op_BScc_xxx(TWord op){}
		void op_BScc_Rn(TWord op){}
		void op_Bsclr_ea(TWord op){}
		void op_Bsclr_aa(TWord op){}
		void op_Bsclr_pp(TWord op){}
		void op_Bsclr_qq(TWord op){}
		void op_Bsclr_S(TWord op){}
		void op_Bset_ea(TWord op){}
		void op_Bset_aa(TWord op){}
		void op_Bset_pp(TWord op){}
		void op_Bset_qq(TWord op){}
		void op_Bset_D(TWord op){}
		void op_Bsr_xxxx(TWord op){}
		void op_Bsr_xxx(TWord op){}
		void op_Bsr_Rn(TWord op){}
		void op_Bsset_ea(TWord op){}
		void op_Bsset_aa(TWord op){}
		void op_Bsset_pp(TWord op){}
		void op_Bsset_qq(TWord op){}
		void op_Bsset_S(TWord op){}
		void op_Btst_ea(TWord op){}
		void op_Btst_aa(TWord op){}
		void op_Btst_pp(TWord op){}
		void op_Btst_qq(TWord op){}
		void op_Btst_D(TWord op){}
		void op_Clb(TWord op){}
		void op_Clr(TWord op);
		void op_Cmp_S1S2(TWord op){}
		void op_Cmp_xxS2(TWord op){}
		void op_Cmp_xxxxS2(TWord op){}
		void op_Cmpm_S1S2(TWord op){}
		void op_Cmpu_S1S2(TWord op){}
		void op_Debug(TWord op){}
		void op_Debugcc(TWord op){}
		void op_Dec(TWord op){}
		void op_Div(TWord op){}
		void op_Dmac(TWord op){}
		void op_Do_ea(TWord op){}
		void op_Do_aa(TWord op){}
		void op_Do_xxx(TWord op){}
		void op_Do_S(TWord op){}
		void op_DoForever(TWord op){}
		void op_Dor_ea(TWord op){}
		void op_Dor_aa(TWord op){}
		void op_Dor_xxx(TWord op){}
		void op_Dor_S(TWord op){}
		void op_DorForever(TWord op){}
		void op_Enddo(TWord op){}
		void op_Eor_SD(TWord op){}
		void op_Eor_xx(TWord op){}
		void op_Eor_xxxx(TWord op){}
		void op_Extract_S1S2(TWord op){}
		void op_Extract_CoS2(TWord op){}
		void op_Extractu_S1S2(TWord op){}
		void op_Extractu_CoS2(TWord op){}
		void op_Ifcc(TWord op){}
		void op_Ifcc_U(TWord op){}
		void op_Illegal(TWord op){}
		void op_Inc(TWord op){}
		void op_Insert_S1S2(TWord op){}
		void op_Insert_CoS2(TWord op){}
		void op_Jcc_xxx(TWord op){}
		void op_Jcc_ea(TWord op){}
		void op_Jclr_ea(TWord op){}
		void op_Jclr_aa(TWord op){}
		void op_Jclr_pp(TWord op){}
		void op_Jclr_qq(TWord op){}
		void op_Jclr_S(TWord op){}
		void op_Jmp_ea(TWord op){}
		void op_Jmp_xxx(TWord op){}
		void op_Jscc_xxx(TWord op){}
		void op_Jscc_ea(TWord op){}
		void op_Jsclr_ea(TWord op){}
		void op_Jsclr_aa(TWord op){}
		void op_Jsclr_pp(TWord op){}
		void op_Jsclr_qq(TWord op){}
		void op_Jsclr_S(TWord op){}
		void op_Jset_ea(TWord op){}
		void op_Jset_aa(TWord op){}
		void op_Jset_pp(TWord op){}
		void op_Jset_qq(TWord op){}
		void op_Jset_S(TWord op){}
		void op_Jsr_ea(TWord op){}
		void op_Jsr_xxx(TWord op){}
		void op_Jsset_ea(TWord op){}
		void op_Jsset_aa(TWord op){}
		void op_Jsset_pp(TWord op){}
		void op_Jsset_qq(TWord op){}
		void op_Jsset_S(TWord op){}
		void op_Lra_Rn(TWord op){}
		void op_Lra_xxxx(TWord op){}
		void op_Lsl_D(TWord op){}
		void op_Lsl_ii(TWord op){}
		void op_Lsl_SD(TWord op){}
		void op_Lsr_D(TWord op){}
		void op_Lsr_ii(TWord op){}
		void op_Lsr_SD(TWord op){}
		void op_Lua_ea(TWord op){}
		void op_Lua_Rn(TWord op){}
		void op_Mac_S1S2(TWord op){}
		void op_Mac_S(TWord op){}
		void op_Maci_xxxx(TWord op){}
		void op_Macsu(TWord op){}
		void op_Macr_S1S2(TWord op){}
		void op_Macr_S(TWord op){}
		void op_Macri_xxxx(TWord op){}
		void op_Max(TWord op){}
		void op_Maxm(TWord op){}
		void op_Merge(TWord op){}
		void op_Move_Nop(TWord op){}
		void op_Move_xx(TWord op){}
		void op_Mover(TWord op){}
		void op_Move_ea(TWord op){}
		void op_Movex_ea(TWord op){}
		void op_Movex_aa(TWord op){}
		void op_Movex_Rnxxxx(TWord op){}
		void op_Movex_Rnxxx(TWord op){}
		void op_Movexr_ea(TWord op){}
		void op_Movexr_A(TWord op){}
		void op_Movey_ea(TWord op){}
		void op_Movey_aa(TWord op){}
		void op_Movey_Rnxxxx(TWord op){}
		void op_Movey_Rnxxx(TWord op){}
		void op_Moveyr_ea(TWord op){}
		void op_Moveyr_A(TWord op){}
		void op_Movel_ea(TWord op){}
		void op_Movel_aa(TWord op){}
		void op_Movexy(TWord op){}
		void opCE_Movexy(TWord op){}
		void op_Movec_ea(TWord op){}
		void op_Movec_aa(TWord op){}
		void op_Movec_S1D2(TWord op){}
		void op_Movec_xx(TWord op){}
		void op_Movem_ea(TWord op){}
		void op_Movem_aa(TWord op){}
		void op_Movep_ppea(TWord op){}
		void op_Movep_Xqqea(TWord op){}
		void op_Movep_Yqqea(TWord op){}
		void op_Movep_eapp(TWord op){}
		void op_Movep_eaqq(TWord op){}
		void op_Movep_Spp(TWord op){}
		void op_Movep_SXqq(TWord op){}
		void op_Movep_SYqq(TWord op){}
		void op_Mpy_S1S2D(TWord op){}
		void op_Mpy_SD(TWord op){}
		void op_Mpy_su(TWord op){}
		void op_Mpyi(TWord op){}
		void op_Mpyr_S1S2D(TWord op){}
		void op_Mpyr_SD(TWord op){}
		void op_Mpyri(TWord op){}
		void op_Neg(TWord op){}
		void op_Nop(TWord op){}
		void op_Norm(TWord op){}
		void op_Normf(TWord op){}
		void op_Not(TWord op){}
		void op_Or_SD(TWord op){}
		void op_Or_xx(TWord op){}
		void op_Or_xxxx(TWord op){}
		void op_Ori(TWord op){}
		void op_Pflush(TWord op){}
		void op_Pflushun(TWord op){}
		void op_Pfree(TWord op){}
		void op_Plock(TWord op){}
		void op_Plockr(TWord op){}
		void op_Punlock(TWord op){}
		void op_Punlockr(TWord op){}
		void op_Rep_ea(TWord op){}
		void op_Rep_aa(TWord op){}
		void op_Rep_xxx(TWord op){}
		void op_Rep_S(TWord op){}
		void op_Reset(TWord op){}
		void op_Rnd(TWord op){}
		void op_Rol(TWord op){}
		void op_Ror(TWord op){}
		void op_Rti(TWord op){}
		void op_Rts(TWord op){}
		void op_Sbc(TWord op){}
		void op_Stop(TWord op){}
		void op_Sub_SD(TWord op){}
		void op_Sub_xx(TWord op){}
		void op_Sub_xxxx(TWord op){}
		void op_Subl(TWord op){}
		void op_subr(TWord op){}
		void op_Tcc_S1D1(TWord op){}
		void op_Tcc_S1D1S2D2(TWord op){}
		void op_Tcc_S2D2(TWord op){}
		void op_Tfr(TWord op){}
		void op_Trap(TWord op){}
		void op_Trapcc(TWord op){}
		void op_Tst(TWord op){}
		void op_Vsl(TWord op){}
		void op_Wait(TWord op){}

		// helpers
		void signextend56to64(const asmjit::x86::Gpq& _reg) const;
		void signextend48to56(const asmjit::x86::Gpq& _reg) const;
		void signextend24to56(const asmjit::x86::Gpq& _reg) const;

		void mask56(const RegGP& _alu) const;

		void signed24To56(const asmjit::x86::Gpq& _r) const;

		// CCR
		void ccr_update_ifZero(CCRBit _bit) const;
		void ccr_update_ifNotZero(CCRBit _bit) const;
		void ccr_update_ifGreater(CCRBit _bit) const;
		void ccr_update_ifGreaterEqual(CCRBit _bit) const;
		void ccr_update_ifLessThan(CCRBit _bit) const;
		void ccr_update_ifLessEqual(CCRBit _bit) const;
		void ccr_update_ifCarry(CCRBit _bit) const;
		void ccr_update_ifParity(CCRBit _bit) const;
		void ccr_update_ifNotParity(CCRBit _bit) const;

		void ccr_update(const RegGP& _value, CCRBit _bit) const;

		void ccr_u_update(const asmjit::x86::Gpq& _alu) const;
		void ccr_e_update(const asmjit::x86::Gpq& _alu) const;
		void ccr_n_update_by55(const asmjit::x86::Gpq& _alu) const;
		void ccr_n_update_by47(const asmjit::x86::Gpq& _alu) const;
		void ccr_s_update(const asmjit::x86::Gpq& _alu) const;

		void ccr_clear(CCRMask _mask) const;
		void ccr_set(CCRMask _mask) const;
		void ccr_dirty(const asmjit::x86::Gpq& _alu);

		void sr_getBitValue(const asmjit::x86::Gpq& _dst, CCRBit _bit) const;

		void XYto56(const asmjit::x86::Gpq& _dst, int _xy) const;
		void XY0to56(const asmjit::x86::Gpq& _dst, int _xy) const;
		void XY1to56(const asmjit::x86::Gpq& _dst, int _xy) const;

		// decode
		void decode_JJJ_read_56(asmjit::x86::Gpq dst, TWord JJJ, bool b);

		// ALU
		void alu_add(TWord ab, RegGP& _v);
		void unsignedImmediateToAlu(const RegGP& _r, const asmjit::Imm& _i) const;
		void alu_add(TWord ab, const asmjit::Imm& _v);
		void alu_add_epilog(TWord ab, RegGP& alu);
		
	private:
		JitBlock& m_block;
		const Opcodes& m_opcodes;
		JitDspRegs& m_dspRegs;
		asmjit::x86::Assembler& m_asm;
		std::map<TWord, asmjit::Label> m_pcLabels;
		TWord m_pcCurrentOp = 0;
		bool m_srDirty = true;
		const bool m_useSRCache;
	};
}
