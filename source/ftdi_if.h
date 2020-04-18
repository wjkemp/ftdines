#ifndef __FTDI_IF_H__
#define __FTDI_IF_H__

#include <stdint.h>

struct ftdi_if_read_result
{
    uint8_t channel_state[2];
};

int ftdi_if_init(void);
int ftdi_if_read(struct ftdi_if_read_result *result);
void ftdi_if_shutdown(void);

#endif
