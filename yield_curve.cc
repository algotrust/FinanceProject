#include "yield_curve.h"
#include "SBB_date.h"

//#include <math.h>

static SBB_date from_dt, to_dt;

int
get_num_periods(long start_dt, long end_dt, int f){
	from_dt.set_from_yyyymmdd(start_dt);
	to_dt.set_from_yyyymmdd(end_dt);

	int p_length = 12/f;

	int period_count = 0;
	while (from_dt != to_dt) {
		to_dt.add_months(-p_length);
		period_count++;
	}
	return period_count;
}

yield_curve::yield_curve(bond* records, int length){
	num_yields = length;
	benchmark = records;
	benchmark_period = new int[num_yields];
	for(int i = 0 ; i < num_yields ; i++){
		benchmark_period[i] = get_num_periods(benchmark[i].SettlementDate, benchmark[i].MaturityDate, benchmark[i].Frequency); 
	}
}

double
yield_curve::get_benchmark_yield(int num_period){
	int min_diff = abs(benchmark_period[0] - num_period), min_index = 0;
	int diff;
	for(int i = 1 ; i < num_yields ; i++){
		diff = abs(benchmark_period[i] - num_period);
		if(diff < min_diff){
			min_index = i;
			min_diff = diff;
		}
	}
	return benchmark[min_index].YieldRate;
}


