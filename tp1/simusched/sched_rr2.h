#ifndef __SCHED_RR2__
#define __SCHED_RR2__

#include <vector>
#include <queue>
#include "basesched.h"

using namespace std;

class SchedRR2 : public SchedBase {
	public:
		SchedRR2(std::vector<int> argn);
        ~SchedRR2();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);
  private:
 
    int proxIdDisponible(int);
    int encontrarPos(int, int);
    int finalizoQuantum(int);
    int encontrarCPU(int);
    int encontrarNucleo();

    struct Cpu {
      int quantum;
      int pid;
      int contador;
      Cpu(int q): quantum(q), pid(0), contador(0) {};
    };

    vector<queue<int> > tareasACorrerPorCPU;
    vector<vector<int> > tareasBloqueadas;
    vector<Cpu> cores;

};

#endif
