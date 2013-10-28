#include "Calculator.h"
#include <math.h>

double
Calculator::bond_PV(){
	double sum = 0;
	for(int i = 0 ; i < num_periods ; i++){
		sum += present_value(ytm_para->CouponRate/ytm_para->Frequency, (ytm_para->YieldRate/100)/ytm_para->Frequency, i+1);
	}
	sum += present_value(100, ytm_para->YieldRate/ytm_para->Frequency, num_periods);
	return sum;
}

double
Calculator::get_YTM_price(){
	return current_price;
}

double
Calculator::get_YTM_DV01(){
	return current_dv01;
}

int
Calculator::get_periods(){
	return num_periods;
}

double
Calculator::get_lgd(){
	return lgd;
}

double
Calculator::calc_risk(){
	return (get_YTM_DV01() * ytm_para->getAmount()) / 100;
}

void
Calculator::set_ytm_para(SBB_instrument_fields *input){
	ytm_para = input;
	num_periods = get_num_periods(ytm_para->SettlementDate, ytm_para->MaturityDate, ytm_para->Frequency);
	if(((bond*)ytm_para) -> Rate_type == bond::spread){
		double yield_bm = y_benchmark->get_benchmark_yield(num_periods);
		((bond*)ytm_para) ->Rate_type = bond::yield;
		ytm_para -> YieldRate = yield_bm + ytm_para->YieldRate/100;
	}
	current_price = calc_price();
	current_dv01 = calc_dv01();
	lgd = rating.LGD_given_SnP_Fitch(((bond*)ytm_para)->Quality)*((bond*)ytm_para)->Amount;
}

double
Calculator::present_value(double future_value, double interest, int period){
	return future_value/pow(1+interest, period);
}

static SBB_date from_dt, to_dt;

int
Calculator::get_num_periods(long start_dt, long end_dt, int f){
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

double
Calculator::calc_dv01(){
	double price, l_price, h_price;
	price = get_YTM_price();
	ytm_para->YieldRate += 0.01;
	l_price = calc_price();
	ytm_para->YieldRate -= 0.02;
	h_price = calc_price();
	ytm_para->YieldRate += 0.01;
	return (price-l_price + h_price-price)/2;
}

double
Calculator::calc_price(){
	double pv_factor = 1/pow((1 + (ytm_para->YieldRate/100)/ytm_para->Frequency), num_periods);
	double coupon_term = (ytm_para->CouponRate/ytm_para->Frequency) * (1- pv_factor) / ((ytm_para->YieldRate/100)/ytm_para->Frequency);
	double priciple_term = 100*pv_factor;
	return coupon_term + priciple_term;
}
