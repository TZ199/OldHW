

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <deque>
#include <queue>
#include "Process.h"
#include "CPU.h"

#define END_OF_TIME 20000
#define CONTEXT_SWITCH_TIME 8
#define TIME_SLICE 84

Process::Process(char pid, unsigned int iat, unsigned int cbt, unsigned int nb, unsigned int it){
	proc_id = pid;
	initial_arrival_time = iat;
	cpu_burst_time = cbt;
	num_bursts = nb;
	io_time = it;
	state = "NaN";
	time_in_state = 0;
	num_bursts_to_go = nb;
	cpu_burst_time_to_go = cbt;
}

char Process::get_proc_id() const {return proc_id;}
unsigned int Process::get_initial_arrival_time() const {return initial_arrival_time;}
unsigned int Process::get_cpu_burst_time() const {return cpu_burst_time;}
unsigned int Process::get_num_bursts() const {return num_bursts;}
bool Process::burst_finished() const {return state == "RUNNING" && cpu_burst_time_to_go == 0;}
bool Process::proc_finished() const {return state == "RUNNING" && num_bursts_to_go == 0;}
bool Process::io_finished() const {return state == "BLOCKED" && time_in_state == io_time;}
unsigned int Process::blocked_until(unsigned int timer) const {return timer+io_time;}

int num_bursts(const std::vector<Process*> &processes_ptr) {
	int result = 0;
	for (unsigned int i = 0; i < processes_ptr.size(); i++)
		result += processes_ptr[i]->get_num_bursts();
	return result;
}

int sum_bursts(const std::vector<Process*> &processes_ptr) {
	int result = 0;
	for (unsigned int i = 0; i < processes_ptr.size(); i++)
		result += (processes_ptr[i]->get_cpu_burst_time() *
		 processes_ptr[i]->get_num_bursts());
	return result;
}

struct burst_time_comparator {
	bool operator() (Process* p1, Process* p2) {
   	return (p1->get_cpu_burst_time() > p2->get_cpu_burst_time())
     	      || (p1->get_proc_id() > p1->get_proc_id());
 	}
};


