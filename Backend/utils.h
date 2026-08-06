#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

#include "structs.h"

// convierte una cadena a minusculas
void toLower(std::string &texto);

// busca una particion montada por su id
montada* buscarMontada(std::vector<disco> &discos,
                       const std::string &id);

// devuelve el disco donde esta montada una particion
disco* buscarDisco(std::vector<disco> &discos,
                   const std::string &id);

#endif