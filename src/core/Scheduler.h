#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Process.h"
#include "ResourceManager.h"
#include <vector>
#include <string>
#include <queue> // Agregado para usar std::queue en Round Robin

// =============================================
//  Algoritmos de planificacion disponibles
// =============================================
enum Algorithm {
    FCFS,       // First Come First Served
    SJF,        // Shortest Job First
    ROUND_ROBIN,// Round Robin
    PRIORITY    // Planificacion por prioridad
};

// =============================================
//  Entrada del log de eventos del sistema
// =============================================
struct LogEntry {
    int tick;
    std::string event;
};

// =============================================
//  Planificador de Procesos
// =============================================
class Scheduler {
private:
    std::vector<Process> processes;  // Todos los procesos del sistema
    std::vector<LogEntry> eventLog;  // Log de eventos del simulador
    Algorithm algorithm;             // Algoritmo activo

    int currentTick;                 // Reloj global del sistema
    int quantum;                     // Quantum para Round Robin

    // --- Helpers internos ---
    void log(std::string msg);
    void printTable();
    void printStats();

    // --- Motores de cada algoritmo ---
    void runFCFS(ResourceManager& rm);
    void runSJF(ResourceManager& rm);
    void runRoundRobin(ResourceManager& rm);
    void runPriority(ResourceManager& rm);

public:
    Scheduler(Algorithm algo, int q);

    void addProcess(Process p);
    void run(ResourceManager& rm);      // Lanza la simulacion completa
    void printEventLog();               // Muestra el log al final
};

#endif // SCHEDULER_H