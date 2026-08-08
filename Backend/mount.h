#ifndef MOUNT_H
#define MOUNT_H

#include <vector>

#include "structs.h"

// analiza el comando mount -path=.. -name=..
void analizarMount(std::vector<parametro> &params);

#endif