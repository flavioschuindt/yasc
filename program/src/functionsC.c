/*************************************************************
 *  PSis  --  2011 / 2012                                    *
 *  YASC - Yet Another Simple Calculator                     *
 *  _______________________________________________________  *
 *                                                           *
 *  Bruno Santos        nº64956     MEAer                    *
 *  Flávio Schuindt     nº74570     MEEC                     *
 *  _______________________________________________________  *
 *                                                           *
 *  Client Functions' declaration                            *
 *                                                           *
 *************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>

#ifdef OWNER
#undef OWNER
#endif
#include <commonC.h>

#include <prototypesC.h>
#include <globalHeader.h>
#include <newvar.h>


void parse_line (char *string){

	int num, caret=0;
	char parameter[MAX_LINE];	/* if parameter is set as smaller than MAX_LINE, it is necessary to implement buffer overrun protection !!! */
	char *endptr, *remainingString=string;


	while( sscanf(remainingString,"%s%n",parameter,&caret) > 0 ) {
		remainingString += caret;	/* moves along the line */

		/* Evaluates parameter; if it is a number, 'D' command is implicit */
		errno = 0;		/* only way of checking over / underflow */
		num = (int) strtol(parameter,&endptr,0);

		/*
		 * CHECK FOR NUMBER
		 */
		if( errno == ERANGE ) {	/* condition of over / underflow */
			fprintf(fout,":: Parsing error!\n:: Number out of range. Use only integers between %ld and %ld.\n:: Try help for assistance.\n",LONG_MIN,LONG_MAX);

		} else if( *endptr == '\0' ) {	/* C99 implies that if it isn't ERANGE, it's 0 */

			handleRequest( 'D', num );	/* communicates with server; 'D' command is implicit whenever a number is detected */

		/* rejects number if it isn't surrounded by white-space exceptions are: '+' and '-' before the number */
		} else if( (*endptr != '\0') && (endptr != parameter) ) {
			fprintf(fout,">> Parsing error!\n>> Invalid number. Use only integers between %ld and %ld.\n>> Try help for assistance.\n",LONG_MIN,LONG_MAX);

		/*
		 * IF IT IS NOT A NUMBER
		 */
		} else if(  (!strcmp(parameter,"+")) || (!strcmp(parameter,"-")) || (!strcmp(parameter,"*")) ||
					(!strcmp(parameter,"/")) || (!strcmp(parameter,"%")) || (!strcmp(parameter,"R")) ||
					(!strcmp(parameter,"T")) || (!strcmp(parameter,"P")) ) {

			handleRequest( parameter[0], 0 );	/* communicates with server; Data=0 for padding */

		} else if( !strcmp(parameter,"I") ) {
			init_session();	/* establishes connection with server */

		} else if( !strcmp(parameter,"K") ) {
			end_session(0);	/* ends connection with server; 0 enables extra output that is relevant here */
			alarm(0);	/* no sense in waiting for idle after 'K' */

		} else if( parameter[0] == ';' ) {	/* end of effective commands (anything past that is interpreted as comments until next NL) */
			break;

		} else if( !strcmp(parameter,"G") ) {
			DBG++;		/* test is done with bitwise AND; true for DBG odd, false for even */
			if( DBG & 1 ) {
				fprintf(fout,":: Debug mode ON\n");
			} else {
				fprintf(fout,":: Debug mode OFF\n");
			}

		} else if( !strcmp(parameter,"HELP") ) {
			FILE *helpFile;
			char line[MAX_LINE];
			helpFile = fopen("./doc/ClientHelpFile.txt","r");
			if( helpFile == NULL ) {
				fprintf(fout,":: Error!\n:: Can't open help file.\n:: Check your working directory.\n");
			} else {
				while( fgets(line,MAX_LINE,helpFile) != NULL ) {
					fprintf(stdout,"%s",line);
				}
			}
			fclose(helpFile);

		} else if( !strcmp(parameter,"EXIT") ) {
			end_session(1);		/* any integer is fine as long as it is not 0 */
			close(clientSocket);
			fprintf(fout,":: Completed successfully\n\n\n");
			fclose(fout);
			exit(0);

		} else {
			fprintf(fout,":: Parsing error!\n:: Unknown command \"%s\" ignored.\n:: Try HELP for assistance.\n",parameter);
		}
	}
}


