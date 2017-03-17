#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "util.h"
#include "queue.h"
#include "multi-lookup.h"

// Global Variables

queue hosts;

pthread_mutex_t queue_lock;
pthread_mutex_t out_lock;

int requestsRemaining = 1;

/*
 * --------------------
 *         MAIN
 * --------------------
 */
 
int main(int argc, char *argv[]) {
    	//ERROR-CHECKING

    	//Number of Arguments
    	if (argc < MINARGS) {
		fprintf(stderr, "Not enough arguments: %d\n", (argc - 1));
		fprintf(stderr, "Usage:\n %s %s\n", argv[0], USAGE);
		return EXIT_FAILURE;
    	}
	
	//Number of Input Files
	int numThreads = argc - 2;
        if (numThreads > MAX_INPUT_FILES) {
		fprintf(stderr, "Too many input files: %d\n", (argc - 2));
		fprintf(stderr, "Maximum number of input files: %d\n", MAX_INPUT_FILES);
		return EXIT_FAILURE;
	}

	//Number of Resolver Threads
	int numResolvers = MIN_RESOLVER_THREADS;
	if (numResolvers > MAX_RESOLVER_THREADS) {
		fprintf(stderr, "Too many resolver threads: %d\n", numResolvers);
		fprintf(stderr, "Maximum number of resovler threads: %d\n", MAX_RESOLVER_THREADS);
		return EXIT_FAILURE;
	} else if (numResolvers < MIN_RESOLVER_THREADS) {
		fprintf(stderr, "Not enough resolver threads: %d\n", numResolvers);
		fprintf(stderr, "Minimum number of resolver threads: %d\n", MIN_RESOLVER_THREADS);
		return EXIT_FAILURE;
	}

	
	pthread_t requestThreads[numThreads];
	pthread_t resolveThreads[numResolvers];

	int rc;

	pthread_mutex_init(&queue_lock, NULL);
	pthread_mutex_init(&out_lock, NULL);

	/* Initialize Queue */
    	if(queue_init(&hosts, -1) == QUEUE_FAILURE){
		fprintf(stderr,
			"error: queue_init failed!\n");
    	}

	for (int i = 0; i < numThreads; i++) {
		rc = pthread_create(&(requestThreads[i]), NULL, request, (void*)argv[i + 1]);
		if (rc) {
			printf("pthread_create() error on requests.");
			exit(EXIT_FAILURE);
		}
	}

	for (int i = 0; i < numResolvers; i++) {
		rc = pthread_create(&(resolveThreads[i]), NULL, resolve, (void*)argv[argc - 1]);
		if (rc) {
			printf("pthread_create() error on resolutions.");
			exit(EXIT_FAILURE);
		}
	}


	for (int i = 0; i < numThreads; i++) {
		pthread_join(requestThreads[i], NULL);
	}
	
	requestsRemaining = 0;

	for (int i = 0; i < numResolvers; i++) {
		pthread_join(resolveThreads[i], NULL);
	}

	printf("Thread creation complete.\n");

	queue_cleanup(&hosts);
	pthread_mutex_destroy(&queue_lock);
	pthread_mutex_destroy(&out_lock);

	return 0;
}


/*
 * --------------------
 *   HELPER FUNCTIONS
 * --------------------
 */

void* request(void* threadid) {
	FILE* inFile = fopen(threadid, "r");
	char hostname[MAX_NAME_LENGTH];
	char* payload_in;

	if (!inFile) {
		perror("Error opening input file.\n");
		return EXIT_FAILURE;
	}

	/* Scan in domain name */
	while (fscanf(inFile, INPUTFS, hostname) > 0) {
		payload_in = malloc(MAX_NAME_LENGTH);
		strncpy(payload_in, hostname, MAX_NAME_LENGTH);
		
		/* Lock the queue*/
		pthread_mutex_lock(&queue_lock);

		/* Test for full queue */
	    	while(queue_is_full(&hosts)){
			/* Unlock and wait if queue is full */
			/*fprintf(stderr,
				"queue_is_full reports that "
				"the queue is full\n");*/
			pthread_mutex_unlock(&queue_lock);
			usleep(rand() % 100 + 1);
			pthread_mutex_lock(&queue_lock);
	    	}

		/* Push hostname to queue */
		if(queue_push(&hosts, payload_in) == QUEUE_FAILURE) {
			fprintf(stderr,	"error: queue_push failed!\n");
				
		}

		/* Unlock queue */
		pthread_mutex_unlock(&queue_lock);
	}

	fclose(inFile);
	free(payload_in);
	return NULL;
}

void* resolve(void* threadid) {
	FILE* outFile = fopen(threadid, "a");
	char* ipString[MAX_IP_LENGTH];
	char* payload_out;

	if(!outFile) {
		perror("Error opening output file.");
		return EXIT_FAILURE;
	}

	/* Read hostname and lookup IP Address */
	pthread_mutex_lock(&queue_lock);
	while(!queue_is_empty(&hosts) && requestsRemaining != 0){
	
		/* Pop hostname of the queue */
		payload_out = queue_pop(&hosts);
		
		/* Check if queue was empty */
		if (payload_out == NULL) {
			pthread_mutex_unlock(&queue_lock);
			usleep(rand() % 100 + 1);
			pthread_mutex_lock(&queue_lock);
		} else {
			/* Unlock queue */
			pthread_mutex_unlock(&queue_lock);


			/* Write to output */
			if (dnslookup(payload_out, ipString, MAX_IP_LENGTH) == UTIL_FAILURE) {
				strncpy(ipString, "", MAX_IP_LENGTH);
			}
		
			printf("%s, %s\n", payload_out, ipString);
			
			pthread_mutex_lock(&out_lock);

			fprintf(outFile, "%s, %s\n", payload_out, ipString);

			pthread_mutex_unlock(&out_lock);	
	
		}
		free(payload_out);
	
	}
	
	fclose(outFile);
	return NULL;
}






