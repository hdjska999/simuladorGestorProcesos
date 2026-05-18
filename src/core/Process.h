
#ifndef PROCESS_H
#define PROCESS_H

#include <string>

// Estados del proceso (PCB State Machine)
enum State {
  NEW,        // Recien creado, esperando admision
  READY,      // En cola de listos, esperando CPU
  RUNNING,    // Ejecutandose en la CPU
  WAITING,    // Bloqueado esperando un recurso (E/S)
  TERMINATED, // Finalizo su ejecucion
  DENIED      // Rechazado por falta de memoria
};

// Bloque de Control de Proceso (PCB)
struct Process {
  int pid;     // Identificador unico del proceso
  State state; // Estado actual en la maquina de estados

  int burstTime;     // Tiempo total de CPU que necesita (en ticks)
  int remainingTime; // Ticks que le faltan para terminar
  int priority;      // Prioridad (1=alta ... 5=baja)
  int memory;        // Memoria solicitada (MB)

  // Estadisticas de planificacion
  int arrivalTime;    // Tick en que llego al sistema
  int startTime;      // Tick en que la CPU lo tomo por primera vez
  int finishTime;     // Tick en que termino
  int waitTime;       // Cuantos ticks estuvo en READY sin CPU
  int turnaroundTime; // finishTime - arrivalTime
  int quantumUsed;    // Ticks usados del quantum actual (Round Robin)

  // Motivo de finalizacion
  std::string exitReason;

  Process(int id, int burst, int prio, int mem, int arrival);

  std::string getStateName() const;
  std::string getStateSymbol() const; // Simbolo corto para la tabla
};

#endif // PROCESS_H