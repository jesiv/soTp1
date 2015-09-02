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
  this->idActual = IDLE_TASK;
}

SchedRR::~SchedRR() {
}


void SchedRR::load(int pid) {
  tarea nuevaTarea(pid);
  this->cola.push_back(nuevaTarea);
  if (this->cola.size() == 0) //esto no deberia ser necesario
    this->idActual = 0;
}

void SchedRR::unblock(int pid) {
  int pos = encontrarPos(pid);
  this->cola[pos].estado = CORRIENDO;
 // for (unsigned int i = 0 ; i < this->cola.size() ; i++) {
 //   if (this->cola[i].pid == pid) {
 //     this->cola[i].bloqueado = false;
 //   }
 // }
  return;
}


int SchedRR::proxIdDisponible() { //invariante: si esta LISTA entonces no esta BLOQUEADA ni CORRIENDO
  int res = IDLE_TASK;

  if(this->unicaTareaCorriendo()) {
    res = this->idActual;
  } else {
    if(this->algunaLista()) {
      res = this->idActual;
      bool encontre = false;
      int posIdActual = encontrarPos(this->idActual);
      int i = ((posIdActual + 1) % this->cola.size());
   
      while (i != posIdActual && !encontre) {
        if (this->cola[i].estado == LISTA) {
          res = this->cola[i].pid;
          encontre = true;
        }
        i = (i+1) % this->cola.size();
       }
     } 
   }
  return res;
}

bool SchedRR::unicaTareaCorriendo(){
  int  contador = 0;
  bool esLaActual = false;
  
  for (unsigned int i = 0; i < this->cola.size(); i++) {
    if (this->cola[i].estado == CORRIENDO){
      contador++;
      if (this->cola[i].pid == this->idActual) esLaActual = true;
    }
  }

  return (contador == 1 && esLaActual);
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

bool SchedRR::estaBloqueada(int pid){ //ver si es necesaria
  int pos = encontrarPos(pid);
  return cola[pos].estado == BLOQUEADA;
}

bool SchedRR::algunaLista() {
  bool res = false;
  unsigned int i = 0;
  while ((i < this->cola.size()) && res) {
    if (this->cola[i].estado == LISTA)
      res = true;
    i++;
  }
  return res;
}


int SchedRR::finalizoQuantum(int pid){ 
  int next_pid = this->proxIdDisponible();

  int posTareaActual = encontrarPos(pid);
  cola[posTareaActual].estado = LISTA;    
  int posTareaSiguiente = encontrarPos(next_pid);
  cola[posTareaSiguiente].estado = CORRIENDO;

  return next_pid;
}


int SchedRR::tick(int cpu, const enum Motivo m) {
  int curr_pid = current_pid(cpu);
  int next_pid; 

  cout << " IdActual: " << curr_pid << " |motivo" << m <<" |cantidad proc: " << this->cola.size() << endl;

  if (m == TICK) {
    if (curr_pid == IDLE_TASK) {
      next_pid = this->proxIdDisponible();
    } else {
      cores[cpu].contador++;
      if (cores[cpu].contador == cores[cpu].quantum) {
        next_pid = finalizoQuantum(curr_pid);
        cores[cpu].contador = 0;
      } else {
        next_pid = curr_pid;
      }    
    }
  }

  if (m == BLOCK) {
    int posTareaActual = encontrarPos(curr_pid);
    cola[posTareaActual].estado = BLOQUEADA;

    next_pid = proxIdDisponible();
    int posProxTarea = encontrarPos(next_pid);
    cola[posProxTarea].estado = CORRIENDO;
    cores[cpu].contador = 0;
  }  

  if (m == EXIT) {
    int pos  = encontrarPos(curr_pid);
    this->cola.erase(this->cola.begin()+pos);    
    next_pid = this->proxIdDisponible();
    cores[cpu].contador = 0;
  }
 
  cout << "proxPros: " << next_pid << endl;
 
 //if ((this->cola.size() == 0) || this->todasBloqueadas() || (curr_pid == IDLE_TASK && this->todasBloqueadas())) {
 //   next_pid = IDLE_TASK; 
 // } else {
 //   this->idActual = next_pid;
 // }
 //
  
  this->idActual = next_pid;
  return next_pid;
}
