#ifndef LIBQMATH_H
#define LIBQMATH_H

#include <stdint.h>

#define QMATH1616_MAXIMUM ((qmath1616_t)0x7FFFFFFF)
#define QMATH1616_MINIMUM ((qmath1616_t)0x80000000)

#define QMATH1616_ONE ((qmath1616_t)0x00010000)

#define QMATH1616_PI ((qmath1616_t)205887)
#define QMATH1616_E ((qmath1616_t)178145)

#define QMATH16_T(x) ((qmath1616_t)(((x) >= 0) ? ((x) * 65536.0 + 0.5) : ((x) * 65536.0 - 0.5)))

typedef int32_t qmath1616_t;


static inline qmath1616_t qmath1616_from_int(int val)
{
	return val * QMATH1616_ONE;
}

static inline int qmath1616_to_int(qmath1616_t val)
{
	if (val >= 0)
		return (val + (QMATH1616_ONE >> 1)) / QMATH1616_ONE;
	else
		return (val - (QMATH1616_ONE >> 1)) / QMATH1616_ONE;
}


static inline qmath1616_t qmath1616_from_float(float val)
{
	return (qmath1616_t)(val * QMATH1616_ONE);
}


static inline qmath1616_t qmath1616_from_dbl(double val)
{
	return (qmath1616_t)(val * QMATH1616_ONE);
}




#endif /* LIBQMATH_H */
