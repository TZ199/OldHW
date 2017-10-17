

class CPU {

public:
  // non preemptive CPU
  CPU(int context_switch_time) {
    running_process = NULL;
    loading = 0;
    loaded = 0;
    t_cs = context_switch_time;
  }
  // preemptive CPU
  CPU(int context_switch_time, int time_slice) {
    running_process = NULL;
    loading = 0;
    loaded = 0;
    t_cs = context_switch_time;
    t_slice = time_slice;
  }
  bool idle() const {return running_process == NULL;}
  Process* current_proc() const {return running_process;}
  void run_proc(Process* p) {running_process = p;}

  int t_cs;
  unsigned int t_slice;
  int loading;
  int loaded;

private:
  Process* running_process;
};
