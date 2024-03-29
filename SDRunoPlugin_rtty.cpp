#include	<sstream>
#include	<unoevent.h>
#include	<iunoplugincontroller.h>
#include	<vector>
#include	<sstream>
#include	<chrono>
#include	<Windows.h>
#include	<mutex>
#include	"SDRunoPlugin_rtty.h"
#include	"SDRunoPlugin_rttyUi.h"

//
//	rtty specifics

#include	"rtty-bandfilter.h"
#include	"rtty-shifter.h"
#include	"utilities.h"
#include	"up-filter.h"

#define  _USE_MATH_DEFINES
#include	<math.h>
//
SDRunoPlugin_rtty::
	     SDRunoPlugin_rtty (IUnoPluginController& controller) :
	                                         IUnoPlugin  (controller),
	                                         m_form   (*this, controller),
	                                         m_worker (nullptr),
	                                         rttyBuffer (128 * 32768),
	                                         passbandFilter (11,
	                                                         -1000,
	                                                         +1000,
	                                                         INRATE),
	                                         theDecimator (INRATE / WORKING_RATE ),
	                                         rttyAudioBuffer (16 * 32768),
	                                         rttyToneBuffer (192) {
	m_controller		= &controller;
	running. store (false);

	rttyPrevsample          = std::complex<float> (0, 0);
	rttyCycleCount          = 0;
	rttyIF                  = RTTY_IF;
	rttyStopbits            = 1;
	rttyBaudrate            = 45;
	rttyShift               = 170;
	rttyParity              = RTTY_PARITY_NONE;
	rttySymbolLength        = (int)(WORKING_RATE / rttyBaudrate + 0.5);
	rttyStoplen             = (int)(WORKING_RATE / rttyBaudrate + 0.5);
	rttyAverager		= new rttyAverage (rttySymbolLength / 3);
	rttyReversed            = false;

	rttyNbits               = 5;
	rttyAfcon               = false;
	rttyMsb                 = false;
	rttyFilterDegree	= 9;
	BPM_Filter		= new rttyBandfilter (rttyFilterDegree,
	                                              RTTY_IF - rttyBaudrate,
	                                              RTTY_IF + rttyBaudrate,
	                                              WORKING_RATE);

	rttyState               = RTTY_RX_STATE_IDLE;
	rttyPluscnt             = 0;
	rttyMincnt              = 0;
	rttyPrevfragment        = 0;
	rttySignalPower         = 0;
	rttyNoisePower          = 0;
	rttyPoscnt              = 0;
	rttyNegcnt              = 0;
	rttyPosFreq             = 0.0;
	rttyNegFreq             = 0.0;
	rttyFreqError           = 0.0;

	rttyTonePhase	= 0;
//	m_controller	-> RegisterStreamProcessor (0, this);
	m_controller	-> RegisterAudioProcessor (0, this);
	m_controller	-> SetDemodulatorType (0,
	                         IUnoPluginController::DemodulatorIQOUT);
        rttyAudioRate	= m_controller -> GetAudioSampleRate (0);
        rttyError         = false;
	rttyToneBuffer. resize (rttyAudioRate);
	for (int i = 0; i < rttyAudioRate; i ++) {
	   float term = (float)i / rttyAudioRate * 2 * M_PI;
	   rttyToneBuffer [i] = std::complex<float> (cos (term), sin (term));
	}
	audioFilter     = new upFilter (25, WORKING_RATE, rttyAudioRate);
	m_worker        = new std::thread (&SDRunoPlugin_rtty::WorkerFunction,
	                                                               this);
}

	SDRunoPlugin_rtty::~SDRunoPlugin_rtty () {
	running. store (false);
	m_worker	-> join ();
	m_controller	-> UnregisterStreamProcessor (0, this);
	m_controller	-> UnregisterAudioProcessor (0, this);
	delete m_worker;
	m_worker = nullptr;
	delete	rttyAverager;
	delete	BPM_Filter;
	delete	audioFilter;
}

void	SDRunoPlugin_rtty::StreamProcessorProcess (channel_t	channel,
	                                           Complex	*buffer,
	                                           int		length,
	                                           bool		&modified) {
	(void)channel; (void)buffer;
	(void)length;
	modified = false;
}

