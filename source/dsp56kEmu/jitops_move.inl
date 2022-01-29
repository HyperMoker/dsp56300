#pragma once

namespace dsp56k
{
	void JitOps::op_Move_Nop(TWord op)
	{
	}

	void JitOps::op_Move_xx(TWord op)
	{
		const auto ddddd = getFieldValue<Move_xx, Field_ddddd>(op);
		const auto iiiiiiii = getFieldValue<Move_xx, Field_iiiiiiii>(op);

		const RegGP i(m_block);
		m_asm.mov(r32(i.get()), asmjit::Imm(iiiiiiii));
		decode_dddddd_write(ddddd, r32(i.get()), true);
	}

	void JitOps::op_Mover(TWord op)
	{
		const auto eeeee = getFieldValue<Mover,Field_eeeee>(op);
		const auto ddddd = getFieldValue<Mover,Field_ddddd>(op);

		const RegGP r(m_block);
		decode_dddddd_read(r32(r.get()), eeeee);
		decode_dddddd_write(ddddd, r32(r.get()));
	}

	void JitOps::op_Move_ea(TWord op)
	{
		const auto mm  = getFieldValue<Move_ea, Field_MM>(op);
		const auto rrr = getFieldValue<Move_ea, Field_RRR>(op);

		const JitReg64 unused;
		updateAddressRegister(unused, mm, rrr, true, true);
	}

	template<Instruction Inst> void JitOps::move_ddddd_MMMRRR(const TWord _op, const EMemArea _area)
	{
		const TWord ddddd = getFieldValue<Inst,Field_dd, Field_ddd>(_op);
		const auto mmm = getFieldValue<Inst,Field_MMM>(_op);
		const auto rrr = getFieldValue<Inst,Field_RRR>(_op);
		const auto write = getFieldValue<Inst,Field_W>(_op);

		const RegGP r(m_block);

		if (write)
		{
			readMem<Inst>(r, _op, _area);
			decode_dddddd_write(ddddd, r32(r.get()));
		}
		else
		{
			decode_dddddd_read(r32(r.get()), ddddd);
			writeMem<Inst>(_op, _area, r);
		}
	}
	template<Instruction Inst> void JitOps::move_ddddd_absAddr(TWord _op, const EMemArea _area)
	{
		const auto ddddd = getFieldValue<Inst,Field_dd, Field_ddd>(_op);
		const auto write = getFieldValue<Inst,Field_W>(_op);
		
		const RegGP r(m_block);

		if (write)
		{
			readMem<Inst>(r, _op, _area);
			decode_dddddd_write(ddddd, r32(r.get()));
		}
		else
		{
			decode_dddddd_read(r32(r.get()), ddddd);
			writeMem<Inst>(_op, _area, r);
		}
	}

	void JitOps::op_Movex_ea(TWord op)
	{
		move_ddddd_MMMRRR<Movex_ea>(op, MemArea_X);
	}

	void JitOps::op_Movey_ea(TWord op)
	{
		move_ddddd_MMMRRR<Movey_ea>(op, MemArea_Y);
	}

	void JitOps::op_Movex_aa(TWord op)
	{
		move_ddddd_absAddr<Movex_aa>(op, MemArea_X);
	}

	void JitOps::op_Movey_aa(TWord op)
	{
		move_ddddd_absAddr<Movey_aa>(op, MemArea_Y);
	}

	template<Instruction Inst> void JitOps::move_Rnxxxx(TWord op, EMemArea _area)
	{
		const TWord DDDDDD	= getFieldValue<Inst,Field_DDDDDD>(op);
		const auto	write	= getFieldValue<Inst,Field_W>(op);
		const TWord rrr		= getFieldValue<Inst,Field_RRR>(op);

		const int shortDisplacement = pcRelativeAddressExt<Inst>();

		RegGP ea(m_block);
		decode_RRR_read(ea, rrr, shortDisplacement);

		const RegGP r(m_block);

		if( write )
		{
			readMemOrPeriph(r, _area, ea, Inst);
			decode_dddddd_write(DDDDDD, r32(r.get()));
		}
		else
		{
			decode_dddddd_read(r32(r.get()), DDDDDD);
			writeMemOrPeriph(_area, ea, r);
		}
	}

