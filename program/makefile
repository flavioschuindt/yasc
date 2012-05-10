#############################################################
#  PSis  --  2011 / 2012                                    #
#  YASC - Yet Another Simple Calculator                     #
#  _______________________________________________________  #
#                                                           #
#  Bruno Santos        nº64956     MEAer                    #
#  Flávio Schuindt     nº74570     MEEC                     #
#  _______________________________________________________  #
#                                                           #
#  MAKEFILE   v1.2                                          #
#                                                           #
#     compile:                  make [S(erver) / C(lient)]  #
#     quick run:                make run*                   #
#     clean:                    make clean                  #
#     clean for distribution:   make distclean              #
#                                                           #
#############################################################

#############################################################
# Compilation
vpath %.c src/
vpath %.h inc/

CC = gcc
# Libs: -pthread: threads; -lm: math
LIBS = -pthread
#DEBUG = -ggdb
WEXTRA = -Wextra

CFLAGS = -Wall $(WEXTRA) -ansi -pedantic -D_POSIX_SOURCE $(DEBUG) $(LIBS) -c -I inc/ -o
LFLAGS = -Wall $(WEXTRA) -ansi -pedantic -D_POSIX_SOURCE $(DEBUG) $(LIBS) -o

.c.o:
	$(CC) $(CFLAGS) obj/$@ $<


# macros and vpath don't work well with *.o's !?
all: C S

S: yascS.o functionsS.o commandsHandler.o
	$(CC) $(LFLAGS) bin/yascS.exe obj/yascS.o obj/functionsS.o obj/commandsHandler.o

C: yascC.o functionsC.o
	$(CC) $(LFLAGS) bin/yascC.exe obj/yascC.o obj/functionsC.o



yascS.o: yascS.c prototypesS.h commonS.h globalHeader.h newvar.h

yascC.o: yascC.c prototypesC.h commonC.h globalHeader.h


functionsC.o: functionsC.c prototypesC.h commonC.h globalHeader.h newvar.h

functionsS.o: functionsS.c prototypesS.h commonS.h globalHeader.h newvar.h

commandsHandler.o: commandsHandler.c prototypesS.h newvar.h


#############################################################
# Quick Run
runS:
	clear
	./bin/yascS.exe 50000

runC:
	clear
	./bin/yascC.exe 10.24.32.112 50000 -g

#############################################################
# Maintenance
clean:
	rm -f obj/*.o bin/*.exe

distclean:
	rm -f obj/*.o
