#ifndef MKFS_H
#define MKFS_H

#include <vector>

#include "structs.h"

// analiza el comando mkfs -id=.. -type=..
void analizarMkfs(std::vector<parametro> &params);

#endif