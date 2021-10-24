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
	for(int i=0; i<num; i++){
		test >> JobList[i].burst_time;
	}
	test.close();

	// sort by arrival time
	sort(JobList.begin(), JobList.end(), comparearrivaltime);

	// start to make time line 
	int time_line = JobList[0].arrival_time;
	int Job_count = 0;
	while(Job_count < num){
		//cout<<"I"<<Job_count<<".  "<<JobList[Job_count].arrival_time<<".  "<<JobList[Job_count].burst_time<<endl;
		JobList[Job_count].waiting_time = time_line - JobList[Job_count].arrival_time;
		time_line += JobList[Job_count].burst_time;
		JobList[Job_count].turnaround_time = time_line - JobList[Job_count].arrival_time;
		Job_count++;
	}

	// calculate total needs...
	int total_wait = 0;
	int total_turnaround = 0;
	for(int i=0; i<num; i++){
		total_wait += JobList[i].waiting_time;
		total_turnaround += JobList[i].turnaround_time;
	}

	// get vector to original order
	sort(JobList.begin(), JobList.end(), compareID);

	// output the file
	ofstream output("output/A1.txt");
	if(output.is_open()){

		output<<left<<setw(15)<<"Process"<<left<<setw(20)<<"Waiting Time"<<left<<setw(15)<<"Turnaround Time"<<"\n";

		for(int i=0; i<num; i++){
			output<<"P["<<JobList[i].ID<<left<<setw(12)<<"]"<<left<<setw(20)<<JobList[i].waiting_time<<left<<setw(15)<<JobList[i].turnaround_time<<"\n";
		}

		output<<"Total waiting time: "<<total_wait<<"\n";
		output<<"Total turnaround time: "<<total_turnaround;
	}



	return 0;
}