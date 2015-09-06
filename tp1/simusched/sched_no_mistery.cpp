#include <vector>
#include <queue>
#include "sched_no_mistery.h"
#include "basesched.h"


SchedNoMistery::SchedNoMistery(vector<int> argn) {  
  int argSize = argn.size();
  misteryQueue m(1);
  this->colas.push_back(m);

  for (int i = 1; i < argSize; i++) {
//    cout << "Argumento: " << i << " es : " << argn[i] << endl; 
    misteryQueue mq(argn[i]);
    this->colas.push_back(mq);
  }
  this->colaActual = 0;
  this->tareaCorriendo = 0;
}


void SchedNoMistery::load(int pid) {
  this->colas[0].cola.push(pid);
  // Todas empiezan en la primer cola de quantum = 1
}

int SchedNoMistery::encontrarBloqueado(int pid) { 
  int res = 0;
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
  for (unsigned int i = 0; i < colas.size(); i++) {
    if (colas[i].cola.size() > 0 && !encontre) {
      res = i;
      encontre = true;
    }
  }
  return res;
}

int SchedNoMistery::proxIdDisponible() {
  int proxC = proxCola();
  int res;
  if (proxC == -1) {
    res = IDLE_TASK;
  } else {
  res = colas[proxC].cola.front();
  this->colas[proxC].cola.pop();
  this->tareaCorriendo = res;
  this->colaActual = proxC;
  }
  return res;
}

int SchedNoMistery::finalizoQuantum(int pid){ 
  int tam = this->colas.size() - 1;
  if (this->colaActual != tam) {
    this->colas[colaActual+1].cola.push(pid); // Lo meto al fondo de la siguiente 
   // cout << "cola a insertar: " << colaActual+1 << endl;
   // cout << "tareaActual: "<< tareaCorriendo << endl;
  } else {
    this->colas[colaActual].cola.push(pid);
  }

  return proxIdDisponible(); 
}

int SchedNoMistery::tick(int cpu, const enum Motivo m) {  
  int curr_pid = current_pid(cpu);
  int next_pid; 
//en tick tengo que actualizar colaActual
  if (m == TICK) {
    if (curr_pid == IDLE_TASK) { 
     // cout << "curr_pid = Idle" << endl;
      next_pid = this->proxIdDisponible();
     // cout << "next_pid = " << next_pid << endl;
    } else {
      colas[colaActual].contador++;
     // cout << "cola a: " << colaActual << "cola contador: " << colas[colaActual].contador << "cola quantum: " << colas[colaActual].quantum << endl;

      if (colas[colaActual].contador == colas[colaActual].quantum) {
      //  cout << "termino el quantum vieja" << endl;
        colas[colaActual].contador = 0;
        next_pid = finalizoQuantum(curr_pid);
      //  cout << "next_pid = " << next_pid << endl;
      } else {
     // cout << "hare3" << endl;
      //  cout << "NO termino el quantum vieja" << endl;
        next_pid = curr_pid;
      //  cout << "next_pid = " << next_pid << endl;
      }
    }
  }

  if (m == BLOCK) {
    //cout << "Cola Actual : " << colaActual << endl;
//    int bloq = this->colas[colaActual].cola.front();
    //cout << "Primer elemento siguiente: " << colas[colaActual].cola.front() << endl;
    //this->colas[colaActual].cola.pop();
    this->bloqueados.push_back(curr_pid);
    colas[colaActual].contador = 0;
    next_pid = this->proxIdDisponible();
    //cout << "Cola Actual : " << colaActual << endl;
    //cout << "Primer elemento siguiente: " << next_pid << endl;
  }

  if (m == EXIT) {
    this->colas[colaActual].contador = 0;
    next_pid = this->proxIdDisponible();
  }

  //cout << "next_pid : " << next_pid << endl;

  return next_pid;
}
