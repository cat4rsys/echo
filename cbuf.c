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
	q->buf[q->first] += v;
	q->first = (q->first+1) % q->cap; 
	q->size ++;
	return 0;
}

int queue_custom_add(queue* q, double v, int fdel) {
	int place = (q->first + fdel) % q->cap;
	q->buf[place] += v;
	return 0;
}

int queue_custom_pop(queue* q, double * v) {
	*v = q->buf[q->last];
	q->buf[q->last] = 0;
	q->last = (q->last+1) % q->cap;
	return 0;
}

int queue_pop(queue* q, double* v) {
	if ( q->size == 0 ) return -1;
	*v = q->buf[q->last];
	q->buf[q->last] = 0;
	q->last = (q->last+1) % q->cap;
	q->size--;
	return 0;
}

void queue_free(queue* q) {
	free(q->buf);
}

size_t queue_size(queue*q) {
	return q->size;
}

void queue_print(queue* q) {
	for (int i = 0; i < q->cap; i++) {
		printf("%lg ", q->buf[i]);
	}
	printf("\n");
}


int main(int argc, char* argv[] ) {
	size_t delay = 10;
	double alpha = 0.5;
	if ( argc > 1 )
		delay = atol(argv[1]);
	if ( argc > 2 ) 
		alpha = atof(argv[2]);

	queue q;
	queue_init(&q, delay);
	double x;
	int fdel;
	while(fscanf(stdin, "%lf %d", &x, &fdel) == 2) {
		double echo = 0;
		queue_custom_pop(&q, &echo);
		//printf("got echo %lg\n", echo);
		printf("%lf\n", x + echo*alpha);
		queue_custom_add(&q, x, fdel);
		queue_add(&q, echo*alpha);
		//queue_print(&q);
	}
	for (int i = 0; i < 10; i++) {
		double echo = 0;
		queue_custom_pop(&q, &echo);
		printf("%lf\n", echo*alpha);
	}
	queue_free(&q);
	
}
