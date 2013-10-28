#include "Calculator.h"
#include "bond.h"
#include "yield_curve.h"

#include <stdio.h>
#include <math.h>
#include <assert.h>

void test_bond_amount(){
	bond test_bond;
	test_bond.Amount = 100;
	assert(test_bond.getAmount() == 100);
}

void test_bond_sig(){
	bond test_bond;
	strcpy(test_bond.Ticker, "AB");
	test_bond.CouponRate = 0.5;
	test_bond.MaturityDate = 20100811;
	char result[100];
	sprintf(result, "%s %f %d", "AB", 0.5, 20100811);
	assert(strcmp(result, test_bond.bond_sig()) == 0);
}

void test_bond(){
	test_bond_amount();
	test_bond_sig();
	printf("Test bond passed!\n");
}

void test_bond_input_file_get1(char *name){
	bond_input_file input(name);
	int input_length;
	bond* ytm_data = input.get_records(input_length);
	//we are testing from the default test file, which have a length of 11
	assert(input_length == 11);
	assert(strcmp(ytm_data[0].SecurityID, "SBB_0001") == 0);
}

void test_bond_input_file_get2(char *name){
	bond_input_file input(name);
	int input_length;
	bond* ytm_data = input.get_records(input_length);
	//we are testing from the default test file, which have a length of 11
	assert(input_length == 11);

	bond test_bond;
	strcpy(test_bond.SecurityID, "SBB_0002");
	strcpy(test_bond.Ticker, "GM");
	strcpy(test_bond.Quality, "BBB");
	test_bond.SettlementDate=20091115;
	test_bond.CouponRate=3.5;
	test_bond.MaturityDate=20111115;
	test_bond.Frequency = 2;
	test_bond.YieldRate = 3.1;
	test_bond.Rate_type = bond::yield;
	test_bond.Amount = -10000;
	assert(strcmp(ytm_data[1].bond_sig(), test_bond.bond_sig())== 0);
}


void test_bond_input_file(char *name){
	test_bond_input_file_get1(name);
	test_bond_input_file_get2(name);
	printf("Test bond_input_file passed!\n");
}

void test_calc_price1(){
	bond test_bond;
	strcpy(test_bond.SecurityID, "TEST_001");
	strcpy(test_bond.Ticker, "GM");
	strcpy(test_bond.Quality, "AA");
	test_bond.SettlementDate=20091115;
	test_bond.CouponRate=3.5;
	test_bond.MaturityDate=20111115;
	test_bond.Frequency = 2;
	test_bond.YieldRate = 3.1;
	test_bond.Rate_type = bond::yield;
	test_bond.Amount = -10000;
	Calculator calc;
	calc.set_ytm_para((SBB_instrument_fields*)&test_bond);
	assert(abs(calc.get_YTM_price() - calc.calc_price()) < 0.0001);
	assert(abs(calc.get_YTM_price() - 100.76993556) < 0.0001);
}


void test_calc_price2(){
	bond test_bond;
	strcpy(test_bond.SecurityID, "TEST_001");
	strcpy(test_bond.Ticker, "GM");
	strcpy(test_bond.Quality, "AA");
	test_bond.SettlementDate=20091115;
	test_bond.CouponRate=3.8;
	test_bond.MaturityDate=20121115;
	test_bond.Frequency = 2;
	test_bond.YieldRate = 3.2;
	test_bond.Rate_type = bond::yield;
	test_bond.Amount = -100000;
	Calculator calc;
	calc.set_ytm_para((SBB_instrument_fields*)&test_bond);
	assert(abs(calc.get_YTM_price() - 101.70335078) < 0.0001);
}

void test_calc_price3(){
	bond test_bond;
	strcpy(test_bond.SecurityID, "TEST_001");
	strcpy(test_bond.Ticker, "GM");
	strcpy(test_bond.Quality, "AA");
	test_bond.SettlementDate=20091115;
	test_bond.CouponRate=3.9;
	test_bond.MaturityDate=20131115;
	test_bond.Frequency = 2;
	test_bond.YieldRate = 3.3;
	test_bond.Rate_type = bond::yield;
	test_bond.Amount = -1000;
	Calculator calc;
	calc.set_ytm_para((SBB_instrument_fields*)&test_bond);
	assert(abs(calc.get_YTM_price() - 102.23117328) < 0.0001);
}

