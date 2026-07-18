cmake_minimum_required(VERSION 4.2)
project(grafos C)

set(CMAKE_C_STANDARD 99)

# Inclui a pasta de cabeçalhos (.h)
include_directories(include)

# Adicione todos os arquivos .c dentro de src aqui
add_executable(grafos
        src/main.c
)
