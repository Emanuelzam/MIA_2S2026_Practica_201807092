#include "rmusr.h"

#include <iostream>

#include "utils.h"

// analiza el comando rmusr, elimina un usuario de la particion
void analizarRmusr(std::vector<parametro> &params) {
    std::cout << "> analizando rmusr" << std::endl;

    // sin parametros repetidos
    std::string repetido;
    if (hayParametroRepetido(params, repetido)) {
        std::cout << "[ERROR] el parametro -" << repetido << " se repitio" << std::endl;
        return;
    }

    // el unico parametro valido es user
    std::string invalido;
    if (hayParametroInvalido(params, {"user"}, invalido)) {
        std::cout << "[ERROR] el parametro -" << invalido << " no es valido para rmusr" << std::endl;
        return;
    }

    // -user es obligatorio
    if (contarParametro(params, "user") == 0) {
        std::cout << "[ERROR] falta el parametro obligatorio -user" << std::endl;
        return;
    }
    std::string user = valorParametro(params, "user");
    if (user.empty()) {
        std::cout << "[ERROR] -user no puede ir vacio" << std::endl;
        return;
    }

    // el usuario debe existir para poder borrarlo
    if (buscarUsuario(estado.usuarios, user) == nullptr) {
        std::cout << "[ERROR] el usuario " << user << " no existe" << std::endl;
        return;
    }

    // quito el usuario del vector
    for (size_t i = 0; i < estado.usuarios.size(); ++i) {
        if (estado.usuarios[i].nombre == user) {
            estado.usuarios.erase(estado.usuarios.begin() + i);
            break;
        }
    }

    std::cout << "[OK] usuario " << user << " eliminado" << std::endl;
}