#include "fdisk.h"

#include <iostream>

#include "utils.h"

//cuenta las particiones primarias y extendidas que ya tiene el disco
static int contarPrimariasExtendidas(disco &d) {
    int contador = 0;
    for (auto &p : d.partes) {
        if (p.tipo != 'l') {
            ++contador;
        }
    }
    return contador;
}

//suma el espacio que ocupan primarias y extendidas
static int sumarPrimariasExtendidas(disco &d) {
    int total = 0;
    for (auto &p : d.partes) {
        if (p.tipo != 'l') {
            total += p.size;
        }
    }
    return total;
}

// devuelve la posicion de la particion extendida, -1 si no existe
static int indiceExtendida(disco &d) {
    for (size_t i = 0; i < d.partes.size(); ++i) {
        if (d.partes[i].tipo == 'e') {
            return static_cast<int>(i);
        }
    }
    return -1;
}

//suma el espacio que ya ocupan las logicas de una extendida
static int sumarLogicas(disco &d, int idxExtendida) {
    int total = 0;
    for (auto &p : d.partes) {
        if (p.padre == idxExtendida) {
            total += p.size;
        }
    }
    return total;
}

//analiza el comando fdisk
void analizarFdisk(std::vector<parametro> &params) {
    std::cout << "> analizando fdisk" << std::endl;

    // ningun parametro puede repetirse
    std::string repetido;
    if (hayParametroRepetido(params, repetido)) {
        std::cout << "[ERROR] el parametro -" << repetido << " se repitio" << std::endl;
        return;
    }

    //los unicos parametros validos
    std::string invalido;
    if (hayParametroInvalido(params, {"size", "unit", "path", "type", "fit", "name"}, invalido)) {
        std::cout << "[ERROR] el parametro -" << invalido << " no es valido para fdisk" << std::endl;
        return;
    }

    //size es obligatorio y mayor que 0
    if (contarParametro(params, "size") == 0) {
        std::cout << "[ERROR] falta el parametro obligatorio -size" << std::endl;
        return;
    }
    int size = 0;
    if (!stringAEntero(valorParametro(params, "size"), size) || size <= 0) {
        std::cout << "Error -size debe ser un numero entero mayor que 0" << std::endl;
        return;
    }

    // -path es obligatorio
    if (contarParametro(params, "path") == 0) {
        std::cout << "Error falta el parametro obligatorio -path" << std::endl;
        return;
    }
    std::string path = valorParametro(params, "path");
    if (path.empty()) {
        std::cout << "Error -path no puede ir vacio" << std::endl;
        return;
    }

    // -name es obligatorio
    if (contarParametro(params, "name") == 0) {
        std::cout << "[ERROR] falta el parametro obligatorio -name" << std::endl;
        return;
    }
    std::string nombre = valorParametro(params, "name");
    if (nombre.empty()) {
        std::cout << "Error -name no puede ir vacio" << std::endl;
        return;
    }

    // el disco tiene que existir en memoria
    disco *d = buscarDisco(estado.discos, path);
    if (d == nullptr) {
        std::cout << "Error no existe un disco en la ruta: " << path
                  << "use mkdisk primero" << std::endl;
        return;
    }

    //-type es opcional, por defecto primaria
    std::string tipo = valorParametro(params, "type");
    toLower(tipo);
    if (tipo.empty()) {
        tipo = "p";
    }
    if (tipo != "p" && tipo != "e" && tipo != "l") {
        std::cout << "Error -type solo acepta P, E o L" << std::endl;
        return;
    }

    //-unit es opcional, por defecto k
    std::string unit = valorParametro(params, "unit");
    toLower(unit);
    if (unit.empty()) {
        unit = "k";
    }
    if (unit != "b" && unit != "k" && unit != "m") {
        std::cout << "Error -unit solo acepta B, K o M" << std::endl;
        return;
    }

    //-fit es opcional, por defecto ff
    std::string fit = valorParametro(params, "fit");
    toLower(fit);
    if (fit.empty()) {
        fit = "ff";
    }
    if (fit != "bf" && fit != "ff" && fit != "wf") {
        std::cout << "[ERROR] -fit solo acepta BF, FF o WF" << std::endl;
        return;
    }

    //pasa el tamano a bytes
    int sizeBytes = size;
    if (unit == "k") {
        sizeBytes = size * 1024;
    } else if (unit == "m") {
        sizeBytes = size * 1024 * 1024;
    }

    //el nombre no puede estar ya usado en ese disco
    for (auto &p : d->partes) {
        if (p.nombre == nombre) {
            std::cout << "[ERROR] ya existe una particion llamada " << nombre
                      << " en ese disco" << std::endl;
            return;
        }
    }

    // alidaciones segun el tipo de particion
    if (tipo == "p") {
        // a lo mucho 4 entre primarias y extendidas
        if (contarPrimariasExtendidas(*d) >= 4) {
            std::cout << "Error ya no caben mas particiones primarias en el disco" << std::endl;
            return;
        }
        //revisar espacio libre
        if (sizeBytes > d->size - sumarPrimariasExtendidas(*d)) {
            std::cout << "Error no hay espacio suficiente en el disco" << std::endl;
            return;
        }
    } else if (tipo == "e") {
        if (contarPrimariasExtendidas(*d) >= 4) {
            std::cout << "Error ya no caben mas particiones en el disco" << std::endl;
            return;
        }
        // solo puede haber una extendida
        if (indiceExtendida(*d) != -1) {
            std::cout << "[ERROR] ya existe una particion extendida en el disco" << std::endl;
            return;
        }
        if (sizeBytes > d->size - sumarPrimariasExtendidas(*d)) {
            std::cout << "[ERROR] no hay espacio suficiente en el disco" << std::endl;
            return;
        }
    } else if (tipo == "l") {
        // para crear logicas hace falta una extendida primero
        int idxExt = indiceExtendida(*d);
        if (idxExt == -1) {
            std::cout << "Error no hay una particion extendida, crea una primero" << std::endl;
            return;
        }
        // las logicas van dentro del espacio de la extendida
        if (sizeBytes > d->partes[idxExt].size - sumarLogicas(*d, idxExt)) {
            std::cout << "Error no hay espacio en la particion extendida" << std::endl;
            return;
        }
    }

    //todo validado, guarda la particion en el disco
    particion nueva;
    nueva.tipo = tipo[0];
    nueva.fit = fit[0];
    nueva.size = sizeBytes;
    nueva.nombre = nombre;
    nueva.padre = (tipo == "l") ? indiceExtendida(*d) : -1;

    d->partes.push_back(nueva);

    std::cout << "[OK] particion " << nombre << " creada en " << path
              << " (" << sizeBytes << " bytes)" << std::endl;
}