void	SDRunoPlugin_rtty::AudioProcessorProcess (channel_t channel,
	                                          float* buffer,
	                                          int length,
	                                          bool& modified) {
//	Handling IQ input, note that SDRuno interchanges I and Q elements
	if (!modified) {
	   for (int i = 0; i < length; i++) {
	      std::complex<float> sample =
                           std::complex<float>(buffer [2 * i +  1],
                                               buffer [2 * i]);
	      sample = passbandFilter.Pass (sample);
              if (theDecimator.Pass (sample, &sample))
                 rttyBuffer.putDataIntoBuffer (&sample, 1);
           }
        }


	if (rttyAudioBuffer. GetRingBufferReadAvailable () >= length * 2) {
	   rttyAudioBuffer. getDataFromBuffer (buffer, length * 2);
	}

	modified = true;
}

void	SDRunoPlugin_rtty::HandleEvent (const UnoEvent& ev) {
	switch (ev. GetType ()) {
	   case UnoEvent::FrequencyChanged:
	      break;

	   case UnoEvent::CenterFrequencyChanged:
	      break;

	   default:
	      m_form. HandleEvent (ev);
	      break;
	}
}

#define	BUFFER_SIZE 4096
void	SDRunoPlugin_rtty::WorkerFunction () {
std::complex<float> buffer [BUFFER_SIZE];

	running. store (true);
	while (running. load ()) {
	   while (running. load () &&
	              (rttyBuffer. GetRingBufferReadAvailable () < BUFFER_SIZE))
	      Sleep (1);
	   if (!running. load ())
	      break;
	   rttyBuffer. getDataFromBuffer (buffer, BUFFER_SIZE);
	   for (int i = 0; i < BUFFER_SIZE; i++) {
	       processSample (buffer [i]);
	   }  
	}

	m_form. show_rttyText ("going down");
	Sleep(1000);
}

static inline
std::complex<float> cmul(std::complex<float> x, float y) {
        return std::complex<float>(real(x) * y, imag(x) * y);
}

//	just for some testing
float	get_db(float z) {
	return 20 * log10 ((z + 0.01) / 2048);
}
/*
 *	feed the sample here
 */
#define StaysInBand(x)  (((RTTY_IF - rttyShift / 10) < (x)) && ((x) < RTTY_IF + rttyShift / 10))
//
void SDRunoPlugin_rtty::processSample(std::complex<float> z) {
double	FreqOffset;
int	res;
std::complex<float> local_copy = z;
std::vector<std::complex<float>> tone (rttyAudioRate / WORKING_RATE);

	locker. lock ();
	z	= BPM_Filter -> Pass (z);
	locker. unlock ();

	audioFilter -> Filter (cmul (z, 20), tone. data ());
	for (int i = 0; i < tone. size (); i ++) {
	   tone [i] *= rttyToneBuffer [rttyTonePhase];
	   rttyTonePhase = (rttyTonePhase + 801) % rttyAudioRate;
	}

	rttyAudioBuffer. putDataIntoBuffer (tone. data (), tone. size () * 2);
//	z	= localShifter. do_shift (z, (int)rttyIF);

// 	then we apply slicing to end up with omega and
//	we compute the frequency offset
	FreqOffset	= arg (conj (rttyPrevsample) * z) *
	                                      WORKING_RATE / (2 * M_PI);
	rttyPrevsample	= z;

	if (FreqOffset > 0) {
	   rttyPoscnt ++;
	   rttyPosFreq += FreqOffset;
	}
	else
	if (FreqOffset < 0) {
	   rttyNegcnt ++;
	   rttyNegFreq += FreqOffset;
	}
	
	FreqOffset	= clamp (FreqOffset, - rttyShift / 2, + rttyShift / 2);
	FreqOffset	= rttyAverager -> filter (FreqOffset);

	rttyBitFragment	= (rttyReversed) ?
	                     (FreqOffset > 0.0) : (FreqOffset < 0.0);
/*
 *	Note: we were experimenting here with the approach to
 *	compute the "average values" of the mark and space
 *	frequencies
 *	Not very successful so far
 */
	res	= this -> addrttyBitFragment ();
	if (res && rttyAfcon) {
	   float FreqAdjust = this -> adjust_IF (FreqOffset);
	   if (StaysInBand (rttyIF + FreqAdjust)) 
	      rttyIF += FreqAdjust;
	}
	else 
	if (res) {
	   if ((rttyPoscnt > 200) && (rttyNegcnt > 200)) {
	      float ferr = - (rttyPosFreq / rttyPoscnt +
	                             rttyNegFreq / rttyNegcnt) / 2;
	      rttyFreqError = decayingAverage (rttyFreqError, ferr, 10);
	      rttyPoscnt = 0; rttyPosFreq = 0.0;
	      rttyNegcnt = 0; rttyNegFreq = 0.0;
	   }
	}

	if (++rttyCycleCount > WORKING_RATE) {
	   rttyCycleCount	= 0;
	   rtty_showFreqCorrection ((float)rttyFreqError);
	   rtty_showIF		((float)rttyIF);
	   rtty_showGuess      ((int)(2.0 * FreqOffset));
	}
}

