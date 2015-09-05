#ifndef __SCHED_NOMIS__
#define __SCHED_NOMIS__

#include <vector>
#include <queue>
#include "basesched.h"

using namespace std;

class SchedNoMistery : public SchedBase {
  public:
    SchedNoMistery(std::vector<int> argn);
    virtual void load(int pid);
    virtual void unblock(int pid);
    virtual int tick(int cpu, const enum Motivo m);  

  private:

    int encontrarBloqueado(int);
    int proxCola();
    int proxIdDisponible();
    int finalizoQuantum();

    struct misteryQueue { 
      int quantum;
      int contador;
      queue<int> cola;
      misteryQueue(int q) : quantum(q){};
    };


    int tareaCorriendo;
    vector<misteryQueue> colas;
    int colaActual;
    vector<int> bloqueados;

    
};

#endif
