#ifndef MKFILE_H
#define MKFILE_H

#include <vector>

#include "structs.h"

// analiza el comando mkfile -path=.. -r -size=.. -cont=..
void analizarMkfile(std::vector<parametro> &params);

#endif