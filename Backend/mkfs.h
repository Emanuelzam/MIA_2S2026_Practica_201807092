#ifndef MKFS_H
#define MKFS_H

#include <vector>
#include <string>

#include "structs.h"

void mkfs(std::vector<std::string> &parametros,
          std::vector<disco> &discos);

#endif