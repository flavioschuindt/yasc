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
#include <arpa/inet.h>


#include <prototypesS.h>
#include <newvar.h>
#include <globalHeader.h>

#ifdef OWNER
#undef OWNER
#endif
#include <commonS.h>


void createInitialServerConditions () {
	master_pthread_t = pthread_self();
	VRB = 0;

	pthread_mutex_init(&p_mutex, NULL);
	pthread_cond_init(&p_cond_var,NULL);

	clients_desc.first = NULL;
	clients_desc.last = NULL;
	clients_desc.next = NULL;
	clients_desc.count = 0;
}


void master_switch () {
	fprintf(stdout,"\n:: Shutting down.\n\n\n");
	exit(0);
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
				print_client_info();

			} else if( !strcmp(parameter,"HELP") ) {
				FILE *helpFile;
				char file_line[MAX_LINE];
				helpFile = fopen("./doc/ServerHelpFile.txt","r");
				if( helpFile == NULL ) {
					fprintf(stdout,":: Error!\n:: Can't open help file.\n:: Check your working directory.\n");
				} else {
					while( fgets(file_line,MAX_LINE,helpFile) != NULL ) {
						fprintf(stdout,"%s",file_line);
					}
					fclose(helpFile);
				}

			} else if( !strcmp(parameter,"V") ) {
				VRB++;		/* test is done with bitwise AND; true for DBG odd, false for even */
				if( VRB & 1 ) {
					fprintf(stdout,":: Verbose mode ON\n");
				} else {
					fprintf(stdout,":: Verbose mode OFF\n");
				}
			} else if( !strcmp(parameter,"F") ) {
				master_switch();
			} else {
				fprintf(stdout,":: Parsing error!\n:: Unknown command \"%s\" ignored.\n:: Try HELP for assistance.\n",parameter);
			}
		}
	}
	pthread_exit(NULL);
}


void print_client_info () {
	int i, j, caret, TAB=20, END_TAB=80;	/* TABS have to be synchronized with the header */
	CLIENT *client;
	STACK_ELEMENT *aux;

	pthread_mutex_lock(&p_mutex);	/* nothing else changes the list BUT there may be asynchronous printing of some results *
									 * due to the fact that handling clients is done with no lock                           *
									 * try turning Verbose OFF for better, cleaner results                                  */

	client = clients_desc.first;

	fprintf(stdout,"\n _______________________________________________________________________________\n");
	fprintf(stdout,  "|                                                                               |\n");
	fprintf(stdout,  "| Clients connected                                                             |\n");
	fprintf(stdout,  "|_______________________________________________________________________________|\n");
	fprintf(stdout,  "| IP                [Stack]                                                     |\n");
	fprintf(stdout,  "|                                                                               |\n");

	for(i=0; i < clients_desc.count; i++) {		/* for each client */

		caret = fprintf(stdout,"| %s",client->IP);

		while( caret < TAB ) {
			caret += fprintf(stdout," ");
		}


		if( client->stack_desc->count > 0 ) {
			caret += fprintf(stdout,"[");
			aux = client->stack_desc->first;
			caret += fprintf(stdout,"%d",aux->operand);	/* 1st element doesn't have a comma */
			aux = aux->next;

			for( j=1; j < client->stack_desc->count; j++ ) {
				if( caret < END_TAB - 20 ) {
					caret += fprintf(stdout,",%d",aux->operand);
				} else {
					caret += fprintf(stdout,",...");
					break;
				}
				aux = aux->next;
			}
			caret += fprintf(stdout,"]");
		}

		while( caret < END_TAB ) {
			caret += fprintf(stdout," ");
		}
		fprintf(stdout,"|\n");


		client=client->next;
	}

	fprintf(stdout,  "|_______________________________________________________________________________|\n");

	pthread_mutex_unlock(&p_mutex);	/* the RC is not this big, but it helps giving a cleaner output */
}


void *manage_pool () {
	int number_of_workers=0;
	pthread_t slaves[MAX_WORKERS];	/* avoids dynamic allocation by having always the maximum size */

	/* blocks signal so that it doesn't interrupt while attending to a client */
	PTH_SIGMSK(SIG_BLOCK,soft_kill_set);


	while( number_of_workers < MIN_WORKERS ) {

		PTH_CREATE(&slaves[number_of_workers], slaveWork, NULL);
		PTH_DTCH(slaves[number_of_workers]);

		number_of_workers++;
	}

	while(1) {
		sleep(POOL_REFRESH_RATE);

		/* increasing clients */
		while( (clients_desc.count > ((CLIENTS_PER_SLAVE * number_of_workers) + POOL_HYSTERESIS)) && (number_of_workers < MAX_WORKERS) ) {
			PTH_CREATE(&slaves[number_of_workers], slaveWork, NULL);
			PTH_DTCH(slaves[number_of_workers]);	/* allows killing them with no join */
			number_of_workers++;

			if( VRB & 1 ) {
				fprintf(stdout,":: +1 slave.\n");
			}
		}

		/* decreasing clients */
		while( (clients_desc.count < ((CLIENTS_PER_SLAVE * number_of_workers) - POOL_HYSTERESIS))  && (number_of_workers > MIN_WORKERS) ) {
			number_of_workers--;
			pthread_kill(slaves[number_of_workers], SIGINT); /* starts killing the ones with greater index */
		}
	}
	pthread_exit(NULL);
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


void add_client ( int FD, char *IP ) {
	CLIENT *newclient;
	STACK_DESCRIPTOR *stack_desc;

	MALL(newclient,1);
	newclient->fd = FD;
	strcpy(newclient->IP,IP);
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
			if (client == clients_desc.first) {		/* Was the first one found? */

				if (clients_desc.count == 1) {		/* Is there only one node in the list? */
					clients_desc.first = NULL;
					clients_desc.last = NULL;
				} else {
					client->next->previous = NULL;		/* Second is the first now */
					clients_desc.first = client->next;	/* Second is the first now */
				}

			} else if (client == clients_desc.last) {	/* Was the last one found? */
				client->previous->next = NULL;			/* Penultimate is the last now */
				clients_desc.last = client->previous;	/* Penultimate is the last now */

			} else {	/* Was founded in the middle of list? */
				client->previous->next = client->next;
				client->next->previous = client->previous;
			}
			clients_desc.count--;
			resetStack(client->stack_desc); /*Not necessary to create a new function to remove all the stack.
											resetStack already frees all nodes in the list, so we only need
											to free the descriptor of the stack.*/
			free(client->stack_desc); /*Remove the descriptor of the stack*/
			free(client); /* Remove the node */
			break;
		/* FD not found */
		} else {
			client=client->next;
		}
	}
	pthread_mutex_unlock(&p_mutex);

	if( VRB & 1 ) {
		fprintf(stdout,":: -1 client.\n");
	}
	pthread_kill(master_pthread_t, SIGCONT);	/* signals master to accept more clients */
}


