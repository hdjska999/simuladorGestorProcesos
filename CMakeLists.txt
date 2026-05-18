cmake_minimum_required(VERSION 3.29)
project(simuladorGestorProcesos)

set(CMAKE_CXX_STANDARD 17)

add_executable(simuladorSO 
    src/main.cpp 
    src/core/Process.cpp 
    src/core/Scheduler.cpp 
    src/core/ResourceManager.cpp 
    src/ui/Menu.cpp 
    src/ipc/MessageQueue.cpp
)
