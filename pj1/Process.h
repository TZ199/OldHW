

class Process {

public:
	Process(char pid, unsigned int iat, unsigned int cbt, unsigned int nb, unsigned int it);
	char get_proc_id() const;
	unsigned int get_initial_arrival_time() const;
	unsigned int get_cpu_burst_time() const;
	unsigned int get_num_bursts() const;
	bool burst_finished() const;
	bool proc_finished() const;
	bool io_finished() const;
	unsigned int blocked_until(unsigned int timer) const;

	// process' timers
	std::string state;
	unsigned int time_in_state;
	unsigned int num_bursts_to_go;
	unsigned int cpu_burst_time_to_go;
	unsigned int last_arrival_time;

private:
	// properties of a process
	char proc_id;
	unsigned int initial_arrival_time;
	unsigned int cpu_burst_time;
	unsigned int num_bursts;
	unsigned int io_time;
};

void simulate_FCFS(std::vector<Process> processes);
