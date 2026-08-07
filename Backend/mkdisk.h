#ifndef MKDISK_H
#define MKDISK_H

#include <vector>

#include "structs.h"

// analiza el comando mkdisk -size=.. -fit=.. -unit=.. -path=..
void analizarMkdisk(std::vector<parametro> &params);

#endif