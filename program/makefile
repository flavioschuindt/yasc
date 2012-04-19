#############################################################
#  PSis  --  2011 / 2012                                    #
#  YASC - Yet Another Simple Calculator                     #
#  _______________________________________________________  #
#                                                           #
#  Bruno Santos        nº64956     MEAer                    #
#  Flávio Schuindt     nº74570     MEEC                     #
#  _______________________________________________________  #
#                                                           #
#  MAKEFILE   v1.1                                          #
#                                                           #
#     compile:                  make S(erver) / C(lient)    #
#     quick run:                make run*                   #
#     clean:                    make clean                  #
#     clean for distribution:   make distclean              #
#                                                           #
#############################################################

#############################################################
# Compilation; set DEBUG to -ggdb if needed
vpath %.c src/
vpath %.h inc/

CC = gcc
CFLAGS = -Wall -ansi -pedantic -c -I inc/ $(DEBUG) -o
LFLAGS = -Wall -ansi -pedantic $(DEBUG) -pthread -o

.c.o:
	$(CC) $(CFLAGS) obj/$@ $<


# macros and vpath don't work well with *.o's !?
S: yascS.o functionsS.o
	$(CC) $(LFLAGS) bin/yascS.exe obj/yascS.o obj/functionsS.o

C: yascC.o functionsC.o
	$(CC) $(LFLAGS) bin/yascC.exe obj/yascC.o obj/functionsC.o


# .c.o
yascS.o: yascS.c prototypesS.h

yascC.o: yascC.c prototypesC.h commonC.h


functionsC.o: functionsC.c prototypesC.h commonC.h

functionsS.o: functionsS.c prototypesS.h


#############################################################
# Quick Run
runS:
	clear
	./bin/yascS.exe

runC:
	clear
	./bin/yascC.exe -s NNNN PPPP -f init/cmd.txt -g -l

#############################################################
# Maintenance
clean:
	rm -f obj/*.o bin/*.exe

distclean:
	rm -f obj/*.o
