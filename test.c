#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "rs232.h"
#include "server.h"

const char *seps = ": ";
char *_;

void parse_string(char *str){
	
	// extract CO2 levels
	_ = strtok(str, seps);
	int co2 = atoi(strtok(NULL, seps));

	// extract temperature
	_  = strtok(NULL, seps);
	double temp = strtod(strtok(NULL, seps), &_);

	// extract humidity
	_ = strtok(NULL, seps);
	double humidity = strtod(strtok(NULL, seps), &_);
}

int main()
{
	const int SIZE = 512;
	const char *mode = "8N1";
  	int bdrate = 115200;     
  	unsigned char buf[SIZE];
	struct sockaddr_in sockaddr;

  	int cport_nr = RS232_GetPortnr("ttyUSB1");
	init_server(&sockaddr);

  	if(RS232_OpenComport(cport_nr, bdrate, mode, 0))
    	return -1;

  	while(1) {
    	
		int bytes = RS232_PollComport(cport_nr, buf, SIZE-1);

		if(bytes <= 0 || bytes >= SIZE)
			continue;

      	buf[bytes] = 0; 

      	for(int i = 0; i < bytes; i++) {
        	if(buf[i] < 32)
          		buf[i] = ' ';
		}

		// if(strchr((char *)buf, ':') != NULL) {
		// 	parse_string((char *)buf);
		// }
		// else {
		// 	handle incorrect data
		// }

		send_to_client(buf);

    	usleep(50000); 
  }

  close_server();

  return 0;
}