	void JitOps::op_Movex_Rnxxxx(TWord op)
	{
		move_Rnxxxx<Movex_Rnxxxx>(op, MemArea_X);
	}

	void JitOps::op_Movey_Rnxxxx(TWord op)
	{
		move_Rnxxxx<Movey_Rnxxxx>(op, MemArea_Y);
	}

	template<Instruction Inst> void JitOps::move_Rnxxx(TWord op, EMemArea _area)
	{
		const TWord ddddd	= getFieldValue<Inst,Field_DDDD>(op);
		const auto write	= getFieldValue<Inst,Field_W>(op);
		const auto aaaaaaa	= getFieldValue<Inst,Field_aaaaaa, Field_a>(op);
		const auto rrr		= getFieldValue<Inst,Field_RRR>(op);

		const int shortDisplacement = signextend<int,7>(aaaaaaa);

		RegGP ea(m_block);
		decode_RRR_read(ea, rrr, shortDisplacement);

		RegGP r(m_block);

		if( write )
		{
			readMemOrPeriph(r, _area, ea, Inst);
			decode_dddddd_write(ddddd, r32(r.get()));
		}
		else
		{
			decode_dddddd_read(r32(r.get()), ddddd);
			writeMemOrPeriph(_area, ea, r);
		}
	}

	void JitOps::op_Movex_Rnxxx(TWord op)
	{
		move_Rnxxx<Movex_Rnxxx>(op, MemArea_X);
	}

	void JitOps::op_Movey_Rnxxx(TWord op)
	{
		move_Rnxxx<Movey_Rnxxx>(op, MemArea_Y);
	}

	void JitOps::op_Movexr_ea(TWord op)
	{
		const TWord F		= getFieldValue<Movexr_ea,Field_F>(op);	// true:Y1, false:Y0
		const TWord ff		= getFieldValue<Movexr_ea,Field_ff>(op);
		const TWord write	= getFieldValue<Movexr_ea,Field_W>(op);
		const TWord d		= getFieldValue<Movexr_ea,Field_d>(op);

		// S2 D2 read
		RegGP ab(m_block);
		transferAluTo24(ab, d);

		const RegGP r(m_block);

		// S1 D1 read
		if( write )
			readMem<Movexr_ea>(r, op, MemArea_X);
		else
			decode_ee_read(r, ff);

		// S2 D2 write
		if(F)		setXY1(1, ab);
		else		setXY0(1, ab);

		ab.release();

		// S1 D1 write
		if(write)
		{
			decode_ee_write(ff, r);
		}
		else
		{
			writeMem<Movexr_ea>(op, MemArea_X, r);
		}
	}

	void JitOps::op_Moveyr_ea(TWord op)
	{
		const bool e		= getFieldValue<Moveyr_ea,Field_e>(op);	// true:X1, false:X0
		const TWord ff		= getFieldValue<Moveyr_ea,Field_ff>(op);
		const bool write	= getFieldValue<Moveyr_ea,Field_W>(op);
		const bool d		= getFieldValue<Moveyr_ea,Field_d>(op);

		// S2 D2 read
		const RegGP ab(m_block);
		transferAluTo24(ab, d);

		// S1 D1 read
		RegGP r(m_block);
		if( write )
		{
			readMem<Moveyr_ea>(r, op, MemArea_Y);
		}
		else
		{
			decode_ff_read(r, ff);
		}

		// S2 D2 write
		{
			if( e )		setXY1(0, ab);
			else		setXY0(0, ab);
		}
	
		// S1 D1 write
		if( write )
		{
			decode_ff_write( ff, r );
		}
		else
		{
			writeMem<Moveyr_ea>(op, MemArea_Y, r);
		}
	}

