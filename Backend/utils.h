#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

#include "structs.h"

// convierte una cadena a minusculas
void toLower(std::string &texto);

// busca un disco por su ruta, devuelve null si no existe
disco* buscarDisco(std::vector<disco> &discos, const std::string &path);

// busca un montaje por su id (ej "341A"), devuelve null si no existe
montaje* buscarMontaje(std::vector<montaje> &montajes, const std::string &id);

// busca un usuario por su nombre, devuelve null si no existe
usuario* buscarUsuario(std::vector<usuario> &usuarios, const std::string &nombre);

#endif