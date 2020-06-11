cmake_minimum_required(VERSION 3.15) # Проверка версии CMake.

# Переменные
set(CMAKE_CXX_STANDARD 17)

# Расположение файлов библиотеки
FILE(GLOB_RECURSE INCLUDES ${GAME_SOURCE}/*.h)
FILE(GLOB_RECURSE SOURCES ${GAME_SOURCE}/*.cpp)

#message("GAME INCLUDES: " ${INCLUDES})
#message("GAME SOURCES: " ${SOURCES})

# Расположение заголовочных файлов
include_directories(${ENGINE_SOURCE})

# Создание статической библиотеки
add_library(${GAME_NAME} STATIC ${INCLUDES} ${SOURCES})
