#include <iostream>

using namespace std;

void merge(int* T, int satrt, int end, int num);
void swap(int* T, int start, int end){
	int temp = T[start];
	T[start] = T[end];
	T[end] = temp;
	return;
}

int main(){

	int array[1000000];
	int temp;
	int index = 0;
	while(cin>>temp){
		array[index] = temp;
		index++;
	}
	merge(array, 0, index-1, index);

	for(int i=0; i<index; i++){
		cout<<array[i]<<" ";
	}

}

void merge(int* T, int start, int end, int num){
	if(num == 1){
		return;
	}
	else if(num == 2){
		if(T[start] > T[end]) swap(T, start, end);
		return;
	}

	// start to separate the array
	int mid = (start + end)/2;
	merge(T, start, mid, (mid-start)+1);
	merge(T, mid+1, end, end-mid);

	// start to merge
	int *left = new int[(mid-start)+2]; // last one is infinity large
	int *right = new int[end-mid+1]; // last one is infinity large

	//left block
	for(int h=0; h<(mid-start+1); h++){
		left[h] = T[start+h];
	}
	left[(mid-start+1)] = 1000001;

	// right block
	for(int h=0; h<(end-mid); h++){
		right[h] = T[mid+1+h];
	}
	right[(end-mid)] = 1000001;	


	int i=0,j=0,index=0;

	while(index < num){
		if(left[i] <= right[j]){
			T[start + index] = left[i];
			i++;
		}
		else{
			T[start + index] = right[j];
			j++;
		}
		index++;
	}

	delete [] left;
	delete [] right;

}



