#include <iostream>
#include <pthread.h>
#include <string>
#include <stdlib.h> //rand_r
#include <time.h>
#include <vector>

using namespace std;
void *merge(void* );
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
	int partner;
	int *T;
}merge_nodes;

int main(){

	int a;
	int array[1000010];
	int index = 0;
	while(cin>>a){
		array[index] = a;
		index ++;
	}

	// change threads count here
	// thread count need to be multiple of 2
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

	int merge_index = -1;
	pthread_t merge_handle[100];
	merge_nodes merge_node[100];

	// vector to record ID
	vector<int> LIST;
	int temp;

	// start to merge the array
	// thread count need to be multiple of 2
	int partner = 2;
	while(partner <= thread_count){
		for(int i=0; i<thread_count; i+=partner){
			if((i+partner*2)*a_slice > index){
				merge_index ++;
				merge_node[merge_index].end = index-1;
			}
			else{
				merge_index ++;
				merge_node[merge_index].end = (i+partner)*a_slice-1;
			}			
			merge_node[merge_index].start = i*a_slice;	
			merge_node[merge_index].partner = partner;
			merge_node[merge_index].T = array;
			
			LIST.push_back(merge_index);
			pthread_create(&merge_handle[merge_index], NULL, merge, (void*) &merge_node[merge_index]);
		}
		while(LIST.size()!=0){		
			pthread_join(merge_handle[LIST.back()], NULL);
			LIST.pop_back();
		}
		partner *=2;
	}

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
	int thread_count = temp->partner;



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

