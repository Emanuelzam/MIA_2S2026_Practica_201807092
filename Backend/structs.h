#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>
#include <vector>

//se usan structs para guardar en memoria

// cada parametro que se manda ya separado.
struct parametro {
    std::string nombre;
    std::string valor;
};

// particion que "se crea" con fdisk y se guarda en el disco
struct particion {
    char tipo; //P primaria E extendida L logica
    char fit; //B best, F first, W worst
    int size; //tamanio en bytes
    std::string nombre; //nombre de la particion (unico en el disco)
    int padre; //solo para logicas: posicion en el vector del
    //disco donde esta la particion extendida que la contiene
};

// disco simulado (se crea con mkdisk)
struct disco {
    int size; // tamanio en bytes
    char fit; // fit por defecto del disco
    std::string path; // ruta del disco
    std::string letra; // letra para el id del mount
    std::vector<particion> partes; // particiones que le pertenecen
};

// particion que ya se monto con el comando mount
struct montaje {
    std::string id; // ej "341A"
    std::string path; // a que disco pertenece
    std::string nombre; // nombre de la particion
    int numParticion; // en que posicion del disco esta
    char tipo; // tipo de particion
};

//usuario que se va agregando con mkusr
struct usuario {
    std::string nombre;
    std::string pass;
    std::string grupo;
};

//aqui se junta el estado de todo el sistema en memoria
//se define coerto esta usando una variable global en analizador.cpp
struct estadoSistema {
    std::vector<disco> discos;
    std::vector<montaje> montajes;
    std::vector<usuario> usuarios;
};

// variable global, se define en analizador.cpp
extern estadoSistema estado;

#endif