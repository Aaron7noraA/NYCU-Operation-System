#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
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
	// new slot 
	int ftime;
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

	int time_qtm;
	test >> time_qtm;
	test.close();

	// sort by arrival time
	sort(JobList.begin(), JobList.end(), comparearrivaltime);

	// num of task already in queue
	int input_task = 0;
	// num of task finished
	int finish = 0;
	// time line
	int time_line = 0;

	// process queue
	queue<process> Queue;

	// finish jobs
	vector<process> Dash;

	// first push one process into queue
	Queue.push(JobList[0]);
	input_task++;

	int temp;
	process inter;
	while(finish < num){

		// pre add the time slot
		if(Queue.front().burst_time <= time_qtm){
			temp = Queue.front().burst_time;
		}
		else{
			temp = time_qtm;
		}
		time_line += temp;

		// check if new task in ? 
		while(input_task < num){
			if(JobList[input_task].arrival_time <= time_line){
				Queue.push(JobList[input_task]);
				input_task++;
			}
			else{
				break;
			}
		}

		// revise time line
		time_line -= temp;


		// start to make time slut
		if(Queue.front().burst_time <= time_qtm){
			if(Queue.front().ftime == 0){
				Queue.front().waiting_time += time_line - Queue.front().arrival_time;
			}
			else{
				Queue.front().waiting_time += time_line - Queue.front().ftime;
			}
			time_line += Queue.front().burst_time;
			Queue.front().turnaround_time = time_line - Queue.front().arrival_time;
			Dash.push_back(Queue.front());
			Queue.pop();
			finish ++;
		}
		else{
			if(Queue.front().ftime == 0){
				Queue.front().waiting_time += time_line - Queue.front().arrival_time;
			}
			else{
				Queue.front().waiting_time += time_line - Queue.front().ftime;
			}
			time_line += time_qtm;
			Queue.front().burst_time -= time_qtm;
			Queue.front().ftime = time_line;
			inter = Queue.front();
			Queue.pop();
			Queue.push(inter);
		}
	}

	// get the order correct
	sort(Dash.begin(), Dash.end(), compareID);

	// total piv
	int total_wait = 0, total_turnaround = 0;
	for(int i=0; i<Dash.size(); i++){
		total_wait += Dash[i].waiting_time;
		total_turnaround += Dash[i].turnaround_time;
	}
	
	// output the file
	ofstream output("output/A3.txt");
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