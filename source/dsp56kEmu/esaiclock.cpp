#include "esaiclock.h"

#include <ios>

#include "dsp.h"
#include "logging.h"

#include "peripherals.h"

namespace dsp56k
{
	void EsaiClock::exec()
	{
		const auto clock = m_periph.getDSP().getInstructionCounter();
		const auto diff = delta(clock, m_lastClock);
		m_lastClock = clock;

		m_cyclesSinceWrite += diff;

		if(m_cyclesSinceWrite < m_cyclesPerSample)
			return;

		m_cyclesSinceWrite -= m_cyclesPerSample;

		auto advanceClock = [](Clock& _c)
		{
			if (++_c.counter > _c.divider)
			{
				_c.counter = 0;
				return true;
			}
			return false;
		};

		for (auto& e : m_esais)
		{
			if(advanceClock(e.tx))
				m_esaisProcessTX.push_back(e.esai);

			if (advanceClock(e.rx))
				m_esaisProcessRX.push_back(e.esai);
		}
#if 0
		for(size_t i=0; i<std::max(m_esaisProcessRX.size(), m_esaisProcessTX.size()); ++i)
		{
			if(i < m_esaisProcessTX.size())
				m_esaisProcessTX[i]->execTX();
			if (i < m_esaisProcessRX.size())
				m_esaisProcessRX[i]->execRX();
		}
#else
		for (auto* e : m_esaisProcessTX)	e->execTX();
		for (auto* e : m_esaisProcessRX)	e->execRX();
#endif
		m_esaisProcessTX.clear();
		m_esaisProcessRX.clear();
	}

	void EsaiClock::setPCTL(const TWord _val)
	{
		if(m_pctl == _val)
			return;

		m_pctl = _val;

		updateCyclesPerSample();
	}

	void EsaiClock::setSamplerate(const uint32_t _samplerate)
	{
		if (m_samplerate == _samplerate)
			return;

		m_samplerate = _samplerate;

		updateCyclesPerSample();
	}

	void EsaiClock::setCyclesPerSample(const uint32_t _cyclesPerSample)
	{
		if(m_fixedCyclesPerSample == _cyclesPerSample)
			return;

		m_fixedCyclesPerSample = _cyclesPerSample;

		updateCyclesPerSample();
	}

	void EsaiClock::setExternalClockFrequency(const uint32_t _freq)
	{
		if(_freq == m_externalClockFrequency)
			return;

		m_externalClockFrequency = _freq;
		updateCyclesPerSample();
	}

	void EsaiClock::updateCyclesPerSample()
	{
		if(m_fixedCyclesPerSample)
		{
			m_cyclesPerSample = m_fixedCyclesPerSample;
		}
		else
		{
			if(!m_pctl)
				return;

			const auto pd = ((m_pctl >> 20) & 15) + 1;
			const auto df = 1 << ((m_pctl >> 12) & 3);
			const auto mf = (m_pctl & 0xfff) + 1;

			const auto speedHz = static_cast<uint64_t>(m_externalClockFrequency) * static_cast<uint64_t>(mf) / (static_cast<uint64_t>(pd) * static_cast<uint64_t>(df));

			if (m_samplerate)
				m_cyclesPerSample = static_cast<uint32_t>((speedHz / m_samplerate) >> 1);	// 2 samples = 1 frame (stereo)
			else
				m_cyclesPerSample = mf * 128 / pd;			// The ratio between external clock and sample period simplifies to this.

			// A more full expression would be m_cyclesPerSample = dsp_frequency / samplerate, where
			// dsp_frequency = m_extClock * mf / pd and samplerate = m_extClock/256

			const auto speedMhz = static_cast<double>(speedHz) / 1000000.0f;
			LOG("Clock speed changed to: " << speedMhz << " Mhz, EXTAL=" << m_externalClockFrequency << " Hz, PCTL=" << HEX(m_pctl) << ", mf=" << HEX(mf) << ", pd=" << HEX(pd) << ", df=" << HEX(df) << " => cycles per sample=" << std::dec << m_cyclesPerSample << ", predefined samplerate=" << m_samplerate);
		}

		m_cyclesSinceWrite = 0;
	}

	void EsaiClock::setEsaiDivider(Esai* _esai, const TWord _dividerTX, const TWord _dividerRX)
	{
		bool found = false;

		for (auto& entry : m_esais)
		{
			if(entry.esai == _esai)
			{
				if(entry.tx.divider == _dividerTX && entry.rx.divider == _dividerRX)
					return;

				entry.tx.divider = _dividerTX;
				entry.rx.divider = _dividerRX;
				found = true;
				break;
			}
		}

		if(!found)
		{
			m_esais.emplace_back(EsaiEntry{ _esai, {_dividerTX}, {_dividerRX} });
			m_esaisProcessRX.reserve(m_esais.size());
			m_esaisProcessTX.reserve(m_esais.size());
			_esai->setClockSource(this);
		}
	}

	bool EsaiClock::setEsaiCounter(const Esai* _esai, const TWord _counterTX, const TWord _counterRX)
	{
		for (auto& esai : m_esais)
		{
			if(esai.esai == _esai)
			{
				esai.tx.counter = _counterTX;
				esai.rx.counter = _counterRX;
				return true;
			}
		}
		return false;
	}

	TWord EsaiClock::getRemainingInstructionsForFrameSync(const TWord _expectedBitValue) const
	{
		if (static_cast<bool>(bittest<TWord, Esai::M_TFS>(m_esais.front().esai->readStatusRegister())) == static_cast<bool>(_expectedBitValue))
		{
			// already reached the desired value
			return 0;
		}

		constexpr TWord offset = 1;

		if (m_cyclesSinceWrite >= m_cyclesPerSample - offset)
			return 0;

		const auto periphCycles = m_periph.getDSP().getRemainingPeripheralsCycles();

		if(periphCycles < offset)
			return 0;

		const auto diff = m_cyclesPerSample - m_cyclesSinceWrite - offset;

		return std::min(diff, periphCycles - offset);
	}

	void EsaiClock::onTCCRChanged(Esai*)
	{
//		for (auto& esai : m_esais)
//			esai.clockCounter = 0;
	}
}
