#include "mkusr.h"

#include <iostream>

#include "utils.h"

// analiza el comando mkusr, crea un usuario en la particion
void analizarMkusr(std::vector<parametro> &params) {
    std::cout << "> analizando mkusr" << std::endl;

    // sin parametros repetidos
    std::string repetido;
    if (hayParametroRepetido(params, repetido)) {
        std::cout << "[ERROR] el parametro -" << repetido << " se repitio" << std::endl;
        return;
    }

    // los unicos parametros validos son user, pass y grp
    std::string invalido;
    if (hayParametroInvalido(params, {"user", "pass", "grp"}, invalido)) {
        std::cout << "[ERROR] el parametro -" << invalido << " no es valido para mkusr" << std::endl;
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
    if (user.size() > 10) {
        std::cout << "[ERROR] -user no puede tener mas de 10 caracteres" << std::endl;
        return;
    }

    // -pass es obligatorio
    if (contarParametro(params, "pass") == 0) {
        std::cout << "[ERROR] falta el parametro obligatorio -pass" << std::endl;
        return;
    }
    std::string pass = valorParametro(params, "pass");
    if (pass.empty()) {
        std::cout << "[ERROR] -pass no puede ir vacio" << std::endl;
        return;
    }
    if (pass.size() > 10) {
        std::cout << "[ERROR] -pass no puede tener mas de 10 caracteres" << std::endl;
        return;
    }

    // -grp es obligatorio
    if (contarParametro(params, "grp") == 0) {
        std::cout << "[ERROR] falta el parametro obligatorio -grp" << std::endl;
        return;
    }
    std::string grp = valorParametro(params, "grp");
    if (grp.empty()) {
        std::cout << "[ERROR] -grp no puede ir vacio" << std::endl;
        return;
    }
    if (grp.size() > 10) {
        std::cout << "[ERROR] -grp no puede tener mas de 10 caracteres" << std::endl;
        return;
    }

    // el usuario no debe existir ya
    if (buscarUsuario(estado.usuarios, user) != nullptr) {
        std::cout << "[ERROR] el usuario " << user << " ya existe" << std::endl;
        return;
    }

    // guardo el usuario en memoria
    usuario nuevo;
    nuevo.nombre = user;
    nuevo.pass = pass;
    nuevo.grupo = grp;

    estado.usuarios.push_back(nuevo);

    std::cout << "[OK] usuario " << user << " creado en el grupo " << grp << std::endl;
}