	void JitOps::op_Movexr_A(TWord op)
	{
		const auto d = getFieldValue<Movexr_A,Field_d>(op);

		{
			const RegGP ab(m_block);
			transferAluTo24(ab, d);

			writeMem<Movexr_A>(op, MemArea_X, ab);
		}
		{
			RegGP x0(m_block);
			getX0(x0);
			transfer24ToAlu(d, x0);
		}
	}

	void JitOps::op_Moveyr_A(TWord op)
	{
		const auto d = getFieldValue<Moveyr_A,Field_d>(op);

		{
			const RegGP ab(m_block);
			transferAluTo24(ab, d);

			writeMem<Moveyr_A>(op, MemArea_Y, ab);
		}
		{
			RegGP y0(m_block);
			getY0(y0);
			transfer24ToAlu(d, y0);
		}
	}

	template<Instruction Inst> void JitOps::move_L(TWord op)
	{
		const auto LLL		= getFieldValue<Inst,Field_L, Field_LL>(op);
		const auto write	= getFieldValue<Inst,Field_W>(op);
		const auto eaType	= effectiveAddressType<Inst>(op);

		const RegGP x(m_block);
		const RegGP y(m_block);

		if( write )
		{
			switch (eaType)
			{
			case Immediate:
				m_asm.mov(r32(x), asmjit::Imm(getOpWordB()));
				m_asm.mov(r32(y), asmjit::Imm(m_opWordB));
				break;
			case Peripherals:
				m_block.mem().readPeriph(x, MemArea_X, getOpWordB(), Inst);
				m_block.mem().readPeriph(y, MemArea_Y, m_opWordB, Inst);
				break;
			case Memory:
				m_block.mem().readDspMemory(x, y, getOpWordB());
				break;
			case Dynamic:
				{
					const RegGP ea(m_block);
					effectiveAddress<Inst>(ea, op);
					m_block.mem().readDspMemory(x, y, ea);
//					readMemOrPeriph(x, MemArea_X, ea, Inst);
//					readMemOrPeriph(y, MemArea_Y, ea, Inst);
				}
				break;
			}

			decode_LLL_write(LLL, r32(x.get()), r32(y.get()));
		}
		else
		{
			decode_LLL_read(LLL, r32(x.get()), r32(y.get()));

			switch (eaType)
			{
			case Immediate:
				assert(false && "unable to write to immediate");
				getOpWordB();
				break;
			case Peripherals:
				m_block.mem().writePeriph(MemArea_X, getOpWordB(), x);
				m_block.mem().writePeriph(MemArea_Y, m_opWordB, y);
				break;
			case Memory:
				m_block.mem().writeDspMemory(getOpWordB(), x.get(), y.get());
				break;
			case Dynamic:
				{
					const RegGP ea(m_block);
					effectiveAddress<Inst>(ea, op);
					m_block.mem().writeDspMemory(ea, x, y);
//					writeMemOrPeriph(MemArea_X, ea, x);
//					writeMemOrPeriph(MemArea_Y, ea, y);
				}
				break;
			}

		}
	}

	inline void JitOps::op_Movel_ea(TWord op)
	{
		move_L<Movel_ea>(op);
	}

	inline void JitOps::op_Movel_aa(TWord op)
	{
		const auto LLL		= getFieldValue<Movel_aa,Field_L, Field_LL>(op);
		const auto write	= getFieldValue<Movel_aa,Field_W>(op);
		const auto ea		= getFieldValue<Movel_aa,Field_aaaaaa>(op);

		const RegGP x(m_block);
		const RegGP y(m_block);

		if( write )
		{
			m_block.mem().readDspMemory(x, MemArea_X, ea);
			m_block.mem().readDspMemory(y, MemArea_Y, ea);

			decode_LLL_write(LLL, r32(x.get()), r32(y.get()));
		}
		else
		{
			decode_LLL_read(LLL, r32(x.get()), r32(y.get()));

			m_block.mem().writeDspMemory(MemArea_X, ea, x);
			m_block.mem().writeDspMemory(MemArea_Y, ea, y);
		}
	}

