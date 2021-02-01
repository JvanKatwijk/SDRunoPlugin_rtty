#
/*
 *    Copyright (C) 2020
 *    Jan van Katwijk (J.vanKatwijk@gmail.com)
 *    Lazy Chair Computing
 *
 *    This file is part of the SDRuno rtty decoder
 *
 *    rtty decoder is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    rtty decoder is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with rtty decoder; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef	__RTTY_SHIFTER__
#define	__RTTY_SHIFTER__
#include	<complex>

class	rttyShifter {
public:
			rttyShifter	(int32_t);
			~rttyShifter	(void);
	std::complex<float>
			do_shift	(std::complex<float>, int32_t);
	void		do_shift	(std::complex<float> *, int32_t, int32_t);
private:
	int32_t		phase;
	int32_t		tableSize;
	std::complex<float>	*table;
};
#endif

