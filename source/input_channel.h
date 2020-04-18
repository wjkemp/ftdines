#ifndef __INPUT_CHANNEL_H__
#define __INPUT_CHANNEL_H__

#include "keymap.h"

#include <stdint.h>

struct input_channel
{
    struct keymap *keymap;
    uint8_t button_state_prev;
};

void input_channel_init(
    struct input_channel *channel,
    struct keymap *keymap);

int input_channel_process(
    struct input_channel *channel,
    uint8_t button_state);

#endif