void simulate_FCFS(std::vector<Process> processes, std::ofstream &out_str) {

	// a ready queue that implements FCFS
	struct FCFS_ready_queue {
    FCFS_ready_queue() {}
    Process* front() const {return queue.front();}
    void pop_front() {queue.pop_front();}
    void push_back(Process* p) {queue.push_back(p);}
    bool empty() const {return queue.empty();}
    std::string content() const {
    	std::string output = "[Q";
    	if (queue.empty()) output+=" empty";
    	else for (std::deque<Process*>::const_iterator itr = queue.begin();
    	          itr!=queue.end(); ++itr) {
				output.push_back(' '); output.push_back((*itr)->get_proc_id());
    	}
    	return output+="]";
    }
    std::deque<Process*> queue;
  };

  // summary statistics
	int total_turnaround_time = 0, total_wait_time = 0, total_cs = 0;

  // initialize CPU and ready queue
  CPU cpu(CONTEXT_SWITCH_TIME);
	FCFS_ready_queue ready_queue;

	// storing all processes to a vector
	std::vector<Process*> processes_ptr;
	for (unsigned int i = 0; i < processes.size(); i++)
		processes_ptr.push_back(&processes[i]);

	unsigned int timer = 0;
	// First interesting event: beginning of simulation
	std::cout << "time " << timer << "ms: Simulator started for FCFS "
	 << ready_queue.content() << std::endl;
  for (; timer < END_OF_TIME; ++timer) {

  	// number of processes finished at this point of time
  	unsigned int num_proc_finished = 0;

		// store registers after a burst (tim_loading was set to -4)
		// or, restore registers when queue is not empty
		if (cpu.idle() && (cpu.loading < 0 || !ready_queue.empty()))
			cpu.loading++;
		if (cpu.idle() && (!ready_queue.empty()))
			cpu.loaded++;

		// at each milisecond, check every processes for "interesting events"
		for (unsigned int i = 0; i < processes_ptr.size(); ++i)
			processes_ptr[i]->time_in_state++;
		for (unsigned int i = 0; i < processes_ptr.size(); ++i) {
			// if a process has arrived
			if (processes_ptr[i]->get_initial_arrival_time() == timer) {
				processes_ptr[i]->last_arrival_time = timer;
				processes_ptr[i]->state = "READY"; processes_ptr[i]->time_in_state = 0;
				ready_queue.push_back(processes_ptr[i]);
				std::cout << "time " << timer << "ms: Process " << processes_ptr[i]->get_proc_id()
				 << " arrived " << ready_queue.content() << std::endl;
			}
		}
		for (unsigned int i = 0; i < processes_ptr.size(); ++i) {
			// if a process is in ready queue
			if (processes_ptr[i]->state == "READY") {
				if (ready_queue.front() == processes_ptr[i]) { // a process is at front of queue
					if (cpu.idle()) { // cpu can accept new process
						if (cpu.loading == cpu.t_cs/2) { // if the process in the front has loaded registers
							total_wait_time += (processes_ptr[i]->time_in_state - cpu.loaded);
							cpu.loaded = 0;
							cpu.run_proc(processes_ptr[i]);
							processes_ptr[i]->state = "RUNNING"; processes_ptr[i]->time_in_state = 0;
							ready_queue.pop_front();
							total_cs++;
							std::cout << "time " << timer << "ms: Process " << processes_ptr[i]->get_proc_id()
							 << " started using the CPU " << ready_queue.content() << std::endl;
						}
					}
				}
			}
		}
		for (unsigned int i = 0; i < processes_ptr.size(); ++i) {
			// if a process is doing CPU burst
			if (processes_ptr[i]->state == "RUNNING" && processes_ptr[i]->time_in_state != 0) {
				processes_ptr[i]->cpu_burst_time_to_go--;
				if (processes_ptr[i]->burst_finished()) { // finished a cpu burst
					processes_ptr[i]->num_bursts_to_go--;
					total_turnaround_time += timer - processes_ptr[i]->last_arrival_time;
					cpu.run_proc(NULL); cpu.loading = -4;
					if (processes_ptr[i]->proc_finished()) { // this process finished its last cpu burst
						processes_ptr[i]->state = "FINISHED"; processes_ptr[i]->time_in_state = 0;
						std::cout << "time " << timer << "ms: Process " << processes_ptr[i]->get_proc_id()
						 << " terminated " << ready_queue.content() << std::endl;
					}
					else { // this is process is going to IO
						processes_ptr[i]->state = "BLOCKED";
						processes_ptr[i]->time_in_state = 0; processes_ptr[i]->cpu_burst_time_to_go = processes_ptr[i]->get_cpu_burst_time();
						std::cout << "time " << timer << "ms: Process " << processes_ptr[i]->get_proc_id()
						 << " completed a CPU burst; " << processes_ptr[i]->num_bursts_to_go
						  << " to go " << ready_queue.content() << std::endl;
 	          std::cout << "time " << timer << "ms: Process " << processes_ptr[i]->get_proc_id()
						 << " blocked on I/O until time " << processes_ptr[i]->blocked_until(timer)
						  << "ms " << ready_queue.content() << std::endl;
					}
				}
			}
		}
		for (unsigned int i = 0; i < processes_ptr.size(); ++i) {
			// if a process is blocked on IO
			if (processes_ptr[i]->state == "BLOCKED" && processes_ptr[i]->time_in_state != 0) {
				if (processes_ptr[i]->io_finished()) { // finished a IO
					processes_ptr[i]->state = "READY"; processes_ptr[i]->time_in_state = 0;
					ready_queue.push_back(processes_ptr[i]);
					processes_ptr[i]->last_arrival_time = timer;
					std::cout << "time " << timer << "ms: Process " << processes_ptr[i]->get_proc_id()
					 << " completed I/O " << ready_queue.content() << std::endl;
				}
			}
			// if a process is finished
			else if (processes_ptr[i]->state == "FINISHED") {
				num_proc_finished++;
			}
		} // processes loop

		// all done!
		if (num_proc_finished == processes_ptr.size() && cpu.idle() && cpu.loading == 0) {
			std::cout << "time " << timer << "ms: Simulator ended for FCFS" << std::endl;

			// Printing summary to output file
			int total_num_bursts = num_bursts(processes_ptr);
			int total_burst_time = sum_bursts(processes_ptr);
  		out_str << "Algorithm FCFS\n";
  		out_str << "-- average CPU burst time: " << (float)total_burst_time/total_num_bursts << " ms\n";
			out_str << "-- average wait time: " << (float)total_wait_time/total_num_bursts << " ms\n";
			out_str << "-- average turnaround time: " << (float)total_turnaround_time/total_num_bursts << " ms\n";
			out_str << "-- total number of context switches: " << total_cs << "\n";
			out_str << "-- total number of preemptions: " << 0 << "\n";
			return;
		}
  } // timer loop
}

