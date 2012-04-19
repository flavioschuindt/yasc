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
 *                                                           *
 *************************************************************/
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include <newvar.h>
#include <prototypesS.h>
#include <globalmacros.h>

extern requests_descriptor *req_desc;
/*MUTEX to control access to protected resources*/
extern pthread_mutex_t request_mutex;

/* global condition variable for our program. assignment initializes it. */
extern pthread_cond_t  got_request; 

int main (int argc, char *argv[]){

	/*Create initial server conditions*/

	unsigned char fbyte[5] = {'+','-','*','/','%'};

	unsigned char sbyte[10] = {'0','1','2','3','4','5','6','7','8','9'};

	struct timespec delay;			 /* used for wasting time */

	request *req;

	int r1 = 0,r2 = 0;

	int i = 0;

	pthread_t *threads;

	srand(time(NULL));

	MALL(req,1)

	createInitialServerConditions();

	MALL(threads,WORKERS);

	for (i=0; i<WORKERS; i++) {
		threads[i] = i;
		pthread_create(&threads[i], NULL, processRequestsListTask, (void*)&threads[i]);
    }

	for (i = 0; i<10000; i++){

		r1 = rand () % 5;
		r2 = rand () % 10;

		addRequest(fbyte[r1],sbyte[r2],&request_mutex,&got_request);
		/*if (rand() > 3*(RAND_MAX/4)) { used for wasting time. why?
	    	delay.tv_sec = 0;
	    	delay.tv_nsec = 10;
	    	nanosleep(&delay, NULL);
		}*/
	}
	sleep(5);
	printf("\nOK. Done!\n");
	/*printf("Num Req: %d\n",req_desc->num_requests);

	req = getRequest(&request_mutex);*/


	/*MALL(req,1)
	req->param1 = 'X';
	req->param2 = &data[3];

	printf("Parametro 1: %c\n",req->param1);
	printf("Parametro 2: %s\n",req->param2);*/

	return 0;

}