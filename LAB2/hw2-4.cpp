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
struct cmp{
	bool operator()(const process &a, const process &b){
		return a.burst_time > b.burst_time;
	}
};


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

	// num of task already in Queue1
	int input_task = 0;
	// num of task finished
	int finish = 0;
	// time line
	int time_line = 0;

	// first process Queue
	queue<process> Queue1;

	// second process Queue
	priority_queue<process, vector<process>, cmp > Queue2;

	// finish jobs
	vector<process> Dash;

	// first push one process into Queue1
	Queue1.push(JobList[0]);
	input_task++;

	// CPU using right
	int power = -1; // default -1

	int temp;
	process inter;
	while(finish < num){

		// the need of insert task
		if(input_task < num){

			// pre add the time slot
			if(Queue1.front().burst_time <= time_qtm){
				temp = Queue1.front().burst_time;
			}
			else{
				temp = time_qtm;
			}
			time_line += temp;

			// check if new task in ? 
			while(input_task < num){
				//cout<<"timeline: "<<time_line<<"  Queue1 size: "<<Queue1.size()<<endl;
				if(JobList[input_task].arrival_time <= time_line){
					Queue1.push(JobList[input_task]);
					//cout<<"timeline: "<<time_line<<"  input task"<<JobList[input_task].ID<<endl;
					input_task++;
					power = -1;

				}
				else{
					break;
				}
			}

			// revise time line
			time_line -= temp;		
		}


		// first level of mutilevel feedback
		if(Queue1.size() > 0){

			// start to make time slut

			// this case mean process is going to be finished
			if(Queue1.front().burst_time <= time_qtm){
				
				Queue1.front().waiting_time += time_line - Queue1.front().arrival_time;
				time_line += Queue1.front().burst_time;

				
				Queue1.front().turnaround_time = time_line - Queue1.front().arrival_time;
				Dash.push_back(Queue1.front());
				//cout<<"Job finish Task"<<Queue1.front().ID<<endl;
				Queue1.pop();
				finish ++;
			}

			// this case mean process hasn't been finish 
			else{
				

				Queue1.front().waiting_time += time_line - Queue1.front().arrival_time;
				//cout<<"timeline: "<<time_line<<"  Job hasn't finish task"<<Queue1.front().ID<<endl;
				//cout<<"timeline: "<<time_line<<"  add wait time"<<time_line - Queue1.front().arrival_time<<endl;
				time_line += time_qtm;
				//cout<<"timeline: "<<time_line<<"  Task"<<Queue1.front().ID<<" "<<Queue1.front().burst_time<<endl;
				//cout<<time_qtm<<endl;
				Queue1.front().burst_time -= time_qtm;
				Queue1.front().ftime = time_line;

				// insert into queue2
				Queue2.push(Queue1.front());
				Queue1.pop();
			}		
		}

		// start to do second level queue
		else{

			// check the power set
			if(power != Queue2.top().ID){
				//cout<<"timeline: "<<time_line<<"  start a new Q2"<<endl;
				power = Queue2.top().ID;

				// renew the data
				inter = Queue2.top();
				Queue2.pop();
				inter.waiting_time += time_line - inter.ftime;
				Queue2.push(inter);
			}
			// because we need to care about first queue
				// so we cut time qtm into 1 sec to execute
			//cout<<"timeline: "<<time_line<<"  DO process"<<Queue2.top().ID<<endl;
			time_line ++;
			inter = Queue2.top();
			Queue2.pop();
			inter.burst_time --;

			// process finally finish
			if(inter.burst_time == 0){
				//cout<<"timeline: "<<time_line<<"  finish process"<<inter.ID<<endl;
				inter.turnaround_time = time_line - inter.arrival_time;
				Dash.push_back(inter);
				finish++;
			}
			else{
				// reset the finish time
				inter.ftime = time_line;
				Queue2.push(inter);	
			}

			

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
	ofstream output("output/A4.txt");
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