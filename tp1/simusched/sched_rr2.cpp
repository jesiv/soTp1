#include "sched_rr2.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedRR2::SchedRR2(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
  int cantCores = argn[1];
  cout << "cantidad de cores " << cantCores << endl;
  queue<int> q;
  vector<int> v;
  for (int i = 0 ; i < cantCores ; i++ ) {
    Cpu c(argn[i+2]);
    cores.push_back(c);
    tareasBloqueadas.push_back(v);
    tareasACorrerPorCPU.push_back(q);
  }
  cout << "longitud de tareasACorrer" << tareasACorrerPorCPU.size() << endl;
  cout << "longitud de tareasBloqueeadad" << tareasBloqueadas.size() << endl;
}

SchedRR2::~SchedRR2() {

}

int SchedRR2::encontrarNucleo() {
  int res = 0;
//  int hare;
//  if (cores[0].pid == -1)
//    hare = 0;
//  if (cores[0].pid != -1)
//    hare = 1;

  int aux = tareasACorrerPorCPU[0].size() + tareasBloqueadas[0].size();
  int min = aux;   
  for (unsigned int i = 1; i < tareasACorrerPorCPU.size(); i++) {
 // if (cores[0].pid == -1)
 //   hare = 0;
 // if (cores[0].pid != -1)
 //   hare = 1;
    aux = tareasACorrerPorCPU[i].size() + tareasBloqueadas[i].size();
    if ( aux < min) {
      min = aux;
      res = i;  
    }    
  }
   cout << "el nucleo elegido es: HAAAAAAAAAAAAAAREEEEEEEE" << res << endl; 
  return res;
}

void SchedRR2::load(int pid) {
  int CPU = encontrarNucleo();
  cout << "tu vieja" << endl;
  tareasACorrerPorCPU[CPU].push(pid);
}

void SchedRR2::unblock(int pid) {
  int CPU = encontrarCPU(pid); //encuentro nucleo del pid
  int pos = encontrarPos(CPU, pid); //encuentroposicion entre las bloqueadas de dicho nucleo
  int tarea = tareasBloqueadas[CPU][pos]; //tarea a desbloquear
  tareasBloqueadas[CPU].erase(tareasBloqueadas[CPU].begin()+pos); //la elimino de las bloqueadas
  tareasACorrerPorCPU[CPU].push(tarea); //la pusheo a las LISTAS
}

int SchedRR2::encontrarCPU(int pid){ //el elemento tiene que estar en alguna CPU
  int res;
  for (unsigned int i = 0 ; i < tareasBloqueadas.size() ; i++){
    for (unsigned int j = 0 ; j < tareasBloqueadas[i].size() ; j++ ) {
      if ( tareasBloqueadas[i][j] == pid )
        res = i; 
    }  
  }
  return res;
}

int SchedRR2::encontrarPos(int CPU, int pid){ //el elemento tiene que estar en el vector
  int res;
  for (unsigned int i = 0 ; i < this->tareasBloqueadas[CPU].size() ; i++){
    if ( tareasBloqueadas[CPU][i] == pid )
      res = i; 
  }
  return res;
}

int SchedRR2::finalizoQuantum(int cpu){ 
  tareasACorrerPorCPU[cpu].push(cores[cpu].pid); // Lo meto al fondo
  cores[cpu].pid = tareasACorrerPorCPU[cpu].front();
  tareasACorrerPorCPU[cpu].pop();
  return cores[cpu].pid;
}

int SchedRR2::proxIdDisponible(int cpu) {
  int res;
  if (tareasACorrerPorCPU[cpu].size() == 0) {
    res = IDLE_TASK;
  } else {
    cores[cpu].pid = tareasACorrerPorCPU[cpu].front(); 
    tareasACorrerPorCPU[cpu].pop();
    res = cores[cpu].pid;
  }
  return res; 
}

int SchedRR2::tick(int cpu, const enum Motivo m) {
  int curr_pid = current_pid(cpu);
  int next_pid; 

  if (m == TICK) {
    if (curr_pid == IDLE_TASK) { 
      next_pid = proxIdDisponible(cpu);
    } else {
      cores[cpu].contador++;
      if (cores[cpu].contador == cores[cpu].quantum) { //si termino el quantum y no mando EXIT, seguro que queda al menos una tarea en la cola de tareaACorrer
        next_pid = finalizoQuantum(cpu);
        cores[cpu].contador = 0;
      } else {
        next_pid = curr_pid;
      }    
    }
  }

  if (m == BLOCK) {
    int bloq = cores[cpu].pid;
    tareasBloqueadas[cpu].push_back(bloq);
    cores[cpu].contador = 0;
    next_pid = proxIdDisponible(cpu);
  }

  if (m == EXIT) {
    next_pid = proxIdDisponible(cpu);
    this->cores[cpu].contador = 0;
  }

  return next_pid;
}