	inline void JitOps::op_Movexy(TWord op)
	{
		const auto MM		= getFieldValue<Movexy,Field_MM>(op);
		const auto RRR		= getFieldValue<Movexy,Field_RRR>(op);
		const auto mm		= getFieldValue<Movexy,Field_mm>(op);
		      auto rr		= getFieldValue<Movexy,Field_rr>(op);
		const auto writeX	= getFieldValue<Movexy,Field_W>(op);
		const auto writeY	= getFieldValue<Movexy,Field_w>(op);
		const auto ee		= getFieldValue<Movexy,Field_ee>(op);
		const auto ff		= getFieldValue<Movexy,Field_ff>(op);

		const TWord regIdxOffset = RRR >= 4 ? 0 : 4;
		rr = (rr + regIdxOffset) & 7;

		if (!writeX && !writeY)
		{
			// we need to read the values first to prevent running out of temps

			const RegGP rx(m_block);
			decode_ee_read(rx, ee);
			const RegGP ry(m_block);
			decode_ff_read(ry, ff);

			const RegGP eaX(m_block);
			decode_XMove_MMRRR(eaX, MM, RRR);

			const RegGP eaY(m_block);
			decode_XMove_MMRRR(eaY, mm, rr);

			m_block.mem().writeDspMemory(eaX, eaY, rx, ry);
			return;
		}

		if(writeX && writeY)
		{
			const RegGP rx(m_block);
			const RegGP ry(m_block);

			{
				RegGP eaX(m_block);
				decode_XMove_MMRRR(eaX, MM, RRR);

				const RegGP eaY(m_block);
				decode_XMove_MMRRR(eaY, mm, rr);

				m_block.mem().readDspMemory(rx, ry, eaX, eaY);
			}

			decode_ee_write(ee, rx);
			decode_ff_write(ff, ry);
			return;
		}

		const RegGP basePtr(m_block);
		m_block.mem().getPMemBasePtr(basePtr);

		if(!writeX)
		{
			const RegGP r(m_block);
			decode_ee_read( r, ee );
			RegGP eaX(m_block);
			decode_XMove_MMRRR(eaX, MM, RRR);
			m_block.mem().writeDspMemory(MemArea_X, eaX, r, basePtr);
		}
		if(!writeY)
		{
			const RegGP r(m_block);
			decode_ff_read( r, ff );
			const RegGP eaY(m_block);
			decode_XMove_MMRRR(eaY, mm, rr);
			m_block.mem().writeDspMemory( MemArea_Y, eaY, r, basePtr);
		}

		if( writeX )
		{
			const RegGP r(m_block);
			RegGP eaX(m_block);
			decode_XMove_MMRRR(eaX, MM, RRR);
			m_block.mem().readDspMemory(r, MemArea_X, eaX, basePtr);
			decode_ee_write( ee, r );
		}

		if( writeY )
		{
			const RegGP r(m_block);
			const RegGP eaY(m_block);
			decode_XMove_MMRRR(eaY, mm, rr);
			m_block.mem().readDspMemory(r, MemArea_Y, eaY, basePtr);
			decode_ff_write( ff, r);
		}
	}

