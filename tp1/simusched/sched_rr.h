#ifndef __SCHED_RR__
#define __SCHED_RR__

#include <vector>
#include <queue>
#include "basesched.h"

using namespace std;

class SchedRR : public SchedBase {
	public:
		SchedRR(std::vector<int> argn);
        ~SchedRR();
		void load(int pid);
		void unblock(int pid);
		int tick(int cpu, const enum Motivo m);
    
  private:

    int proxIdDisponible();
    int encontrarPos(int);
    bool estaBloqueada(int);
    bool todasBloqueadas();

 struct tarea {
    int pid;
    bool corriendo;
    bool bloqueado;
    tarea(int id): pid(id), bloqueado(false) {}; 
  };

  struct Cpu {
    bool ocupado;
    int quantum;
    int contador;
    Cpu(int q): ocupado(false), quantum(q), contador(0) {};
  };

    vector<tarea> cola; 
    vector<Cpu> cores;
    int idActual;

};

#endif
