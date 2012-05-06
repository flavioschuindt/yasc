/*************************************************************
 *  PSis  --  2011 / 2012                                    *
 *  YASC - Yet Another Simple Calculator                     *
 *  _______________________________________________________  *
 *                                                           *
 *  Bruno Santos        nº64956     MEAer                    *
 *  Flávio Schuindt     nº74570     MEEC                     *
 *  _______________________________________________________  *
 *                                                           *
 *  Server's functions                                       *
 *                                                           *
 *************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include <prototypesS.h>
#include <newvar.h>
#include <globalHeader.h>

#ifdef OWNER
#undef OWNER
#endif
#include <commonS.h>


void createInitialServerConditions() {
	pthread_mutex_init( &request_mutex, NULL);
	pthread_cond_init( &got_request, NULL);

	req_desc.first = NULL;
	req_desc.last = NULL;
	req_desc.num_requests = 0;

	stack_desc.first = NULL;
	stack_desc.count = 0;
}

void addRequest(unsigned char param1, unsigned char param2, pthread_mutex_t *p_mutex, pthread_cond_t *p_cond_var) {
	REQUEST req;
	req.param1 = param1;
	req.param2 = param2;
	req.next = NULL;

	/* Lock the mutex, to assure exclusive access to the list.*/
	/* In this case, exclusive access to req_desc*/
    pthread_mutex_lock(p_mutex);

	if (req_desc.num_requests == 0){
		req_desc.first = &req;
		req_desc.last = &req;
	}else{
		req_desc.last->next = &req;
		req_desc.last = &req;
	}
	req_desc.num_requests++;

	/* unlock mutex */
    pthread_mutex_unlock(p_mutex);
    /* signal the condition variable - there's a new request to handle */
    pthread_cond_signal(p_cond_var);

}

REQUEST *getRequest(pthread_mutex_t *p_mutex) {

	REQUEST *req;
	MALL(req,1);
	/* lock the mutex, to assure exclusive access to the list */
    pthread_mutex_lock(p_mutex);
	if (req_desc.num_requests > 0){ /*Is there any request on the list?*/
		req = req_desc.first;
		req_desc.first = req->next;
		req_desc.num_requests--;
		if (req_desc.num_requests == 0){
			req_desc.last = NULL;
		}
	}else{
		req = NULL;
	}
	/* unlock mutex */
    pthread_mutex_unlock(p_mutex);

	return req;

}

void handleParticularRequest(REQUEST *req)
{

	printf("\nParametro 1: %c\n",req->param1);
	printf("\nParametro 2: %c\n",req->param2);
	printf("\nNum Req: %d\n",req_desc.num_requests);

	/*P.S.: The stack never has 0 elements, except before the first request.*/

	if (req->param1 == 'D'){ /*Is the client trying to send a data?*/

		STACK_ELEMENT element;

		stack_desc.count++;

		element.operand = req->param2;
		element.next = stack_desc.first;

		stack_desc.first = &element;
	}

}

void *processRequestsListTask(void *data)
{

	REQUEST *req;
	MALL(req,1)

	pthread_mutex_lock(&request_mutex);

	while(1){
		/*Always stays in a loop getting requests from the list and processing*/
		if (req_desc.num_requests > 0){ /*Is there any requests in the list waiting to be processed?*/

			req = getRequest(&request_mutex);

			if (req != NULL){
				/*A request was successfully got in the list*/
				pthread_mutex_unlock(&request_mutex); /*Ok, now others threads can access the list and process other requests*/
				handleParticularRequest(req);
				free(req); /*The request was successfully processed and now it doesn't exist in the memory anymore*/
				pthread_mutex_lock(&request_mutex);
			}
		}else{
			/*There is no request in the requests' list.
			So, we use condition variables to lock this specific thread and force it to wait until a new request arrives
			It's important to note that when we use pthread_cond_wait the thread running at this point will
			wait until a new request is available in the list. Also, in this moment the lock is released and it gives
			access to protected resources to other threads. The return from pthread_cond_wait locks the mutex again. So, we don't
			need to take care of this*/
			 	pthread_cond_wait(&got_request, &request_mutex);
		}
	}
}

void *handleClient ( void *fd ) {
	int FD = (int) fd;

	while(1) {
		unsigned int *num;
		PACKAGE outPackage, inPackage;

		MALL(num,1);

		read(FD,(void *)&inPackage,COM_SIZE);
		sscanf(inPackage.num,"%X",num);
		fprintf(stdout, "%c\t%d\n", inPackage.msg, *num);

		outPackage.msg = 'V';
		sprintf(outPackage.num,"%X",-2485224);
		write(FD,(void *)&outPackage,COM_SIZE);
	}

}