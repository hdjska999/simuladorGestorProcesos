# Simulador de Gestor de Procesos

**Breve descripción:** Simulador interactivo en C++ que modela la administración de procesos y recursos (RAM) de un Sistema Operativo mediante planificación por ticks y visualización en tiempo real.

---

### 📚 Información del Curso
- **Materia:** Sistemas Operativos
- **Institución:** [Nombre de la Universidad o Institución]
- **Semestre:** Semestre 2026-1
- **Profesor:** [Nombre del Profesor]

### 👥 Integrantes del Equipo
- [Nombre del integrante 1]
- [Nombre del integrante 2]

---

### 🚀 Características Implementadas

1. **Algoritmos de Planificación:**
   - **FCFS** (First-Come, First-Served).
   - **SJF** (Shortest Job First).
   - **Round Robin** (con Quantum configurable).
   - **Prioridad** (Ejecución basada en niveles de prioridad).

2. **Gestión de Recursos (RAM):**
   - Sistema con memoria total limitada (4096 MB).
   - Asignación y liberación dinámica de RAM por cada proceso.
   - Registro estadístico del "Max. RAM Usada" (pico histórico) para análisis de rendimiento.

3. **Interfaz Dinámica en Consola:**
   - Generación aleatoria de procesos (Tiempos de ráfaga, Memoria, Prioridad y Tiempo de llegada).
   - Tabla visual en tiempo real que muestra el avance de cada proceso (RUNNING, READY, WAITING, TERMINATED, DENIED) tick a tick.
   - Log detallado de eventos ocurridos en el sistema.
   - Tabla final de estadísticas (tiempos de espera promedio, tiempos de retorno y motivos de salida).

4. **IPC (Comunicación entre procesos):**
   - Sistema base de paso de mensajes usando buzones (`MessageQueue`).

### 🛠️ Tecnologías y Requisitos
- **Lenguaje:** C++17
- **Herramientas de Construcción:** CMake (versión mínima 3.29)
- **Entorno recomendado:** CLion, Visual Studio o terminal (g++ / MinGW en Windows).

### ⚙️ Cómo compilar y ejecutar
1. Clona el repositorio:
   ```bash
   git clone https://github.com/hdjska999/simuladorGestorProcesos.git
   ```
2. Carga el proyecto en tu IDE (como CLion) para que lea el `CMakeLists.txt`.
3. Ejecuta el target `simuladorSO`.