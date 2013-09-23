#include "Calculator.h"

#include <stdio.h>

int main(int argc, char* argv[]){
	if(argc != 2){
		printf("Usage: price_calc inputfile\n");
		exit(0);
	}
	SBB_util u;
	START_TIMER(u);
	SBB_instrument_input_file input(argv[1]);
	SBB_instrument_fields* ytm_data;
	int input_length = 0;
	ytm_data = input.get_records(input_length);
	Calculator calc;
	for(int i = 0 ; i < input_length ; i++){
		calc.set_ytm_para(ytm_data+i);
		printf("%s Bond Price: %.8f, DV01: %.8f\n", ytm_data[i].SecurityID, calc.calc_YTM(), calc.calc_YTM_DV01());
	}
	END_TIMER(u);
	
	input.free_records();


	return 0;	
}
