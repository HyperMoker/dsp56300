// DSP 56362 ESAI - Enhanced Serial AUDIO Interface

#pragma once

#include "esxi.h"
#include "logging.h"
#include "bitfield.h"

namespace dsp56k
{
	class EsxiClock;
	class Dma;
	class Disassembler;
	class IPeripherals;
	class DSP;

	class Esai : public Esxi
	{
	public:
		enum AddressesX
		{
			M_PCRC	= 0xFFFFBF, // Port C GPIO Control Register
			M_PRRC	= 0xFFFFBE, // Port C Direction Register
			M_PDRC	= 0xFFFFBD, // Port C GPIO Data Register
			M_RSMB	= 0xFFFFBC, // ESAI Receive Slot Mask Register B (RSMB)
			M_RSMA	= 0xFFFFBB, // ESAI Receive Slot Mask Register A (RSMA)
			M_TSMB	= 0xFFFFBA, // ESAI Transmit Slot Mask Register B (TSMB)
			M_TSMA	= 0xFFFFB9, // ESAI Transmit Slot Mask Register A (TSMA)
			M_RCCR	= 0xFFFFB8, // ESAI Receive Clock Control Register (RCCR)
			M_RCR	= 0xFFFFB7, // ESAI Receive Control Register (RCR)
			M_TCCR	= 0xFFFFB6, // ESAI Transmit Clock Control Register (TCCR)
			M_TCR	= 0xFFFFB5, // ESAI Transmit Control Register (TCR)
			M_SAICR	= 0xFFFFB4, // ESAI Control Register (SAICR)
			M_SAISR	= 0xFFFFB3, // ESAI Status Register (SAISR)

			M_RX3	= 0xFFFFAB, // ESAI Receive Data Register 3 (RX3)
			M_RX2	= 0xFFFFAA, // ESAI Receive Data Register 2 (RX2)
			M_RX1	= 0xFFFFA9, // ESAI Receive Data Register 1 (RX1)
			M_RX0	= 0xFFFFA8, // ESAI Receive Data Register 0 (RX0)
			M_TSR	= 0xFFFFA6, // ESAI Time Slot Register (TSR)
			M_TX5	= 0xFFFFA5, // ESAI Transmit Data Register 5 (TX5)
			M_TX4	= 0xFFFFA4, // ESAI Transmit Data Register 4 (TX4)
			M_TX3	= 0xFFFFA3, // ESAI Transmit Data Register 3 (TX3)
			M_TX2	= 0xFFFFA2, // ESAI Transmit Data Register 2 (TX2)
			M_TX1	= 0xFFFFA1, // ESAI Transmit Data Register 1 (TX1)
			M_TX0	= 0xFFFFA0, // ESAI Transmit Data Register 0 (TX0)
		};

		enum AddressesY
		{
			M_RSMB_1  = 0xFFFF9C, // ESAI_1 Receive Slot Mask Register B (RSMB)
			M_RSMA_1  = 0xFFFF9B, // ESAI_1 Receive Slot Mask Register A (RSMA)
			M_TSMB_1  = 0xFFFF9A, // ESAI_1 Transmit Slot Mask Register B (TSMB)
			M_TSMA_1  = 0xFFFF99, // ESAI_1 Transmit Slot Mask Register A (TSMA)
			M_RCCR_1  = 0xFFFF98, // ESAI_1 Receive Clock Control Register (RCCR)
			M_RCR_1   = 0xFFFF97, // ESAI_1 Receive Control Register (RCR)
			M_TCCR_1  = 0xFFFF96, // ESAI_1 Transmit Clock Control Register (TCCR)
			M_TCR_1   = 0xFFFF95, // ESAI_1 Transmit Control Register (TCR)
			M_SAICR_1 = 0xFFFF94, // ESAI_1 Control Register (SAICR)
			M_SAISR_1 = 0xFFFF93, // ESAI_1 Status Register (SAISR)
			M_RX3_1   = 0xFFFF8B, // ESAI_1 Receive Data Register 3 (RX3)
			M_RX2_1   = 0xFFFF8A, // ESAI_1 Receive Data Register 2 (RX2)
			M_RX1_1   = 0xFFFF89, // ESAI_1 Receive Data Register 1 (RX1)
			M_RX0_1   = 0xFFFF88, // ESAI_1 Receive Data Register 0 (RX0)
			M_TSR_1   = 0xFFFF86, // ESAI_1 Time Slot Register (TSR)
			M_TX5_1   = 0xFFFF85, // ESAI_1 Transmit Data Register 5 (TX5)
			M_TX4_1   = 0xFFFF84, // ESAI_1 Transmit Data Register 4 (TX4)
			M_TX3_1   = 0xFFFF83, // ESAI_1 Transmit Data Register 3 (TX3)
			M_TX2_1   = 0xFFFF82, // ESAI_1 Transmit Data Register 2 (TX2)
			M_TX1_1   = 0xFFFF81, // ESAI_1 Transmit Data Register 1 (TX1)
			M_TX0_1   = 0xFFFF80, // ESAI_1 Transmit Data Register 0 (TX0)
		};

