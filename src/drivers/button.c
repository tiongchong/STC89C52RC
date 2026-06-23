#include <stc89c52rc/drivers/button.h>

void drv_button_init(const drv_button_t *button)
{
    if (button == 0) {
        return;
    }

    hal_gpio_write(&button->pin, true);
}

bool drv_button_is_pressed(const drv_button_t *button)
{
    bool level;

    if (button == 0) {
        return false;
    }

    level = hal_gpio_read(&button->pin);
    return (button->polarity == DRV_BUTTON_ACTIVE_LOW) ? !level : level;
}