void test_calc_dv011(){
	bond test_bond;
	strcpy(test_bond.SecurityID, "TEST_001");
	strcpy(test_bond.Ticker, "GM");
	strcpy(test_bond.Quality, "AA");
	test_bond.SettlementDate=20091115;
	test_bond.CouponRate=3.5;
	test_bond.MaturityDate=20111115;
	test_bond.Frequency = 2;
	test_bond.YieldRate = 3.1;
	test_bond.Rate_type = bond::yield;
	test_bond.Amount = -10000;
	Calculator calc;
	calc.set_ytm_para((SBB_instrument_fields*)&test_bond);
	assert(abs(calc.get_YTM_DV01() - calc.calc_dv01()) < 0.0001);
	assert(abs(calc.get_YTM_DV01() - 0.01934243) < 0.0001);
}


void test_calc_dv012(){
	bond test_bond;
	strcpy(test_bond.SecurityID, "TEST_001");
	strcpy(test_bond.Ticker, "GM");
	strcpy(test_bond.Quality, "AA");
	test_bond.SettlementDate=20091115;
	test_bond.CouponRate=3.8;
	test_bond.MaturityDate=20121115;
	test_bond.Frequency = 2;
	test_bond.YieldRate = 3.2;
	test_bond.Rate_type = bond::yield;
	test_bond.Amount = -100000;
	Calculator calc;
	calc.set_ytm_para((SBB_instrument_fields*)&test_bond);
	assert(abs(calc.get_YTM_DV01() - 0.02867869) < 0.0001);
}

void test_calc_dv013(){
	bond test_bond;
	strcpy(test_bond.SecurityID, "TEST_001");
	strcpy(test_bond.Ticker, "GM");
	strcpy(test_bond.Quality, "AA");
	test_bond.SettlementDate=20091115;
	test_bond.CouponRate=3.9;
	test_bond.MaturityDate=20131115;
	test_bond.Frequency = 2;
	test_bond.YieldRate = 3.3;
	test_bond.Rate_type = bond::yield;
	test_bond.Amount = -1000;
	Calculator calc;
	calc.set_ytm_para((SBB_instrument_fields*)&test_bond);
	assert(abs(calc.get_YTM_DV01() - 0.03767066) < 0.0001);
}

void test_calc_risk(){
	bond test_bond;
	strcpy(test_bond.SecurityID, "TEST_001");
	strcpy(test_bond.Ticker, "GM");
	strcpy(test_bond.Quality, "AA");
	test_bond.SettlementDate=20091115;
	test_bond.CouponRate=3.9;
	test_bond.MaturityDate=20131115;
	test_bond.Frequency = 2;
	test_bond.YieldRate = 3.3;
	test_bond.Rate_type = bond::yield;
	test_bond.Amount = -1000;
	Calculator calc;
	calc.set_ytm_para((SBB_instrument_fields*)&test_bond);
	assert(abs(calc.calc_risk() + 0.37670661) < 0.0001);
}


void test_calculator(){
	test_calc_price1();
	test_calc_price2();
	test_calc_price3();
	test_calc_dv011();
	test_calc_dv012();
	test_calc_dv013();
	test_calc_risk();
	printf("Test calculator passed!\n");
}

void test_yield_curve_input1(char *yield_file){
	int length;
	bond* yield_data;
	bond_input_file yield_input(yield_file);
	yield_data = yield_input.get_records(length);
	yield_curve yield_benchmark(yield_data, length);
	assert(abs(yield_benchmark.get_benchmark(0)->YieldRate - 4.0)<0.0001);
}

void test_yield_curve_input2(char *yield_file){
	int length;
	bond* yield_data;
	bond_input_file yield_input(yield_file);
	yield_data = yield_input.get_records(length);
	yield_curve yield_benchmark(yield_data, length);
	assert(yield_benchmark.get_benchmark(1)->MaturityDate == 20111015);
}

