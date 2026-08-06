#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>

#include "analizador.h"


const std::string CARNET = "201807092";

int main() {
    std::cout << "******************************************" << std::endl;
    std::cout << " ANALIZADOR DE COMANDOS EXT2 (Practica 1) " << std::endl;
    std::cout << " escribe un comando, 'exit' para salir    " << std::endl;
    std::cout << "******************************************" << std::endl;

    bool seguir = true;
    std::string linea;

    // loop principal, se sale cuando se escribe exit
    while (seguir) {
        std::cout << std::endl << "> ";

        if (!std::getline(std::cin, linea)) {
            break;
        }

        // el exit se revisa antes que nada, sin importar mayusculas
        std::string aux = linea;
        std::transform(aux.begin(), aux.end(), aux.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        if (aux == "exit") {
            seguir = false;
            continue;
        }

        ejecutarComando(linea);
    }

    std::cout << "== fin del analizador ==" << std::endl;
    return 0;
}