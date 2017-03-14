#include <pthread>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "util.h"
#include "queue.h"
#include "multi-lookup.h"


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
	int numFiles = argc - 2
        if (numFiles > MAX_INPUT_FILES) {
		fprintf(stderr, "Too many input files: %d\n", (argc - 2));
		fprintf(stderr, "Maximum number of input files: %d\n", MAX_INPUT_FILES);
		return EXIT_FAILURE;
	} else {
		FILE* input[numFiles]
	}

	//Number of Resolver Threads
	int numResolvers = sysconf(_SC_NPROCESSORS_ONLN);
	if (numResolvers > MAX_RESOLVER_THREADS) {
		fprintf(stderr, "Too many resolver threads: %d\n", numResolvers);
		fprintf(stderr, "Maximum number of resovler threads: %d\n", MAX_RESOLVER_THREADS);
		return EXIT_FAILURE;
	} else if (numResolvers < MIN_RESOLVER_THREADS) {
		fprintf(stderr, "Not enough resolver threads: %d\n", numResolvers);
		fprintf(stderr, "Minimum number of resolver threads: %d\n", MIN_RESOLVER_THREADS);
		return EXIT_FAILURE;
	}

	
	pthread_t requestThreads[numFiles];
	pthread_t resolveThreads[numResolvers];

	// Still Implementing...


	return EXIT_SUCCESS;
}


/*
 * --------------------
 *   HELPER FUNCTIONS
 * --------------------
 */

void* request(void* threadid) {
	//Implement Request Function


}

void* resolve(void* threadid) {
	//Implement Resolve Function


}






