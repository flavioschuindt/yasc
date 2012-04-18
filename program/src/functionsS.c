#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include <prototypesS.h>
#include <newvarS.h>
#include <globalmacros.h>

extern requests_descriptor *req_desc;

/*MUTEX to control access to protected resources*/
pthread_mutex_t request_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

/* global condition variable for our program. assignment initializes it. */
pthread_cond_t  got_request = PTHREAD_COND_INITIALIZER;

void createInitialServerConditions()

{
	MALL(req_desc,1);
	req_desc->first = NULL;
	req_desc->last = NULL;
	req_desc->num_requests = 0;
}

void addRequest(unsigned char param1, unsigned char param2,
				   pthread_mutex_t* p_mutex,
	    		   pthread_cond_t*  p_cond_var)
{
	
	request *req;
	MALL(req,1);
	req->param1 = param1;
	req->param2 = param2;
	req->next = NULL;

	/* Lock the mutex, to assure exclusive access to the list.*/
	/* In this case, exclusive access to req_desc*/
    pthread_mutex_lock(p_mutex);

	if (req_desc->num_requests == 0){
		req_desc->first = req;
		req_desc->last = req;
		req_desc->num_requests = 1;
	}else{
		req_desc->last->next = req;
		req_desc->last = req;
		req_desc->num_requests++;
	}

	/* unlock mutex */
    pthread_mutex_unlock(p_mutex);
    /* signal the condition variable - there's a new request to handle */
    pthread_cond_signal(p_cond_var);

}

request* getRequest(pthread_mutex_t* p_mutex)
{

	request *req;
	MALL(req,1);
	/* lock the mutex, to assure exclusive access to the list */
    pthread_mutex_lock(p_mutex);
	if (req_desc->num_requests > 0){ /*Is there any request on the list?*/
		req = req_desc->first;
		req_desc->first = req->next;
		req_desc->num_requests--;
		if (req_desc->num_requests == 0){
			req_desc->last = NULL;
		}
	}else{
		req = NULL;
	}
	/* unlock mutex */
    pthread_mutex_unlock(p_mutex);

	return req;
		
}

void handleParticularRequest(request *req)
{

	printf("\nParametro 1: %c\n",req->param1);
	printf("\nParametro 2: %c\n",req->param2);
	printf("\nNum Req: %d\n",req_desc->num_requests);
	
}

void* processRequestsListTask(void* data)
{

	request *req;

	MALL(req,1)

	pthread_mutex_lock(&request_mutex);
	
	while(1){
		/*Always stays in a loop getting requests from the list and processing*/
		if (req_desc->num_requests > 0){ /*Is there any requests in the list waiting to be processed?*/

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