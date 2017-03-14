#ifndef MULTI-LOOKUP_H
#define MULTI-LOOKUP_H

#include <pthread>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "util.h"

#define MAX_INPUT_FILES 10
#define MAX_RESOLVER_THREADS 10
#define MIN_RESOLVER_THREADS 2
#define MAX_NAME_LENGTH 1025
#define MAX_IP_LENGTH INET6_ADDRSTRLEN
#define MINARGS 3
#define USAGE "<inputFilePath> <outputFilePath>"
#define INPUTFS "%1024s"

typedef struct thread {
	queue* hostnames;
	FILE* threadFile;
	pthread_mutex_t* bufferMutex;
	pthread_mutex_t* openMutex;  
};

void* request(void* id);
void* resolve(void* id);





#endif
