#include <vector>
#include <queue>
#include "sched_no_mistery.h"
#include "basesched.h"

using namespace std;

SchedNoMistery::SchedNoMistery(vector<int> argn) {  
  int argSize = argn.size();
  misteryQueue m(1);
  this->colas.push_back(m);

  for (int i = 0; i < argSize; i++) {
    misteryQueue mq(argn[i]);
    this->colas.push_back(mq);
  }
  this->colaActual = 0;
}


void SchedNoMistery::load(int pid) {
  this->colas[0].cola.push(pid);
  // Todas empiezan en la primer cola de quantum = 1
}

int SchedNoMistery::encontrarBloqueado(int pid) { 
  int res;
  for (unsigned int i = 0 ; i < this->bloqueados.size() ; i++){
    if (this->bloqueados[i] == pid )
      res = i; 
  }
  return res;
}
// Busqueda secuencial en el vector de bloqueados

void SchedNoMistery::unblock(int pid) {  
  int pos = encontrarBloqueado(pid);
  int desBloq = this->bloqueados[pos];
  bloqueados.erase(bloqueados.begin()+pos);
  colas[0].cola.push(desBloq);  //encolo pid desbloqueado a la cola de quantum 1
}

int SchedNoMistery::proxCola() {
  int res = -1; //todas las colas estan vacias
  bool encontre = false;
  for (int i = 0; i < colas.size(); i++) {
    if (colas[i].size() > 0 && !encontre) {
      res = i;
      encontre = true;
    }
  }
  return res;
}

int SchedNoMistery::proxIdDisponible() {
  int proxC = proxCola();
  if (proxC == -1) {
    res = IDLE_TASK;
  } else {
  int res = colas[proxC].cola.front();
  this->colas[proxC].cola.pop();
  this->tareaCorriendo = res;
  this->colaActual = proxC;
  }
  return res;
}

int SchedRR::finalizoQuantum(int cpu){ 
  this->colas[colaActual+1].push(tareaCorriendo); // Lo meto al fondo de la siguiente 
  this->tareaCorriendo = proxIdDisponible();

  return tareaCorriendo; 
}

int SchedNoMistery::tick(int cpu, const enum Motivo m) {  
  int curr_pid = current_pid(cpu);
  int next_pid; 
//en tick tengo que actualizar colaActual
  if (m == TICK) {
    if (curr_pid == IDLE_TASK) { 
      next_pid = this->proxIdDisponible();
    } else {
      colas[colaActual].contador++;
      if (cores[colaActual].contador == cores[colaActual].quantum) {
        next_pid = finalizoQuantum(cpu);
        colas[colaActual].contador = 0;
      } else {
        next_pid = curr_pid;
      }
    }
  }

  if (m == BLOCK) {
    int bloq = this->colas[colaActual].front();
    this->colas[colaActual].pop()
    this->tareasBloqueadas.push_back(bloq);
    colas[colaActual].contador = 0;
    next_pid = this->proxIdDisponible();
  }

  if (m == EXIT) {
    next_pid = this->proxIdDisponible();
    this->cores[colaActual].contador = 0;
  }

  //cout << "next_pid : " << next_pid << endl;

  return next_pid;
}
