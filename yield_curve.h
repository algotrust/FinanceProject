#ifndef __YIELD_CURVE_H__
#define __YIELD_CURVE_H__
#include "bond.h"
class yield_curve{
	public:
		yield_curve(bond* records, int length);
		double get_benchmark_yield(int num_period);
		bond* get_benchmark(int index){
			return benchmark+index;
		};

	private:
		bond* benchmark;
		int num_yields;
		int* benchmark_period;
};
#endif
