#include "Calculator.h"
#include "bond.h"

#include <stdio.h>

int main(int argc, char* argv[]){
	if(argc != 3){
		printf("Usage: price_calc inputfile yieldcurve_file\n");
		exit(0);
	}
	SBB_util u;
	START_TIMER(u);
	bond_input_file input(argv[1]);
	bond_input_file yield_input(argv[2]);
	bond* ytm_data, *yield_data;

	int input_length = 0;
	yield_data = yield_input.get_records(input_length);
	yield_curve yield_benchmark(yield_data, input_length);

	ytm_data = input.get_records(input_length);
	Calculator calc;
	double total_risk = 0.0;
	calc.set_yield_curve(&yield_benchmark);
	bond_result* results = new bond_result[input_length];

	int largest_long= 0, largest_short = 0;
	char* largest_long_id, *largest_short_id;
	double largest_risk = 0;
	char* largest_risk_id;
	int amount = 0;
	for(int i = 0 ; i < input_length ; i++){
		calc.set_ytm_para(ytm_data+i);
		double risk = calc.calc_risk();
		results[i].price = calc.get_YTM_price();
		results[i].dv01 = calc.get_YTM_DV01();
		results[i].risk = risk;
		results[i].lgd = calc.get_lgd();

		if(abs(risk) > largest_risk){
			largest_risk = abs(risk);
			largest_risk_id = (ytm_data+i)->SecurityID;
		}
		total_risk += risk;
		amount = (ytm_data+i)->Amount;
		if(amount <0 && abs(amount) > largest_short){
			largest_short = abs(amount);
			largest_short_id = (ytm_data+i)->SecurityID;
		}
		else if(amount > 0 && amount > largest_long){
			largest_long = amount;
			largest_long_id = (ytm_data+i)->SecurityID;
		}
	}

	input.write_results(argv[1], results, input_length);
	FILE *f = fopen("results.txt", "w");
	fprintf(f, "%s %d\n", largest_long_id, largest_long);
	fprintf(f, "%s %d\n", largest_short_id, largest_short);
	fprintf(f, "%s %.3f\n", largest_risk_id, largest_risk);
	fprintf(f, "%.3f\n", total_risk);

	END_TIMER(u);
	
	input.free_records();


	return 0;	
}
