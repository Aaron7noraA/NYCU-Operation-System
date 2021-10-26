#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

struct process
{
	int ID;
	int arrival_time;
	int burst_time;
	int waiting_time;
	int turnaround_time;
};

bool comparearrivaltime(const process &a, const process &b)
{
    return a.arrival_time < b.arrival_time;
}
bool compareID(const process &a, const process &b)
{
    return a.ID < b.ID;
}

bool compareremain(const process &a, const process &b)
{
	if(a.burst_time == b.burst_time){
		return a.arrival_time > b.arrival_time;
	}
    return a.burst_time > b.burst_time;
}


int main(int argc, char const *argv[])
{

	// get the test file
	ifstream test(argv[1]);
	int num;

	// get process number
	test >> num;

	// create process lsit by vector
	vector<process> JobList(num);

	// identify each process by ID, arrial time
	for(int i=0; i<num; i++){
		JobList[i].ID = i+1;
		test >> JobList[i].arrival_time;
	} 


	// input burst_time in each process slot
	int temp;
	for(int i=0; i<num; i++){
		test >> temp;
		JobList[i].burst_time = temp;
	}
	test.close();


	// sort by arrival time
	sort(JobList.begin(), JobList.end(), comparearrivaltime);

	// calculate first job
	int time_line = JobList[0].arrival_time + JobList[0].burst_time;
	vector<process> Dash;
	JobList[0].waiting_time = 0;
	JobList[0].turnaround_time = time_line - JobList[0].arrival_time;
	Dash.push_back(JobList[0]);

	// make a process Queue
	vector<process> Queue;

	// start to make time line
	for(int i=1; i<num; i++){
		if(JobList[i].arrival_time <= time_line){
			Queue.push_back(JobList[i]);
		}
		else{
			i--;
			sort(Queue.begin(), Queue.end(), compareremain);
			Queue[Queue.size()-1].waiting_time = time_line - Queue[Queue.size()-1].arrival_time;
			time_line += Queue[Queue.size()-1].burst_time;
			Queue[Queue.size()-1].turnaround_time = time_line - Queue[Queue.size()-1].arrival_time;
			Dash.push_back(Queue[Queue.size()-1]);
			Queue.pop_back();
		}
	}

	// after input all process start to finish it
	sort(Queue.begin(), Queue.end(), compareremain);
	while(Queue.size() > 0){	
		Queue[Queue.size()-1].waiting_time = time_line - Queue[Queue.size()-1].arrival_time;
		time_line += Queue[Queue.size()-1].burst_time;
		Queue[Queue.size()-1].turnaround_time = time_line - Queue[Queue.size()-1].arrival_time;
		Dash.push_back(Queue[Queue.size()-1]);
		Queue.pop_back();		
	}

	// sort it by ID to get output
	sort(Dash.begin(), Dash.end(), compareID);

	int total_wait=0, total_turnaround = 0;
	for(int i=0; i<num; i++){
		total_wait += Dash[i].waiting_time;
		total_turnaround += Dash[i].turnaround_time;
	}

	// output the file
	ofstream output("output/A2.txt");
	if(output.is_open()){

		output<<left<<setw(15)<<"Process"<<left<<setw(20)<<"Waiting Time"<<left<<setw(15)<<"Turnaround Time"<<"\n";

		for(int i=0; i<num; i++){
			output<<"P["<<Dash[i].ID<<left<<setw(12)<<"]"<<left<<setw(20)<<Dash[i].waiting_time<<left<<setw(15)<<Dash[i].turnaround_time<<"\n";
		}

		output<<"Total waiting time: "<<total_wait<<"\n";
		output<<"Total turnaround time: "<<total_turnaround;
	}
	return 0;
}