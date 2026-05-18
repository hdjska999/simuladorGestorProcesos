#include "Scheduler.h"
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>

using namespace std;

// Constructor simple
Scheduler::Scheduler(Algorithm algo, int q) {
    algorithm = algo;
    quantum = q;
    currentTick = 0;
}

// Agregar proceso a la lista
void Scheduler::addProcess(Process p) {
    processes.push_back(p);
}

// Guardar mensaje en el log
void Scheduler::log(string msg) {
    LogEntry entrada;
    entrada.tick = currentTick;
    entrada.event = msg;
    eventLog.push_back(entrada);
}

// Mostrar la tabla de procesos en consola
void Scheduler::printTable() {
    // Pausa para que se vea el avance (300ms)
    this_thread::sleep_for(chrono::milliseconds(300));

    cout << "\n";
    cout << "  TICK [" << currentTick << "]" << endl;

    // Calcular RAM libre sumando la de los procesos activos
    int usedMem = 0;
    for (int i = 0; i < processes.size(); i++) {
        if (processes[i].state == RUNNING || processes[i].state == READY || processes[i].state == WAITING) {
            usedMem += processes[i].memory;
        }
    }
    cout << "  RAM libre: " << (4096 - usedMem) << " MB" << endl;

    // Imprimir encabezado
    cout << "  +---------+--------+----------+-------+----------+----------+----------+" << endl;
    cout << "  |   PID   | PRIO   |  ESTADO  | MEM   | BURST    | RESTANTE | ESPERA   |" << endl;
    cout << "  +---------+--------+----------+-------+----------+----------+----------+" << endl;

    // Imprimir cada proceso
    for (int i = 0; i < processes.size(); i++) {
        
        string marker = "  ";
        if (processes[i].state == RUNNING)    marker = ">>";
        if (processes[i].state == TERMINATED) marker = "--";
        if (processes[i].state == DENIED)     marker = "XX";

        cout << marker
             << "| " << setw(7) << processes[i].pid           << " "
             << "| " << setw(6) << processes[i].priority      << " "
             << "| " << setw(8) << processes[i].getStateSymbol() << " "
             << "| " << setw(4) << processes[i].memory << "MB" << " "
             << "| " << setw(4) << processes[i].burstTime << "t " << " "
             << "| " << setw(4) << processes[i].remainingTime << "t " << " "
             << "| " << setw(4) << processes[i].waitTime << "t " << " "
             << "|" << endl;
    }

    cout << "  +---------+--------+----------+-------+----------+----------+----------+" << endl;
    cout.flush();
}

// Estadisticas finales
void Scheduler::printStats() {
    cout << "\n";
    cout << "  +=========================================================+" << endl;
    cout << "  |               ESTADISTICAS FINALES                      |" << endl;
    cout << "  +=========================================================+" << endl;
    cout << "  | PID   | BURST   | ESPERA  | RETORNO   | MOTIVO          |" << endl;
    cout << "  +-------+---------+---------+-----------+-----------------+" << endl;

    double totalWait = 0;
    double totalTurn = 0;
    int counted = 0;

    for (int i = 0; i < processes.size(); i++) {
        if (processes[i].state == DENIED) {
            continue;
        }

        cout << "  | " << setw(5) << processes[i].pid
             << " | " << setw(5) << processes[i].burstTime << " t"
             << " | " << setw(5) << processes[i].waitTime  << " t"
             << " | " << setw(7) << processes[i].turnaroundTime << " t"
             << " | " << setw(15) << processes[i].exitReason
             << " |" << endl;
             
        totalWait += processes[i].waitTime;
        totalTurn += processes[i].turnaroundTime;
        counted++;
    }

    if (counted > 0) {
        cout << "  +-------+---------+---------+-----------+-----------------+" << endl;
        cout << "  | Tiempo prom. espera  : " << fixed << setprecision(2) << (totalWait / counted) << " ticks" << endl;
        cout << "  | Tiempo prom. retorno : " << fixed << setprecision(2) << (totalTurn / counted) << " ticks" << endl;
    }
    cout << "  +=========================================================+" << endl;
}

// Lanzar simulacion dependiendo del algoritmo
void Scheduler::run(ResourceManager& rm) {
    cout << "\n  *** Iniciando simulacion con " << processes.size() << " procesos ***\n";

    if (algorithm == FCFS) {
        runFCFS(rm);
    } else if (algorithm == SJF) {
        runSJF(rm);
    } else if (algorithm == ROUND_ROBIN) {
        runRoundRobin(rm);
    } else if (algorithm == PRIORITY) {
        runPriority(rm);
    }

    printStats();
}

