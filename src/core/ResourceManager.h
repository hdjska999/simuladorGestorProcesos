
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

// =============================================
//  Gestor de Recursos del Sistema
//  Simula: RAM limitada (4096 MB)
// =============================================
class ResourceManager {
private:
    int totalMemory;  // RAM total del sistema (MB)
    int usedMemory;   // RAM actualmente ocupada (MB)
    int peakMemory;   // Pico maximo de RAM ocupada en la simulacion

public:
    ResourceManager();

    bool requestMemory(int amount);  // Pide RAM; retorna false si no hay
    void releaseMemory(int amount);  // Libera RAM al terminar proceso

    int getTotalMemory() const;
    int getUsedMemory()  const;
    int getFreeMemory()  const;
    int getPeakMemory()  const;

    void showResources() const;
};

#endif // RESOURCE_MANAGER_H