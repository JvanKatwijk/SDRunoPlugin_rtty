#pragma once

#include <thread>
#include <mutex>
#include <atomic>
#include <iunoplugincontroller.h>
#include <iunoplugin.h>
#include <iunostreamobserver.h>
#include <iunoaudioobserver.h>
//#include <iunoaudioprocessor.h>
#include <iunostreamobserver.h>
#include <iunoannotator.h>
//
#include "SDRunoPlugin_rttyUi.h"
//
//	for the payload we have
class	upFilter;
#include	"ringbuffer.h"
#include	<stdint.h>
#include	"rtty-shifter.h"
#include	"decimator-25.h"
#include	"rtty-bandfilter.h"
class		rttyAverage;
/*
 *      defines, local for the rttyDetector stuff
 */
#define RTTY_RX_STATE_IDLE      0
#define RTTY_RX_STATE_START     1
#define RTTY_RX_STATE_DATA      2
#define RTTY_RX_STATE_PARITY    3
#define RTTY_RX_STATE_STOP      4
#define RTTY_RX_STATE_STOP2     5

#define BAUDOT_LETS             0
#define BAUDOT_FIGS             1

#define         RTTY_IF         0
#define         DECIMATOR       5
#define         INRATE          (2000000 / 32)
#define         INTERM_RATE     (INRATE / DECIMATOR)
#define         WORKING_RATE    12000

class SDRunoPlugin_rtty : public IUnoPlugin,
	                             public IUnoStreamProcessor,
	                             public IUnoAudioProcessor {
public:
		SDRunoPlugin_rtty (IUnoPluginController& controller);
	virtual ~SDRunoPlugin_rtty ();

	virtual const
	char*	GetPluginName() const override { return "rtty decoder"; }
	virtual
	void	StreamProcessorProcess (channel_t channel,
	                                Complex *buffer,
	                                int length,
	                                bool& modified) override;
        virtual
	void	AudioProcessorProcess (channel_t channel,
	                               float *buffer,
	                               int length, bool& modified) override;

	// IUnoPlugin
	virtual
	void	HandleEvent(const UnoEvent& ev) override;
	void    set_rttyAfcon		(const std::string&);
	void    set_rttyReverse		(const std::string&);
	void    set_rttyShift		(const std::string&);
	void    set_rttyBaudrate	(const std::string&);
	void    set_rttyParity		(const std::string&);
	void    set_rttyMsb		(const std::string&);
	void    set_rttyNbits		(const std::string&);
	void    set_rttyStopbits	(const std::string&);
	enum Parities {
		RTTY_PARITY_NONE = 0,
		RTTY_PARITY_EVEN = 1,
		RTTY_PARITY_ODD = 2,
		RTTY_PARITY_ZERO = 3,
		RTTY_PARITY_ONE = 4
	};
private:
	std::mutex              m_lock;
	SDRunoPlugin_rttyUi     m_form;
	std::mutex		locker;
	IUnoPluginController	*m_controller;
	RingBuffer<Complex>	rttyBuffer;
	rttyShifter		theMixer;
	rttyBandfilter		passbandFilter;
	decimator_25		theDecimator;
	rttyShifter		localShifter;
	RingBuffer<float>	rttyAudioBuffer;
	std::vector<std::complex<float>> rttyToneBuffer;
        std::vector<std::complex<float>> convBuffer;
        int                     convIndex;
        int16_t                 mapTable_int   [WORKING_RATE / 100];
        float                   mapTable_float [WORKING_RATE / 100];

	int			rttyTonePhase;
	int			SDRplay_Rate;
	bool			rttyError;
	void			WorkerFunction		();
	void			testInput		(std::complex<float>);
	void			process			(std::complex<float>);
	int			resample		(std::complex<float>,
	                                                 std::complex<float> *);
	void			processSample		(std::complex <float>);
	void			rtty_setup		();
	std::thread* 		m_worker;
	std::atomic<bool>	running;
	int			centerFrequency;
	int			selectedFrequency;

	void	                rtty_setReverse		(bool);
	uint32_t	        reverseBits		(uint32_t, uint32_t);
	unsigned char		pickup_char		(void);

	uint8_t	                check_parity		(uint32_t,
	                                         uint32_t,  unsigned char);
	uint8_t		        odd			(uint32_t);
	void	                check_baudrate		(int16_t, int16_t);
	double	                adjust_IF		(double);
	unsigned char	addrttyBitFragment	(void);
	unsigned char	BaudottoASCII		(unsigned char);

	double		rttyS2N			(void);
	rttyAverage	*rttyAverager;
	int16_t         rttyCycleCount;
	unsigned char	rttyBitFragment;
	int16_t         rttyPoscnt;
	int16_t         rttyNegcnt;
	double          rttyPosFreq;
	double          rttyNegFreq;
	double          rttyFreqError;
	std::complex<float>	rttyPrevsample;
	rttyBandfilter	*BPM_Filter;
	upFilter	*audioFilter;
	int		rttyAudioRate;
	bool		rttyAfcon;
	double          rttyShift;
	double		rttyIF;
	int16_t		rttySymbolLength;
	int16_t		rttyNbits;
	unsigned char	rttyParity;
	int16_t         rttyStopbits;
	bool            rttyMsb;
	int16_t         rttyStoplen;
	bool            rttyReversed;
	int16_t         rttyFilterDegree;
	int16_t         rttyPhaseacc;
	int16_t         rttyState;
	int16_t         rttyCounter;
	int16_t         rttyBitcntr;
	int16_t         rttyRxdata;
	unsigned char	rttyRxmode;
	double          rttyBaudrate;
	int16_t         rttyPluscnt;
	int16_t         rttyMincnt;
	int16_t         rttyPrevfragment;
	std::string     rttyText;
	double          rttySignalPower;
	double          rttyNoisePower;
//
//	former signals, now handled locally
	void            rtty_showStrength	(float);
	void		rtty_showFreqCorrection	(float);
	void		rtty_showBaudRate	(int);
	void		rtty_showGuess		(int);
	void		rtty_clrText		();
	void		rtty_addText		(char);
};
