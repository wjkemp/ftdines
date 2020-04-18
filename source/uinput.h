#ifndef __UINPUT_H__
#define __UINPUT_H__

#include "keymap.h"

int uinput_init(struct keymap **keymaps, size_t keymap_count);
int uinput_send_input_event(uint16_t key_code, int event);
int uinput_sync_input_events(void);
void uinput_shutdown(void);

#endif
