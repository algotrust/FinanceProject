#ifndef __BOND_H__
#define __BOND_H__
#include "SBB_io.h"
#define BOND_TICKER_LENGTH 8
#define BOND_QUALITY_LENGTH 8
class bond : public SBB_instrument_fields {
	public:
		enum rate_type{
			yield = 0x01,
			spread,
			other
		};

		bond():SBB_instrument_fields(){
			Ticker[0] = 0;
			Rate_type = yield;
			Quality[0] = 0;
			Amount = 0;
		};

		int getAmount(){
			return Amount;
		};

		void show(){
			printf("ID: %s Ticker: %s SettleDate: %d Coupon: %.2f MatDate: %d Frequency: %d Rate_type: %d  Yield: %.3f Quality: %s Amount: %d\n",
				SecurityID,
				Ticker,
				SettlementDate,
				CouponRate,
				MaturityDate,
				Frequency,
				Rate_type,
				YieldRate,
				Quality,
				Amount);
		};

		char *bond_sig(){
			char *result = new char[SBB_LINE_BUFFER_LENGTH];
			sprintf(result, "%s %f %d", Ticker, CouponRate, MaturityDate);
			return result;
		}

		char Ticker[BOND_TICKER_LENGTH];
		int Rate_type;
		char Quality[BOND_QUALITY_LENGTH];
		int Amount;
};

class bond_input_file : public SBB_instrument_input_file{
	public:
		bond_input_file(const char* filename):SBB_instrument_input_file(filename){};
		bond* get_records(int& length);
		void free_records();

	private:
		bond *bond_array;
};
#endif
