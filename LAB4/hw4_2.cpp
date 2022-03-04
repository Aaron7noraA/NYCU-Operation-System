#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

using namespace std;

int sum = 0;
sem_t sem;

void* Monte(void* rank);

int main(){

	int thread_count;
	int NP;
	cin>>thread_count;
	cin>>NP;

	// semophore
	sem_init(&sem, 0, 1);

	// thread ID
	pthread_t thread_handles[4];

	long  work_load = NP/thread_count;
	long last = NP - 3*work_load;


	for(int i=0; i<thread_count; i++){
		if(i == thread_count-1){
			pthread_create(&thread_handles[i], NULL, Monte, (void*) last);
		}
		else{
			pthread_create(&thread_handles[i], NULL, Monte, (void*) work_load);
		}
	}

	for(int i=0; i<thread_count; i++){
		pthread_join(thread_handles[i], NULL);
	}

	cout<<"get: "<<sum<<endl;
	double pi = 4 * sum / (double) NP;
	cout<<"Pi= "<<pi<<endl;



}

void* Monte(void* rank){
	long my_work = (long) rank;

	double x,y;
	unsigned int seed = time(NULL);
	for(int i=0; i<my_work; i++){
		x = (double)rand_r(&seed)/RAND_MAX * 2.0 - 1.0;
		y = (double)rand_r(&seed)/RAND_MAX * 2.0 - 1.0;
		if(x*x + y*y <= 1){
			sem_wait(&sem);
			sum++;
			sem_post(&sem);
		}
	}
	//printf("from thread %ld finish\n",my_rank);
	return NULL;
}