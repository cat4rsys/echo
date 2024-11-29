#include<stdlib.h>
#include<stdio.h>

typedef struct _queue {
	double* buf;
	size_t cap;
	size_t first;
	size_t last;
	size_t size;
} queue;

int queue_init(queue* q, size_t cap) {
	*q = (queue){calloc(sizeof(double), cap), cap, 0, 0, 0}; 
	return q->buf != NULL;
}

int queue_add(queue* q, double v) {
	if ( q->size >= q->cap ) return -1;
	q->buf[q->first] = v;
	q->first = (q->first+1) % q->cap; 
	q->size ++;
	return 0;
}

int queue_pop(queue* q, double* v) {
	if ( q->size == 0 ) return -1;
	*v = q->buf[q->last];
	q->last = (q->last+1) % q->cap;
	q->size --;
	return 0;
}

void queue_free(queue* q) {
	free(q->buf);
}

size_t queue_size(queue*q) {
	return q->size;
}


int main(int argc, char* argv[] ) {
	size_t leftDelay = 10;
	size_t rightDelay = 10;
	double alpha = 0.5;

	if ( argc > 1 )
		leftDelay = atol(argv[1]);
	if ( argc > 2 ) 
		rightDelay = atof(argv[2]);
	if ( argc > 3 ) 
		alpha = atof(argv[3]);

	queue leftQ;
	queue_init(&leftQ, leftDelay);
	queue rightQ;
	queue_init(&rightQ, rightDelay);

	double leftSignal;
	double rightSignal;

	while(fscanf(stdin, "%lf %lf", &leftSignal, &rightSignal) == 2) {
		double leftEcho = 0;
		double rightEcho = 0;

		if ( queue_size(&leftQ) == leftDelay ) {
			queue_pop(&leftQ, &leftEcho) || printf("%lf\n", leftSignal + leftEcho*alpha);
		}
		else {
			printf("%lf\n", leftSignal);
		}

		if ( queue_size(&rightQ) == rightDelay ) {
			queue_pop(&rightQ, &rightEcho) || printf("%lf\n", rightSignal + rightEcho*alpha);
		}
		else {
			printf("%lf\n", rightSignal);
		}

		queue_add(&rightQ, leftSignal + alpha*leftEcho);
		queue_add(&leftQ, rightSignal + alpha*rightEcho);
	}

	while (queue_size(&leftQ) && queue_size(&rightQ)) {
		double echo = 0;
		queue_pop(&rightQ, &echo);
		printf("%lf ", echo*alpha);

		queue_pop(&leftQ, &echo);
		printf("%lf\n", echo*alpha);
	}

	while (queue_size(&rightQ)) {
		double echo = 0;
		queue_pop(&rightQ, &echo);
		printf("%lf\n", echo*alpha);
	}

	while (queue_size(&leftQ)) {
		double echo = 0;
		queue_pop(&leftQ, &echo);
		printf("\t%lf\n", echo*alpha);
	}

	queue_free(&rightQ);
	queue_free(&leftQ);
	
}
