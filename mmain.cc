#include "SBB_util.h"
#include <stdio.h>
#include <math.h>

int main(int argc, char* argv[]){
	SBB_util u;
	START_TIMER(u);
	printf("Hello World!\nThis is an simple test!\n");
	int i = 100000000;
	long n = 5;
	while(i--) n++;
	printf("Here n is %ld\n", n);	
	END_TIMER(u);
	return 0;	
}

double present_value(double future_value, double interest, int period){
	return future_value/pow(1+interest, period);
}

double bond_PV(double coupon, double maturity, double interest, int n_period){
	double sum = 0;
	for(int i = 0 ; i < n_period ; i++){
		sum += present_value(coupon, interest, i+1);
	}
	sum += present_value(maturity, interest, n_period);
	return sum;
}
