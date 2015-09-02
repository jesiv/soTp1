#ifndef __SCHED_RR__
#define __SCHED_RR__

#include <vector>
#include <queue>
#include "basesched.h"

using namespace std;

enum Estado {CORRIENDO, LISTA, BLOQUEADA};

class SchedRR : public SchedBase {
	public:
		SchedRR(std::vector<int> argn);
        ~SchedRR();
		void load(int pid);
		void unblock(int pid);
		int tick(int cpu, const enum Motivo m);
    
  private:
 
    int proxIdDisponible(int);
    int encontrarPos(int);
    int finalizoQuantum(int);
    bool cpusVacios();

    struct Cpu {
      int quantum;
      int pid;
      int contador;
      Cpu(int q): quantum(q), pid(0), contador(0) {};
    };

    queue<int> tareasACorrer;
    vector<int> tareasBloqueadas;
    vector<Cpu> cores;

};

#endif
