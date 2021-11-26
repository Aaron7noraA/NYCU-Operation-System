#include <iostream>


using namespace std;
void merge(int *T, int start, int end, int thread_count);
void bobble(int*, int ,int );
void swap(int* T, int start, int end){
	int temp = T[start];
	T[start] = T[end];
	T[end] = temp;
}
int a_slice;

int main(){

	int a;
	int array[1000000];
	int index = 0;
	while(cin>>a){
		array[index] = a;
		index ++;
	}
	int thread_count = 4;
	int thread_work = index/4;
	a_slice = index/thread_count;

	for(int i=0; i<thread_count; i++){
		if(i == thread_count-1){
			bobble(array, i*thread_work, index-1);
			break;
		}
		bobble(array, i*thread_work, (i+1)*thread_work-1);
	}

	// start to merge
	merge(array, 0, index-1, thread_count);

	for(int i=0; i<index; i++){
		cout<<array[i]<<" ";
	}




	return 0;
}

void bobble(int* T, int start, int end){
	for(int i=start+1; i<=end; i++){
		for(int j=i; j>=start+1; j--){
			if(T[j] < T[j-1]){
				swap(T, j, j-1);
			}
		}
	}
}

void merge(int *T, int start, int end, int thread_count){


	if(thread_count == 1){
		return;
	}	
	
	// left slice
	merge(T, start, start+(thread_count/2)*a_slice-1, thread_count/2);
	// right slice
	merge(T, start+(thread_count/2)*a_slice , end, thread_count - (thread_count/2));


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
	return;
	



	
}

