
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
on the spectrum display an rtty signal is visible, tuning is, however,
difficult. Most amateur transmissions are very short, and on the spectrum 
display one really cannot see whether or not the tuned frequency is
more or less in the middle between mark and space.

Here, as well as for other signals with a small footprint - the additional
spectrum display, showing only a fraction of the band, is very useful,
especially when the zoom function is used.


The implementation of the decoder is taken from the rtty decoder of the
swradio-8 implementation.





