#include "tasks.h"

using namespace std;

void TaskCPU(int pid, vector<int> params) { // params: n
	uso_CPU(pid, params[0]); // Uso el CPU n milisegundos.
}

void TaskIO(int pid, vector<int> params) { // params: ms_pid, ms_io,
	uso_CPU(pid, params[0]); // Uso el CPU ms_pid milisegundos.
	uso_IO(pid, params[1]); // Uso IO ms_io milisegundos.
}

void TaskAlterno(int pid, vector<int> params) { // params: ms_pid, ms_io, ms_pid, ...
	for(int i = 0; i < (int)params.size(); i++) {
		if (i % 2 == 0) uso_CPU(pid, params[i]);
		else uso_IO(pid, params[i]);
	}
}

int generate(unsigned int *seed, int bmin, int bmax){
  int distancia = bmax - bmin + 1;
  int valor = rand_r(seed);
  return (valor % distancia) + bmin;
}

// Ejercicio 1
void TaskConsola(int pid, vector<int> params){
  int n = params[0];
  int bmin = params[1];
  int bmax = params[2];
  int tiempo;
  unsigned int seed;
  seed = time(NULL);
  for(int i = 0; i < n; i++){
    tiempo = generate(&seed, bmin, bmax);
    uso_IO(pid, tiempo);    
  }  
  return;
}

// Ejercicio 2
void TaskRolando(int pid, vector<int> params) {
  int tiempo = params[0];
  uso_CPU(pid, tiempo);
}

void TaskCancion(int pid, vector<int> params) {
  int tiempoBloqueo = params[0];
  unsigned int seed;
  seed = time(NULL);
  int tiempo;
  for(int i = 0; i < tiempoBloqueo; i++) {
    tiempo = generate(&seed, 2, 4);
    uso_IO(pid, tiempo);    
  }  
}

void TaskInternet(int pid, vector<int> params) {
  int tiempoBloqueo = params[0];
  unsigned int seed;
  seed = time(NULL);
  int tiempo;
  for(int i = 0; i < tiempoBloqueo; i++) {
    tiempo = generate(&seed, 2, 4);
    uso_IO(pid, tiempo);    
  }  
}

// Ejercicio 3
void TaskBatch(int pid, vector<int> params) {
  int tiempoCpu = params[0];
  int cantBloqueos = params[1];
  int tiempoTotal = tiempoCpu - cantBloqueos;
  unsigned int seed = time(NULL);
  
  while (tiempoTotal > 0 && cantBloqueos > 0) {
    int decision = (rand_r(&seed)) % 2;
    if (decision) {
      uso_IO(pid, 1);
      cantBloqueos--;
    } else {
      uso_CPU(pid, 1);
      tiempoTotal--;
    }
  }

  if (tiempoTotal != 0) {
    while (tiempoTotal > 0) {
      uso_CPU(pid, 1);
      tiempoTotal--;
    }
  } else {
    while (cantBloqueos > 0) {
      uso_IO(pid, 1);
        cantBloqueos--;
    }
  }
}

void tasks_init(void) {
	/* Todos los tipos de tareas se deben registrar acá para poder ser usadas.
	 * El segundo parámetro indica la cantidad de parámetros que recibe la tarea
	 * como un vector de enteros, o -1 para una cantidad de parámetros variable. */
	register_task(TaskCPU, 1);
	register_task(TaskIO, 2);
	register_task(TaskAlterno, -1);
  //Ejercicio 1
	register_task(TaskConsola, 3);
  //Ejercicio 2 
	register_task(TaskRolando, 1);
	register_task(TaskCancion, 1);
	register_task(TaskInternet, 1);
  //Ejercicio 3
  register_task(TaskBatch, 2);

}
