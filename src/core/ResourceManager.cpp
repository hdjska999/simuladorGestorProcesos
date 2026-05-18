#include "ResourceManager.h"
#include <iostream>
#include <iomanip>

using namespace std;

// RAM total del sistema = 4096 MB
ResourceManager::ResourceManager() {
    totalMemory = 4096;
    usedMemory  = 0;
    peakMemory  = 0;
}

// Solicitar memoria; retorna false si no hay suficiente
bool ResourceManager::requestMemory(int amount) {
    if (usedMemory + amount > totalMemory) {
        return false;   // DENIED: sin memoria suficiente
    }
    usedMemory += amount;
    if (usedMemory > peakMemory) {
        peakMemory = usedMemory;
    }
    return true;
}

// Liberar memoria cuando el proceso termina
void ResourceManager::releaseMemory(int amount) {
    usedMemory -= amount;
    if (usedMemory < 0) usedMemory = 0;
}

// --- Getters ---
int ResourceManager::getTotalMemory() const { return totalMemory; }
int ResourceManager::getUsedMemory()  const { return usedMemory;  }
int ResourceManager::getFreeMemory()  const { return totalMemory - usedMemory; }
int ResourceManager::getPeakMemory()  const { return peakMemory;  }

// Muestra estado actual de la RAM
void ResourceManager::showResources() const {
    double pctPeak = (double)peakMemory / totalMemory * 100.0;
    double pctEnd  = (double)usedMemory / totalMemory * 100.0;

    cout << "\n+----------------------------------+" << endl;
    cout << "|    RESUMEN FINAL DE RECURSOS     |" << endl;
    cout << "+----------------------------------+" << endl;
    cout << "| RAM Total      : " << setw(5) << totalMemory   << " MB         |" << endl;
    cout << "| Max. RAM Usada : " << setw(5) << peakMemory    << " MB         |" << endl;
    cout << "| RAM Final Usada: " << setw(5) << usedMemory    << " MB         |" << endl;
    cout << "| Uso Maximo     : " << setw(4) << fixed << setprecision(1) << pctPeak << " %         |" << endl;
    cout << "+----------------------------------+" << endl;
}