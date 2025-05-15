#ifndef SAVE_H_
#define SAVE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "appmenu.h"
#include "callbacks.h"

menu_screen_t *load_app_from_file(const char *save_file);

#endif //SAVE_H_