		enum RsbmBits
		{
			// RSMB Register bits
			M_RS31 = 15,	M_RS30 = 14,	M_RS29 = 13,	M_RS28 = 12,	M_RS27 = 11,	M_RS26 = 10,	M_RS25 = 9,		M_RS24 = 8,
			M_RS23 = 7,		M_RS22 = 6,		M_RS21 = 5,		M_RS20 = 4,		M_RS19 = 3,		M_RS18 = 2,		M_RS17 = 1,		M_RS16 = 0,
		};

		enum RsmaBits
		{
			// RSMA Register bits
			M_RS15 = 15,	M_RS14 = 14,	M_RS13 = 13,	M_RS12 = 12,	M_RS11 = 11,	M_RS10 = 10,	M_RS9 = 9,		M_RS8 = 8,
			M_RS7 = 7,		M_RS6 = 6,		M_RS5 = 5,		M_RS4 = 4,		M_RS3 = 3,		M_RS2 = 2,		M_RS1 = 1,		M_RS0 = 0,
		};

		enum TsmbBits
		{
			// TSMB Register bits
			M_TS31 = 15,	M_TS30 = 14,	M_TS29 = 13,	M_TS28 = 12,	M_TS27 = 11,	M_TS26 = 10,	M_TS25 = 9,		M_TS24 = 8,
			M_TS23 = 7,		M_TS22 = 6,		M_TS21 = 5,		M_TS20 = 4,		M_TS19 = 3,		M_TS18 = 2,		M_TS17 = 1,		M_TS16 = 0,
		};

		enum TsmaBits
		{
			// TSMA Register bits
			M_TS15 = 15,	M_TS14 = 14,	M_TS13 = 13,	M_TS12 = 12,	M_TS11 = 11,	M_TS10 = 10,	M_TS9 = 9,		M_TS8 = 8,
			M_TS7 = 7,		M_TS6 = 6,		M_TS5 = 5,		M_TS4 = 4,		M_TS3 = 3,		M_TS2 = 2,		M_TS1 = 1,		M_TS0 = 0,
		};

		enum RccrBits
		{
			// RCCR Register bits
			M_RHCKD = 23,				// Receiver High Frequency Clock Direction
			M_RFSD = 22,				// Receiver Frame Sync Signal Direction
			M_RCKD = 21,				// Receiver Clock Source Direction
			M_RHCKP = 20,				// Receiver High Frequency Clock Polarity
			M_RFSP = 19,				// Receiver Frame Sync Polarity
			M_RCKP = 18,				// Receiver Clock Polarity

			M_RFP = 0x3C000,			// Rx High Frequency Clock Divider Mask
			M_RFP3 = 17,				// Rx High Frequency Clock Divider Bit 3
			M_RFP2 = 16,				// Rx High Frequency Clock Divider Bit 2
			M_RFP1 = 15,				// Rx High Frequency Clock Divider Bit 1
			M_RFP0 = 14,				// Rx High Frequency Clock Divider Bit 0

			M_RDC = 0x3E00,
			M_RDC4 = 13,				// Rx Frame Rate Divider Control Bit 4
			M_RDC3 = 12,				// Rx Frame Rate Divider Control Bit 3
			M_RDC2 = 11,				// Rx Frame Rate Divider Control Bit 2
			M_RDC1 = 10,				// Rx Frame Rate Divider Control Bit 1
			M_RDC0 = 9,					// Rx Frame Rate Divider Control Bit 0
			M_RPSR = 8,					// Receiver Prescaler Range