void test_yield_curve_get_yield1(char *yield_file){
	int length;
	bond* yield_data;
	bond_input_file yield_input(yield_file);
	yield_data = yield_input.get_records(length);
	yield_curve yield_benchmark(yield_data, length);
	assert(abs(yield_benchmark.get_benchmark_yield(5*2) - 2.0) < 0.00001);
}

void test_yield_curve_get_yield2(char *yield_file){
	int length;
	bond* yield_data;
	bond_input_file yield_input(yield_file);
	yield_data = yield_input.get_records(length);
	yield_curve yield_benchmark(yield_data, length);
	assert(abs(yield_benchmark.get_benchmark_yield(15*2) - 3.0) < 0.00001);
}

void test_yield_curve_get_yield3(char *yield_file){
	int length;
	bond* yield_data;
	bond_input_file yield_input(yield_file);
	yield_data = yield_input.get_records(length);
	yield_curve yield_benchmark(yield_data, length);
	assert(abs(yield_benchmark.get_benchmark_yield(21*2) - 3.0) < 0.00001);
}

void test_yield_curve(char *yield_file){
	test_yield_curve_input1(yield_file);
	test_yield_curve_input2(yield_file);
	test_yield_curve_get_yield1(yield_file);
	test_yield_curve_get_yield2(yield_file);
	printf("Test yield curve passed!\n");
}

void test_calculator_spread1(char *yield_file){
	bond test_bond;
	strcpy(test_bond.SecurityID, "TEST_001");
	strcpy(test_bond.Ticker, "GM");
	strcpy(test_bond.Quality, "AA");
	test_bond.SettlementDate=20091115;
	test_bond.CouponRate=3.5;
	test_bond.MaturityDate=20111115;
	test_bond.Frequency = 2;
	test_bond.YieldRate = 210;
	test_bond.Rate_type = bond::spread;
	test_bond.Amount = -10000;
	Calculator calc;

	int length;
	bond* yield_data;
	bond_input_file yield_input(yield_file);
	yield_data = yield_input.get_records(length);
	yield_curve yield_benchmark(yield_data, length);

	calc.set_yield_curve(&yield_benchmark);
	calc.set_ytm_para((SBB_instrument_fields*)&test_bond);

	assert(abs(calc.get_YTM_price() - calc.calc_price()) < 0.0001);
	assert(abs(calc.get_YTM_price() - 100.76993556) < 0.0001);
}

void test_calculator_spread2(char *yield_file){
	bond test_bond;
	strcpy(test_bond.SecurityID, "TEST_001");
	strcpy(test_bond.Ticker, "GM");
	strcpy(test_bond.Quality, "AA");
	test_bond.SettlementDate=20091115;
	test_bond.CouponRate=3.9;
	test_bond.MaturityDate=20131115;
	test_bond.Frequency = 2;
	test_bond.YieldRate = 130;
	test_bond.Rate_type = bond::spread;
	test_bond.Amount = -1000;
	Calculator calc;

	int length;
	bond* yield_data;
	bond_input_file yield_input(yield_file);
	yield_data = yield_input.get_records(length);
	yield_curve yield_benchmark(yield_data, length);

	calc.set_yield_curve(&yield_benchmark);
	calc.set_ytm_para((SBB_instrument_fields*)&test_bond);

	assert(abs(calc.get_YTM_price() - 102.23117328) < 0.0001);
}

void test_calculator_spread(char *yield_file){
	test_calculator_spread1(yield_file);
	test_calculator_spread2(yield_file);
	printf("Test calculator spread passed!\n");
}


int main(int argc, char* argv[]){
	if(argc != 3){
		printf("Usage: test_lib testfile yield_curve_file\n");
		exit(0);
	}
	test_bond();
	test_bond_input_file(argv[1]);
	test_calculator();
	test_yield_curve(argv[2]);
	test_calculator_spread(argv[2]);
	printf("All tests passed!\n");
}

