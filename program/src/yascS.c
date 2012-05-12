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
	sigset_t set;
	struct sockaddr_in serverAddr, clientAddr;
	char *endptr;
	pthread_t poolManager, serverManager;


	/* >>> MASTER thread <<< */
	createInitialServerConditions();
	pid = getpid();
	/* argument parsing; setup */
	if( argc == 2 ) {

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

	} else {
		fprintf(stderr,">> Setup error!\n>> Too many arguments.\n>> Aborting.\n");
		exit(-1);
	}


	/* every thread inherits this mask                 *
	 * allow handling the signal and thus being killed */
	SIG_EMPTYSET(set);
	SIG_ADDSET(set,SIGUSR1);
	PTH_SIGMSK(set);
	/* every slave inherits this handler */
	signal(SIGUSR1,thread_suicide);
	/* to kill threads call 'pthread_kill(pthread_t *, SIGUSR1);' */


/** Launch Services ************************************/
	/* >>> ADMIN interface <<< */
	PTH_CREATE(&serverManager, parse_line, NULL);
	/* >>> POOL MANAGER <<< */
	PTH_CREATE(&poolManager, manage_pool, NULL);
/*******************************************************/


	/* listens for incoming connections; and accepts them */
	listen(primarySocket,SOMAXCONN);

	while(1) {

		if( clients_desc.count < MAX_CLIENTS ) {	/* as we don't lock the mutex here, MAX_CLIENTS is not an actual maximum, it has some hysteresis !!! needs to be tested */
			clientAddr_len = sizeof(clientAddr);
			/* secondarySocket is only a temporary holder of the file descriptor */
			secondarySocket = accept(primarySocket,(struct sockaddr *) &clientAddr,(socklen_t*)&clientAddr_len);

			/* allows assynchronous reading from socket */
			fcntl(secondarySocket, F_SETFL, O_NONBLOCK);

			add_client(secondarySocket);

		} else {		/* enough clients for now */
			pause();
		}
	}


	PTH_JOIN(serverManager,NULL);
	PTH_JOIN(poolManager,NULL);

	return 0;

}
