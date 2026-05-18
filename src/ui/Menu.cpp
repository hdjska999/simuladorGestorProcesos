#include "Menu.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using namespace std;

// =============================================
//  Pantalla de bienvenida en ASCII
// =============================================
void Menu::showWelcome() {
    cout << "\n";
    cout << "  ╔══════════════════════════════════════════════════════╗" << endl;
    cout << "  ║        SIMULADOR DE GESTOR DE PROCESOS               ║" << endl;
    cout << "  ║            Sistemas Operativos - 2026                ║" << endl;
    cout << "  ╠══════════════════════════════════════════════════════╣" << endl;
    cout << "  ║  Tick de reloj = 1 unidad de tiempo del SO           ║" << endl;
    cout << "  ║  RAM total del sistema = 4096 MB                     ║" << endl;
    cout << "  ╚══════════════════════════════════════════════════════╝" << endl;
    cout << "\n";
}

// =============================================
//  Solicitar configuracion al usuario
//  Solo 2 preguntas: algoritmo y cantidad de procesos
// =============================================
SimConfig Menu::getConfig() {
    SimConfig cfg;
    cfg.quantum = 2; // valor por defecto

    // --- Seleccion de algoritmo ---
    cout << "  Seleccione algoritmo de planificacion:\n\n";
    cout << "    [1] FCFS        - First Come First Served\n";
    cout << "    [2] SJF         - Shortest Job First\n";
    cout << "    [3] Round Robin - Quantum configurable\n";
    cout << "    [4] Prioridad   - Proceso de mayor prioridad primero\n";
    cout << "\n  Opcion: ";

    int algoOpt;
    cin >> algoOpt;

    switch (algoOpt) {
        case 1:  cfg.algorithm = FCFS;        break;
        case 2:  cfg.algorithm = SJF;         break;
        case 3:  cfg.algorithm = ROUND_ROBIN; break;
        case 4:  cfg.algorithm = PRIORITY;    break;
        default: cfg.algorithm = FCFS;        break;
    }

    // Quantum solo si eligio Round Robin
    if (cfg.algorithm == ROUND_ROBIN) {
        cout << "  Ingrese quantum (ticks por proceso): ";
        cin >> cfg.quantum;
        if (cfg.quantum < 1) cfg.quantum = 2;
    }

    // --- Cantidad de procesos ---
    cout << "  Cuantos procesos desea simular? (1-10): ";
    cin >> cfg.numProcesses;
    if (cfg.numProcesses < 1)  cfg.numProcesses = 1;
    if (cfg.numProcesses > 10) cfg.numProcesses = 10;

    return cfg;
}

// =============================================
//  Generar procesos con datos ALEATORIOS
//  y mostrar tabla de configuracion inicial
// =============================================
void Menu::generateProcesses(Scheduler& scheduler, const SimConfig& cfg) {
    srand((unsigned)time(nullptr));

    cout << "\n";
    cout << "  Generando " << cfg.numProcesses << " procesos con datos aleatorios...\n\n";

    // Tabla de procesos generados
    cout << "  +-----+----------+----------+--------+-----------+" << endl;
    cout << "  | PID | BURST(t) | MEM (MB) | PRIO   | LLEGADA(t)|" << endl;
    cout << "  +-----+----------+----------+--------+-----------+" << endl;

    for (int i = 1; i <= cfg.numProcesses; i++) {

        // Valores aleatorios dentro de rangos razonables
        int burst    = 1  + rand() % 8;   // 1 a 8 ticks
        int memory   = 128 + (rand() % 15) * 64; // 128, 192, 256 ... hasta 1024 MB
        int priority = 1  + rand() % 5;   // Prioridad 1 (alta) a 5 (baja)
        int arrival  = (i - 1) * (rand() % 2); // La mayoria llegan en tick 0, algunos en 1

        cout << "  | " << setw(3) << i
             << " | " << setw(6) << burst << " t  "
             << " | " << setw(6) << memory << " MB"
             << " | " << setw(6) << priority
             << " | " << setw(5)  << arrival << " t   "
             << " |" << endl;

        Process p(i, burst, priority, memory, arrival);
        scheduler.addProcess(p);
    }

    cout << "  +-----+----------+----------+--------+-----------+" << endl;
    cout << "\n  Iniciando simulacion...\n";
}