void simulate_SJF(std::vector<Process> processes, std::ofstream &out_str) {

	// a ready queue that implements SJF
	struct SJF_ready_queue {

    SJF_ready_queue() {}
    Process* front() const {return queue.top();}
    void pop_front() {queue.pop();}
    void push_back(Process* p) {queue.push(p);}
    bool empty() const {return queue.empty();}
    std::string content() const {
    	std::priority_queue<Process*, std::vector<Process*>, burst_time_comparator> tmp_queue = queue;
    	std::string output = "[Q";
    	if (queue.empty()) output+=" empty";
    	else while (!tmp_queue.empty()) {
				output.push_back(' ');
				output.push_back(tmp_queue.top()->get_proc_id());
				tmp_queue.pop();
    	}
    	return output+="]";
    }
    std::priority_queue<Process*, std::vector<Process*>, burst_time_comparator> queue;
  };

  // summary statistics
	int total_turnaround_time = 0, total_wait_time = 0, total_cs = 0;

  // initialize CPU and ready queue
  CPU cpu(CONTEXT_SWITCH_TIME);
	SJF_ready_queue ready_queue;

	// storing all processes to a vector
	std::vector<Process*> processes_ptr;
	for (unsigned int i = 0; i < processes.size(); i++)
		processes_ptr.push_back(&processes[i]);

	unsigned int timer = 0;
	// First interesting event: beginning of simulation
	std::cout << "time " << timer << "ms: Simulator started for SJF "
	 << ready_queue.content() << std::endl;
  for (; timer < END_OF_TIME; ++timer) {

  	// number of processes finished at this point of time
  	unsigned int num_proc_finished = 0;

		// store registers after a burst (tim_loading was set to -4)
		// or, restore registers when queue is not empty
		if (cpu.idle() && (cpu.loading < 0 || !ready_queue.empty()))
			cpu.loading++;
		if (cpu.idle() && (!ready_queue.empty()))
			cpu.loaded++;

		// at each milisecond, check every processes for "interesting events"
		for (unsigned int i = 0; i < processes_ptr.size(); ++i)
			processes_ptr[i]->time_in_state++;
		for (unsigned int i = 0; i < processes_ptr.size(); ++i) {
			// if a process has arrived
			if (processes_ptr[i]->get_initial_arrival_time() == timer) {
				processes_ptr[i]->last_arrival_time = timer;
				processes_ptr[i]->state = "READY"; processes_ptr[i]->time_in_state = 0;
				ready_queue.push_back(processes_ptr[i]);
				std::cout << "time " << timer << "ms: Process " << processes_ptr[i]->get_proc_id()
				 << " arrived " << ready_queue.content() << std::endl;
			}
		}
		for (unsigned int i = 0; i < processes_ptr.size(); ++i) {
			// if a process is in ready queue
			if (processes_ptr[i]->state == "READY") {
				if (ready_queue.front() == processes_ptr[i]) { // a process is at front of queue
					if (cpu.idle()) { // cpu can accept new process
						if (cpu.loading == cpu.t_cs/2) { // if the process in the front has loaded registers
							total_wait_time += (processes_ptr[i]->time_in_state - cpu.loaded);
							cpu.loaded = 0;
							cpu.run_proc(processes_ptr[i]);
							processes_ptr[i]->state = "RUNNING"; processes_ptr[i]->time_in_state = 0;
							ready_queue.pop_front();
							total_cs++;
							std::cout << "time " << timer << "ms: Process " << processes_ptr[i]->get_proc_id()
							 << " started using the CPU " << ready_queue.content() << std::endl;
						}
					}
				}
			}
		}
		for (unsigned int i = 0; i < processes_ptr.size(); ++i) {
			// if a process is doing CPU burst
			if (processes_ptr[i]->state == "RUNNING" && processes_ptr[i]->time_in_state != 0) {
				processes_ptr[i]->cpu_burst_time_to_go--;
				if (processes_ptr[i]->burst_finished()) { // finished a cpu burst
					processes_ptr[i]->num_bursts_to_go--;
					total_turnaround_time += timer - processes_ptr[i]->last_arrival_time;
					cpu.run_proc(NULL); cpu.loading = -4;
					if (processes_ptr[i]->proc_finished()) { // this process finished its last cpu burst
						processes_ptr[i]->state = "FINISHED"; processes_ptr[i]->time_in_state = 0;
						std::cout << "time " << timer << "ms: Process " << processes_ptr[i]->get_proc_id()
						 << " terminated " << ready_queue.content() << std::endl;
					}
					else { // this is process is going to IO
						processes_ptr[i]->state = "BLOCKED";
						processes_ptr[i]->time_in_state = 0; processes_ptr[i]->cpu_burst_time_to_go = processes_ptr[i]->get_cpu_burst_time();
						std::cout << "time " << timer << "ms: Process " << processes_ptr[i]->get_proc_id()
						 << " completed a CPU burst; " << processes_ptr[i]->num_bursts_to_go
						  << " to go " << ready_queue.content() << std::endl;
 	          std::cout << "time " << timer << "ms: Process " << processes_ptr[i]->get_proc_id()
						 << " blocked on I/O until time " << processes_ptr[i]->blocked_until(timer)
						  << "ms " << ready_queue.content() << std::endl;
					}
				}
			}
		}
		for (unsigned int i = 0; i < processes_ptr.size(); ++i) {
			// if a process is blocked on IO
			if (processes_ptr[i]->state == "BLOCKED" && processes_ptr[i]->time_in_state != 0) {
				if (processes_ptr[i]->io_finished()) { // finished a IO
					processes_ptr[i]->state = "READY"; processes_ptr[i]->time_in_state = 0;
					ready_queue.push_back(processes_ptr[i]);
					processes_ptr[i]->last_arrival_time = timer;
					std::cout << "time " << timer << "ms: Process " << processes_ptr[i]->get_proc_id()
					 << " completed I/O " << ready_queue.content() << std::endl;
				}
			}
			// if a process is finished
			else if (processes_ptr[i]->state == "FINISHED") {
				num_proc_finished++;
			}
		} // processes loop

		// all done!
		if (num_proc_finished == processes_ptr.size() && cpu.idle() && cpu.loading == 0) {
			std::cout << "time " << timer << "ms: Simulator ended for SJF" << std::endl;

			// Printing summary to output file
			int total_num_bursts = num_bursts(processes_ptr);
			int total_burst_time = sum_bursts(processes_ptr);
  		out_str << "Algorithm SJF\n";
  		out_str << "-- average CPU burst time: " << (float)total_burst_time/total_num_bursts << " ms\n";
			out_str << "-- average wait time: " << (float)total_wait_time/total_num_bursts << " ms\n";
			out_str << "-- average turnaround time: " << (float)total_turnaround_time/total_num_bursts << " ms\n";
			out_str << "-- total number of context switches: " << total_cs << "\n";
			out_str << "-- total number of preemptions: " << 0 << "\n";
			return;
		}
  } // timer loop
}

