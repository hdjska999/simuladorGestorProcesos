#include "Process.h"

// =============================================
//  Constructor del PCB
// =============================================
Process::Process(int id, int burst, int prio, int mem, int arrival) {
    pid           = id;
    burstTime     = burst;
    remainingTime = burst;
    priority      = prio;
    memory        = mem;
    arrivalTime   = arrival;
    state         = NEW;

    startTime     = -1;   // -1 = aun no ha corrido
    finishTime    = -1;
    waitTime      = 0;
    turnaroundTime= 0;
    quantumUsed   = 0;
    exitReason    = "";
}

// =============================================
//  Nombre legible del estado
// =============================================
std::string Process::getStateName() const {
    switch (state) {
        case NEW:        return "NEW";
        case READY:      return "READY";
        case RUNNING:    return "RUNNING";
        case WAITING:    return "WAITING";
        case TERMINATED: return "TERMINATED";
        case DENIED:     return "DENIED";
        default:         return "UNKNOWN";
    }
}

// =============================================
//  Simbolo corto para la tabla de consola
// =============================================
std::string Process::getStateSymbol() const {
    switch (state) {
        case NEW:        return "NEW   ";
        case READY:      return "READY ";
        case RUNNING:    return "RUN   ";
        case WAITING:    return "WAIT  ";
        case TERMINATED: return "DONE  ";
        case DENIED:     return "DENIED";
        default:         return "??????";
    }
}