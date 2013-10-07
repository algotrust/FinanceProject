#ifndef __CALCULATOR_H__
#define __CALCULATOR_H__
#include "SBB_io.h"
#include "SBB_date.h"
#include "SBB_util.h"

class Calculator{
	public:
		Calculator(){
			ytm_para = NULL;
			num_periods = 0;
		};
		~Calculator(){
		};
		double bond_PV();

		double get_YTM_price();
		double get_YTM_DV01();
		double calc_risk();

		void show(){
			ytm_para->show();
		};

		/*
		 * This function is responsible to set the ytm_para
		 * and num_periods 
		 * Before calling the calc_YTM and calc_YTM_DV01, 
		 * we should call this to set up first.
		 *
		 */
		void set_ytm_para(SBB_instrument_fields *input);

		/**
		 * Even thought these functions are in public access, do not call
		 * them because get functions take less running time;
		 */
		double calc_dv01();
		double calc_price();

		
	private:
		SBB_instrument_fields* ytm_para;
		int num_periods;
		double present_value(double future_value, double interest, int period);
		int get_num_periods(long start_dt, long end_dt, int f);

		//use these local variable to reduce tiems we calculate
		double current_dv01;
		double current_price;
};
#endif