void simulate_RR(std::vector<Process> processes, std::ofstream &out_str) {

	// a ready queue that implements FCFS
	struct FCFS_ready_queue {
    FCFS_ready_queue() {}
    Process* front() const {return queue.front();}
    void pop_front() {queue.pop_front();}
    void push_back(Process* p) {queue.push_back(p);}
    bool empty() const {return queue.empty();}
    std::string content() const {
    	std::string output = "[Q";
    	if (queue.empty()) output+=" empty";
    	else for (std::deque<Process*>::const_iterator itr = queue.begin();
    	          itr!=queue.end(); ++itr) {
				output.push_back(' '); output.push_back((*itr)->get_proc_id());
    	}
    	return output+="]";
    }
    std::deque<Process*> queue;
  };

  // summary statistics
	int total_turnaround_time = 0, total_wait_time = 0, total_cs = 0, total_preemptions = 0;

  // initialize CPU and ready queue
  CPU cpu(CONTEXT_SWITCH_TIME, TIME_SLICE);
	FCFS_ready_queue ready_queue;

	// storing all processes to a vector
	std::vector<Process*> processes_ptr;
	for (unsigned int i = 0; i < processes.size(); i++)
		processes_ptr.push_back(&processes[i]);

	unsigned int timer = 0;
	// First interesting event: beginning of simulation
	std::cout << "time " << timer << "ms: Simulator started for RR "
	 << ready_queue.content() << std::endl;
  for (; timer < END_OF_TIME; ++timer) {

  	// number of processes finished at this point of time
  	unsigned int num_proc_finished = 0;

		// store registers after a burst (tim_loading was set to -4)
		// or, restore registers when queue is not empty
		if (cpu.idle() && (cpu.loading < 0 || !ready_queue.empty()))
			cpu.loading++;
		if (cpu.idle() && (!ready_queue.empty()))
			cpu.loaded++;

		// at each milisecond, check every processes for "interesting events"
		for (unsigned int i = 0; i < processes_ptr.size(); ++i)
			processes_ptr[i]->time_in_state++;

		for (unsigned int i = 0; i < processes_ptr.size(); ++i) {
			// if a process has arrived
			if (processes_ptr[i]->get_initial_arrival_time() == timer) {
				processes_ptr[i]->last_arrival_time = timer;
				processes_ptr[i]->state = "READY"; processes_ptr[i]->time_in_state = 0;
				ready_queue.push_back(processes_ptr[i]);
				std::cout << "time " << timer << "ms: Process " << processes_ptr[i]->get_proc_id()
				                             << " arrived " << ready_queue.content() << std::endl;
			}
		}
		for (unsigned int i = 0; i < processes_ptr.size(); ++i) {
			// if a process is in ready queue
			if (processes_ptr[i]->state == "READY") {
				if (ready_queue.front() == processes_ptr[i]) { // a process is at front of queue
					if (cpu.idle()) { // cpu can accept new process
						if (cpu.loading == cpu.t_cs/2) { // if the process in the front has loaded registers
							total_wait_time += (processes_ptr[i]->time_in_state - cpu.loaded);
							cpu.loaded = 0;
							cpu.run_proc(processes_ptr[i]);
							processes_ptr[i]->state = "RUNNING"; processes_ptr[i]->time_in_state = 0;
							ready_queue.pop_front();
							total_cs++;
							std::cout << "time " << timer << "ms: Process " << processes_ptr[i]->get_proc_id()
							 << " started using the CPU " << ready_queue.content() << std::endl;
						}
					}
				}
			}
		}
		for (unsigned int i = 0; i < processes_ptr.size(); ++i) {
			// if a process is doing CPU burst
			if (processes_ptr[i]->state == "RUNNING" && processes_ptr[i]->time_in_state != 0) {
				processes_ptr[i]->cpu_burst_time_to_go--;
				if (processes_ptr[i]->burst_finished()) { // finished a cpu burst
					processes_ptr[i]->num_bursts_to_go--;
					total_turnaround_time += (timer - processes_ptr[i]->last_arrival_time);
					cpu.run_proc(NULL); cpu.loading = -4;
					if (processes_ptr[i]->proc_finished()) { // this process finished its last cpu burst
						processes_ptr[i]->state = "FINISHED"; processes_ptr[i]->time_in_state = 0;
						std::cout << "time " << timer << "ms: Process " << processes_ptr[i]->get_proc_id()
						 << " terminated " << ready_queue.content() << std::endl;
					}
					else { // this is process is going to IO
						processes_ptr[i]->state = "BLOCKED";
						processes_ptr[i]->time_in_state = 0; processes_ptr[i]->cpu_burst_time_to_go = processes_ptr[i]->get_cpu_burst_time();
						std::cout << "time " << timer << "ms: Process " << processes_ptr[i]->get_proc_id()
						 << " completed a CPU burst; " << processes_ptr[i]->num_bursts_to_go
						  << " to go " << ready_queue.content() << std::endl;
 	          std::cout << "time " << timer << "ms: Process " << processes_ptr[i]->get_proc_id()
						 << " blocked on I/O until time " << processes_ptr[i]->blocked_until(timer)
						  << "ms " << ready_queue.content() << std::endl;
					}
				}
				else if (processes_ptr[i]->time_in_state == cpu.t_slice) { // time slice expired
					if (!ready_queue.empty()) { // preemption because ready queue is not empty
						cpu.loading = -4;
						processes_ptr[i]->state = "READY";
						processes_ptr[i]->time_in_state = 0;
						total_preemptions++;
						cpu.run_proc(NULL);
						ready_queue.push_back(processes_ptr[i]);
						std::cout << "time " << timer << "ms: Time slice expired; process "
						 << processes_ptr[i]->get_proc_id() << " preempted with " << processes_ptr[i]->cpu_burst_time_to_go
						  << "ms to go " << ready_queue.content() << std::endl;
					}
					else { // no preemption because ready queue is empty
						processes_ptr[i]->time_in_state = 0;
						std::cout << "time " << timer
						 << "ms: Time slice expired; no preemption because ready queue is empty "
						  << ready_queue.content() << std::endl;
					}
				}
			}
		}
		for (unsigned int i = 0; i < processes_ptr.size(); ++i) {
			// if a process is blocked on IO
			if (processes_ptr[i]->state == "BLOCKED" && processes_ptr[i]->time_in_state != 0) {
				if (processes_ptr[i]->io_finished()) { // finished a IO
					processes_ptr[i]->state = "READY"; processes_ptr[i]->time_in_state = 0;
					ready_queue.push_back(processes_ptr[i]);
					processes_ptr[i]->last_arrival_time = timer;
					std::cout << "time " << timer << "ms: Process " << processes_ptr[i]->get_proc_id()
					 << " completed I/O " << ready_queue.content() << std::endl;
				}
			}
			// if a process is finished
			else if (processes_ptr[i]->state == "FINISHED") {
				num_proc_finished++;
			}
		} // processes loop

		// all done!
		if (num_proc_finished == processes_ptr.size() && cpu.idle() && cpu.loading == 0) {
			std::cout << "time " << timer << "ms: Simulator ended for RR" << std::endl;

			// Printing summary to output file
			int total_num_bursts = num_bursts(processes_ptr);
			int total_burst_time = sum_bursts(processes_ptr);
  		out_str << "Algorithm RR\n";
  		out_str << "-- average CPU burst time: " << (float)total_burst_time/total_num_bursts << " ms\n";
			out_str << "-- average wait time: " << (float)total_wait_time/total_num_bursts << " ms\n";
			out_str << "-- average turnaround time: " << (float)total_turnaround_time/total_num_bursts << " ms\n";
			out_str << "-- total number of context switches: " << total_cs << "\n";
			out_str << "-- total number of preemptions: " << total_preemptions << "\n";
			return;
		}
  } // timer loop
}
