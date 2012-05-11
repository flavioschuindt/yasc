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
#include <sys/socket.h>


#include <prototypesS.h>
#include <newvar.h>
#include <globalHeader.h>

#ifdef OWNER
#undef OWNER
#endif
#include <commonS.h>


void createInitialServerConditions () {
	pthread_mutex_init(&p_mutex, NULL);
	pthread_cond_init(&p_cond_var,NULL);

	clients_desc.first = NULL;
	clients_desc.last = NULL;
	clients_desc.next = NULL;
	clients_desc.count = 0;
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
				fprintf(stdout,">> Shutting down.\n\n\n");		/*********   incomplete (?) -> detached threads    ****************/
				pthread_exit(NULL);
			} else {
				fprintf(stdout,">> Parsing error!\n>> Unknown command \"%s\" ignored.\n>> Try help for assistance.\n",parameter);
			}
		}
	}

	pthread_exit(NULL);
	return NULL;
}


void *manage_pool () {
	int i;
	pthread_t slaves[MAX_WORKERS];	/* avoids dynamic allocation by having always the maximum size */

	i=0;
	while( i < MIN_WORKERS ) {

		PTH_CREATE(&slaves[i], slaveWork, NULL);
		PTH_DTCH(slaves[i]);

		i++;
	}
	pause();
/*
1. check rate of increasing work
1.1 if positive, launch more slaves
1.2 if negative, kill slaves
*/
	pthread_exit(NULL);
	return NULL;
}


CLIENT get_client () {

	CLIENT client;
	client = *(clients_desc.next);
	if (clients_desc.count > 1){ /* Only updates if count is higher than one */
		if (clients_desc.next == clients_desc.last) { /* Is time to rotate? */
			clients_desc.next = clients_desc.first;
		}else{
			clients_desc.next = clients_desc.next->next;
		}
	}

    return client;
}


void add_client ( int FD ) {
	CLIENT *newclient;
	STACK_DESCRIPTOR *stack_desc;
	MALL(newclient,1);
	newclient->fd = FD;
	newclient->next = NULL;
	/*Create a stack descriptor for this client*/
	MALL(stack_desc,1);
	stack_desc->first = NULL;
	stack_desc->count = 0;

	newclient->stack_desc = stack_desc;

	pthread_mutex_lock(&p_mutex);
	if (clients_desc.count == 0){
		newclient->previous = NULL;
		clients_desc.first = newclient;
		clients_desc.last = newclient;
		clients_desc.next = newclient;
	}else{
		newclient->previous = clients_desc.last;
		clients_desc.last->next = newclient;
		clients_desc.last = newclient;
	}
	clients_desc.count++;
	pthread_mutex_unlock(&p_mutex);
	/* signal the condition variable - there's a new FD to handle */
	pthread_cond_signal(&p_cond_var);
}


void remove_client ( int client_fd ) {			/* !!!!!!!!!!!!!!!! needs to be revised */
	int i;
	CLIENT *client;
	MALL(client,1);

	pthread_mutex_lock(&p_mutex);
	client = clients_desc.first;
	for(i=0; i < clients_desc.count; i++) {
		if(client->fd == client_fd) {
			/* FD found. Rearranging the list. */
			if (client == clients_desc.first) {	/* Was the first one found? */
				if (clients_desc.count == 1) {	/* Is there only one node in the list? */
					clients_desc.first = NULL;
					clients_desc.last = NULL;
				} else {
					client->next->previous = NULL; /*Second is the first now*/
					clients_desc.first = client->next; /*Second is the first now*/
				}
			} else if (client == clients_desc.last) { /* Was the last one found? */
				client->previous->next = NULL; /* Penultimate is the last now */
				clients_desc.last = client->previous; /* Penultimate is the last now */
			} else { /* Was founded in the middle of list? */
				client->previous->next = client->next;
				client->next->previous = client->previous;
			}
			clients_desc.count--;
			free(client); /* Remove the node */
			break;
		} else {
			client=client->next;
		}
	}
	if( clients_desc.count < MAX_CLIENTS ) {
		kill(pid, SIGALRM);
	}
	pthread_mutex_unlock(&p_mutex);
}


