#include "core/Process.h"
#include "core/Scheduler.h"
#include "core/ResourceManager.h"
#include "ui/Menu.h"

#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

int main() {

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    // 1. Pantalla de bienvenida
    Menu::showWelcome();

    char again = 's';

    do {
        // 2. El usuario elige algoritmo y cantidad de procesos
        SimConfig cfg = Menu::getConfig();

        // 3. Crear el planificador con el algoritmo elegido
        Scheduler scheduler(cfg.algorithm, cfg.quantum);

        // 4. Crear gestor de recursos
        ResourceManager rm;

        // 5. Generar procesos aleatorios y agregarlos al scheduler
        Menu::generateProcesses(scheduler, cfg);

        // 6. Lanzar simulacion completa (tick a tick, con tabla en pantalla)
        scheduler.run(rm);

        // 7. Mostrar log completo de eventos
        scheduler.printEventLog();

        // 8. Mostrar estado final de la RAM
        rm.showResources();

        // 9. Repetir con otro algoritmo?
        cout << "\n  Desea simular con otro algoritmo? (s/n): ";
        cin >> again;

    } while (again == 's' || again == 'S');

    cout << "\n  Fin del simulador. Hasta luego.\n\n";
    return 0;
}