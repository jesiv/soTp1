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
  this->idActual = -1;
}

SchedRR::~SchedRR() {

}


void SchedRR::load(int pid) {
  tarea nuevaTarea(pid);
  this->cola.push_back(nuevaTarea);
  if (this->cola.size() == 0)
    this->idActual = 0;
}

void SchedRR::unblock(int pid) {
  for (unsigned int i = 0 ; i < this->cola.size() ; i++) {
    if (this->cola[i].pid == pid) {
      this->cola[i].bloqueado = false;
    }
  }
}

bool SchedRR::estaBloqueada(int pid){
  int pos = encontrarPos(pid);
  return cola[pos].bloqueado;
}

int SchedRR::proxIdDisponible() {
  int res = this->idActual;
  bool encontre = false;
  int n = encontrarPos(this->idActual);
  int i = ((n + 1) % this->cola.size());
//  cout << "cola.size() : " << this->cola.size() << endl;
//  cout << "posncnon n: " << n << endl;
  //  cout << "posicion i: " << i << endl;
    //while (i != (idActual % ((unsigned int)this->cola.size())) && !encontre) {
    while (i != n && !encontre) {
      if (!this->cola[i].bloqueado && !cola[i].corriendo) {
        res = this->cola[i].pid;
        encontre = true;
      }
      i = (i+1) % this->cola.size();
    }
    if (!encontre && estaBloqueada(res)) {
      res = IDLE_TASK;  
    }

    return res;
}

int SchedRR::encontrarPos(int pid){ //el elemento tiene que estar en el vector
  int res = -1;
  for (unsigned int i = 0 ; i < this->cola.size() ; i++){
    if (cola[i].pid == pid ) {
      res = i; 
    }  
  }
  return res;
   
}

bool SchedRR::todasBloqueadas() {
  bool res = true;
  unsigned int i = 0;
  while ((i < this->cola.size()) && res) {
    if (!this->cola[i].bloqueado && !this->cola[i].corriendo)
      res = false;
    i++;
  }
  return res;
}

int SchedRR::tick(int cpu, const enum Motivo m) {
  int curr_pid = current_pid(cpu);
  int next_pid;
  

  if (m == TICK) {
    if (cola.size() != 0 && curr_pid == IDLE_TASK && !this->todasBloqueadas()) {
      this->idActual = proxIdDisponible();
      next_pid = this->idActual;
      curr_pid = this->idActual;
    } else {
      cores[cpu].contador++;
      if (cores[cpu].contador == cores[cpu].quantum) {
        cola[encontrarPos(curr_pid)].corriendo = false;
        next_pid = this->proxIdDisponible();
        cores[cpu].contador = 0;
      } else {
        next_pid = curr_pid;
      }
      
    }
  }

  if (m == BLOCK) {
    int posActual = encontrarPos(curr_pid);
    if ( !(this->cola[posActual].bloqueado) )
      this->cola[posActual].bloqueado = true;
    next_pid = this->proxIdDisponible();
    cores[cpu].contador = 0;
    cola[encontrarPos(curr_pid)].corriendo = false;
  }
 
  if (m == EXIT) {
    next_pid = this->proxIdDisponible();
    int pos = encontrarPos(curr_pid);
    this->cola.erase(this->cola.begin()+pos);    
    cores[cpu].contador = 0;
  }
 
  if ((this->cola.size() == 0) || this->todasBloqueadas() || (curr_pid == IDLE_TASK && this->todasBloqueadas())) {
    next_pid = IDLE_TASK; 
  } else {
    this->idActual = next_pid;
  }
  
  if (next_pid != IDLE_TASK)
    cola[encontrarPos(next_pid)].corriendo = true;
  return next_pid;

}