			M_RPM = 0xFF,				// Receiver Prescale Modulus Select Mask
			M_RPM7 = 7,					// Receiver Prescale Modulus Select Bit 7
			M_RPM6 = 6,					// Receiver Prescale Modulus Select Bit 6
			M_RPM5 = 5,					// Receiver Prescale Modulus Select Bit 5
			M_RPM4 = 4,					// Receiver Prescale Modulus Select Bit 4
			M_RPM3 = 3,					// Receiver Prescale Modulus Select Bit 3
			M_RPM2 = 2,					// Receiver Prescale Modulus Select Bit 2
			M_RPM1 = 1,					// Receiver Prescale Modulus Select Bit 1
			M_RPM0 = 0,					// Receiver Prescale Modulus Select Bit 0
		};

		enum RcrBits
		{
			// RCR Register bits
			M_RLIE = 23,				// Receive Last Slot Interrupt Enable
			M_RIE = 22,					// Receive Interrupt Enable
			M_REDIE = 21,				// Receive Even Slot Data Interrupt Enable
			M_REIE = 20,				// Receive Exception Interrupt Enable
			M_RPR = 19,					// Receiver Section Personal Reset
			M_RFSR = 16,				// Receiver Frame Sync Relative Timing
			M_RFSL = 15,				// Receiver Frame Sync Length

			M_RSWS = 0x7C00,			// Receiver Slot and Word Select Mask
			M_RSWS4 = 14,				// Receiver Slot and Word Select Bit 4
			M_RSWS3 = 13,				// Receiver Slot and Word Select Bit 3
			M_RSWS2 = 12,				// Receiver Slot and Word Select Bit 2
			M_RSWS1 = 11,				// Receiver Slot and Word Select Bit 1
			M_RSWS0 = 10,				// Receiver Slot and Word Select Bit 0

			M_RMOD = 0x300,
			M_RMOD1 = 9,				// Receiver Network Mode Control Bit 1
			M_RMOD0 = 8,				// Receiver Network Mode Control Bit 0
			M_RWA = 7,					// Receiver Word Alignment Control
			M_RSHFD = 6,				// Receiver Shift Direction

			M_REM = 0xF,				// Receive Enable Mask
			M_RE3 = 3,					// Receive 3 Enable
			M_RE2 = 2,					// Receive 2 Enable
			M_RE1 = 1,					// Receive 1 Enable
			M_RE0 = 0,					// Receive 0 Enable
		};

		enum TccrBits
		{
			// TCCR Register bits
			M_THCKD = 23,				// Transmit High Frequency Clock Direction
			M_TFSD = 22,				// Transmit Frame Sync Signal Direction
			M_TCKD = 21,				// Transmit Clock Source Direction
			M_THCKP = 20,				// Transmit High Frequency Clock Polarity
			M_TFSP = 19,				// Transmit Frame Sync Polarity
			M_TCKP = 18,				// Transmit Clock Polarity

			M_TFP = 0x3C000,			// Tx High Frequency Clock Divider
			M_TFP3 = 17,				// Tx High Frequency Clock Divider Bit 3
			M_TFP2 = 16,				// Tx High Frequency Clock Divider Bit 2
			M_TFP1 = 15,				// Tx High Frequency Clock Divider Bit 1
			M_TFP0 = 14,				// Tx High Frequency Clock Divider Bit 0

			M_TDC = 0x3E00,
			M_TDC4 = 13,				// Tx Frame Rate Divider Control Bit 4
			M_TDC3 = 12,				// Tx Frame Rate Divider Control Bit 3
			M_TDC2 = 11,				// Tx Frame Rate Divider Control Bit 2
			M_TDC1 = 10,				// Tx Frame Rate Divider Control Bit 1
			M_TDC0 = 9,					// Tx Frame Rate Divider Control Bit 0
			M_TPSR = 8,					// Transmit Prescaler Range

			M_TPM = 0xFF,				// Transmit Prescale Modulus Select
			M_TPM7 = 7,					// Transmit Prescale Modulus Select Bit 7
			M_TPM6 = 6,					// Transmit Prescale Modulus Select Bit 6
			M_TPM5 = 5,					// Transmit Prescale Modulus Select Bit 5
			M_TPM4 = 4,					// Transmit Prescale Modulus Select Bit 4
			M_TPM3 = 3,					// Transmit Prescale Modulus Select Bit 3
			M_TPM2 = 2,					// Transmit Prescale Modulus Select Bit 2
			M_TPM1 = 1,					// Transmit Prescale Modulus Select Bit 1
			M_TPM0 = 0,					// Transmit Prescale Modulus Select Bit 0
		};

