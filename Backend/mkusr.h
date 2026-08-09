#ifndef MKUSR_H
#define MKUSR_H

#include <vector>

#include "structs.h"

// analiza el comando mkusr -user=.. -pass=.. -grp=..
void analizarMkusr(std::vector<parametro> &params);

#endif