	inline void JitOps::op_Movec_ea(TWord op)
	{
		const auto ddddd = getFieldValue<Movec_ea,Field_DDDDD>(op);
		const auto write = getFieldValue<Movec_ea,Field_W>(op);

		if( write )
		{
			const RegGP r(m_block);
			readMem<Movec_ea>(r, op);

			decode_ddddd_pcr_write( ddddd, r32(r.get()));
		}
		else
		{
			const RegGP r(m_block);
			decode_ddddd_pcr_read(r32(r.get()), ddddd);

			writeMem<Movec_ea>(op, r);
		}
	}
	inline void JitOps::op_Movec_aa(TWord op)
	{
		const auto ddddd	= getFieldValue<Movec_aa,Field_DDDDD>(op);
		const auto write	= getFieldValue<Movec_aa,Field_W>(op);

		if( write )
		{
			RegGP r(m_block);
			readMem<Movec_aa>(r, op);
			decode_ddddd_pcr_write( ddddd, r32(r.get()) );
		}
		else
		{
			const auto addr = getFieldValue<Movec_aa,Field_aaaaaa>(op);
			const auto area = getFieldValueMemArea<Movec_aa>(op);

			const RegGP r(m_block);
			decode_ddddd_pcr_read(r32(r.get()), ddddd);
			m_block.mem().writeDspMemory(area, addr, r);
		}
	}
	inline void JitOps::op_Movec_S1D2(TWord op)
	{
		const auto write	= getFieldValue<Movec_S1D2,Field_W>(op);
		const auto eeeeee	= getFieldValue<Movec_S1D2,Field_eeeeee>(op);
		const auto ddddd	= getFieldValue<Movec_S1D2,Field_DDDDD>(op);

		const RegGP r(m_block);

		if( write )
		{
			decode_dddddd_read(r32(r.get()), eeeeee);
			decode_ddddd_pcr_write(ddddd, r32(r.get()));
		}
		else
		{
			decode_ddddd_pcr_read(r32(r.get()), ddddd);
			decode_dddddd_write(eeeeee, r32(r.get()));
		}
	}

	inline void JitOps::op_Movec_xx(TWord op)
	{
		const auto iiiiiiii	= getFieldValue<Movec_xx, Field_iiiiiiii>(op);
		const auto ddddd	= getFieldValue<Movec_xx,Field_DDDDD>(op);

		const RegGP r(m_block);
		m_asm.mov(r32(r.get()), asmjit::Imm(iiiiiiii));
		decode_ddddd_pcr_write( ddddd, r32(r.get()));
	}

	inline void JitOps::op_Movem_ea(TWord op)
	{
		const auto write	= getFieldValue<Movem_ea,Field_W>(op);
		const auto dddddd	= getFieldValue<Movem_ea,Field_dddddd>(op);

		if( write )
		{
			RegGP r(m_block);
			readMem<Movem_ea>(r, op, MemArea_P);
			decode_dddddd_write( dddddd, r32(r.get()));
		}
		else
		{
			const RegGP ea(m_block);
			const auto eaType = effectiveAddress<Movem_ea>(ea, op);

			const RegGP r(m_block);
			decode_dddddd_read(r32(r.get()), dddddd);

			const RegGP compare(m_block);

			if (eaType == Dynamic)	m_block.mem().readDspMemory(compare, MemArea_P, ea);
			else					m_block.mem().readDspMemory(compare, MemArea_P, m_opWordB);

			const auto skip = m_asm.newLabel();
			m_asm.cmp(compare, r.get());
			m_asm.jz(skip);

			if (eaType == Dynamic)	m_block.mem().writeDspMemory(MemArea_P, ea, r);
			else					m_block.mem().writeDspMemory(MemArea_P, m_opWordB, r);

			m_block.mem().mov(m_block.pMemWriteAddress(), r32(ea.get()));
			m_block.mem().mov(m_block.pMemWriteValue(), r32(r.get()));

			m_asm.bind(skip);

			m_resultFlags |= WritePMem;
		}
	}

