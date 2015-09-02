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
 
    int proxIdDisponible();
    int encontrarPos(int);
    int finalizoQuantum(int);
    bool algunaLista();
    bool unicaTareaCorriendo();
    bool estaBloqueada(int);
    bool todasBloqueadas();

 struct tarea {

    int pid;
   // bool corriendo;
   // bool bloqueado;
    enum Estado estado;
   // tarea(int id): pid(id), bloqueado(false) {}; 
    tarea(int id): pid(id), estado(LISTA) {}; 

  };

  struct Cpu {
    int quantum;
    int contador;
    Cpu(int q): quantum(q), contador(0) {};
  };

    vector<tarea> cola; 
    vector<Cpu> cores;
    int idActual;

};

#endif
