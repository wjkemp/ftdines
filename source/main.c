#include "debug.h"
#include "ftdi_if.h"
#include "input_channel.h"
#include "uinput.h"

#include <stdio.h>
#include <unistd.h>

static int initialize();
static int process();
static void shutdown();

static struct keymap *keymaps[2];
struct input_channel input_channels[2];


int main(void)
{
    while (1) {
        if (initialize() == 0) {
            ftdines_printf("Running...\n");
            while (process() == 0);
            shutdown();
        }
        sleep(10);
    }

    return 0;
}

static int initialize()
{
    int r;

    ftdines_printf("Initializing\n");

    // Load the keymaps
    keymaps[0] = keymap_create_from_file("/etc/ftdines/keymap1.cfg");
    if (keymaps[0] == NULL) {
        ftdines_printerr("Could not create keymap[0]\n");
        goto err_keymap_create_0;
    }

    keymaps[1] = keymap_create_from_file("/etc/ftdines/keymap2.cfg");
    if (keymaps[1] == NULL) {
        ftdines_printerr("Could not create keymap[1]\n");
        goto err_keymap_create_1;
    }

    // Initialize uinput
    r = uinput_init(keymaps, 2);
    if (r) {
        ftdines_printerr("Could not initialize uinput\n");
        goto err_uinput_init;
    }

    // Initialize FTDI interface
    r = ftdi_if_init();
    if (r) {
        ftdines_printerr("Could not initialize FTDI interface\n");
        goto err_ftdi_if_init;
    }

    // Initialize the two input channels
    input_channel_init(&input_channels[0], keymaps[0]);
    input_channel_init(&input_channels[1], keymaps[1]);

    // Success
    return 0;

err_ftdi_if_init:
    uinput_shutdown();

err_uinput_init:
    keymap_destroy(keymaps[1]);

err_keymap_create_1:
    keymap_destroy(keymaps[0]);

err_keymap_create_0:
    return -1;
}

static int process()
{
    struct ftdi_if_read_result read_result;
    int r;


    r = ftdi_if_read(&read_result);
    if (r) {
        return r;
    }

    r = input_channel_process(&input_channels[0], read_result.channel_state[0]);
    if (r) {
        return r;
    }

    r = input_channel_process(&input_channels[1], read_result.channel_state[1]);
    if (r) {
        return r;
    }
}

static void shutdown()
{
    ftdines_printf("Shutting down\n");

    ftdi_if_shutdown();
    uinput_shutdown();
    keymap_destroy(keymaps[1]);
    keymap_destroy(keymaps[0]);
}