// =============================================
//  ALGORITMO 1: FCFS (First Come First Served)
// =============================================
void Scheduler::runFCFS(ResourceManager& rm) {
    log("Iniciando FCFS");

    // Paso 1: Admitir procesos
    for (int i = 0; i < processes.size(); i++) {
        if (rm.requestMemory(processes[i].memory)) {
            processes[i].state = READY;
            log("P" + to_string(processes[i].pid) + " admitido");
        } else {
            processes[i].state = DENIED;
            processes[i].exitReason = "Sin RAM";
            log("P" + to_string(processes[i].pid) + " DENEGADO");
        }
    }

    printTable();

    // Paso 2: Ejecutar los procesos uno por uno en orden
    for (int i = 0; i < processes.size(); i++) {
        
        // Si no fue admitido, lo ignoramos
        if (processes[i].state == DENIED) {
            continue;
        }

        // Poner en ejecucion
        processes[i].state = RUNNING;
        processes[i].startTime = currentTick;
        log("P" + to_string(processes[i].pid) + " -> RUNNING");

        // Bucle de ejecucion (Tick por Tick)
        while (processes[i].remainingTime > 0) {
            currentTick++;

            // Sumar tiempo de espera a los demas procesos que estan READY
            for (int j = 0; j < processes.size(); j++) {
                if (processes[j].state == READY) {
                    processes[j].waitTime++;
                }
            }

            processes[i].remainingTime--;
            printTable();
        }

        // Terminar proceso
        processes[i].state = TERMINATED;
        processes[i].finishTime = currentTick;
        processes[i].turnaroundTime = processes[i].finishTime - processes[i].arrivalTime;
        processes[i].exitReason = "Normal";

        rm.releaseMemory(processes[i].memory);
        log("P" + to_string(processes[i].pid) + " TERMINADO");

        printTable();
    }
}

// =============================================
//  ALGORITMO 2: SJF (Shortest Job First)
// =============================================
void Scheduler::runSJF(ResourceManager& rm) {
    log("Iniciando SJF");

    // Paso 1: Admitir procesos
    for (int i = 0; i < processes.size(); i++) {
        if (rm.requestMemory(processes[i].memory)) {
            processes[i].state = READY;
            log("P" + to_string(processes[i].pid) + " admitido");
        } else {
            processes[i].state = DENIED;
            processes[i].exitReason = "Sin RAM";
        }
    }

    printTable();

    int totalProcesos = processes.size();
    int procesosTerminados = 0;

    // Contar los denegados como terminados para no buscar infinitamente
    for (int i = 0; i < processes.size(); i++) {
        if (processes[i].state == DENIED) {
            procesosTerminados++;
        }
    }

    // Mientras falten procesos por terminar
    while (procesosTerminados < totalProcesos) {
        
        // Buscar el proceso READY con el MENOR tiempo (burstTime)
        int indexElegido = -1;
        int menorTiempo = 999999; // Numero muy grande

        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].state == READY) {
                if (processes[i].burstTime < menorTiempo) {
                    menorTiempo = processes[i].burstTime;
                    indexElegido = i;
                }
            }
        }

        // Si no hay nadie listo, avanzamos el tiempo (CPU ociosa)
        if (indexElegido == -1) {
            currentTick++;
            printTable();
            continue;
        }

        // Poner en ejecucion el proceso elegido
        processes[indexElegido].state = RUNNING;
        processes[indexElegido].startTime = currentTick;
        log("P" + to_string(processes[indexElegido].pid) + " -> RUNNING");

        // Bucle de ejecucion (Tick por Tick)
        while (processes[indexElegido].remainingTime > 0) {
            currentTick++;

            // Sumar tiempo de espera a los demas
            for (int i = 0; i < processes.size(); i++) {
                if (processes[i].state == READY) {
                    processes[i].waitTime++;
                }
            }

            processes[indexElegido].remainingTime--;
            printTable();
        }

        // Terminar proceso
        processes[indexElegido].state = TERMINATED;
        processes[indexElegido].finishTime = currentTick;
        processes[indexElegido].turnaroundTime = processes[indexElegido].finishTime - processes[indexElegido].arrivalTime;
        processes[indexElegido].exitReason = "Normal";

        rm.releaseMemory(processes[indexElegido].memory);
        log("P" + to_string(processes[indexElegido].pid) + " TERMINADO");
        printTable();
        
        procesosTerminados++;
    }
}

