#include "uinput.h"
#include "debug.h"

#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

static int uinput_fd;

static int uinput_register_keymap(struct keymap *keymap);

int uinput_init(struct keymap **keymaps, size_t keymap_count)
{
    struct uinput_user_dev uidev;
    int r;
    int i;

    uinput_fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (uinput_fd < 0) {
        ftdines_printerr("%s\n", strerror(errno));
        goto err_open;
    }

    r = ioctl(uinput_fd, UI_SET_EVBIT, EV_KEY);
    if (r) {
        ftdines_printerr("%s\n", strerror(errno));
        goto err_ioctl;
    }

    // Register the keymaps
    for (i=0; i < keymap_count; ++i) {
       r = uinput_register_keymap(keymaps[i]);
       if (r) {
           goto err_ioctl;
       }
    }

    memset(&uidev, 0, sizeof(uidev));
    snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "ftdines");
    uidev.id.bustype = BUS_USB;
    uidev.id.vendor  = 0x1;
    uidev.id.product = 0x1;
    uidev.id.version = 1;

    r = write(uinput_fd, &uidev, sizeof(uidev));
    if (r < 0) {
        ftdines_printerr("%s\n", strerror(errno));
        goto err_ioctl;
    }

    r = ioctl(uinput_fd, UI_DEV_CREATE);
    if (r) {
        ftdines_printerr("%s\n", strerror(errno));
        goto err_ioctl;
    }

    return 0;

err_ioctl:
    close(uinput_fd);

err_open:
    return -1;
}

int uinput_send_input_event(uint16_t key_code, int event)
{
    struct input_event ev;

    memset(&ev, 0, sizeof(ev));
    ev.type = EV_KEY;
    ev.code = key_code;
    ev.value = event;

    ssize_t bytes_written = write(uinput_fd, &ev, sizeof(ev));

    if (bytes_written < 0) {
        return -1;
    }

    if (bytes_written != sizeof(struct input_event)) {
        return -1;
    }

    return 0;
}

int uinput_sync_input_events(void)
{
    struct input_event ev;

    memset(&ev, 0, sizeof(struct input_event));
    ev.type = EV_SYN;
    ev.code = 0;
    ev.value = 0;

    ssize_t bytes_written = write(uinput_fd, &ev, sizeof(ev));

    if (bytes_written < 0) {
        ftdines_printerr("%s\n", strerror(errno));
        return -1;
    }

    if (bytes_written != sizeof(struct input_event)) {
        return -1;
    }

    return 0;
}

void uinput_shutdown()
{
    ioctl(uinput_fd, UI_DEV_DESTROY);
    close(uinput_fd);
}

static int uinput_register_keymap(struct keymap *keymap)
{
    size_t i;
    int r = 0;

    for (i=0; i < keymap->entry_count; ++i) {
        r = ioctl(uinput_fd, UI_SET_KEYBIT, keymap->entries[i].key_code);
        if (r) {
            ftdines_printerr("%s\n", strerror(errno));
            break;
        }
    }

    return r;
}
