#ifndef PAMTOUCH_H
#define PAMTOUCH_H

#include <stdbool.h>

void touch_start(int);
void touch_stop();
bool touch_finished();
char *touch_pin();

#endif