void SDRunoPlugin_rtty::rtty_setup (void) {
	if (rttyStopbits == 1)
	   rttyStoplen	= (int)(WORKING_RATE / rttyBaudrate + 0.5);
	else
	if (rttyStopbits == 2)
	   rttyStoplen	= (int)(1.5 * WORKING_RATE / rttyBaudrate + 0.5);
	else
	   rttyStoplen	= (int)(2.0 * WORKING_RATE / rttyBaudrate + 0.5);

	rttyIF		= RTTY_IF;
	locker. lock ();
	delete		BPM_Filter;
	BPM_Filter	= new  rttyBandfilter (rttyFilterDegree,
	                                    RTTY_IF - (int)(3 * rttyBaudrate / 4), 
	                                    RTTY_IF + (int)(3 * rttyBaudrate / 4),
	                                    WORKING_RATE);
	rttySymbolLength	= (int)(WORKING_RATE / rttyBaudrate + 0.5);
	delete		rttyAverager;
	rttyAverager		= new rttyAverage (rttySymbolLength / 3);
	rttyState		= RTTY_RX_STATE_IDLE;
	rttyPluscnt		= 0;
	rttyMincnt		= 0;
	rttyPrevfragment	= 0;
	rttySignalPower		= 0;
	rttyNoisePower		= 0;

	rttyPoscnt		= 0;
	rttyNegcnt		= 0;
	rttyPosFreq		= 0.0;
	rttyNegFreq		= 0.0;
	rttyFreqError		= 0.0;
	locker. unlock ();
}

void SDRunoPlugin_rtty::rtty_setReverse (bool n) {
	rttyReversed = n;
	rtty_setup ();
}

uint32_t SDRunoPlugin_rtty::reverseBits (uint32_t in, uint32_t n) {
	if (n <= 1)
	   return in;
	return ((in & 01) << (n - 1)) | reverseBits (in >> 1, n - 1);
}

unsigned char SDRunoPlugin_rtty::pickup_char (void) {
uint8_t	data_mask	= (1 << rttyNbits) - 1;
uint8_t	parbit 		= (rttyRxdata >> rttyNbits) & 01;
uint8_t	data;

	if (rttyParity != RTTY_PARITY_NONE) {
	   if (parbit != check_parity (rttyRxdata, rttyNbits, rttyParity)) {
//	      fprintf (stderr, "Parity error");
	      return 0;
	   }
	}

	data	= (int)(rttyRxdata & data_mask);
	if (rttyMsb)
	   data = reverseBits (data, rttyNbits);

	if (rttyNbits == 5)
	   return BaudottoASCII (data);

	return data;
}

/*
 *	Ideally the frequency f is either -rttyShift / 2 or rttyShift / 2
 */
double SDRunoPlugin_rtty::adjust_IF (double f) {

	f	= f > 0 ? rttyShift / 2 - f : - rttyShift / 2 - f;
	if (fabs (f) < rttyShift / 2)
	   return -f / 512;

	return 0.0;
}

