#include "dsp.h"
#include "interrupts.h"
#include "hdi08.h"

namespace dsp56k
{
	void HDI08::exec()
	{
		if (!bittest(m_hpcr, HPCR_HEN)) 
			return;

		if (m_pendingRXInterrupts > 0 && bittest(m_hcr, HCR_HRIE))
		{
			--m_pendingRXInterrupts;
			m_periph.getDSP().injectInterrupt(Vba_Host_Receive_Data_Full);
		}
		else if (bittest(m_hcr, HCR_HTIE) && m_pendingTXInterrupts > 0)
		{
			--m_pendingTXInterrupts;
			m_periph.getDSP().injectInterrupt(Vba_Host_Transmit_Data_Empty);
		}
	}

	TWord HDI08::readRX(Instruction _inst)
	{
		if (m_data.empty())
		{
			LOG("Empty read");
			return 0;
		}

		TWord res;

		switch (_inst)
		{
		case Btst_pp:
		case Btst_D:
		case Btst_qq:
		case Btst_ea:
		case Btst_aa:
			res = m_data.front();
			break;
		default:
			res = m_data.pop_front();
			break;
		}

		return res;
	}

	void HDI08::writeRX(const TWord* _data, const size_t _count)
	{
		for (size_t i = 0; i < _count; ++i)
		{
			m_data.waitNotFull();
			m_data.push_back(_data[i] & 0x00ffffff);
			if (bittest(m_hpcr, HPCR_HEN) && bittest(m_hcr, HCR_HRIE)) {
				++m_pendingRXInterrupts;
			}
		}
	}

	void HDI08::clearRX()
	{
		m_data.clear();
	}

	void HDI08::setHostFlags(const char _flag0, const char _flag1)
	{
		dsp56k::bitset<TWord, HSR_HF0>(m_hsr, _flag0);
		dsp56k::bitset<TWord, HSR_HF1>(m_hsr, _flag1);
		LOG("Write HostFlags, HSR " << HEX(m_hsr));
	}

	bool HDI08::dataRXFull() const
	{
		return m_data.full();
	}

	void HDI08::terminate()
	{
		while(!m_data.full())
			m_data.push_back(0);
	}

	bool HDI08::hasTX() const
	{
		return !m_dataTX.empty();
	}

	uint32_t HDI08::readTX()
	{
		m_dataTX.waitNotEmpty();
		return m_dataTX.pop_front();
	}

	void HDI08::writeTX(TWord _val)
	{
		m_dataTX.waitNotFull();
		m_dataTX.push_back(_val);
		//LOG("Write HDI08 HOTX " << HEX(_val));
		++m_pendingTXInterrupts;
	}

	void HDI08::writeControlRegister(TWord _val)
	{
		//LOG("Write HDI08 HCR " << HEX(_val));
		m_hcr = _val;
	}
};
