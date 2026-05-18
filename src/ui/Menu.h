
#ifndef MENU_H
#define MENU_H

#include "../core/Scheduler.h"

// =============================================
//  Configuracion elegida por el usuario al inicio
// =============================================
struct SimConfig {
    Algorithm  algorithm;   // Algoritmo seleccionado
    int        numProcesses;// Cantidad de procesos a generar
    int        quantum;     // Solo para Round Robin
};

// =============================================
//  Interfaz de usuario (CLI)
// =============================================
class Menu {
public:
    // Muestra pantalla de bienvenida y obtiene configuracion
    static SimConfig getConfig();

    // Muestra una pantalla de bienvenida ASCII
    static void showWelcome();

    // Genera procesos con datos aleatorios segun la config
    static void generateProcesses(Scheduler& scheduler, const SimConfig& cfg);
};

#endif // MENU_H