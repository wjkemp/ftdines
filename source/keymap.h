#ifndef __KEYMAP_H__
#define __KEYMAP_H__

#include <stdint.h>
#include <stddef.h>


#define JOYSTICK_RIGHT      0x01
#define JOYSTICK_LEFT       0x02
#define JOYSTICK_DOWN       0x04
#define JOYSTICK_UP         0x08
#define JOYSTICK_SELECT     0x20
#define JOYSTICK_START      0x10
#define JOYSTICK_B          0x40
#define JOYSTICK_A          0x80


struct keymap_entry
{
    uint8_t button_mask;
    uint16_t key_code;
};

struct keymap
{
    struct keymap_entry *entries;
    size_t entry_count;
};

struct keymap *keymap_create_from_file(const char *path);
void keymap_destroy(struct keymap *keymap);

#endif
