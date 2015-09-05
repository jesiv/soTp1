#include <vector>
#include <queue>
#include "sched_rr.h"
#include "basesched.h"
#include <iostream>

SchedRR::SchedRR(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
  int cores = argn[1];
  for (int i = 0 ; i < cores ; i++ ) {
    Cpu c(argn[i+2]);
    this->cores.push_back(c);
  }
}

SchedRR::~SchedRR() {
}

void SchedRR::load(int pid) {
  this->tareasACorrer.push(pid);
}


void SchedRR::unblock(int pid) {
  int pos = encontrarPos(pid);
  int sig = this->tareasBloqueadas[pos];
  this->tareasBloqueadas.erase(this->tareasBloqueadas.begin()+pos);
  this->tareasACorrer.push(sig);
}

int SchedRR::encontrarPos(int pid){ //el elemento tiene que estar en el vector
  int res = 0;
  for (unsigned int i = 0 ; i < this->tareasBloqueadas.size() ; i++){
    if (this->tareasBloqueadas[i] == pid )
      res = i; 
  }
  return res;
}

int SchedRR::finalizoQuantum(int cpu){ 
  this->tareasACorrer.push(this->cores[cpu].pid); // Lo meto al fondo
  this->cores[cpu].pid = this->tareasACorrer.front();
  this->tareasACorrer.pop();
  return this->cores[cpu].pid;
}

int SchedRR::proxIdDisponible(int cpu) {
  int res;
  if (tareasACorrer.size() == 0) {
    res = IDLE_TASK;
  } else {
    cores[cpu].pid = this->tareasACorrer.front(); 
    this->tareasACorrer.pop();
    res = cores[cpu].pid;
  }
  return res; 
}

bool SchedRR::cpusVacios() {
  vector<Cpu>::iterator itA;
  bool vacios = true;
  for (itA = this->cores.begin() ; itA != this->cores.end() ; itA++) {
    if (itA->contador != 0)
      vacios = false;
  }
  return vacios;
}

int SchedRR::tick(int cpu, const enum Motivo m) {
  int curr_pid = current_pid(cpu);
  int next_pid; 

//  cout << "curr_pid : " << curr_pid << "cpu : " << cpu << " cpu : " << cpu << " motivo : " << m << endl; 

  if (m == TICK) {
    if (curr_pid == IDLE_TASK) { //&& cpusVacios()) {
    // cout << "aaaaaaaaaaaaaaaaaaaaaaahola  " << endl;
      next_pid = this->proxIdDisponible(cpu);
    } else {
      cores[cpu].contador++;
      if (cores[cpu].contador == cores[cpu].quantum) {
        next_pid = finalizoQuantum(cpu);
        cores[cpu].contador = 0;
      } else {
        next_pid = curr_pid;
      }    
    }
  }

  if (m == BLOCK) {
    int bloq = this->cores[cpu].pid;
    this->tareasBloqueadas.push_back(bloq);
    cores[cpu].contador = 0;
    next_pid = this->proxIdDisponible(cpu);
  }

  if (m == EXIT) {
    next_pid = this->proxIdDisponible(cpu);
    this->cores[cpu].contador = 0;
  }

  //cout << "next_pid : " << next_pid << endl;

  return next_pid;
}
