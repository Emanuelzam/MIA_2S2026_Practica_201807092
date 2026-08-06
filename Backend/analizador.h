#ifndef ANALIZADOR_H
#define ANALIZADOR_H

#include <string>
#include <vector>

#include "structs.h"

// separa la linea en el comando y sus parametros.
// si hay un problema de sintaxis deja el mensaje en `error`.
bool analizarLinea(const std::string &linea, std::string &comando,
                   std::vector<parametro> &params, std::string &error);

// es el que llaman desde main, ve de que comando se trata y manda
// a su analizador correspondiente
void ejecutarComando(const std::string &linea);

#endif