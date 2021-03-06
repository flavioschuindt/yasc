/*************************************************************
 *  PSis  --  2011 / 2012                                    *
 *  YASC - Yet Another Simple Calculator                     *
 *  _______________________________________________________  *
 *                                                           *
 *  Bruno Santos        nº64956     MEAer                    *
 *  Flávio Schuindt     nº74570     MEEC                     *
 *  _______________________________________________________  *
 *                                                           *
 *  YASC Server  v0.5                                        *
 *     expected arguments                                    *
 *      <port>            specify listening port             *
 *                                                           *
 *     options                                               *
 *      -v                verbose mode                       *
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
#include <arpa/inet.h>
#include <fcntl.h>
#include <signal.h>


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


	/* >>> MASTER thread <<< */
	createInitialServerConditions();

	/* argument parsing; setup */
	if( (argc > 1) && (argc < 4) ) {

		errno = 0;		/* only way of checking over / underflow */
		port = (unsigned int) strtol(argv[1],&endptr,0);
		/* is it a number ? */
		if( (errno == ERANGE) || ((*endptr != '\0') && (endptr != argv[1])) ) {	/* C99 implies that if it isn't ERANGE, it's 0 */
			fprintf(stderr,">> Setup error!\n>> Missing argument <listening port>.\n>> Aborting.\n");
			exit(-1);
		}

		if( (primarySocket = socket(AF_INET,SOCK_STREAM,0)) < 0 ) {
			fprintf(stderr,">> Setup error!\n>> Failed to open socket.\n>> Aborting.\n");
			exit(-1);
		}

		memset(&serverAddr,0, sizeof(serverAddr));
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.s_addr = INADDR_ANY;
		serverAddr.sin_port = htons(port);

		if( bind(primarySocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0 ) {
			fprintf(stderr,">> Setup error!\n>> Failed to bind socket.\n>> Aborting.\n");
			exit(-1);
		}


		if( (argc == 3) && (strcmp(argv[2],"-v") == 0) ) {
			VRB = 1;
			fprintf(stdout,":: Verbose mode ON\n");
		}

	} else {
		fprintf(stderr,">> Setup error!\n>> Bad arguments.\n>> Aborting.\n");
		exit(-1);
	}


	/* every thread access this mask                       *
	 * allowing to handle the signal and thus being killed *
	 * in the same way                                     */
	SIG_EMPTYSET(soft_kill_set);
	SIG_ADDSET(soft_kill_set,SIGINT);


/** Launch Services ************************************/
	/* >>> ADMIN interface <<< */
	PTH_CREATE(&serverManager, parse_line, NULL);
	/* >>> POOL MANAGER <<< */
	PTH_CREATE(&poolManager, manage_pool, NULL);
/*******************************************************/


	/* only for this thread */
	signal(SIGINT,master_switch);	/* currently it only allows for a fancier exit with ctrl+c */


	/* listens for incoming connections; and accepts them */
	listen(primarySocket,SOMAXCONN);


	{
		char IP[INET_ADDRSTRLEN];

		while(1) {

			/* as we don't lock the mutex here, MAX_CLIENTS is not an actual maximum, it has some hysteresis !!! needs to be tested *
			 * scratch that: actually it is, because here is the only place we call add_client()                                    *
			 * most that can happen is being temporarily blocked before reaching the maximum                                        */
			if( clients_desc.count < MAX_CLIENTS ) {
				clientAddr_len = sizeof(clientAddr);
				/* secondarySocket and clientAddr are only temporary holders of the file descriptor and client address */
				secondarySocket = accept(primarySocket,(struct sockaddr *) &clientAddr,(socklen_t*)&clientAddr_len);

				/* allows asynchronous reading from socket */
				fcntl(secondarySocket, F_SETFL, O_NONBLOCK);

				/* gets IP address in string format */
				inet_ntop( AF_INET, &clientAddr.sin_addr.s_addr, IP, INET_ADDRSTRLEN );

				add_client(secondarySocket,IP);


				if( VRB & 1 ) {
					fprintf(stdout,":: +1 client at address %s\n",IP);
				}


			} else {		/* enough clients for now */
				sleep(DOORMAN_DOZE);	/* to delete in favour of a conditional mutex */
				/*pause();*/
			}
		}
	}

	return 0;
}
