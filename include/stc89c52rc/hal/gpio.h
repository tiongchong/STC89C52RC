#ifndef STC89C52RC_HAL_GPIO_H
#define STC89C52RC_HAL_GPIO_H

#include <stdint.h>

typedef struct {
    uint8_t port;
    uint8_t bit;
} hal_gpio_pin_t;

#define HAL_GPIO_PIN(port_, bit_) { (uint8_t)(port_), (uint8_t)(bit_) }

void hal_gpio_write(const hal_gpio_pin_t *pin, uint8_t high);
uint8_t hal_gpio_read(const hal_gpio_pin_t *pin);
void hal_gpio_toggle(const hal_gpio_pin_t *pin);
void hal_gpio_set_port(uint8_t port, uint8_t value);
uint8_t hal_gpio_get_port(uint8_t port);

#endif
