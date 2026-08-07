#include "rmdisk.h"

#include <iostream>

#include "utils.h"

// analiza el comando rmdisk, solo acepta el parametro path
void analizarRmdisk(std::vector<parametro> &params) {
    std::cout << "> analizando rmdisk" << std::endl;

    // sin parametros repetidos
    std::string repetido;
    if (hayParametroRepetido(params, repetido)) {
        std::cout << "[ERROR] el parametro -" << repetido << " se repitio" << std::endl;
        return;
    }

    // el unico parametro valido es path
    std::string invalido;
    if (hayParametroInvalido(params, {"path"}, invalido)) {
        std::cout << "[ERROR] el parametro -" << invalido << " no es valido para rmdisk" << std::endl;
        return;
    }

    if (contarParametro(params, "path") == 0) {
        std::cout << "[ERROR] falta el parametro obligatorio -path" << std::endl;
        return;
    }
    std::string path = valorParametro(params, "path");
    if (path.empty()) {
        std::cout << "[ERROR] -path no puede ir vacio" << std::endl;
        return;
    }

    // el disco debe existir en memoria para poder eliminarlo
    if (buscarDisco(estado.discos, path) == nullptr) {
        std::cout << "[ERROR] no existe un disco en la ruta: " << path << std::endl;
        return;
    }

    // quito el disco del vector de discos
    for (size_t i = 0; i < estado.discos.size(); ++i) {
        if (estado.discos[i].path == path) {
            estado.discos.erase(estado.discos.begin() + i);
            break;
        }
    }

    std::cout << "[OK] disco eliminado: " << path << std::endl;
}