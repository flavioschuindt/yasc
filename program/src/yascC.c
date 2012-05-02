/*************************************************************
 *  PSis  --  2011 / 2012                                    *
 *  YASC - Yet Another Simple Calculator                     *
 *  _______________________________________________________  *
 *                                                           *
 *  Bruno Santos        nº64956     MEAer                    *
 *  Flávio Schuindt     nº74570     MEEC                     *
 *  _______________________________________________________  *
 *                                                           *
 *  YASC Client  v0.1                                        *
 *     order independent options                             *
 *      -s name port      specify server                     *
 *      -f "*.txt"        open batch file                    *
 *                          (txt extension not enforced)     *
 *      -g                start in debug mode                *
 *      -l                log: suppress output               *
 *                          (generates log.txt)              *
 *                                                           *
 *************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
/*#include <arpa/inet.h>*/


#define OWNER	/* owner of global variables */
#include <commonC.h>
#include <prototypesC.h>



int main( int argc, char *argv[] ) {

	int i=1, s=0, f=0, port, clientSocket;
	char *fileName, *endptr, line[MAX_LINE];
	time_t now;
	struct tm *local;
	struct hostent *server;
	struct sockaddr_in serverAddr;
	FILE *fin=stdin;	/* defaults to shell */

	fout = stdout;		/* defaults to shell */
	DBG = 0;


	/* argument parsing; setup */
	if( argc > 1 ) {

		while( i < argc ) {

			/* -s */
			if( (strcmp(argv[i],"-s") == 0) && (s == 0) ) {		/* UNDER CONSTRUCTION */

				errno = 0;		/* only way of checking over / underflow */
				port = (int) strtol(argv[i+2],&endptr,0);
				server = gethostbyname(argv[i+1]);

				if ( (*endptr == '\0') && (errno == 0) && (server != NULL) ) {	/* valid hostname and port */
						s=1;	/* -s can be used successfully only once */
						i+=2;	/* jumps the name and port argument; but not if the file is not valid */
				} else if( server == NULL ) {
					fprintf(stderr,">> Setup error!\n>> Invalid Hostname.\n");
				} else {
					fprintf(stderr,">> Setup error!\n>> Invalid Port.\n");
				}

			/* -f */
			} else if( (strcmp(argv[i],"-f") == 0) && (f == 0) ) {

				fin = fopen(argv[i+1],"r");

				if( fin != NULL ) {
					f=1;	/* -f can be used successfully only once */
					fileName = argv[i+1];
					now = time((time_t *)NULL);
					local = localtime(&now);
					fprintf(fout,"=====================================\n");
					fprintf(fout," Logging output for \"%s\"\n",fileName);
					fprintf(fout," %d/%d/%d\t--\t%d:%d:%d\n", local->tm_mday, local->tm_mon, local->tm_year + 1900, local->tm_hour, local->tm_min, local->tm_sec);
					fprintf(fout,"=====================================\n\n");
					i++;	/* jumps the name argument; but not if the file isn't valid */
				} else {
					fprintf(stderr,">> Setup error!\n>> File \"%s\" not found.\n", argv[i+1]);
				}


			/* -l */
			} else if( strcmp(argv[i],"-l") == 0 ) {		/* repeated use of the flag won't undo the effect;                       *
															 * not very useful if the command line is used, but possible nonetheless */
				fout = fopen("log.txt","a");
				if( fout == NULL ) {
					fprintf(stderr,">> Setup error!\n>> Can't open log file.\n>> Redirecting output to \"stdout\".");
					fout = stdout;
				}


			/* -g */
			} else if( strcmp(argv[i],"-g") == 0 ) {		/* repeated use of the flag won't undo the effect */
				DBG=1;


			/* invalid argument */
			} else {
				fprintf(stderr,">> Setup error!\n>> Invalid argument \"%s\" ignored.\n", argv[i]);
			}


			i++;
		}
	}
/**************************** if(s==0) => bad hostname or port, ask for another; option to abort */


	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if( clientSocket > 0 ) {

		bzero( (char *)&serverAddr, sizeof(serverAddr));
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(port);
		bcopy( (char *)server->h_addr_list[0], (char *)&serverAddr.sin_addr.s_addr, server->h_length);


		if( connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
			fprintf(stderr,">> Setup error!\n>> Can't establish connection with server.\n");
			return 0;
		}

	} else {
		printf(">> Setup error!\n>> Can't open socket.\n");
	}




	if( DBG & 1 ) {
		fprintf(fout,DBG_ON);
	}


	/* reads successive lines from source; repeats at the end of each set of instruction */
	while( fgets(line,MAX_LINE,fin) != NULL ) {
		parse_line(line);
	}


	return 0;
}
