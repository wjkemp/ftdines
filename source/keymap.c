#include "keymap.h"
#include "debug.h"

#include <stdlib.h>
#include <string.h>
#include <libconfig.h>
#include <linux/input.h>
#include <linux/uinput.h>


static const struct keymap_entry keymap_default[] =
{
    {
        .button_mask = JOYSTICK_RIGHT,
        .key_code = KEY_RIGHT
    },
    {
        .button_mask = JOYSTICK_LEFT,
        .key_code = KEY_LEFT
    },
    {
        .button_mask = JOYSTICK_DOWN,
        .key_code = KEY_DOWN
    },
    {
        .button_mask = JOYSTICK_UP,
        .key_code = KEY_UP
    },
    {
        .button_mask = JOYSTICK_SELECT,
        .key_code = KEY_Z
    },
    {
        .button_mask = JOYSTICK_START,
        .key_code = KEY_X
    },
    {
        .button_mask = JOYSTICK_B,
        .key_code = KEY_B
    },
    {
        .button_mask = JOYSTICK_A,
        .key_code = KEY_A
    }
};

static void keymap_update_from_config(
    config_t *cfg,
    struct keymap *keymap,
    uint8_t button_mask,
    const char *config_key);

static uint16_t keymap_string_to_keycode(const char *str);

struct keymap *keymap_create_from_file(const char *path)
{
    struct keymap *keymap = (struct keymap *)malloc(sizeof(struct keymap));
    if (keymap) {

         config_t cfg;
         config_init(&cfg);

         keymap->entry_count = sizeof(keymap_default) / sizeof(keymap_default[0]);
         keymap->entries = (struct keymap_entry *)malloc(sizeof(keymap_default));
         memcpy(keymap->entries, keymap_default, sizeof(keymap_default));

         if (config_read_file(&cfg, path)) {

             keymap_update_from_config(&cfg, keymap, JOYSTICK_RIGHT, "JOYSTICK_RIGHT");
             keymap_update_from_config(&cfg, keymap, JOYSTICK_LEFT, "JOYSTICK_LEFT");
             keymap_update_from_config(&cfg, keymap, JOYSTICK_DOWN, "JOYSTICK_DOWN");
             keymap_update_from_config(&cfg, keymap, JOYSTICK_UP, "JOYSTICK_UP");
             keymap_update_from_config(&cfg, keymap, JOYSTICK_SELECT, "JOYSTICK_SELECT");
             keymap_update_from_config(&cfg, keymap, JOYSTICK_START, "JOYSTICK_START");
             keymap_update_from_config(&cfg, keymap, JOYSTICK_B, "JOYSTICK_B");
             keymap_update_from_config(&cfg, keymap, JOYSTICK_A, "JOYSTICK_A");

         } else {
             ftdines_printerr("Could not load configuration file '%s', falling back to default keymap\n", path);
         }

        config_destroy(&cfg);
    }

    return keymap;
}

void keymap_destroy(struct keymap *keymap)
{
    //free(keymap->entries);
    free(keymap);
}

static void keymap_update_from_config(
    config_t *cfg,
    struct keymap *keymap,
    uint8_t button_mask,
    const char *config_key)
{
    size_t i;

    for (i=0; i < keymap->entry_count; ++i) {
        if (keymap->entries[i].button_mask == button_mask) {
            const char *config_value;
            if (config_lookup_string(cfg, config_key, &config_value)) {
                keymap->entries[i].key_code = keymap_string_to_keycode(config_value);
            }

            return;
        }
    }
}

static uint16_t keymap_string_to_keycode(const char *str)
{
    if (strcmp(str, "A") == 0) return KEY_A;
    if (strcmp(str, "B") == 0) return KEY_B;
    if (strcmp(str, "C") == 0) return KEY_C;
    if (strcmp(str, "D") == 0) return KEY_D;
    if (strcmp(str, "E") == 0) return KEY_E;
    if (strcmp(str, "F") == 0) return KEY_F;
    if (strcmp(str, "G") == 0) return KEY_G;
    if (strcmp(str, "H") == 0) return KEY_H;
    if (strcmp(str, "I") == 0) return KEY_I;
    if (strcmp(str, "J") == 0) return KEY_J;
    if (strcmp(str, "K") == 0) return KEY_K;
    if (strcmp(str, "L") == 0) return KEY_L;
    if (strcmp(str, "M") == 0) return KEY_M;
    if (strcmp(str, "N") == 0) return KEY_N;
    if (strcmp(str, "O") == 0) return KEY_O;
    if (strcmp(str, "P") == 0) return KEY_P;
    if (strcmp(str, "Q") == 0) return KEY_Q;
    if (strcmp(str, "R") == 0) return KEY_R;
    if (strcmp(str, "S") == 0) return KEY_S;
    if (strcmp(str, "T") == 0) return KEY_T;
    if (strcmp(str, "U") == 0) return KEY_U;
    if (strcmp(str, "V") == 0) return KEY_V;
    if (strcmp(str, "W") == 0) return KEY_W;
    if (strcmp(str, "X") == 0) return KEY_X;
    if (strcmp(str, "Y") == 0) return KEY_Y;
    if (strcmp(str, "Z") == 0) return KEY_Z;

    if (strcmp(str, "0") == 0) return KEY_0;
    if (strcmp(str, "1") == 0) return KEY_1;
    if (strcmp(str, "2") == 0) return KEY_2;
    if (strcmp(str, "3") == 0) return KEY_3;
    if (strcmp(str, "4") == 0) return KEY_4;
    if (strcmp(str, "5") == 0) return KEY_5;
    if (strcmp(str, "6") == 0) return KEY_6;
    if (strcmp(str, "7") == 0) return KEY_7;
    if (strcmp(str, "8") == 0) return KEY_8;
    if (strcmp(str, "9") == 0) return KEY_9;

    return KEY_RESERVED;
}
