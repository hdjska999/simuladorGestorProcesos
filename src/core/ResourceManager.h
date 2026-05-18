
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

class ResourceManager {
private:
  int totalMemory; // RAM total del sistema (MB)
  int usedMemory;  // RAM actualmente ocupada (MB)
  int peakMemory;  // El maximo de RAM ocupada en la simulacion

public:
  ResourceManager();

  bool requestMemory(int amount); // Pide RAM; retorna false si no hay
  void releaseMemory(int amount); // Libera RAM al terminar proceso

  int getTotalMemory() const;
  int getUsedMemory() const;
  int getFreeMemory() const;
  int getPeakMemory() const;

  void showResources() const;
};

#endif // RESOURCE_MANAGER_H