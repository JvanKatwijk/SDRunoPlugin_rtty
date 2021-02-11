
-----------------------------------------------------------------
Simple rtty decoder plugin
-----------------------------------------------------------------

![overview](/rtty-example.png?raw=true)

-----------------------------------------------------------------------
RTTY
-----------------------------------------------------------------------

RTTY can be heard (a.o) around 14080 Khz. The signal is a phase shift keying
signal, a signal a mark and a space signal.
While on e.g. 4583 these is a signal with nautical information with
a shift of 850 Hz between the mark and the space signal.

Used in amateur bands, the standard for the shift is 170 Hz - usually
with a baudrate of 45.

As seen, the plugin has a number of comboboxes with which shift, baudrate etc
can be set, default settings are for the signal aas appearing in the
amateur bands.

The plugin assumes an inputrate of 2000000 / 32 (i.e. 62.5 KHz),
Tuning to amateur signals, e.g. around 14085KHz, requires some training.
The large spectrum display is 62.5 KHz, the shift of most rtty transmissions
in amateur bands is 170 Hz, so while one can - usually clearly - see
the signal on the big screen, tuning to app the middle between the
mark and space signal is not always easy.
Happily, the SDRuno platform provides a "zoom" spectrum display, a
smaller display with a button to zoom in. As can be seen on the
picture, the zooming factor choosen was such that the spectrum showed
1.6 Khz, and the mark and space signals are clearly recognizable
around the center, i.e.the "0".

The rtty plugin has a combobox with which the afc can be set. The
picture shows that the afc is on, and the computed frequency offset
(used for correction) is 7 Hz, the remaining frequency error (the second
label) is then zero.

The easiest way to tune is by coarse tuning on the main display,
and - looking at the soom display - adapting the frequency with the
mousewheel.

While most amateur transmissions use a shift of 170 Hz and a baudrate
of 45, there are other transmissions on shortwave that use different
shift/rate pairs. On 4583 e.g. there is a transmission with some nautical
information that uses 850 Hz as shift, a baudrate of 50 and with the
mark and space signals reversed.

The implementation of the decoder is taken from the rtty decoder of the
swradio-8 implementation.

