#include <vector>
#include <queue>
#include "sched_rr.h"
#include "basesched.h"
#include <iostream>

SchedRR::SchedRR(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
  for (unsigned int i = 1 ; i < argn.size() ; i++ ) {
    Cpu c(argn[i]);
    this->cores.push_back(c);
  }
  this->proxTarea = IDLE_TASK;
}

SchedRR::~SchedRR() {

}


void SchedRR::load(int pid) {
  this->cola.push_back(pid);
}

void SchedRR::unblock(int pid) {
  for (unsigned int i = 0 ; i < this->cola.size() ; i++) {
    if (this->cola[i].pid == pid) {
      this->cola[i].bloqueado = false;
    }
  }
}

int SchedRR::proxDisponible() {
  int res = IDLE_TASK;
  bool encontre = false;
  unsigned int i = this->proxTarea+1 % this->cola.size();
  while (i != (proxTarea % (unsigned int)this->cola.size()) && !encontre) {
    if (!this->cola[i].bloqueado) {
      res = this->cola[i].pid;
      encontre = true;
    }
    i = (i+1) % this->cola.size();
  }
  return res;
}

int SchedRR::tick(int cpu, const enum Motivo m) {
  int curr_pid = current_pid(cpu);
  int next_pid;

  if (m == TICK) {

  }
  if (m == BLOCK) {
    if (!(this->cola[curr_pid].bloqueado))
      this->cola[curr_pid].bloqueado = true;
  }
  if (m == EXIT) {
  
  }
  this->proxTarea = this->proxDisponible();
  next_pid = this->proxTarea;

  return next_pid;
}
