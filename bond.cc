#include "bond.h"

bond*
bond_input_file::get_records(int &length){

	length = get_record_count();

	bond_array = new bond[length];

	if(NULL == bond_array){
		fprintf(stderr,"calloc failed - errno: %d\n", errno);
		return bond_array;
	}
	int comment_count = 0;

	_line_buf[0] = ' ';
	int non_comments_line_count_read = 0; 
	char *token;
	while(fgets(_line_buf,SBB_LINE_BUFFER_LENGTH,_file)){

		//printf("SBB CHECK file line: %s\n", _line_buf);

		if('#' == _line_buf[0]) {
			continue; // skip through comments
			comment_count++;
		}

		// the line should be:
		// SecurityID  Ticker SettlementDate CouponRate MaturityDate Frequency Rate_type YieldRate Quality Amount

		// SecurityID
		token = strtok(_line_buf," ");
		if(NULL == token) { 
			fprintf(stderr,"line parsing failed on buf: %s\n", _line_buf); 
			exit(1); 
		} 
		//printf("SecurityID: %s\n", token);
		strcpy(bond_array[non_comments_line_count_read].SecurityID, token);

		// Ticker 
		token = strtok(NULL," ");
		if(NULL == token) { 
			fprintf(stderr,"line parsing failed on buf: %s\n", _line_buf); 
			exit(1); 
		} 
		//printf("SecurityID: %s\n", token);
		strcpy(bond_array[non_comments_line_count_read].Ticker, token);

		// SettlementDate
		token = strtok(NULL," ");
		if(NULL == token) { 
			fprintf(stderr,"line parsing failed on buf: %s\n", _line_buf); 
			exit(1); 
		}
		//printf("SettlementDate: %s\n", token); 
		bond_array[non_comments_line_count_read].SettlementDate = atoi(token);

		// CouponRate
		token = strtok(NULL," ");
		if(NULL == token ) { 
			fprintf(stderr,"line parsing failed on buf: %s\n", _line_buf); 
			exit(1); 
		}
		//printf("CouponRate: %s\n", token);
		bond_array[non_comments_line_count_read].CouponRate = atof(token);

		// MaturityDate
		token = strtok(NULL," ");
		//printf("MaturityDate: %s\n", token);
		bond_array[non_comments_line_count_read].MaturityDate = atoi(token);				

		// Frequency
		token = strtok(NULL," ");
		//printf("Frequency: %s\n", token);
		bond_array[non_comments_line_count_read].Frequency = atoi(token);				
		// Rate_type 
		token = strtok(NULL," ");
		if(NULL == token) { 
			fprintf(stderr,"line parsing failed on buf: %s\n", _line_buf); 
			exit(1); 
		} 
		if(strcmp(token, "YIELD") == 0){
			bond_array[non_comments_line_count_read].Rate_type = bond::yield;
		}
		else if(strcmp(token, "SPREAD") == 0){
			bond_array[non_comments_line_count_read].Rate_type = bond::spread;
		}
		else{
			bond_array[non_comments_line_count_read].Rate_type = bond::other;
		
		}

		// Yield (later we will load in spread and a string token of an OTR. Also will have a separate file

		token = strtok(NULL," ");
		if(NULL == token ) { 
			fprintf(stderr,"line parsing failed on buf: %s\n", _line_buf); 
			exit(1); 
		}
		//printf("YieldRate is:  %s\n", token);
		bond_array[non_comments_line_count_read].YieldRate = atof(token);

		// Quality 
		token = strtok(NULL," ");
		if(NULL == token) { 
			fprintf(stderr,"line parsing failed on buf: %s\n", _line_buf); 
			exit(1); 
		} 
		//printf("SecurityID: %s\n", token);
		strcpy(bond_array[non_comments_line_count_read].Quality, token);


		//amount
		token = strtok(NULL," ");
		if(NULL == token ) { 
			fprintf(stderr,"line parsing failed on buf: %s\n", _line_buf); 
			exit(1); 
		}
		//printf("YieldRate is:  %s\n", token);
		bond_array[non_comments_line_count_read].Amount = atoi(token);

		non_comments_line_count_read++;
	}

	//printf("SBB lines read: %d \n", non_comments_line_count_read);

	return bond_array;
}

void
bond_input_file::free_records(){
	delete[] bond_array;
}

void
bond_input_file::write_results(const char *filename, bond_result* result,  int length){
	FILE *outfile = fopen("temp_result.tmp", "w");

	rewind(_file);

	_line_buf[0] = ' ';
	char *token;
	int index = 0;

	while(fgets(_line_buf,SBB_LINE_BUFFER_LENGTH,_file)){
		if(index >= length){
			fprintf(stderr, "error here, index is larger than length\n");
			exit(1);
		}

		if('#' == _line_buf[0]) {
			fprintf(outfile, "%s", _line_buf);
			continue; // skip through comments
		}

		// the line should be:
		// SecurityID  Ticker SettlementDate CouponRate MaturityDate Frequency Rate_type YieldRate Quality Amount
		int temp_len = strlen(_line_buf);
		_line_buf[temp_len-1] = 0;
		fprintf(outfile, "%s %.3f %.3f %.3f %.3f\n", _line_buf, result[index].price, result[index].dv01, result[index].risk, result[index].lgd);
		index++;
	}
	fclose(outfile);

	remove(filename);
	rename("temp_result.tmp", filename);

	return;

}
