#include <iostream>
#include <pthread.h>
#include <semaphore.h>



using namespace std;


void *series(void* HAHA );

// semaphore 
sem_t sem[3];

// totoal conut 
long long int cnt[3]={0};

typedef struct 
{
	char *T;
	int start;
	int end;
}nodes;

int main(){



	int thread_count, length;
	char array[100000000];
	pthread_t thread_handle[4];


	cin>>thread_count;
	cin>>length;


	// count a workload of one thread
	int thread_work = length/thread_count;

	// information nodes
	nodes node[4];

	// input of the list
	for(int i=0; i<length; i++){
		cin>>array[i];
	}

	// initialization of semaphore
    for(int i=0; i<3; i++){
        sem_init(&sem[i], 0, 1);
    }

    // create threads
	for(int i=0; i<thread_count; i++){
		if(i == thread_count-1){
			node[i].T = array;
			node[i].start = i*thread_work;
			node[i].end = length-1;
			pthread_create(&thread_handle[i], NULL, series, (void*) &node[i]);
			break;
		}
		node[i].T = array;
		node[i].start = i*thread_work;
		node[i].end = (i+1)*thread_work-1;
		pthread_create(&thread_handle[i], NULL, series, (void*) &node[i]);
	}

	// wait for the whole threads to finish
	for(int i=0; i<thread_count; i++){
		pthread_join(thread_handle[i], NULL);
	}

	for(int i=0; i<3; i++){
		cout<<i<<": "<<cnt[i]<<endl;
	}

}

void *series(void* HAHA ){
	nodes* temp = (nodes*) HAHA;
	int start = temp->start;
	int end = temp->end;
	char *T = temp->T;
	for(int i=start; i<=end; i++){
		if(T[i] == '0'){
			sem_wait(&sem[0]);
			cnt[0] += 1;
			sem_post(&sem[0]);
		}
		else if(T[i] == '1'){
			sem_wait(&sem[1]);
			cnt[1] += 1;
			sem_post(&sem[1]);
		}
		else{
			sem_wait(&sem[2]);
			cnt[2] += 1;
			sem_post(&sem[2]);
		}
	}
}





