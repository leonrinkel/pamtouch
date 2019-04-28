#include "touch.h"

#include <stdlib.h>

#include <CoreFoundation/CoreFoundation.h>

typedef struct {
    float x, y;
} point_t;

typedef struct {
    uint8_t _dontcare1[20];
    uint8_t state;
    uint8_t _dontcare2[8];
    point_t position;
    uint8_t _dontcare3[56];
} MTData;

typedef void *MTDeviceRef;
typedef int (*MTContactCallbackFunction)(
    int, MTData*, int, double, int);

MTDeviceRef MTDeviceCreateDefault();
void MTRegisterContactFrameCallback(
    MTDeviceRef, MTContactCallbackFunction);
void MTDeviceStart(MTDeviceRef, int);
void MTDeviceStop(MTDeviceRef);

static int pin_index = 0;
static int pin_len = 0;
static char *pin;

static MTDeviceRef dev;

char touch_map(
    float x,
    float y
)
{

    // Top Row
    // Left Column 
    if (
        (x >= 0 && x < 0.333) &&
        (y >= 0.666 && y <= 1)
    ) return '1';
    
    // Top Row
    // Middle Column
    if (
        (x >= 0.333 && x < 0.666) &&
        (y >= 0.666 && y <= 1)
    ) return '2';
    
    // Top Row
    // Right Column
    if (
        (x >= 0.666 && x <= 1) &&
        (y >= 0.666 && y <= 1)
    ) return '3';

    // Middle Row
    // Left Column
    if (
        (x >= 0 && x < 0.333) &&
        (y >= 0.333 && y < 0.666)
    ) return '4';

    // Middle Row
    // Middle Column
    if (
        (x >= 0.333 && x < 0.666) &&
        (y >= 0.333 && y < 0.666)
    ) return '5';

    // Middle Row
    // Right Column
    if (
        (x >= 0.666 && x <= 1) &&
        (y >= 0.333 && y < 0.666)
    ) return '6';

    // Bottom Row
    // Left Column
    if (
        (x >= 0 && x < 0.333) &&
        (y >= 0 && y < 0.333)
    ) return '7';

    // Bottom Row
    // Middle Column
    if (
        (x >= 0.333 && x < 0.666) &&
        (y >= 0 && y < 0.333)
    ) return '8';

    // Bottom Row
    // Right Column
    if (
        (x >= 0.666 && x <= 1) &&
        (y >= 0 && y < 0.333)
    ) return '9';

  return 0;

}

void touch_event(
    float x,
    float y
)
{

    int number = touch_map(x, y);
    if (number == 0) return;

    if (pin_index < pin_len)
        pin[pin_index++] = touch_map(x, y);

}

int touch_handler(
    int _dontcare1,
    MTData *data, int len,
    double _dontcare2,
    int _dontcare3
)
{

  for (int i = 0; i < len; i++)
  {
    
    if (data->state == 1)
        touch_event(
            data->position.x,
            data->position.y
        );
    
    data += sizeof(MTData);

  }

  return 0;

}

void touch_start(int _pin_len)
{

    pin_len = _pin_len;
    pin = (char *) malloc(
        pin_len * sizeof(char));

    dev = MTDeviceCreateDefault();
    MTRegisterContactFrameCallback(
        dev, touch_handler);
    MTDeviceStart(dev, 0);

}

void touch_stop()
{
    MTDeviceStop(dev);
    free(pin);
}

bool touch_finished()
{ return pin_index == pin_len; }

char *touch_pin()
{ return pin; }
