#ifndef __SCHED_NOMIS__
#define __SCHED_NOMIS__

#include <vector>
#include <queue>
#include "basesched.h"

class SchedNoMistery : public SchedBase {
  public:
    SchedNoMistery(std::vector<int> argn);
    virtual void load(int pid);
    virtual void unblock(int pid);
    virtual int tick(int cpu, const enum Motivo m);  

  private:
    typedef struct misteryQueue{ 
      int quantum;
      queue cola;
      misteryQueue(int q) : quantum(q), cola() {};
    } misteryQueue;

    vector<misteryQueue> colas;
    int colaActual;
    vector<int> bloqueados;
};

#endif
