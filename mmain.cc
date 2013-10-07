#include "Calculator.h"
#include "bond.h"

#include <stdio.h>

int main(int argc, char* argv[]){
	if(argc != 2){
		printf("Usage: price_calc inputfile\n");
		exit(0);
	}
	SBB_util u;
	START_TIMER(u);
	bond_input_file input(argv[1]);
	bond* ytm_data;
	int input_length = 0;
	ytm_data = input.get_records(input_length);
	Calculator calc;
	double total_risk = 0.0;
	for(int i = 0 ; i < input_length ; i++){
		calc.set_ytm_para(ytm_data+i);
		double risk = calc.calc_risk();
		printf("%s Bond ID: %s Bond Price: %.8f, DV01: %.8f, Risk: %.8f\n", ytm_data[i].SecurityID, ytm_data[i].bond_sig(), calc.get_YTM_price(),calc.get_YTM_DV01(), risk); 

		total_risk += risk;
	}

	printf("Total risk: %.8f\n", total_risk);
	END_TIMER(u);
	
	input.free_records();


	return 0;	
}
