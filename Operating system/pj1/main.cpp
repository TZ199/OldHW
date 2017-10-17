

#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "Process.h"

void simulate_FCFS(std::vector<Process> processes, std::ofstream &out_str);
void simulate_SJF(std::vector<Process> processes, std::ofstream &out_str);
void simulate_RR(std::vector<Process> processes, std::ofstream &out_str);
std::vector<std::string> split_by(std::string line, char delimiter) {
  std::vector<std::string> result; std::string tmp = "";
  for (unsigned int i = 0; i < line.length(); ++i) {
    if (line[i] != delimiter) tmp.push_back(line[i]);
    else {result.push_back(tmp); tmp.clear();}
  }
  result.push_back(tmp);
  return result;
}

int main(int argc, char const *argv[]) {

  // handling errors
	if (argc != 3) {
		std::cerr << "ERROR: Invalid arguments\n"
     "USAGE: ./a.out <input-file> <stats-output-file>" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::ifstream in_str(argv[1]);
	if (!in_str.good()) {
    std::cerr << "Can't open " << argv[1] << " to read.\n";
    exit(1);
  }
  std::ofstream out_str(argv[2]);
  if (!out_str.good()) {
    std::cerr << "Can't open " << argv[2] << " to write.\n";
    exit(1);
  }

  // parsing input file to a vector of Process
  std::vector<Process> processes;
  std::string line;
  while (std::getline(in_str, line)) {
    if (line[0] == '#' || line.empty()) continue;
    std::vector<std::string> proc_info = split_by(line, '|');
    if (proc_info.size() != 5 ) {
      std::cerr << "ERROR: Invalid input file format" << std::endl;
		  exit(EXIT_FAILURE);
    }
    processes.push_back(Process(proc_info[0][0], atoi(proc_info[1].c_str()),
     atoi(proc_info[2].c_str()), atoi(proc_info[3].c_str()), atoi(proc_info[4].c_str())));
  }
  in_str.close();

  // simulations
  out_str << std::fixed << std::setprecision(2);
  simulate_FCFS(processes, out_str); std::cout << std::endl;
  simulate_SJF(processes, out_str); std::cout << std::endl;
  simulate_RR(processes, out_str); //std::cout << std::endl;
  out_str.close();

	return EXIT_SUCCESS;
}