void handleRequest ( char Req, int Data ) {

	int returningData[1];
	PACKAGE outPackage, inPackage;

	signal(SIGPIPE,SIG_IGN);	/* instead of handling the signal, we handle write() error */

	outPackage.msg = Req;
	sprintf(outPackage.num,"%X",Data);

	/* synchronous handling of requests; read() blocks the client until there is an answer to read */
	errno = 0;
	write(clientSocket,(void *)&outPackage,COM_SIZE);
	if( (errno == EPIPE) || (errno == EAGAIN) || (errno == EWOULDBLOCK) || (errno == ECONNRESET) ){
		close(clientSocket);
		fprintf(fout, ":: Error!\n:: No server connection.\n:: Try 'I'.\n");
	} else {
		errno = 0;
		read(clientSocket,(void *)&inPackage,COM_SIZE);
		if( (errno == EAGAIN) || (errno == EWOULDBLOCK) || (errno == ENOTCONN) || (errno == ECONNRESET) ){
			close(clientSocket);
			fprintf(fout, ":: Error!\n:: No server connection.\n:: Try 'I'.\n");
		} else if( errno == ETIMEDOUT ){

			close(clientSocket);
			fprintf(fout, ":: Error!\n:: Timed out.\n:: Try again.\n");
		} else {
			sscanf(inPackage.num,"%X", (unsigned int *) returningData);	/* converts string (hexadecimal integer) to normal integer */
			alarm(TIME_OUT);
			if( DBG & 1 ) {
				/*if( Req == 'D' ) {
					fprintf(fout, "DEBUG:\t%c%d=> : =>%c\n", Req, Data, inPackage.msg);
				} else if( (Req == 'R') || (Req == 'T') || (Req == 'P') ){*/
				fprintf(fout, "DEBUG:\t%c=> : =>%c%d\n", Req, inPackage.msg, *returningData);
				/*} else {
					fprintf(fout, "DEBUG:\t%c=> : =>%c\n", Req, inPackage.msg);
				}*/
			} else if( ((Req == 'R') || (Req == 'T') || (Req == 'P')) && (inPackage.msg == 'V') ) {
				fprintf(fout, ":: %d\n", *returningData);

			} else if( inPackage.msg == 'E' ) {
				fprintf(fout, ":: Error!\n:: \"%c\" Command failed with error code %d.\n", Req, *returningData);
			} else if( inPackage.msg == 'I' ) {
				fprintf(fout, ":: Server error!\n:: Stack reinitialized.\n");
			}
		}
	}
}


void init_session () {

	int gai_result;
	PACKAGE outPackage, inPackage;

	if( (clientSocket = socket(AF_INET,SOCK_STREAM,0)) < 0 ) {
		fprintf(stderr,">> Error!\n>> Failed to open socket.\n");
		exit(-1);
	}

	memset(&hints,0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;	/* IPV4 or IPV6 */
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_CANONNAME;

	if( (gai_result = getaddrinfo(name, port,&hints,&server)) != 0 ) {
		fprintf(stderr,">> Error!\n>> %s\n", gai_strerror(gai_result));
		exit(-1);
	}


	for( pntAddr = server; pntAddr != NULL; pntAddr = pntAddr->ai_next ) {

		if( connect(clientSocket, (struct sockaddr *)pntAddr->ai_addr, pntAddr->ai_addrlen) != -1 ) {
			break;

		} else if(pntAddr->ai_next==NULL){
			fprintf(fout,":: Error!\n:: Failed to establish connection\n");
		}

	}

	signal(SIGPIPE,SIG_IGN);

	outPackage.msg = 'I';
	sprintf(outPackage.num,"%X",0);	/* padding */

	errno = 0;
	write(clientSocket,(void *)&outPackage,COM_SIZE);
	if( (errno == EPIPE) || (errno == EAGAIN) || (errno == EWOULDBLOCK) || (errno == ECONNRESET) ){
		close(clientSocket);
		fprintf(fout, ":: Error!\n:: No server connection.\n:: Try again.\n");
	} else {
		read(clientSocket,(void *)&inPackage,COM_SIZE);
		if( (errno == EAGAIN) || (errno == EWOULDBLOCK) || (errno == ENOTCONN) || (errno == ECONNRESET) ){
			close(clientSocket);
			fprintf(fout, ":: Error!\n:: No server connection.\n:: Try again.\n");
		} else {
			if( DBG & 1 ) {
				fprintf(fout, "DEBUG:\tI=> : =>%c\n", inPackage.msg);

			} /*else if( inPackage.msg == 'E' ) {*/								/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
				/*fprintf(fout, ":: Error!\n:: Rejected connection.\n");*/		/* probably doesn't make sense to leave these else's here; dependes on server implementation */

			/*} else if( inPackage.msg == 'I' ) {
				fprintf(fout, ":: Server error!\n");
			}

			if( inPackage.msg == 'V' ) {*/
			alarm(TIME_OUT);	/* only set if there is connection */
			/*}*/
		}
	}
}


void end_session ( int mode ) {

	PACKAGE outPackage;

	signal(SIGPIPE,SIG_IGN);
	outPackage.msg = 'K';
	sprintf(outPackage.num,"%X",0);	/* padding */

	errno = 0;
	write(clientSocket,(void *)&outPackage,COM_SIZE);
	if( (errno == EPIPE) || (errno == EAGAIN) || (errno == EWOULDBLOCK) || (errno == ECONNRESET) ){
		if( mode == 0 ) {		/* if it is called from within 'EXIT' procedure, it assumes that this error is normal; ex. 'K EXIT' shall not issue this message */
			fprintf(fout, ":: Error!\n:: Failed to contact server.\n");
		}
	} else 	if( DBG & 1 ) {
		fprintf(fout, "DEBUG:\tK=>\n" );
	}

	close(clientSocket);
}


void timeout_handler () {

	signal(SIGALRM, SIG_IGN);
	end_session(1);
	fprintf(fout,TIME_OUT_MSG);
	signal(SIGALRM,timeout_handler);
}
