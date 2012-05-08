/*************************************************************
 *  PSis  --  2011 / 2012                                    *
 *  YASC - Yet Another Simple Calculator                     *
 *  _______________________________________________________  *
 *                                                           *
 *  Bruno Santos        nº64956     MEAer                    *
 *  Flávio Schuindt     nº74570     MEEC                     *
 *  _______________________________________________________  *
 *                                                           *
 *  YASC Server  v0.1                                        *
 *     expected arguments                                    *
 *      <port>            specify listening port             *
 *                                                           *
 *     options                                               *
 *      -g                start in debug mode                *
 *                                                           *
 *************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>


#include <newvar.h>
#include <prototypesS.h>
#include <globalHeader.h>

#define OWNER	/* owner of global variables */
#include <commonS.h>


int main( int argc, char *argv[] ) {

	int primarySocket, secondarySocket, clientAddr_len;
	unsigned int port;
	struct sockaddr_in serverAddr, clientAddr;
	char *endptr;

/*
	REQUEST *req;
	int r1=0, r2=0;*/
	pthread_t poolManager, serverManager;


	/* >>> MASTER thread <<< */

	pthread_mutex_init(&p_mutex, NULL);
	pthread_cond_init(&p_cond_var,NULL);

	/* argument parsing; setup */
	if( argc == 2 ) {

		errno = 0;		/* only way of checking over / underflow */
		port = (unsigned int) strtol(argv[1],&endptr,0);
		/* is it a number ? */
		if( (errno == ERANGE) || ((*endptr != '\0') && (endptr != argv[1])) ) {	/* C99 implies that if it isn't ERANGE, it's 0 */
			fprintf(stderr,">> Setup error!\n>> Missing argument <listening port>\n");
			exit(-1);
		}

		if( (primarySocket = socket(AF_INET,SOCK_STREAM,0)) < 0 ) {
			fprintf(stderr,">> Setup error!\n>> Failed to open socket.\n");
			exit(-1);
		}

		memset(&serverAddr,0, sizeof(serverAddr));
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.s_addr = INADDR_ANY;
		serverAddr.sin_port = htons(port);

		if( bind(primarySocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0 ) {
			fprintf(stderr,">> Setup error!\n>> Failed to bind socket.\n");
			exit(-1);
		}

	} else {
		fprintf(stderr,">> Setup error!\n>> Wrong arguments. Aborting.\n");
		exit(-1);
	}



/** Services *******************************************/
	/* >>> ADMIN interface <<< */
	PTH_CREATE(&serverManager, parse_line, NULL);
	/* >>> POOL MANAGER <<< */
	PTH_CREATE(&poolManager, manage_pool, NULL);
/*******************************************************/


	/* listens for incoming connections; and accepts them */
	listen(primarySocket,SOMAXCONN);
/*	while( 1 ){

		if( $Struct.numbOfClients < MAX_WORKERS ) {
			clientAddr_len = sizeof(clientAddr);
			* secondarySocket is only a temporary holder of the file descriptor *
			secondarySocket = accept(primarySocket,(struct sockaddr *) &clientAddr,(socklen_t*)&clientAddr_len);

			1. write socket id to $Struct
			2. signal thread to fetch socket id

		}
	}
*/



/*
	srand(time(NULL));

	MALL(req,1)
	createInitialServerConditions();

	MALL(threads,WORKERS)

	for (i=0; i<WORKERS; i++) {
		threads[i] = i;
		pthread_create(&threads[i], NULL, processRequestsListTask, (void*)&threads[i]);
    }

	for (i = 0; i<10000; i++){

		r1 = rand () % 5;
		r2 = rand () % 10;

		addRequest(fbyte[r1],sbyte[r2],&request_mutex,&got_request);
		*if (rand() > 3*(RAND_MAX/4)) { used for wasting time. why?
	    	delay.tv_sec = 0;
	    	delay.tv_nsec = 10;
	    	nanosleep(&delay, NULL);
		}*
	}
	sleep(5);
	printf("\nOK. Done!\n");
	*printf("Num Req: %d\n",req_desc->num_requests);

	req = getRequest(&request_mutex);*


	*MALL(req,1)
	req->param1 = 'X';
	req->param2 = &data[3];

	printf("Parametro 1: %c\n",req->param1);
	printf("Parametro 2: %s\n",req->param2);*/

	return 0;

}
