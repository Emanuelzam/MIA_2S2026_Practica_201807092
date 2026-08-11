#include "mkdisk.h"

#include <iostream>

#include "utils.h"

// analiza el comando mkdisk
// valida cada parametro y si todo esta bien guarda el disco en memoria
void analizarMkdisk(std::vector<parametro> &params) {
    std::cout << "> analizando mkdisk" << std::endl;

    // ningun parametro puede repetirse
    std::string repetido;
    if (hayParametroRepetido(params, repetido)) {
        std::cout << "[ERROR] el parametro -" << repetido << " se repitio" << std::endl;
        return;
    }

    // los unicos parametros validos son size, fit, unit y path
    std::string invalido;
    if (hayParametroInvalido(params, {"size", "fit", "unit", "path"}, invalido)) {
        std::cout << "[ERROR] el parametro -" << invalido << " no es valido para mkdisk" << std::endl;
        return;
    }

    // -size es obligatorio y debe ser un entero positivo
    if (contarParametro(params, "size") == 0) {
        std::cout << "[ERROR] falta el parametro obligatorio -size" << std::endl;
        return;
    }
    int size = 0;
    if (!stringAEntero(valorParametro(params, "size"), size) || size <= 0) {
        std::cout << "[ERROR] -size debe ser un numero entero mayor que 0" << std::endl;
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

    // el disco no debe haberse creado antes en la misma ruta
    if (buscarDisco(estado.discos, path) != nullptr) {
        std::cout << "[ERROR] ya existe un disco en esa ruta: " << path << std::endl;
        return;
    }

    // -fit es opcional, si no viene se usa ff
    std::string fit = valorParametro(params, "fit");
    toLower(fit);
    if (fit.empty()) {
        fit = "ff";
    }
    if (fit != "ff" && fit != "bf" && fit != "wf") {
        std::cout << "[ERROR] -fit solo acepta BF, FF o WF" << std::endl;
        return;
    }

    // -unit es opcional, si no viene se usa m
    std::string unit = valorParametro(params, "unit");
    toLower(unit);
    if (unit.empty()) {
        unit = "m";
    }
    if (unit != "k" && unit != "m") {
        std::cout << "[ERROR] -unit solo acepta K o M" << std::endl;
        return;
    }

    //convierte el tamano a bytes segun la unidad
    //se usa long long porque 3000 m son mas de 3000 millones
    long long sizeBytes = size;
    if (unit == "k") {
        sizeBytes = size * 1024LL;
    } else {
        sizeBytes = size * 1024LL * 1024LL;
    }

    // guardo el disco en el estado global
    disco nuevo;
    nuevo.size = sizeBytes;
    nuevo.fit = fit[0];
    nuevo.path = path;

    estado.discos.push_back(nuevo);

    std::cout << "[OK] disco creado en memoria: " << path
              << " (" << sizeBytes << " bytes)" << std::endl;
}