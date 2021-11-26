#include <iostream>


using namespace std;
void bobble(int*, int ,int );
void swap(int* T, int start, int end){
	int temp = T[start];
	T[start] = T[end];
	T[end] = temp;
}


int main(){

	int a;
	int array[1000000];
	int index = 0;
	while(cin>>a){
		array[index] = a;
		index ++;
	}
	bobble(array, 0, index-1);
	for(int i=0; i<index; i++){
		cout<<array[i]<<" ";
	}
	return 0;
}

void bobble(int* T, int start, int end){
	for(int i=1; i<=end; i++){
		for(int j=i; j>=1; j--){
			if(T[j] < T[j-1]){
				swap(T, j, j-1);
			}
		}
	}
}

