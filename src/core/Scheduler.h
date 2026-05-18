#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Process.h"
#include "ResourceManager.h"
#include <queue> //std::queue en Round Robin
#include <string>
#include <vector>

// Los algoritmos de planificacion disponibles en etiquetas para una mejor
// redaccion
enum Algorithm {
  FCFS,        // First Come First Served
  SJF,         // Shortest Job First
  ROUND_ROBIN, // Round Robin
  PRIORITY     // Prioridad
};

// Guarda en qué momento pasó (tick) y qué fue lo que pasó (event)
struct LogEntry {
  int tick;
  std::string event;
};

// Clase Planificador
class Scheduler {
private:                          // Motor interno, no interactua con el usuario
  std::vector<Process> processes; // Lista de todos los procesos del sistema
  std::vector<LogEntry>
      eventLog; // Lista de todos los mensajes de los eventos del simulador
  Algorithm algorithm; // Es el algoritmo activo al momento

  int currentTick; // Reloj global del sistema
  int quantum;     // Quantum especifico para Round Robin

  void log(std::string msg); // Para crear un logentry y añadirselo a la lista
  void printTable();
  void printStats();

  // Motores
  void runFCFS(ResourceManager &rm);
  void runSJF(ResourceManager &rm);
  void runRoundRobin(ResourceManager &rm);
  void runPriority(ResourceManager &rm);

public:
  Scheduler(Algorithm algo, int q); // Constructor

  void addProcess(Process p);    // Agregar un proceso al sistema
  void run(ResourceManager &rm); // Lanza la simulacion completa
  void printEventLog();          // Muestra el log al final
};

#endif // SCHEDULER_H