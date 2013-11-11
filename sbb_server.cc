#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <math.h>

#include "sbb_socket.h"
#include "Calculator.h"
#include "bond.h"
#include "SBB_util.h"

char*
message_handler(char* message, char* inputfile, char* yieldcurve_file){
	char* result = new char[MSGSIZE];
	bond_input_file input(inputfile);
	bond_input_file yield_input(yieldcurve_file);
	bond* ytm_data, *yield_data;

	int input_length = 0;
	yield_data = yield_input.get_records(input_length);
	yield_curve yield_benchmark(yield_data, input_length);


	ytm_data = input.get_records(input_length);
	Calculator calc;
	double total_risk = 0.0;
	calc.set_yield_curve(&yield_benchmark);

	double risk_t2 = 0.0;
	calc.set_ytm_para(yield_data+1);
	risk_t2 = calc.get_YTM_DV01();

	
	bond temp;

	int amount = 0;
	double bucket_30_sum_risk = 0.0;
	double value_up100 = 0.0;
	double value_down100 = 0.0;
	for(int i = 0 ; i < input_length ; i++){
		calc.set_ytm_para(ytm_data+i);
		double risk = calc.calc_risk();
		//check if in the 30 bucket
		if(calc.get_periods()/ytm_data[i].Frequency > 20){
			bucket_30_sum_risk += risk;
		}
		ytm_data[i].YieldRate += 1;
		calc.set_ytm_para(ytm_data+i);
		value_up100 += calc.get_YTM_price()*(ytm_data+i)->Amount;
		ytm_data[i].YieldRate -= 2;
		calc.set_ytm_para(ytm_data+i);
		value_down100 += calc.get_YTM_price()*(ytm_data+i)->Amount;
		ytm_data[i].YieldRate += 1;
	}
	bucket_30_sum_risk *= -100;

	printf("%.3f, %.3f, %.3f\n", bucket_30_sum_risk/risk_t2, value_up100/100, value_down100/100);
	sprintf(result, "%.3f, %.3f, %.3f", bucket_30_sum_risk/risk_t2, value_up100/100, value_down100/100);
	return result;
}

int main(int argc, char* argv[])
{
	/* 
	 * get an internet domain socket 
	 */
	int sd;
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	/* 
	 * set up the socket structure 
	 */
	struct sockaddr_in	sock_addr;

	memset(&sock_addr, 0, sizeof(sock_addr));
	sock_addr.sin_family = AF_INET;

#ifdef SBB_ANY
	/* set to INADDR_ANY if want server to be open to any client on any machine */
	sock_addr.sin_addr.s_addr = INADDR_ANY;
#else
	char	hostname[128];
	/*
	 *  we'll default to this host and call a section 3 func to get this host
	 */
	if( gethostname(hostname,sizeof(hostname)) ){
		fprintf(stderr," SBB gethostname(...) failed errno: %d\n", errno);
		exit(1);
	}
	printf("SBB gethostname() local hostname: \"%s\"\n", hostname);

	/*
	 * set up socket structure for our host machine
	 */
	struct hostent *hp;
	if ((hp = gethostbyname(hostname)) == 0) {
		fprintf(stderr,"SBB gethostbyname(...) failed errno: %d exiting...\n", errno);
		exit(1);
	}
	sock_addr.sin_addr.s_addr = ((struct in_addr *)(hp->h_addr))->s_addr;
#endif
	sock_addr.sin_port = htons(PORT);

	/* 
	 * bind the socket to the port number 
	 */
	if (bind(sd, (struct sockaddr *) &sock_addr, sizeof(sock_addr)) == -1) {
		perror("bind");
		exit(1);
	}

	/* 
	 * advertise we are available on this socket/port
	 */
	if (listen(sd, 5) == -1) {
		perror("listen");
		exit(1);
	}

	/* 
	 * wait for a client to connect 
	 */
	struct sockaddr_in	sock_addr_from_client;
	socklen_t addrlen = sizeof(sock_addr_from_client); 
	int sd_current;
	if ((sd_current = accept(sd, (struct sockaddr *)  &sock_addr_from_client, &addrlen)) == -1) {
		fprintf(stderr,"SBB accept(...) failed errno: %d  exiting...\n", errno);
		exit(1);
	}
	printf("SBB client ip address: %s port: %x\n",
			inet_ntoa(sock_addr_from_client.sin_addr),
			PORT);

	//		ntohs(sock_addr_from_client.sin_port));

	/*
	 * block on socket waiting for client message
	 */
	int ret = 0;
	char	msg[MSGSIZE];
	fprintf(stderr," SBB: sizeof msg: %lu\n", sizeof(msg));

	SBB_util u;
	while ((ret = recv(sd_current, msg, sizeof(msg), 0)) > 0) {

		u.start_clock();
		double real_time, user_time, system_time;
		char *result= message_handler(msg, argv[1], argv[2]);
		u.end_clock(real_time, user_time, system_time);
		sprintf(msg, "%s %.3f %.3f %.3f\n", result, real_time, user_time, system_time);

		/* 
		 * ack back to the client 
		 */
		if (send(sd_current, msg, strlen(msg), 0) == -1) {
			fprintf(stderr,"SBB send(...) failed errno: %d exiting...\n", errno);
			exit(1);
		}
	}


	if( 0 == ret ) {
		printf("SBB client exited...\n");
		/* For TCP sockets	
		 * the return value 0 means the peer has closed its half side of the connection 
		 */
	}
	else if( -1 == ret ) {
		fprintf(stderr,"SBB recv(...) returned failed - errno: %d exiting...\n", errno);	
		exit(1);
	}

	close(sd_current); 
	close(sd);
}

