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
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>


#include <prototypesS.h>
#include <newvar.h>
#include <globalHeader.h>

#ifdef OWNER
#undef OWNER
#endif
#include <commonS.h>

void createInitialServerConditions() {
	fds_desc->first = NULL;
	fds_desc->last = NULL;
	fds_desc->next = NULL;
	fds_desc->count = 0;
}

int getFDAndUpdateNextFD(){
	int fd;
	fd = fds_desc->next->FD;

	if(fds_desc->count == 0){ /*Is there something to be processed?*/
		fd = -1;
	}else if (fds_desc->count > 1){ /*Only updates if count is higher than one*/
		if (fds_desc->next == fds_desc->last) { /*Is time to rotate?*/
			fds_desc->next = fds_desc->first;
		}else{
			fds_desc->next = fds_desc->next->next;
		}
	}
    return fd;
}

void removeFD(int FDToBeSearched){
	int i;
	FD *fd;
	MALL(fd,1)
	fd = fds_desc->first;
	for (i=0; i<fds_desc->count; i++){
		if (fd->FD == FDToBeSearched){
			/*FD founded. Rearranging the list.*/
			if (fd == fds_desc->first){ /*Was the first one founded?*/
				if (fds_desc->count == 1){ /*Is there only one node in the list?*/
					fds_desc->first = NULL;
					fds_desc->last = NULL;
				}else{
					fd->next->previous = NULL; /*Second is the first now*/
					fds_desc->first = fd->next; /*Second is the first now*/
				}
			}else if(fd == fds_desc->last){ /* Was the last one found? */
				fd->previous->next = NULL; /* Penultimate is the last now */
				fds_desc->last = fd->previous; /* Penultimate is the last now */
			}else{ /* Was founded in the middle of list?*/
				fd->previous->next = fd->next;
				fd->next->previous = fd->previous;
			}
			fds_desc->count--;
			free(fd); /*Remove the node*/
			break;
		}else{
			fd=fd->next;
		}
	}
}

void addFD(int FDToBeAdded){
	FD *fd;
	MALL (fd,1)
	fd->FD = FDToBeAdded;
	fd->next = NULL;
	if (fds_desc->count == 0){
		fd->previous = NULL;
		fds_desc->first = fd;
		fds_desc->last = fd;
	}else{
		fd->previous = fds_desc->last;
		fds_desc->last->next = fd;
		fds_desc->last = fd;
	}
	fds_desc->count++;
    /* signal the condition variable - there's a new FD to handle */
    pthread_cond_signal(&p_cond_var);
}

void handleParticularFD(int fd)
{

	/*P.S.: The stack never has 0 elements, except before the first request.*/

	/*TODO: Implement request process

	if (req->param1 == 'D'){ Is the client trying to send a data?

		STACK_ELEMENT element;

		stack_desc.count++;

		element.operand = req->param2;
		element.next = stack_desc.first;

		stack_desc.first = &element;
	}*/

}

void *processFDsListTask(void *data)
{

	int fd;

	pthread_mutex_lock(&p_mutex);

	while(1){
		/*Always stays in a loop getting FDs from the list and processing*/
		if (fds_desc->count > 0){ /*Is there any FD in the list waiting to be processed?*/

			fd = getFDAndUpdateNextFD();

			if (fd != -1){
				/*A FD was successfully got in the list*/
				pthread_mutex_unlock(&p_mutex); /*Ok, now others threads can access the list and process other requests*/
				/*TODO: Implement what which thread will do*/
				handleParticularFD(fd);
				pthread_mutex_lock(&p_mutex);
			}
		}else{
			/*There is no FD in the FDs' list.
			So, we use condition variables to lock this specific thread and force it to wait until a new FD arrives
			It's important to note that when we use pthread_cond_wait the thread running at this point will
			wait until a new FD is available in the list. Also, in this moment the lock is released and it gives
			access to protected resources to other threads. The return from pthread_cond_wait locks the mutex again. So, we don't
			need to take care of this*/
			 	pthread_cond_wait(&p_cond_var, &p_mutex);
		}
	}
}


void *handle_client ( void *fd ) {
	int FD = (int) fd;

	signal(SIGPIPE,SIG_IGN);	/* instead of handling the signal, we handle write() error */

	while(1) {
		int num[1];
		PACKAGE outPackage, inPackage;

		errno = 0;
		read(FD,(void *)&inPackage,COM_SIZE);
		if( (errno == EAGAIN) || (errno == EWOULDBLOCK) || (errno == ENOTCONN) || (errno == ECONNRESET) ){
			shutdown(FD, SHUT_RDWR);
			close(FD);
			pthread_exit(NULL);
		} else if( errno == ETIMEDOUT ){
			shutdown(FD, SHUT_RDWR);
			close(FD);
			pthread_exit(NULL);
		}

		sscanf(inPackage.num,"%X", (unsigned int *) num);
		fprintf(stdout, "%c\t%d\n", inPackage.msg, *num);

		outPackage.msg = 'V';
		sprintf(outPackage.num,"%X",-2485224);

		errno = 0;
		write(FD,(void *)&outPackage,COM_SIZE);
		if( (errno == EPIPE) || (errno == EAGAIN) || (errno == EWOULDBLOCK) || (errno == ECONNRESET) ){
			shutdown(FD, SHUT_RDWR);
			close(FD);
			pthread_exit(NULL);
		}
	}
}


void *parse_line () {
	int caret=0;
	char line[MAX_LINE], parameter[MAX_LINE];	/* if parameter is set as smaller than MAX_LINE, it is necessary to implement buffer overrun protection !!! */
	char *remainingString;

	while( fgets(line,MAX_LINE,stdin) != NULL ) {

		remainingString = line;
		while( sscanf(remainingString,"%s%n",parameter,&caret) > 0 ) {
			remainingString += caret;	/* moves along the line */

			if( !strcmp(parameter,"M") ) {

				/* print info                      *****************          TO DO           ****************/

			} else if( !strcmp(parameter,"help") ) {
				fprintf(stdout,">> No soup for you!\n");
				/* open help pages                          *************		    TO DO             *************/

			} else if( !strcmp(parameter,"F") ) {
				fprintf(stdout,">> Shutting down.\n\n\n");
				exit(0);

			} else {
				fprintf(stdout,">> Parsing error!\n>> Unknown command \"%s\" ignored.\n>> Try help for assistance.\n",parameter);
			}
		}
	}

	return NULL;
}


void *manage_pool () {
	int i;
	pthread_t *slaves;
	MALL(slaves,MAX_WORKERS);	/* avoids dynamic allocation by having always the maximum size */

	i=1;
	while( i < MIN_WORKERS ) {

		PTH_CREATE(&slaves[i], handle_client, NULL);
		pthread_detach( slaves[i] );

		i++;
	}


/*
1. check rate of increasing work
1.1 if positive, launch more slaves
1.2 if negative, kill slaves
*/


	return NULL;
}
