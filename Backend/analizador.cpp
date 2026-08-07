#include "analizador.h"

#include <iostream>

#include "mkdisk.h"
#include "rmdisk.h"
#include "utils.h"

// aqui se guarda todo lo que se va "creando" durante la sesion
estadoSistema estado;

//Analisis lexico

//separa la linea por espacios pero
//tambien quita los comentarios que empiecen con #
static void tokenizar(const std::string &linea, std::vector<std::string> &tokens) {
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
}


//Analisis sintactico

// revisa que la linea tenga la forma "comando -param=valor ..."
bool analizarLinea(const std::string &linea, std::string &comando,
                   std::vector<parametro> &params, std::string &error) {
    std::vector<std::string> tokens;
    tokenizar(linea, tokens);

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


static void mostrarDetectado(const std::string &comando, std::vector<parametro> &params) {
    std::cout << ">> comando detectado: " << comando << std::endl;
    if (params.empty()) {
        std::cout << ">> sin parametros" << std::endl;
        return;
    }
    for (auto &p : params) {
        if (p.valor.empty()) {
            std::cout << "   - " << p.nombre << " (flag)" << std::endl;
        } else {
            std::cout << "   - " << p.nombre << " = " << p.valor << std::endl;
        }
    }
    std::cout << ">> [PENDIENTE] el analizador de este comando se agrega luego" << std::endl;
}

void ejecutarComando(const std::string &linea) {
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
    } else if (comando == "fdisk" || comando == "mount" ||
               comando == "mkfs"  || comando == "mkusr" ||
               comando == "rmusr" || comando == "mkfile") {
        // los analizadores que faltan se van agregando de a uno
        mostrarDetectado(comando, params);
    } else {
        std::cout << "[ERROR] el comando '" << comando
                  << "' no existe o no se reconoce" << std::endl;
    }
}