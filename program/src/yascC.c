/*************************************************************
 *  PSis  --  2011 / 2012                                    *
 *  YASC - Yet Another Simple Calculator                     *
 *  _______________________________________________________  *
 *                                                           *
 *  Bruno Santos        nº64956     MEAer                    *
 *  Flávio Schuindt     nº74570     MEEC                     *
 *  _______________________________________________________  *
 *                                                           *
 *  YASC Client  v0.9                                        *
 *     expected arguments                                    *
 *      <name> <port>     specify server                     *
 *                                                           *
 *     options                                               *
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
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>


#define OWNER	/* owner of global variables */
#include <commonC.h>
#include <prototypesC.h>
#include <globalHeader.h>


int main( int argc, char *argv[] ) {

	int i, f=0;
	char *fileName, line[MAX_LINE];
	time_t now;
	struct tm *local;


	FILE *fin=stdin;	/* defaults to shell */
	fout = stdout;		/* defaults to shell */

	DBG = 0;

	/* argument parsing; setup */
	if( argc > 2 ) {
		name = argv[1];		/* good name and port is only checked when one issues a 'I' command */
		port = argv[2];

		i = 3;
		while( i < argc ) {

			/* -f */
			if( (strcmp(argv[i],"-f") == 0) && (f == 0) ) {

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
					fprintf(stderr,":: Setup error!\n:: File \"%s\" not found.\n", argv[i+1]);
					fout = stdout;		/* defaults to shell */
				}

			/* -l */
			} else if( strcmp(argv[i],"-l") == 0 ) {		/* repeated use of the flag won't undo the effect;                       *
															 * not very useful if the command line is used, but possible nonetheless */
				fout = fopen("log.txt","a");
				if( fout == NULL ) {
					fprintf(stderr,":: Setup error!\n:: Can't open log file.\n:: Redirecting output to \"stdout\".");
					fout = stdout;
				} else {
					fprintf(fout,":: Silent mode ON");
				}

			/* -g */
			} else if( strcmp(argv[i],"-g") == 0 ) {		/* repeated use of the flag won't undo the effect */
				DBG=1;

			/* invalid argument */
			} else {
				fprintf(stderr,":: Setup error!\n:: Invalid argument \"%s\" ignored.\n", argv[i]);
			}

			i++;
		}
	} else {
		fprintf(stderr,">> Setup error!\n>> Missing hostname and port.");
		exit(0);
	}

	if( DBG & 1 ) {
		fprintf(fout,":: Debug mode ON\n");
	}

	/*signal(SIGALRM,timeout_handler);*/

	init = 0;

	/* reads successive lines from source; repeats at the end of each set of instruction */
	while( fgets(line,MAX_LINE,fin) != NULL ) {
		parse_line(line);
	}

	fclose(fin);
	fclose(fout);

	return 0;
}
