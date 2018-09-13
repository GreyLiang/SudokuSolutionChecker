/**
 * A pthread program illustrating how to
 * create a simple thread and some of the pthread API
 * This program implements the summation function where
 * the summation operation is run as a separate thread.
 *
 * Most Unix/Linux/OS X users
 * gcc thrd.c -lpthread
 *
 * Figure 4.11
 *
 * @author Gagne, Galvin, Silberschatz
 * Operating System Concepts  - Tenth Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_CHILD_THREADS 15

int sum[NUM_CHILD_THREADS]; /* this data is shared by the thread(s) */

void *runner(void *param); /* the thread */

typedef struct {
int upper;
int iValue;
} run_param;

int main(int argc, char *argv[])
{
	int i;
	pthread_t tid[NUM_CHILD_THREADS]; /* the thread identifier for child threads */
	pthread_attr_t attr[NUM_CHILD_THREADS]; /* set of attributes for the thread */
	run_param thrParam[NUM_CHILD_THREADS];

	if (argc != 2) {
		fprintf(stderr,"usage: thrd-posix <0 or positive integer value>\n");
		/*exit(1);*/
		return -1;
	}

	if (atoi(argv[1]) < 0) {
		fprintf(stderr,"Argument %d must be non-negative\n",atoi(argv[1]));
		/*exit(1);*/
		return -1;
	}

	/* get the >= 0 value for upper */
	for (i = 0; i < NUM_CHILD_THREADS; i++) {
		thrParam[i].upper = atoi(argv[1]);
		thrParam[i].iValue = i;
	}

	/* get the default attributes */
	for (i = 0; i < NUM_CHILD_THREADS; i++)
		pthread_attr_init(&(attr[i]));

	/* create the threads */
	for (i = 0; i < NUM_CHILD_THREADS; i++)
	{
		pthread_create(&(tid[i]),&(attr[i]),runner, &(thrParam[i]));
	}

	/* now wait for the thread to exit */
	for (i = 0; i < NUM_CHILD_THREADS; i++) {
		pthread_join(tid[i],NULL);
	}

	for (i = 0; i < NUM_CHILD_THREADS; i++) {
		printf("sum[%d] = %d\n", i, sum[i]);
	}
}

/**
 * The thread will begin control in this function
 */
void *runner(void *param) 
{
	run_param *inP;
	int i, up;
	int iV;
	pthread_t self;

	inP = (run_param *)param;
	up = inP->upper;
	iV = inP->iValue;

	sum[iV] = 0;

	if (up > 0) {
		for (i = 1; i <= up; i++)
			sum[iV] += i;
	}

	self = pthread_self();
	printf("ending child thread #%d (tid = %lX)\n", iV, (unsigned long)self);

	pthread_exit(0);
}