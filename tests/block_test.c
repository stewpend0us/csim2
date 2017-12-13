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
			firstOrderLag(-100, &tau, NULL),
			firstOrderLag(-1, &tau, NULL),
			firstOrderLag(0, &tau, NULL),
		};
	ALL_BAD(blocks);
	return NULL;
}

char *first_order_lag_null_tau()
{
	struct StrictlyProperBlock blocks[] =
		{
			firstOrderLag(-100, NULL, NULL),
			firstOrderLag(-1, NULL, NULL),
			firstOrderLag(0, NULL, NULL),
			firstOrderLag(1, NULL, NULL),
			firstOrderLag(2, NULL, NULL),
			firstOrderLag(3, NULL, NULL),
			firstOrderLag(100, NULL, NULL),
		};
	ALL_BAD(blocks);
	return NULL;
}

char *first_order_lag_bad_tau()
{
	double tau[] = {0, -10, INFINITY, -INFINITY};
	struct StrictlyProperBlock blocks[] =
		{
			firstOrderLag(-11, &tau[0], NULL),
			firstOrderLag(-1, &tau[0], NULL),
			firstOrderLag(0, &tau[0], NULL),
			firstOrderLag(1, &tau[0], NULL),
			firstOrderLag(1, &tau[1], NULL),
			firstOrderLag(1, &tau[2], NULL),
			firstOrderLag(1, &tau[3], NULL),
			firstOrderLag(2, &tau[0], NULL),
			firstOrderLag(2, &tau[1], NULL),
			firstOrderLag(2, &tau[2], NULL),
			firstOrderLag(3, &tau[0], NULL),
			firstOrderLag(3, &tau[1], NULL),
			firstOrderLag(4, &tau[0], NULL),
		};
	ALL_BAD(blocks);
	return NULL;
}

char *first_order_lag_construction()
{
	double tau[] = {1, 2, 13.5};
	struct StrictlyProperBlock blocks[] =
		{
			firstOrderLag(1, &tau[0], NULL),
			firstOrderLag(1, &tau[1], NULL),
			firstOrderLag(1, &tau[2], NULL),
			firstOrderLag(2, &tau[0], NULL),
			firstOrderLag(2, &tau[1], NULL),
			firstOrderLag(3, &tau[0], NULL),
		};
	ALL_GOOD(blocks);
	return NULL;
}

RUN_TESTS(
	first_order_lag_bad_count,
	first_order_lag_null_tau,
	first_order_lag_construction, );