unsigned char SDRunoPlugin_rtty::addrttyBitFragment (void) {
uint8_t c;
uint8_t	res	= 0;

	switch (rttyState) {
	   case RTTY_RX_STATE_IDLE:
	      if (rttyBitFragment == 0) {
	         rttyState = RTTY_RX_STATE_START;
	         rttyCounter = rttySymbolLength / 2;
	      }
	      break;

	   case RTTY_RX_STATE_START:
	      if (--rttyCounter == 0) {
	         if (rttyBitFragment == 0) {	/* read the data	*/
	            rttyCounter	= rttySymbolLength;
	            rttyBitcntr	= 0;
	            rttyRxdata		= 0;
	            res			= 1;
	            rttyState		= RTTY_RX_STATE_DATA;
	         }
	         else
	            rttyState = RTTY_RX_STATE_IDLE;
	      }
	      break;

	   case RTTY_RX_STATE_DATA:
	      if (--rttyCounter <= 0) {
	         rttyRxdata	|= rttyBitFragment << rttyBitcntr;
	         rttyBitcntr ++;
	         rttyCounter	= rttySymbolLength;
	         res		= 1;
	      }

	      if (rttyBitFragment == 1)
	         rttyPluscnt ++;
	      else
	         rttyMincnt ++;
	      if (rttyBitFragment != rttyPrevfragment) {
	         check_baudrate (rttyMincnt, rttyPluscnt);
	         rttyMincnt		= 0;
	         rttyPluscnt		= 0;
	      }

	      rttyPrevfragment = rttyBitFragment;
	      if (rttyBitcntr >= rttyNbits) {
	         res = 1;
	         if (rttyParity == RTTY_PARITY_NONE)
	            rttyState = RTTY_RX_STATE_STOP;
	         else
	            rttyState = RTTY_RX_STATE_PARITY;
	      }
	      break;

	   case RTTY_RX_STATE_PARITY:
	      if (--rttyCounter == 0) {
	         rttyState = RTTY_RX_STATE_STOP;
	         rttyRxdata |= rttyBitFragment << rttyBitcntr++;
	         rttyCounter = rttyStoplen;
	      }
	      break;

	   case RTTY_RX_STATE_STOP:
	      if (--rttyCounter == 0) {
	         if (rttyBitFragment == 1) {
	            c = pickup_char();
	       rtty_addText (c);
	         }
	         else
	            c = 'F';
	         rttyState = RTTY_RX_STATE_STOP2;
	         rttyCounter = rttySymbolLength / 2;
	      }
	      break;

	   case RTTY_RX_STATE_STOP2:
	      if (--rttyCounter == 0)
	         rttyState = RTTY_RX_STATE_IDLE;
	      break;
	}

	return res;
}

static
char letters [32] = {
	'\0',	'E',	'\n',	'A',	' ',	'S',	'I',	'U',
	'\r',	'D',	'R',	'J',	'N',	'F',	'C',	'K',
	'T',	'Z',	'L',	'W',	'H',	'Y',	'P',	'Q',
	'O',	'B',	'G',	'·',	'M',	'X',	'V',	'·'
};

///*
// * ITA-2 version of the figures case.
// */
//static unsigned char figures[32] = {
//	'\0',	'3',	'\n',	'-',	' ',	'\'',	'8',	'7',
//	'\r',	'·',	'4',	'\a',	',',	'·',	':',	'(',
//	'5',	'+',	')',	'2',	'·',	'6',	'0',	'1',
//	'9',	'?',	'·',	'·',	'.',	'/',	'=',	'·'
//};
///*
// * U.S. version of the figures case.
// */
//static unsigned char figures[32] = {
//	'\0',	'3',	'\n',	'-',	' ',	'\a',	'8',	'7',
//	'\r',	'$',	'4',	'\'',	',',	'!',	':',	'(',
//	'5',	'"',	')',	'2',	'#',	'6',	'0',	'1',
//	'9',	'?',	'&',	'·',	'.',	'/',	';',	'·'
//};
/*
 * A mix of the two. This is what seems to be what people actually use.
 */
static
char figures [32] = {
	'\0',	'3',	'\n',	'-',	' ',	'\'',	'8',	'7',
	'\r',	'$',	'4',	'\a',	',',	'!',	':',	'(',
	'5',	'+',	')',	'2',	'H',	'6',	'0',	'1',
	'9',	'?',	'&',	'·',	'.',	'/',	'=',	'·'
};

unsigned char SDRunoPlugin_rtty::BaudottoASCII (unsigned char data) {
unsigned char out = 0;

	switch (data) {
	   case 0x1F:		/* letters */
	      rttyRxmode = BAUDOT_LETS;
	      break;

	   case 0x1B:		/* figures */
	      rttyRxmode = BAUDOT_FIGS;
	      break;

	   case 0x04:		/* unshift-on-space */
	      rttyRxmode = BAUDOT_LETS;
	      return ' ';
	      break;

	   default:
	      if (rttyRxmode == BAUDOT_LETS)
	         out = letters [data];
	      else
	         out = figures [data];
	      break;
	}

	return out;
}
/*
 * Parity function. Return one if `w' has odd number of ones, zero otherwise.
 */
unsigned char SDRunoPlugin_rtty::odd (uint32_t w) {
int16_t	i;
int16_t	b = 0;
	for (i = 0; w != 0; i ++) {
	   b += (w & 01);
	   w >>= +1;
	}
	return b & 01;
}