		enum TcrBits
		{
			// TCR Register bits
			M_TLIE = 23,				// Transmit Last Slot Interrupt Enable
			M_TIE = 22,					// Transmit Interrupt Enable
			M_TEDIE = 21,				// Transmit Even Slot Data Interrupt Enable
			M_TEIE = 20,				// Transmit Exception Interrupt Enable
			M_TPR = 19,					// Transmit Section Personal Reset
			M_PADC = 17,				// Transmit Zero Padding Control
			M_TFSR = 16,				// Transmit Frame Sync Relative Timing
			M_TFSL = 15,				// Transmit Frame Sync Length

			M_TSWS = 0x7C00,			// Tx Slot and Word Length Select Mask
			M_TSWS4 = 14,				// Tx Slot and Word Length Select Mask 4
			M_TSWS3 = 13,				// Tx Slot and Word Length Select Mask 3
			M_TSWS2 = 12,				// Tx Slot and Word Length Select Mask 2
			M_TSWS1 = 11,				// Tx Slot and Word Length Select Mask 1
			M_TSWS0 = 10,				// Tx Slot and Word Length Select Mask 0

			M_TMOD = 0x300,
			M_TMOD1 = 9,				// Transmit Network Mode Control
			M_TMOD0 = 8,				// Transmit Network Mode Control
			M_TWA = 7,					// Transmit Word Alignment
			M_TSHFD = 6,				// Transmit Shift Direction

			M_TEM = 0x3F,				// Transmit Enable Mask
			M_TE5 = 5,					// Transmit 5 Enable
			M_TE4 = 4,					// Transmit 4 Enable
			M_TE3 = 3,					// Transmit 3 Enable
			M_TE2 = 2,					// Transmit 2 Enable
			M_TE1 = 1,					// Transmit 1 Enable
			M_TE0 = 0,					// Transmit 0 Enable
		};

		enum CrBits
		{
			// control bits of SAICR
			M_ALC = 8,					// Alignment Control
			M_TEBE = 7,					// Transmit External Buffer Enable
			M_SYN = 6,					// Synchronous Mode Selection
			M_OF2 = 2,					// Serial Output Flag 2
			M_OF1 = 1,					// Serial Output Flag 1
			M_OF0 = 0,					// Serial Output Flag 0
		};

		enum SrBits
		{
			// status bits of SAISR
			M_TODE = 17,				// Transmit Odd-Data Register Empty
			M_TEDE = 16,				// Transmit Even-Data Register Empty
			M_TDE = 15,					// Transmit Data Register Empty
			M_TUE = 14,					// Transmit Underrun Error Flag
			M_TFS = 13,					// Transmit Frame Sync Flag
			M_RODF = 10,				// Receive Odd-Data Register Full
			M_REDF = 9,					// Receive Even-Data Register Full
			M_RDF = 8,					// Receive Data Register Full
			M_ROE = 7,					// Receiver Overrun Error Flag
			M_RFS = 6,					// Receive Frame Sync Flag
			M_IF2 = 2,					// Serial Input Flag 2
			M_IF1 = 1,					// Serial Input Flag 1
			M_IF0 = 0,					// Serial Input Flag 0
		};

		explicit Esai(IPeripherals& _periph, EMemArea _area, Dma* _dma = nullptr);

		void setDSP(DSP* _dsp);

		void execTX() override;
		void execRX() override;

		TWord hasEnabledTransmitters() const override { return getEnabledTransmitters(); }
		TWord hasEnabledReceivers() const override { return getEnabledReceivers(); }

		uint32_t getTransmitFrameSync() const
		{
			return bittest<TWord, M_TFS>(readStatusRegister());
		}

		uint32_t getReceiveFrameSync() const
		{
			return bittest<TWord, M_RFS>(readStatusRegister());
		}

		const TWord& readStatusRegister() const;

		void writestatusRegister(TWord _val)
		{
//			LOG("Write ESAI SR " << HEX(_val));
			m_sr = _val;
		}