	inline void JitOps::op_Movep_ppea(TWord op)
	{
		const TWord pp		= getFieldValue<Movep_ppea,Field_pppppp>(op) + 0xffffc0;
		const TWord mmmrrr	= getFieldValue<Movep_ppea,Field_MMM, Field_RRR>(op);
		const auto write	= getFieldValue<Movep_ppea,Field_W>(op);
		const EMemArea sp	= getFieldValue<Movep_ppea,Field_s>(op) ? MemArea_Y : MemArea_X;
		const EMemArea sm	= getFieldValueMemArea<Movep_ppea>(op);

		RegGP ea(m_block);
		const auto eaType = effectiveAddress<Movep_ppea>(ea, op);

		// TODO: handle eaType to skip read/write MemOrPeriph
		if( write )
		{
			if( mmmrrr == MMMRRR_ImmediateData )
			{
				m_block.mem().writePeriph(sp, pp, ea);
			}
			else
			{
				const RegGP r(m_block);
				readMemOrPeriph(r, sm, ea, Movep_ppea);
				m_block.mem().writePeriph(sp, pp, r);
			}
		}
		else
		{
			const RegGP r(m_block);
			m_block.mem().readPeriph(r, sp, pp, Movep_ppea);
			writeMemOrPeriph(sm, ea, r);
		}
	}

	template <Instruction Inst> void JitOps::movep_qqea(TWord op, const EMemArea _area)
	{
		const auto qAddr	= getFieldValue<Inst,Field_qqqqqq>(op) + 0xffff80;
		const auto write	= getFieldValue<Inst,Field_W>(op);

		if( write )
		{
			const RegGP r(m_block);
			readMem<Inst>(r, op);
			m_block.mem().writePeriph(_area, qAddr, r );
		}
		else
		{
			const RegGP r(m_block);			
			m_block.mem().readPeriph(r, _area, qAddr, Inst);
			writeMem<Inst>(op, r);
		}
	}

	inline void JitOps::op_Movep_Xqqea(TWord op)
	{
		movep_qqea<Movep_Xqqea>(op, MemArea_X);
	}
	inline void JitOps::op_Movep_Yqqea(TWord op)
	{
		movep_qqea<Movep_Yqqea>(op, MemArea_Y);
	}

	template <Instruction Inst> void JitOps::movep_sqq(TWord op, const EMemArea _area)
	{
		const TWord addr	= getFieldValue<Inst,Field_q, Field_qqqqq>(op) + 0xffff80;
		const TWord dddddd	= getFieldValue<Inst,Field_dddddd>(op);
		const auto	write	= getFieldValue<Inst,Field_W>(op);

		const RegGP r(m_block);

		if( write )
		{
			decode_dddddd_read(r32(r.get()), dddddd );
			m_block.mem().writePeriph(_area, addr, r);
		}
		else
		{
			m_block.mem().readPeriph(r,_area, addr, Inst);
			decode_dddddd_write( dddddd, r32(r.get()));
		}
	}

	inline void JitOps::op_Movep_SXqq(TWord op)
	{
		movep_sqq<Movep_SXqq>(op, MemArea_X);
	}

	inline void JitOps::op_Movep_SYqq(TWord op)
	{
		movep_sqq<Movep_SYqq>(op, MemArea_Y);
	}

	inline void JitOps::op_Movep_Spp(TWord op)
	{
		const TWord pppppp	= getFieldValue<Movep_Spp,Field_pppppp>(op) + 0xffffc0;
		const TWord dddddd	= getFieldValue<Movep_Spp,Field_dddddd>(op);
		const EMemArea area = getFieldValue<Movep_Spp,Field_s>(op) ? MemArea_Y : MemArea_X;
		const auto	write	= getFieldValue<Movep_Spp,Field_W>(op);

		const RegGP r(m_block);

		if( write )
		{
			decode_dddddd_read(r32(r.get()), dddddd );
			m_block.mem().writePeriph(area, pppppp, r);
		}
		else
		{
			m_block.mem().readPeriph(r, area, pppppp, Movep_Spp);
			decode_dddddd_write(dddddd, r32(r.get()));
		}
	}
}
