#include "libqmath.h"


qmath1616_t qmath1616_add(qmath1616_t a, qmath1616_t b)
{
	uint32_t aa = a, bb = b;
	uint32_t sum = aa + bb;

	if (!((aa ^ bb) & 0x80000000) && ((aa ^ sum) & 0x80000000))
		return QMATH1616_OVERFLOW;

	return sum;
}


qmath1616_t qmath1616_sub(qmath1616_t a, qmath1616_t b)
{
	uint32_t aa = a, bb = b;
	uint32_t diff = aa - bb;

	if (((aa ^ bb) & 0x80000000) && ((aa ^ diff) & 0x80000000))
		return QMATH1616_OVERFLOW;

	return diff;
}


qmath1616_t qmath1616_mul(qmath1616_t inArg0, qmath1616_t inArg1)
{
	uint32_t product_lo_tmp;
	qmath1616_t result;

	int32_t A = (inArg0 >> 16), C = (inArg1 >> 16);
	uint32_t B = (inArg0 & 0xFFFF), D = (inArg1 & 0xFFFF);

	int32_t AC = A*C;
	int32_t AD_CB = A*D + C*B;
	uint32_t BD = B*D;

	int32_t product_hi = AC + (AD_CB >> 16);

	/* carry happends here - handle it */
	uint32_t ad_cb_temp = AD_CB << 16;
	uint32_t product_lo = BD + ad_cb_temp;
	if (product_lo < BD)
		product_hi++;

	if (product_hi >> 31 != product_hi >> 15)
		return QMATH1616_OVERFLOW;

	/* handle final rounding here */
	product_lo_tmp = product_lo;
	product_lo -= 0x8000;
	product_lo -= (uint32_t)product_hi >> 31;
	if (product_lo > product_lo_tmp)
		product_hi--;

	result = (product_hi << 16) | (product_lo >> 16);
	result += 1;

	return result;
}


static inline uint8_t clz(uint32_t x)
{
	uint8_t result = 0;

	if (x == 0)
		return 32;

	while (!(x & 0xF0000000))
		result += 4; x <<= 4;

	while (!(x & 0x80000000))
		result += 1; x <<= 1;

	return result;
}


qmath1616_t qmath1616_div(qmath1616_t a, qmath1616_t b)
{
	uint32_t remainder, divider, quotient;
	qmath1616_t result;
	int bit_pos;

	remainder = (a >= 0) ? a : (-a);
	divider = (b >= 0) ? b : (-b);
	quotient = 0;
	bit_pos = 17;

	if (b == 0)
		return QMATH1616_MINIMUM;

	while (remainder && bit_pos >= 0) {
		int shift;
		uint32_t div;

		shift = clz(remainder);
		if (shift > bit_pos)
			shift = bit_pos;

		 remainder <<= shift;
		 bit_pos -= shift;

		 div = remainder / divider;
		 remainder = remainder % divider;
		 quotient += div << bit_pos;

		 if (div & ~(0xFFFFFFFF >> bit_pos))
			 return QMATH1616_OVERFLOW;

		 remainder <<= 1;
		 bit_pos--;
	}
	quotient++;
	result = quotient >> 1;

	if ((a ^ b) & 0x80000000) {
		if (result == QMATH1616_MINIMUM)
			return QMATH1616_OVERFLOW;
		result = -result;
	}

	return result;
}

qmath1616_t qmath1616_mod(qmath1616_t x, qmath1616_t y)
{
	x %= y;

	return x;
}


