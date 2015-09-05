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
}

int SchedNoMistery::encontrarBloqueado(int pid) { 
  int res;
  for (unsigned int i = 0 ; i < this->bloqueados.size() ; i++){
    if (this->bloqueados[i] == pid )
      res = i; 
  }
  return res;
}

void SchedNoMistery::unblock(int pid) {  
  int pos = encontrarBloqueado(pid);

}

int SchedNoMistery::tick(int cpu, const enum Motivo m) {  
}
