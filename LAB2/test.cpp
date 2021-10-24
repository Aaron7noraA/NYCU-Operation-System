#include <iostream>
#include <queue>

using namespace std;

struct cmp{
	bool operator()(const int &a, const int &b){
		return a > b;
	}
};


int main(){
	
	priority_queue<int, vector<int>, cmp> Q1;
	Q1.push(1);
	Q1.push(3);
	Q1.push(6);
	Q1.push(10);
	Q1.push(2);
	int num = Q1.size();
	for(int i=0; i<num; i++){
		cout<<Q1.top()<<endl;
		Q1.pop();
	}


}
