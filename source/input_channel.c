#include "input_channel.h"
#include "uinput.h"

void input_channel_init(
    struct input_channel *channel,
    struct keymap *keymap)
{
    channel->keymap = keymap;
    channel->button_state_prev = 0;
}

int input_channel_process(
    struct input_channel *channel,
    uint8_t button_state)
{
    size_t i;
    int r;

    // Send input events
    for (i=0; i < channel->keymap->entry_count; ++i) {

        // Set event
        if ((button_state & channel->keymap->entries[i].button_mask) && (!(channel->button_state_prev & channel->keymap->entries[i].button_mask))) {
            r = uinput_send_input_event(channel->keymap->entries[i].key_code, 1);
            if (r) {
                return r;
            }
        }

        // Clear event
        if (!(button_state & channel->keymap->entries[i].button_mask) && (channel->button_state_prev & channel->keymap->entries[i].button_mask)) {
            r = uinput_send_input_event(channel->keymap->entries[i].key_code, 0);
            if (r) {
                return r;
            }
        }
    }

    // Sync input events
    r = uinput_sync_input_events();
    if (r) {
        return r;
    }

    // Update previous button state
    channel->button_state_prev = button_state;

    return 0;
}
