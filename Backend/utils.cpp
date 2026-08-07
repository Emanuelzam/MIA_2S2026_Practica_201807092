#include "utils.h"

#include <algorithm>
#include <cctype>

//pasa un string a minusculas
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

//cuenta cuantas veces se manda un mismo parametro
int contarParametro(std::vector<parametro> &params, const std::string &nombre) {
    int contador = 0;
    for (auto &p : params) {
        if (p.nombre == nombre) {
            ++contador;
        }
    }
    return contador;
}

//obtener el valor de un parametro, si no esta devuelve vacio
std::string valorParametro(std::vector<parametro> &params, const std::string &nombre) {
    for (auto &p : params) {
        if (p.nombre == nombre) {
            return p.valor;
        }
    }
    return "";
}

//revisa que todos los parametros esten en la lista permitida
bool hayParametroInvalido(std::vector<parametro> &params,
                          std::initializer_list<std::string> permitidos,
                          std::string &invalido) {
    for (auto &p : params) {
        bool permitido = false;
        for (auto &nombre : permitidos) {
            if (p.nombre == nombre) {
                permitido = true;
                break;
            }
        }
        if (!permitido) {
            invalido = p.nombre;
            return true;
        }
    }
    return false;
}

//revisa si un parametro se escribio mas de una vez
bool hayParametroRepetido(std::vector<parametro> &params, std::string &repetido) {
    for (auto &p : params) {
        if (contarParametro(params, p.nombre) > 1) {
            repetido = p.nombre;
            return true;
        }
    }
    return false;
}

// convierte texto a entero, solo acepta numeros
bool stringAEntero(const std::string &texto, int &valor) {
    if (texto.empty()) {
        return false;
    }
    for (char c : texto) {
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    try {
        valor = std::stoi(texto);
    } catch (...) {
        return false;
    }
    return true;
}