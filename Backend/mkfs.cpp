#include "mkfs.h"

#include <iostream>

#include "utils.h"

// analiza el comando mkfs, formatea la particion a ext2
void analizarMkfs(std::vector<parametro> &params) {
    std::cout << "> analizando mkfs" << std::endl;

    // sin parametros repetidos
    std::string repetido;
    if (hayParametroRepetido(params, repetido)) {
        std::cout << "[ERROR] el parametro -" << repetido << " se repitio" << std::endl;
        return;
    }

    // los unicos parametros validos son id y type
    std::string invalido;
    if (hayParametroInvalido(params, {"id", "type"}, invalido)) {
        std::cout << "[ERROR] el parametro -" << invalido << " no es valido para mkfs" << std::endl;
        return;
    }

    // -id es obligatorio
    if (contarParametro(params, "id") == 0) {
        std::cout << "[ERROR] falta el parametro obligatorio -id" << std::endl;
        return;
    }
    std::string id = valorParametro(params, "id");
    if (id.empty()) {
        std::cout << "[ERROR] -id no puede ir vacio" << std::endl;
        return;
    }

    // -type es opcional el unico valor permitido es full
    std::string tipo = valorParametro(params, "type");
    toLower(tipo);
    if (tipo.empty()) {
        tipo = "full"; // valor por defecto
    }
    if (tipo != "full") {
        std::cout << "[ERROR] -type solo acepta full" << std::endl;
        return;
    }

    // si el id no esta montado igual se simula el formateo
    montaje *m = buscarMontaje(estado.montajes, id);
    if (m == nullptr) {
        std::cout << "[OK] el id " << id << " no estaba montado, se simula el formateo" << std::endl;
        return;
    }

    //guarda el formateo de la particion montada
    m->formateada = true;

    std::cout << "[OK] particion " << m->nombre << " formateada a ext2" << std::endl;
}