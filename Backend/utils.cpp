#include "utils.h"

#include <algorithm>
#include <cctype>

// pasar una cadena a minusculas
void toLower(std::string &texto) {

    std::transform(texto.begin(),
                   texto.end(),
                   texto.begin(),
                   [](unsigned char c) {
                       return std::tolower(c);
                   });
}


// buscar una particion montada
montada* buscarMontada(std::vector<disco> &discos,
                       const std::string &id) {

    for (auto &d : discos) {

        for (auto &p : d.particiones) {

            if (p.id == id) {
                return &p;
            }

        }

    }

    return nullptr;
}


// devolver el disco al que pertenece una particion
disco* buscarDisco(std::vector<disco> &discos,
                   const std::string &id) {

    for (auto &d : discos) {

        for (auto &p : d.particiones) {

            if (p.id == id) {
                return &d;
            }

        }

    }

    return nullptr;
}