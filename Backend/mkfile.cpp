#include "mkfile.h"

#include <iostream>

#include "utils.h"

// analiza el comando mkfile, crea un archivo (en memoria no se escribe nada)
void analizarMkfile(std::vector<parametro> &params) {
    std::cout << "> analizando mkfile" << std::endl;

    // sin parametros repetidos
    std::string repetido;
    if (hayParametroRepetido(params, repetido)) {
        std::cout << "[ERROR] el parametro -" << repetido << " se repitio" << std::endl;
        return;
    }

    // los unicos parametros validos son path, r, size y cont
    std::string invalido;
    if (hayParametroInvalido(params, {"path", "r", "size", "cont"}, invalido)) {
        std::cout << "[ERROR] el parametro -" << invalido << " no es valido para mkfile" << std::endl;
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

    // -r es un flag, no puede llevar un valor
    if (contarParametro(params, "r") > 0 && !valorParametro(params, "r").empty()) {
        std::cout << "[ERROR] -r es un flag y no lleva valor" << std::endl;
        return;
    }

    // -size es opcional, entero no negativo (0 esta permitido)
    if (contarParametro(params, "size") > 0) {
        int size = 0;
        if (!stringAEntero(valorParametro(params, "size"), size)) {
            std::cout << "[ERROR] -size debe ser un numero entero" << std::endl;
            return;
        }
    }

    // -cont es opcional, solo debe venir una ruta de donde copiar
    if (contarParametro(params, "cont") > 0 && valorParametro(params, "cont").empty()) {
        std::cout << "[ERROR] -cont no puede ir vacio" << std::endl;
        return;
    }

    std::cout << "[OK] archivo creado: " << path << std::endl;
}