		TWord readReceiveControlRegister() const
		{
			return m_rcr;
		}
		
		TWord readReceiveClockControlRegister() const
		{
			return m_rccr;
		}
		
		TWord readTransmitControlRegister() const
		{
			return m_tcr;
		}

		TWord readTransmitClockControlRegister() const
		{
			return m_tccr;
		}

		void writeReceiveControlRegister(TWord _val);

		void writeTransmitControlRegister(TWord _val);

		void writeTransmitClockControlRegister(TWord _val);

		void writeControlRegister(TWord _val)
		{
			LOG("Write ESAI CR " << HEX(_val));
			m_cr = _val;
		}

		void writeReceiveClockControlRegister(TWord _val);

		void writeTX(uint32_t _index, TWord _val);
		TWord readRX(uint32_t _index);

		TWord readTSMA() const
		{
			return m_tsma;
		}

		TWord readTSMB() const
		{
			return m_tsmb;
		}

		void writeTSMA(TWord _tsma);
		void writeTSMB(TWord _tsmb);

		static void setSymbols(Disassembler& _disasm, EMemArea _area);

		EMemArea getMemArea() const { return m_area; }

		uint32_t getTxFrameCounter() const { return m_txFrameCounter; }

		uint32_t getTxWordCount() const
		{
			return (m_tccr & M_TDC) >> M_TDC0;
		}

		uint32_t getRxWordCount() const
		{
			return (m_rccr & M_RDC) >> M_RDC0;
		}

		uint32_t getTxClockDivider() const
		{
			return (m_tccr & M_TFP) >> M_TFP0;
		}

		uint32_t getRxClockDivider() const
		{
			return (m_rccr & M_RFP) >> M_RFP0;
		}

		uint32_t getTxClockPrescale() const
		{
			return (m_tccr & M_TPM) >> M_TPM0;
		}

		uint32_t getRxClockPrescale() const
		{
			return (m_rccr & M_RPM) >> M_RPM0;
		}

		// divide by 1 (true) or 8 (false)
		bool getTxClockPrescalerRange() const
		{
			return (m_tccr & M_TPSR) != 0;
		}

		// divide by 1 (true) or 8 (false)
		bool getRxClockPrescalerRange() const
		{
			return (m_rccr & M_RPSR) != 0;
		}

		TWord getEnabledReceivers() const
		{
			return m_rcr & M_REM;
		}

		TWord getEnabledTransmitters() const
		{
			return m_tcr & M_TEM;
		}

		std::string getTccrAsString() const;
		std::string getRccrAsString() const;

		std::string getTcrAsString() const;
		std::string getRcrAsString() const;

		const auto& getSR() const { return m_sr; }

	private:
		bool inputEnabled(uint32_t _index) const	{ return m_rcr.test(static_cast<RcrBits>(_index)); }
		bool outputEnabled(uint32_t _index) const	{ return m_tcr.test(static_cast<TcrBits>(_index)); }

		void injectInterrupt(TWord _interrupt) const;
		void readSlotFromFrame();
		void writeSlotToFrame();

		IPeripherals& m_periph;
		const EMemArea m_area;
		const TWord m_vba;							// base address for interrupts differs between ESAI and ESAI_1 (on DSP 56367)
		Dma* const m_dma;
		Bitfield<uint32_t, SrBits, 18> m_sr;		// status register
		TWord m_cr = 0;								// control register

		Bitfield<uint32_t, TcrBits, 24> m_tcr;		// transmit control register
		Bitfield<uint32_t, RcrBits, 24> m_rcr;		// receive control register

		TWord m_rccr = 0;							// receive clock control register
		TWord m_tccr = 0;							// transmit clock control register

		TxSlot m_tx;								// Words written by the DSP 
		RxSlot m_rx;								// Words for the DSP to read

		TxFrame m_txFrame;
		RxFrame m_rxFrame;
		
		uint32_t m_writtenTX = 0;
		uint32_t m_readRX = 0;
		uint32_t m_txSlotCounter = 0;
		uint32_t m_txFrameCounter = 0;
		uint32_t m_rxSlotCounter = 0;
		uint32_t m_rxFrameCounter = 0;

		TWord m_tsma = 0xffff;
		TWord m_tsmb = 0xffff;

		TWord m_vbaRead = 0;
	};
}
