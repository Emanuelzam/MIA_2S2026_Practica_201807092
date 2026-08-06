#include "utils.h"

#include <algorithm>
#include <cctype>

// pasa un string a minusculas
void toLower(std::string &texto) {
    std::transform(texto.begin(), texto.end(), texto.begin(),
                   [](unsigned char c) {
                       return std::tolower(c);
                   });
}

// buscar un disco comparando por su ruta
disco* buscarDisco(std::vector<disco> &discos, const std::string &path) {
    for (auto &d : discos) {
        if (d.path == path) {
            return &d;
        }
    }
    return nullptr;
}

// buscar un montaje por su id
montaje* buscarMontaje(std::vector<montaje> &montajes, const std::string &id) {
    for (auto &m : montajes) {
        if (m.id == id) {
            return &m;
        }
    }
    return nullptr;
}

// buscar un usuario por su nombre
usuario* buscarUsuario(std::vector<usuario> &usuarios, const std::string &nombre) {
    for (auto &u : usuarios) {
        if (u.nombre == nombre) {
            return &u;
        }
    }
    return nullptr;
}