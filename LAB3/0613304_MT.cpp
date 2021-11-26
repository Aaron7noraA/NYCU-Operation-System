#include <iostream>
#include <pthread.h>
#include <string>
#include <stdlib.h> //rand_r
#include <time.h>

using namespace std;
void *merge(void* TATA);
void *bobble(void* HAHA );
void swap(int* T, int start, int end){
	int temp = T[start];
	T[start] = T[end];
	T[end] = temp;
}
int a_slice;

typedef struct 
{
	int *T;
	int start;
	int end;
}nodes;

typedef struct
{
	int start;
	int end;
	int thread_count;
	int *T;
}merge_nodes;
merge_nodes merge_node[100];

pthread_t merge_thread[100];
int merge_thread_index = -1;

int main(){

	int a;
	int array[1000000];
	int index = 0;
	while(cin>>a){
		array[index] = a;
		index ++;
	}

	// change threads count here
	int thread_count = 4;
	int thread_work = index/4;
	a_slice = index/thread_count;

	// create thread ID, you can craete upto 100 threads
	pthread_t thread_handle[100];

	// create thread passing in info node
	nodes node[100];
	


	for(int i=0; i<thread_count; i++){
		if(i == thread_count-1){
			node[i].T = array;
			node[i].start = i*thread_work;
			node[i].end = index-1;
			pthread_create(&thread_handle[i], NULL, bobble, (void*) &node[i]);
			break;
		}
		node[i].T = array;
		node[i].start = i*thread_work;
		node[i].end = (i+1)*thread_work-1;
		pthread_create(&thread_handle[i], NULL, bobble, (void*) &node[i]);
	}
	
	// wait for the whole threads to finish
	for(int i=0; i<thread_count; i++){
		pthread_join(thread_handle[i], NULL);
	}

	// start to merge
	merge_thread_index++;
	merge_node[merge_thread_index].T = array;
	merge_node[merge_thread_index].start = 0;
	merge_node[merge_thread_index].end = index-1;
	merge_node[merge_thread_index].thread_count = thread_count;

	merge(&merge_node[merge_thread_index]);

	for(int i=0; i<index; i++){
		cout<<array[i]<<" ";
	}




	return 0;
}

void *bobble(void* HAHA){
	nodes* temp = (nodes*) HAHA;
	int start = temp->start;
	int end = temp->end;
	int *T = temp->T;
	for(int i=start+1; i<=end; i++){
		for(int j=i; j>=start+1; j--){
			if(T[j] < T[j-1]){
				swap(T, j, j-1);
			}
		}
	}
	return NULL;
}

void *merge(void* TATA){

	merge_nodes* temp = (merge_nodes*) TATA;
	int* T = temp->T;
	int start = temp->start;
	int end = temp->end;
	int thread_count = temp->thread_count;

	if(thread_count == 1){
		return NULL;
	}	

	// record the next two thread
	int t1 = merge_thread_index;
	int t2 = merge_thread_index + 1;

	// left slice
	merge_thread_index ++;
	merge_node[merge_thread_index].T = T;
	merge_node[merge_thread_index].start = start;
	merge_node[merge_thread_index].end = start+(thread_count/2)*a_slice-1;
	merge_node[merge_thread_index].thread_count = thread_count/2;
	pthread_create(&merge_thread[merge_thread_index], NULL, merge, (void*) &merge_node[merge_thread_index]);
	

	// right slice
	merge_thread_index ++;
	merge_node[merge_thread_index].T = T;
	merge_node[merge_thread_index].start = start+(thread_count/2)*a_slice;
	merge_node[merge_thread_index].end = end;
	merge_node[merge_thread_index].thread_count = thread_count - (thread_count/2);
	pthread_create(&merge_thread[merge_thread_index], NULL, merge, (void*) &merge_node[merge_thread_index]);

	
	 pthread_join(merge_thread[t1], NULL);
	 pthread_join(merge_thread[t2], NULL);
	


	int left_num = (thread_count/2)*a_slice;
	int right_num  = end - (thread_count/2)*a_slice - start + 1;

	int *left  = new int[left_num+1];
	int *right = new int[right_num+1];


	
	// assign to a new array
	for(int h=0; h<left_num; h++){
		left[h] = T[start+h];
	}
	left[left_num] = 1000001;

	// assign to a new array
	for(int h=0; h<right_num; h++){
		right[h] = T[start+(thread_count/2)*a_slice+h];
	}
	right[right_num] = 1000001;	


	int i=0, j=0, index=0;
	while(index<(end-start+1)){
		if(left[i] <= right[j]){
			T[start + index] = left[i];
			i++;
		}
		else{
			T[start +index] = right[j];
			j++;
		}
		index ++;
	}



	delete [] left;
	delete [] right;
	return NULL;
	



	
}