/*void create_stack(int fd) {*/
	/*P.S.: The stack never has 0 elements, except before the first request.*/
	/*TODO: Implement request process

	if (req->param1 == 'D'){ Is the client trying to send a data?

		STACK_ELEMENT element;

		stack_desc.count++;

		element.operand = req->param2;
		element.next = stack_desc.first;

		stack_desc.first = &element;
	}*/
/*}*/


void* slaveWork() {
	CLIENT client;
	pthread_mutex_lock(&p_mutex);

	while(1) {
		/* Always stays in a loop getting FDs from the list and processing */
		if( clients_desc.count > 0 ) { /* Is there any FD in the list waiting to be processed? */

			client = get_client();

			pthread_mutex_unlock(&p_mutex); /*Ok, now others threads can access the list and process other requests*/

			handle_client(client);

			pthread_mutex_lock(&p_mutex);

		} else {
			fprintf(stdout,"\nNinguém no server, waiting...\n");
			fflush(stdout);
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


void handle_client ( CLIENT client ) {
	int /*num_bytes=0,*/ num[1],resp;
	PACKAGE outPackage, inPackage;

	signal(SIGPIPE,SIG_IGN);	/* instead of handling the signal, we handle write() error */

	errno = 0;
	read(client.fd,(void *)&inPackage,COM_SIZE);
	if (errno != EWOULDBLOCK){
		if( (errno == EAGAIN) || (errno == ENOTCONN) || (errno == ECONNRESET) || (errno == ETIMEDOUT) ){
			remove_client(client.fd);
			close(client.fd);
		}

		switch(inPackage.msg){
			case 'D':
					sscanf(inPackage.num,"%X", (unsigned int *) num);
					cmd_D(num,client.stack_desc);
					outPackage = mountResponsePackage(V,0,outPackage);
					break;
			case '+':
					if (client.stack_desc->count >= 2){
						cmd_add(client.stack_desc);
						outPackage = mountResponsePackage(V,0,outPackage);
					} else {
						outPackage = mountResponsePackage(E,0,outPackage);
					}
					break;
			case '-':
					if (client.stack_desc->count >= 2){
						cmd_sub(client.stack_desc);
						outPackage = mountResponsePackage(V,0,outPackage);
					} else {
						outPackage = mountResponsePackage(E,0,outPackage);
					}
					break;
			case '*':
					if (client.stack_desc->count >= 2){
						cmd_mult(client.stack_desc);
						outPackage = mountResponsePackage(V,0,outPackage);
					} else {
						outPackage = mountResponsePackage(E,0,outPackage);
					}
					break;
			case '/':
					if (client.stack_desc->count >= 2 && client.stack_desc->first->next->operand != 0){
						/*It has at least two operands and the divisor is not zero*/
						cmd_div(client.stack_desc);
						outPackage = mountResponsePackage(V,0,outPackage);
					} else {
						outPackage = mountResponsePackage(E,0,outPackage);
					}
					break;
			case '%':
					if (client.stack_desc->count >= 2){
						cmd_reminder(client.stack_desc);
						outPackage = mountResponsePackage(V,0,outPackage);
					} else {
						outPackage = mountResponsePackage(E,0,outPackage);
					}
					break;
			case 'R':
					resp = cmd_R(client.stack_desc);
					outPackage = mountResponsePackage(V,resp,outPackage);
					break;
			case 'T':
					resp = cmd_T(client.stack_desc);
					outPackage = mountResponsePackage(V,resp,outPackage);
					break;
			case 'P':
					resp = cmd_P(client.stack_desc);
					outPackage = mountResponsePackage(V,resp,outPackage);
					break;
			case 'I':
					outPackage = mountResponsePackage(V,0,outPackage);
					break;
			case 'K':
					remove_client(client.fd);
					close(client.fd);
					return;
					break;
		}
/* DEMO */
		/*sscanf(inPackage.num,"%X", (unsigned int *) num);
		fprintf(stdout, "%c\t%d\n", inPackage.msg, *num);*/

/* DEMO */

		errno = 0;
		write(client.fd,(void *)&outPackage,COM_SIZE);
		if( (errno == EPIPE) || (errno == EAGAIN) || (errno == ECONNRESET) ){
			remove_client(client.fd);
			close(client.fd);
		}
	}
}

void wakeUp(){
	return;
}

