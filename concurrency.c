#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include "mt.h"


pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;
struct args{
	long tid;
	long sleep_time;
};

void *consumer(void *tid) {
	//init variables
	long cSleep, ctid;
	struct args *a = (struct args*)tid;

	//loop infinitely
	while(1) {
		//check ranges of values
		if(a->sleep_time<10 && a->sleep_time>1 && a->tid>-1 && a->tid<32) {
			//lock the thread
			pthread_mutex_lock (&mymutex);
			//store the sleep value in a
			cSleep = a->sleep_time;
			//store the tid value in a
			ctid = a->tid;

			//reset sleep time and tid to -1
			a->sleep_time = -1;
			a->tid = -1;

			//unlock the thread
			pthread_mutex_unlock (&mymutex);
			//sleep
			sleep(cSleep);

			//return and print the amount of work completed during the process
			return (void *)printf("Thread ID: %ld\nUnits of work: %ld\n\n", ctid, cSleep);
		}
	}
}

int main(int argc, char **argv) {
	//init variables
	unsigned long init[4] = {0x123, 0x234, 0x345, 0x456};
	unsigned long length = 4;
	long i, j, max, cSleep, pSleep;
	unsigned int eax, ebx, ecx, edx;
	int mt = 0;
	char vendor[13];

	//print number of consumers
	printf("Number of consumers that will be created: %s \n", argv[1]);
	//store number so I don't keep having to use atoi
	max = atoi(argv[1]);

	eax = 0x01;

	__asm__ __volatile__(
			"cpuid;"
			: "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
			: "a"(eax)
			);

	//use mt19937
	mt = 1;
	time_t t;
	init_genrand((unsigned) time(&t));

	//create an array of threads of length specified in input
	pthread_t threads[max];
	//create array of struct args of length specified in input
	struct args a[max];

	//init j
	j = 0;

	//repeat for the number specified in input
	for(i=0; i<max; i++) {
		//create thread
		pthread_create(&(threads[i]), NULL, consumer, (void *)( &a[j]));
		//increment j to match i (unless j has wrapped)
		j++;

		//check that j hasn't wrapped
		if(i==32) {
			//if it has, reset j to zero
			j = 0;
		}
	}

	//repeat for each desired thread
	for(i=0; i<max; i++) {
		//lock the thread
		pthread_mutex_lock (&mymutex);
		//set value of tid to i (edit the buffer)
		a[i].tid = i;
		//generate a sleep time
		cSleep = genrand_int32() % 8;
		a[i].sleep_time = cSleep + 2;

		//unlock the thread
		pthread_mutex_unlock(&mymutex);
		//generate a sleep time
		pSleep = genrand_int32() % 5;
		sleep(pSleep+2);
		//print output
		printf("Producer slept for %ld seconds and finished tid %ld.\n", pSleep + 2, i);
	}

	for(i=0; i<max; i++) {
		pthread_join(threads[i], NULL);
	}
		

	return 0;
}
