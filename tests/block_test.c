#include <math.h>
#include "minunit.h"
#include "firstOrderLag.h"

#define ALL_GOOD(blocks)                                                                   \
	{                                                                                      \
		for (size_t i = 0; i < sizeof(blocks) / sizeof(blocks[0]); i++)                    \
			mu_assert(good_block(blocks[i]), "block %zu is bad and it should be good", i); \
	}

#define ALL_BAD(blocks)                                                                     \
	{                                                                                       \
		for (size_t i = 0; i < sizeof(blocks) / sizeof(blocks[0]); i++)                     \
			mu_assert(!good_block(blocks[i]), "block %zu is good and it should be bad", i); \
	}

char *first_order_lag_bad_count()
{
	double tau = 5;
	struct StrictlyProperBlock blocks[] =
		{
			firstOrderLag(-100, &tau),
			firstOrderLag(-1, &tau),
			firstOrderLag(0, &tau),
		};
	ALL_BAD(blocks);
	return NULL;
}

char *first_order_lag_null_tau()
{
	struct StrictlyProperBlock blocks[] =
		{
			firstOrderLag(-100, NULL),
			firstOrderLag(-1, NULL),
			firstOrderLag(0, NULL),
			firstOrderLag(1, NULL),
			firstOrderLag(2, NULL),
			firstOrderLag(3, NULL),
			firstOrderLag(100, NULL),
		};
	ALL_BAD(blocks);
	return NULL;
}

char *first_order_lag_bad_tau()
{
	double tau[] = {0, -10, INFINITY, -INFINITY};
	struct StrictlyProperBlock blocks[] =
		{
			firstOrderLag(-11, &tau[0]),
			firstOrderLag(-1, &tau[0]),
			firstOrderLag(0, &tau[0]),
			firstOrderLag(1, &tau[0]),
			firstOrderLag(1, &tau[1]),
			firstOrderLag(1, &tau[2]),
			firstOrderLag(1, &tau[3]),
			firstOrderLag(2, &tau[0]),
			firstOrderLag(2, &tau[1]),
			firstOrderLag(2, &tau[2]),
			firstOrderLag(3, &tau[0]),
			firstOrderLag(3, &tau[1]),
			firstOrderLag(4, &tau[0]),
		};
	ALL_BAD(blocks);
	return NULL;
}

char *first_order_lag_construction()
{
	double tau[] = {1, 2, 13.5};
	struct StrictlyProperBlock blocks[] =
		{
			firstOrderLag(1, &tau[0]),
			firstOrderLag(1, &tau[1]),
			firstOrderLag(1, &tau[2]),
			firstOrderLag(2, &tau[0]),
			firstOrderLag(2, &tau[1]),
			firstOrderLag(3, &tau[0]),
		};
	ALL_GOOD(blocks);
	return NULL;
}

RUN_TESTS(
	first_order_lag_bad_count,
	first_order_lag_null_tau,
	first_order_lag_construction, );
