
-----------------------------------------------------------------
Simple rtty decoder plugin
-----------------------------------------------------------------
The rty decoder for SDRuno is - as the name suggests - a plugin for
decoding rtty signals transmitted on shortwave

![overview](/rtty-example.png?raw=true)

-----------------------------------------------------------------------------
Installing the plugin  READ THIS FIRST
-----------------------------------------------------------------------------

Since rtty is a small band signal (for amateur modes 170 Hzz in this plugin),
the samplerate used as input for the plugin is *62500* samples/second.

**On the main widget select samplerate 2000000, and decimation factor 32**.

![overview](/drm-main-widget.png?raw=true)

The plugin itself can be stored in the folder for community plugins

The plugin is - as other plugins - developed under MSVC. Its functioning
depends on lots of other "dll's" (Dynamic Load Libraries);

If a 0x000012f error is encountered on trying to load the plugin,
it means that dll's from the Visual C++ Redistributable(VS 2015) are
not found.

-----------------------------------------------------------------------
RTTY
-----------------------------------------------------------------------

RTTY can be heard (a.o) around 14080 Khz. The signal is a phase shift keying
signal, a signal a mark and a space signal.
While on e.g. 4583 these is a signal with nautical information with
a shift of 850 Hz between the mark and the space signal, signals from
amateurs ususally have a shift of 170 Hz.

As seen, the plugin has a number of comboboxes with which shift, baudrate etc
can be set, default settings are for the signal aas appearing in the
amateur bands.

Tuning to amateur signals, e.g. around 14085KHz, requires some training.
The large spectrum display is 62.5 KHz, the shift of most rtty transmissions
in amateur bands is 170 Hz, so while one can - usually clearly - see
the signal on the big screen, tuning to app the middle between the
mark and space signal is not always easy.
Happily, the SDRuno platform provides a "zoom" function on the
main spectrum display, and a
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