unsigned char SDRunoPlugin_rtty::check_parity (uint32_t c,
	   		   uint32_t nbits, unsigned char par) {
	c &= (1 << nbits) - 1;

	switch (par) {
	default:
	case RTTY_PARITY_NONE:
	   return 1;

	case RTTY_PARITY_ODD:
	   return odd (c);

	case RTTY_PARITY_EVEN:
	   return odd (c) == 0;

	case RTTY_PARITY_ZERO:
	   return 0;

	case RTTY_PARITY_ONE:
	   return 1;
	}
}
/*
 *	very experimental. While composing the bits of the character
 *	we count the number of samples with a given sign
 *	Obviously, this stands (if anything) for one or more
 *	bits, we'll count a while.
 */

int32_t	inrange (int32_t t, int32_t l, int32_t h) {
	return (l <= t && t <= h);
}

void SDRunoPlugin_rtty::check_baudrate (int16_t mincnt, int16_t pluscnt) {
int16_t	baudrate;

	mincnt = mincnt > pluscnt ? mincnt : pluscnt;
	if (mincnt < WORKING_RATE / 1000) /* arbitrary value */
	   return;

	baudrate = WORKING_RATE / mincnt;

	if (inrange (baudrate, 40, 47))
	   baudrate = 45;
	else
	if (inrange (baudrate, 47, 55))
	   baudrate = 50;
	else
	if (inrange (baudrate, 65, 85))
	   baudrate = 75;
	else
	if (inrange (baudrate, 90, 110))
	   baudrate = 100;
	else
	   baudrate = 0;

	if (baudrate != 0)
	   rtty_showBaudRate (baudrate);
}

double	SDRunoPlugin_rtty::rttyS2N (void) {
	return 0;
}
//
//	These functions are called from the GUI
void	SDRunoPlugin_rtty::set_rttyAfcon	(const std::string &s) {
	rttyAfcon = s == "afc on";
	rtty_setup ();
}

void	SDRunoPlugin_rtty::set_rttyReverse	(const std::string &s) {
	rtty_setReverse (s == "normal");
}

void	SDRunoPlugin_rtty::set_rttyShift	(const std::string &s) {
int16_t n       = std::stoi (s);
	rttyShift = n;
	rtty_setup ();
}

void	SDRunoPlugin_rtty::set_rttyBaudrate	(const std::string &s) {
int16_t n =  std::stoi (s);
	rttyBaudrate = n;
	rtty_setup ();
}

void	SDRunoPlugin_rtty::set_rttyParity	(const std::string &s) {
	if (s == "par none")
	   rttyParity = RTTY_PARITY_NONE;
	else
	if (s == "par one")
	   rttyParity = RTTY_PARITY_ONE;
	else
	if (s == "par odd")
	   rttyParity = RTTY_PARITY_ODD;
	else
	if (s == "par even")
	   rttyParity = RTTY_PARITY_EVEN;
	else
	if (s == "par zero")
	   rttyParity = RTTY_PARITY_ZERO;
	else
	   rttyParity = RTTY_PARITY_NONE;

	rtty_setup ();
}

void	SDRunoPlugin_rtty::set_rttyMsb	(const std::string &s) {
      rttyMsb = s == "msb true";
}

void	SDRunoPlugin_rtty::set_rttyNbits	(const std::string &s) {
int     nbits;

	nbits = (s == "8 bits") ? 8 : (s == "7 bits") ? 7 : 5;
	rttyNbits = nbits;
	rtty_setup ();
}

void	SDRunoPlugin_rtty::set_rttyStopbits	(const std::string &s) {
int16_t stopbits;

	stopbits = (s == "2 stop") ? 3 : (s == "1.5 stop") ? 2 : 1;
	rttyStopbits = stopbits;
	rtty_setup ();
}
//
//	These functions display values on the gui
//
void	SDRunoPlugin_rtty::rtty_showIF	(float v) {
	m_form. show_rttyIF ((int)v);
}

void	SDRunoPlugin_rtty::rtty_showFreqCorrection	(float v) {
	m_form. show_freqCorrection ((int)v);
}

void	SDRunoPlugin_rtty::rtty_showBaudRate		(int bd)  {
	m_form. show_rttyBaudRate (bd);
}

void	SDRunoPlugin_rtty::rtty_showGuess		(int g)	  {
	m_form. show_rttyGuess (g);
}

static	std::string textLine;
void	SDRunoPlugin_rtty::rtty_clrText			()	  {
	m_form.  show_rttyText ("");
	textLine	= "";
}

void	SDRunoPlugin_rtty::rtty_addText			(char c)  {
	if (c < ' ')
	   c = ' ';
	textLine. append (1, c);
	if (textLine. length () > 60)
	   textLine. erase (0, 1);
	m_form. show_rttyText (textLine);
}
  
