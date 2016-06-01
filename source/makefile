#makefile for test

all:client server midserver
.PHONY:all

client: remotedbapi.c cloudcmd.c taskqueue.c socketapi.c clouddbapi.c
	gcc remotedbapi.c cloudcmd.c taskqueue.c socketapi.c clouddbapi.c -o client

server:servernew.c servercmd.c dbapi.c taskqueue.c serversocket.c 
	gcc servernew.c servercmd.c dbapi.c taskqueue.c serversocket.c  -o server -ltokyocabinet
	
midserver: masterserver.c
	gcc masterserver.c -o masterserver

clean:
	rm client server masterserver *.hdb

