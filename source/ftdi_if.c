#include "debug.h"
#include "ftdi_if.h"

#include <ftdi.h>
#include <stdio.h>

static struct ftdi_context ftdi_ctx;

int ftdi_if_init(void)
{
    int r;

    // Open FTDI
    r = ftdi_init(&ftdi_ctx);
    if (r) {
        ftdines_printerr("ftdi_init() failed with r=%d\n", r);
        goto err_ftdi_init;
    }

    r = ftdi_usb_open(&ftdi_ctx, 0x0403, 0x6001);
    if (r) {
        ftdines_printerr("ftdi_usb_open() failed with r=%d\n", r);
        goto err_ftdi_usb_open;
    }

    return 0;

err_ftdi_usb_open:
    ftdi_deinit(&ftdi_ctx);

err_ftdi_init:
    return -1;
}

int ftdi_if_read(struct ftdi_if_read_result *result)
{
    int i;
    int r;

    // Latch
    r = ftdi_set_bitmode(&ftdi_ctx, 0x31, 0x20);
    if (r) {
        return r;
    }

    usleep(100);

    r = ftdi_set_bitmode(&ftdi_ctx, 0x30, 0x20);
    if (r) {
        return r;
    }

    usleep(100);

    // Clock
    uint8_t input[2] = { 0, 0 };
    uint8_t pins;

    for (i=0; i < 8; ++i) {

        r = ftdi_read_pins(&ftdi_ctx, &pins);
        if (r) {
            return r;
        }

        input[0] <<= 1;
        input[0] |= ((pins & 0x04) >> 2);

        input[1] <<= 1;
        input[1] |= ((pins & 0x08) >> 3);

        usleep(100);
        r = ftdi_set_bitmode(&ftdi_ctx, 0x32, 0x20);
        if (r) {
            return r;
        }

        usleep(100);
        r = ftdi_set_bitmode(&ftdi_ctx, 0x30, 0x20);
        if (r) {
            return r;
        }
    }

    result->channel_state[0] = ~input[0];
    result->channel_state[1] = ~input[1];

    return 0;
}

void ftdi_if_shutdown(void)
{
    ftdi_usb_close(&ftdi_ctx);
    ftdi_deinit(&ftdi_ctx);
}