// =============================================
//  ALGORITMO 3: ROUND ROBIN
// =============================================
void Scheduler::runRoundRobin(ResourceManager& rm) {
    log("Iniciando Round Robin (quantum=" + to_string(quantum) + ")");

    queue<int> colaListos; // Guarda la posicion (indice) del proceso en la lista

    // Paso 1: Admitir procesos y meter a la cola
    for (int i = 0; i < processes.size(); i++) {
        if (rm.requestMemory(processes[i].memory)) {
            processes[i].state = READY;
            colaListos.push(i);
            log("P" + to_string(processes[i].pid) + " admitido a la cola");
        } else {
            processes[i].state = DENIED;
            processes[i].exitReason = "Sin RAM";
        }
    }

    printTable();

    // Mientras haya procesos en la cola
    while (!colaListos.empty()) {
        
        // Sacar el primero de la cola
        int index = colaListos.front();
        colaListos.pop();

        processes[index].state = RUNNING;
        
        // Si es la primera vez que corre, guardar su startTime
        if (processes[index].startTime == -1) {
            processes[index].startTime = currentTick;
        }
        
        processes[index].quantumUsed = 0;
        log("P" + to_string(processes[index].pid) + " -> RUNNING");

        // Ejecutar mientras tenga quantum y tiempo restante
        while (processes[index].quantumUsed < quantum && processes[index].remainingTime > 0) {
            currentTick++;
            processes[index].quantumUsed++;
            processes[index].remainingTime--;

            // Aumentar espera a los demas procesos que estan READY
            for (int i = 0; i < processes.size(); i++) {
                if (processes[i].state == READY && i != index) {
                    processes[i].waitTime++;
                }
            }

            printTable();
        }

        // Revisar que paso con el proceso
        if (processes[index].remainingTime == 0) {
            // Ya termino completamente
            processes[index].state = TERMINATED;
            processes[index].finishTime = currentTick;
            processes[index].turnaroundTime = processes[index].finishTime - processes[index].arrivalTime;
            processes[index].exitReason = "Normal";
            
            rm.releaseMemory(processes[index].memory);
            log("P" + to_string(processes[index].pid) + " TERMINADO");

        } else {
            // Se le acabo el quantum pero no ha terminado
            processes[index].state = READY;
            colaListos.push(index); // Lo volvemos a meter al final de la cola
            log("P" + to_string(processes[index].pid) + " quantum agotado -> READY");
        }
        
        printTable();
    }
}

// =============================================
//  ALGORITMO 4: PRIORIDAD
// =============================================
void Scheduler::runPriority(ResourceManager& rm) {
    log("Iniciando Planificacion por Prioridad");

    // Paso 1: Admitir procesos
    for (int i = 0; i < processes.size(); i++) {
        if (rm.requestMemory(processes[i].memory)) {
            processes[i].state = READY;
            log("P" + to_string(processes[i].pid) + " admitido");
        } else {
            processes[i].state = DENIED;
            processes[i].exitReason = "Sin RAM";
        }
    }

    printTable();

    int totalProcesos = processes.size();
    int procesosTerminados = 0;

    for (int i = 0; i < processes.size(); i++) {
        if (processes[i].state == DENIED) {
            procesosTerminados++;
        }
    }

    // Mientras falten procesos
    while (procesosTerminados < totalProcesos) {
        
        // Buscar el proceso READY con la MEJOR prioridad (el numero mas chiquito)
        int indexElegido = -1;
        int mejorPrioridad = 999999;

        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].state == READY) {
                if (processes[i].priority < mejorPrioridad) {
                    mejorPrioridad = processes[i].priority;
                    indexElegido = i;
                }
            }
        }

        // Si no hay, CPU espera
        if (indexElegido == -1) {
            currentTick++;
            printTable();
            continue;
        }

        // Ejecutar
        processes[indexElegido].state = RUNNING;
        processes[indexElegido].startTime = currentTick;
        log("P" + to_string(processes[indexElegido].pid) + " -> RUNNING");

        while (processes[indexElegido].remainingTime > 0) {
            currentTick++;

            for (int i = 0; i < processes.size(); i++) {
                if (processes[i].state == READY) {
                    processes[i].waitTime++;
                }
            }

            processes[indexElegido].remainingTime--;
            printTable();
        }

        // Terminar
        processes[indexElegido].state = TERMINATED;
        processes[indexElegido].finishTime = currentTick;
        processes[indexElegido].turnaroundTime = processes[indexElegido].finishTime - processes[indexElegido].arrivalTime;
        processes[indexElegido].exitReason = "Normal";

        rm.releaseMemory(processes[indexElegido].memory);
        log("P" + to_string(processes[indexElegido].pid) + " TERMINADO");
        printTable();
        
        procesosTerminados++;
    }
}

// Imprimir log de eventos final
void Scheduler::printEventLog() {
    cout << "\n  +=========================================================+" << endl;
    cout << "  |                   LOG DE EVENTOS                        |" << endl;
    cout << "  +=========================================================+" << endl;
    for (int i = 0; i < eventLog.size(); i++) {
        cout << "  | [T=" << setw(3) << eventLog[i].tick << "] "
             << left << setw(50) << eventLog[i].event
             << " |" << endl;
    }
    cout << "  +=========================================================+" << endl;
}
