#include "mount.h"

#include <iostream>

#include "analizador.h"
#include "utils.h"

//Analiza el comando mount, monta una particion y le asigna un id
//el id queda como ultimos 2 digitos del carnet + numero de montaje
//en el disco + letra del disco (ej: 921A)
void analizarMount(std::vector<parametro> &params) {
    std::cout << "> analizando mount" << std::endl;

    // sin parametros repetidos
    std::string repetido;
    if (hayParametroRepetido(params, repetido)) {
        std::cout << "[ERROR] el parametro -" << repetido << " se repitio" << std::endl;
        return;
    }

    // los unicos parametros validos son path y name
    std::string invalido;
    if (hayParametroInvalido(params, {"path", "name"}, invalido)) {
        std::cout << "[ERROR] el parametro -" << invalido << " no es valido para mount" << std::endl;
        return;
    }

    // -path es obligatorio
    if (contarParametro(params, "path") == 0) {
        std::cout << "[ERROR] falta el parametro obligatorio -path" << std::endl;
        return;
    }
    std::string path = valorParametro(params, "path");
    if (path.empty()) {
        std::cout << "[ERROR] -path no puede ir vacio" << std::endl;
        return;
    }

    // -name es obligatorio
    if (contarParametro(params, "name") == 0) {
        std::cout << "[ERROR] falta el parametro obligatorio -name" << std::endl;
        return;
    }
    std::string nombre = valorParametro(params, "name");
    if (nombre.empty()) {
        std::cout << "[ERROR] -name no puede ir vacio" << std::endl;
        return;
    }

    // el disco debe existir
    disco *d = buscarDisco(estado.discos, path);
    if (d == nullptr) {
        std::cout << "[ERROR] no existe un disco en la ruta: " << path << std::endl;
        return;
    }

    // la particion debe existir dentro de ese disco
    int idxPart = -1;
    for (size_t i = 0; i < d->partes.size(); ++i) {
        if (d->partes[i].nombre == nombre) {
            idxPart = static_cast<int>(i);
            break;
        }
    }
    if (idxPart == -1) {
        std::cout << "[ERROR] la particion " << nombre << " no existe en el disco" << std::endl;
        return;
    }

    // no se puede montar dos veces la misma particion
    for (auto &m : estado.montajes) {
        if (m.path == path && m.nombre == nombre) {
            std::cout << "[ERROR] la particion " << nombre << " ya estaba montada" << std::endl;
            return;
        }
    }

    // al disco se le asigna una letra en el orden en que
    // se va montando por primera vez
    if (d->letra.empty()) {
        int conLetra = 0;
        for (auto &dis : estado.discos) {
            if (!dis.letra.empty()) {
                ++conLetra;
            }
        }
        d->letra = std::string(1, static_cast<char>('A' + conLetra));
    }

    //contando cuantas particiones de ese disco ya estan montadas
    int numMontadas = 0;
    for (auto &m : estado.montajes) {
        if (m.path == path) {
            ++numMontadas;
        }
    }
    ++numMontadas;

    //armando id con los ultimos 2 digitos del carnet
    std::string base = CARNET.substr(CARNET.size() - 2);
    std::string id = base + std::to_string(numMontadas) + d->letra;

    //guardando el montaje en el estado
    montaje nuevo;
    nuevo.id = id;
    nuevo.path = path;
    nuevo.nombre = nombre;
    nuevo.numParticion = idxPart;
    nuevo.tipo = d->partes[idxPart].tipo;

    estado.montajes.push_back(nuevo);

    std::cout << "[OK] particion " << nombre << " montada con id " << id << std::endl;
}