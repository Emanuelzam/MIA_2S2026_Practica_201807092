#ifndef UTILS_H
#define UTILS_H

#include <initializer_list>
#include <string>
#include <vector>

#include "structs.h"

// convierte una cadena a minusculas
void toLower(std::string &texto);

//busca un disco por su ruta, devuelve null si no existe
disco* buscarDisco(std::vector<disco> &discos, const std::string &path);

//busca un montaje por su id, devuelve null si no existe
montaje* buscarMontaje(std::vector<montaje> &montajes, const std::string &id);

//busca un usuario por su nombre, devuelve null si no existe
usuario* buscarUsuario(std::vector<usuario> &usuarios, const std::string &nombre);

//cuantas veces aparece un parametro en el comando
int contarParametro(std::vector<parametro> &params, const std::string &nombre);

//devuelve el valor de un parametro (vacio si no existe)
std::string valorParametro(std::vector<parametro> &params, const std::string &nombre);

//true si hay un parametro que no se esperaba, deja su nombre en invalido
bool hayParametroInvalido(std::vector<parametro> &params,
                          std::initializer_list<std::string> permitidos,
                          std::string &invalido);

// true si algun parametro se repitio, deja su nombre en repetido
bool hayParametroRepetido(std::vector<parametro> &params, std::string &repetido);

//convierte un string a entero, false si no era un numero valido
bool stringAEntero(const std::string &texto, int &valor);

#endif