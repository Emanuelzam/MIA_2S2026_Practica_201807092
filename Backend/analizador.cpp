#include "analizador.h"

#include <iostream>

#include "fdisk.h"
#include "mkdisk.h"
#include "mkfile.h"
#include "mkfs.h"
#include "mkusr.h"
#include "mount.h"
#include "rmdisk.h"
#include "rmusr.h"
#include "utils.h"

//aqui se guarda todo lo que se va creando durante la sesion
estadoSistema estado;

//carnet de aqui se sacan los ultimos 2 digitos
//para armar el id del comando mount
std::string CARNET = "201807092";

//Analisis lexico

//separa la linea por espacios pero
//tambien quita los comentarios que empiecen con #
//al final marca si quedaron comillas sin cerrar
static void tokenizar(const std::string &linea, std::vector<std::string> &tokens,
                      bool &comillas_sin_cerrar) {
    std::string token;
    bool adentro_comillas = false;

    for (size_t i = 0; i < linea.size(); ++i) {
        char c = linea[i];

        // comentario, se ignora todo lo que sigue
        if (c == '#' && !adentro_comillas) {
            break;
        }

        //las comillas no se guardan, solo sirven para poder usar
        //espacios dentro de un parametro
        if (c == '"') {
            adentro_comillas = !adentro_comillas;
            continue;
        }

        if (c == ' ' && !adentro_comillas) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else {
            token += c;
        }
    }

    if (!token.empty()) {
        tokens.push_back(token);
    }

    comillas_sin_cerrar = adentro_comillas;
}


//Analisis sintactico

// revisa que la linea tenga la forma "comando -param=valor ..."
bool analizarLinea(const std::string &linea, std::string &comando,
                   std::vector<parametro> &params, std::string &error) {
    std::vector<std::string> tokens;
    bool comillas_sin_cerrar = false;
    tokenizar(linea, tokens, comillas_sin_cerrar);

    // las comillas que no se cierran son un error lexico
    if (comillas_sin_cerrar) {
        error = "comillas sin cerrar";
        return false;
    }

    if (tokens.empty()) {
        error = "linea vacia o solo un comentario";
        return false;
    }

    // el primer token siempre es el comando
    comando = tokens[0];
    toLower(comando);

    // un comando no puede empezar con guion
    if (comando[0] == '-') {
        error = "falta el nombre del comando, sobra el guion al inicio";
        return false;
    }

    // de aqui en adelante cada token debe ser un parametro
    for (size_t i = 1; i < tokens.size(); ++i) {
        std::string &tok = tokens[i];

        // todo parametro tiene que empezar con '-'
        if (tok.empty() || tok[0] != '-') {
            error = "parametro invalido: '" + tok + "' (debe llevar - adelante)";
            return false;
        }

        std::string cuerpo = tok.substr(1); // le quito el guion

        // hay dos tipos de parametro:
        //   con valor:  -size=3000
        //   solo flag:  -r   (lo usa mkfile)
        parametro p;
        size_t posIgual = cuerpo.find('=');
        if (posIgual == std::string::npos) {
            p.nombre = cuerpo;   // es un flag sin valor
        } else {
            p.nombre = cuerpo.substr(0, posIgual);
            p.valor = cuerpo.substr(posIgual + 1);
        }

        if (p.nombre.empty()) {
            error = "parametro sin nombre: '" + tok + "'";
            return false;
        }

        // el nombre del parametro no distingue mayusculas
        toLower(p.nombre);
        params.push_back(p);
    }

    return true;
}

//Comandos


void ejecutarComando(const std::string &linea) {
    // las lineas vacias o que solo son comentarios se ignoran,
    // no cuentan como un error
    size_t ini = linea.find_first_not_of(" \t");
    if (ini == std::string::npos || linea[ini] == '#') {
        return;
    }

    std::string comando;
    std::vector<parametro> params;
    std::string error;

    // primero el analisis lexico y sintactico general
    if (!analizarLinea(linea, comando, params, error)) {
        std::cout << "[ERROR] " << error << std::endl;
        return;
    }

    // dependiendo del comando se manda a su analizador especifico
    if (comando == "mkdisk") {
        analizarMkdisk(params);
    } else if (comando == "rmdisk") {
        analizarRmdisk(params);
    } else if (comando == "fdisk") {
        analizarFdisk(params);
    } else if (comando == "mount") {
        analizarMount(params);
    } else if (comando == "mkfs") {
        analizarMkfs(params);
    } else if (comando == "mkusr") {
        analizarMkusr(params);
    } else if (comando == "rmusr") {
        analizarRmusr(params);
    } else if (comando == "mkfile") {
        analizarMkfile(params);
    } else {
        std::cout << "[ERROR] el comando '" << comando
                  << "' no existe o no se reconoce" << std::endl;
    }
}