void* slaveWork() {
	CLIENT client;
	sigset_t pending_signals;

	pthread_mutex_lock(&p_mutex);
	while(1) {
		/* Always stays in a loop getting FDs from the list and processing */
		if( clients_desc.count > 0 ) { /* Is there any FD in the list waiting to be processed? */
			client = get_client();
			pthread_mutex_unlock(&p_mutex);
			/* Ok, now others threads can access the list and process other requests */

			handle_client(client);

			/* KILLING ZONE *******************************/
			/* it has to be outside the RC !!! */
			/* is it time to die already? ;( */
			if( sigpending(&pending_signals) < 0 ) {
				fprintf(stdout,":: ERROR!\n:: A slave failed to check pending signals.\n:: Killing the slave. Another one shall be issued if necessary.\n");
				break;
			} else if( sigismember(&pending_signals, SIGINT) ) {
				break;
			}
			/***********************************************/

			pthread_mutex_lock(&p_mutex);
		} else {
			pthread_cond_wait(&p_cond_var, &p_mutex);
			/*There is no FD in the FDs' list.
			So, we use condition variables to lock this specific thread and force it to wait until a new FD arrives
			It's important to note that when we use pthread_cond_wait the thread running at this point will
			wait until a new FD is available in the list. Also, in this moment the lock is released and it gives
			access to protected resources to other threads. The return from pthread_cond_wait locks the mutex again. So, we don't
			need to take care of this*/
		}
	}

	if( VRB & 1 ) {
		fprintf(stdout,":: -1 slave.\n");
	}
	pthread_exit(NULL);
}


void handle_client ( CLIENT client ) {
	int num[1];
	PACKAGE outPackage, inPackage;

	signal(SIGPIPE,SIG_IGN);	/* instead of handling the signal, we handle write() error */

	errno = 0;
	read(client.fd,(void *)&inPackage,COM_SIZE);
	if (errno != EWOULDBLOCK){
		if( (errno == EAGAIN) || (errno == ENOTCONN) || (errno == ECONNRESET) || (errno == ETIMEDOUT) ){
			close(client.fd);
			remove_client(client.fd);
		}

/* DEMO */
		/*sscanf(inPackage.num,"%X", (unsigned int *) num);
		fprintf(stdout, "%c\t%d\n", inPackage.msg, *num);*/
/* DEMO */

		switch(inPackage.msg){
			case 'D':
					sscanf(inPackage.num,"%X", (unsigned int *) num);
					outPackage = cmd_D(num,client.stack_desc,outPackage);
					break;
			case '+':
					outPackage = cmd_add(client.stack_desc,outPackage);
					break;
			case '-':
					outPackage = cmd_sub(client.stack_desc,outPackage);
					break;
			case '*':
					outPackage = cmd_mult(client.stack_desc,outPackage);
					break;
			case '/':
					outPackage = cmd_div(client.stack_desc,outPackage);
					break;
			case '%':
					outPackage = cmd_reminder(client.stack_desc,outPackage);
					break;
			case 'R':
					outPackage = cmd_R(client.stack_desc,outPackage);
					break;
			case 'T':
					outPackage = cmd_T(client.stack_desc,outPackage);
					break;
			case 'P':
					outPackage = mountResponsePackage('V', client.stack_desc->count, outPackage);
					break;
			case 'I':
					resetStack(client.stack_desc);
					outPackage = mountResponsePackage('V',OK,outPackage);
					break;
			case 'K':
					close(client.fd);
					remove_client(client.fd);
					return;		/* nothing else to do */
			default:
					outPackage = mountResponsePackage('E',BAD_CMD,outPackage);	/* bad command */
		}

		errno = 0;
		write(client.fd,(void *)&outPackage,COM_SIZE);
		if( (errno == EPIPE) || (errno == EAGAIN) || (errno == ECONNRESET) ){
			close(client.fd);
			remove_client(client.fd);
		}
	}
}
