// /*AFLA*/ /*
// /*AFLA*/  * Per-device information from the pin control system.
// /*AFLA*/  * This is the stuff that get included into the device
// /*AFLA*/  * core.
// /*AFLA*/  *
// /*AFLA*/  * Copyright (C) 2012 ST-Ericsson SA
// /*AFLA*/  * Written on behalf of Linaro for ST-Ericsson
// /*AFLA*/  * This interface is used in the core to keep track of pins.
// /*AFLA*/  *
// /*AFLA*/  * Author: Linus Walleij <linus.walleij@linaro.org>
// /*AFLA*/  *
// /*AFLA*/  * License terms: GNU General Public License (GPL) version 2
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef PINCTRL_DEVINFO_H
// /*AFLA*/ #define PINCTRL_DEVINFO_H
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PINCTRL
// /*AFLA*/ 
// /*AFLA*/ /* The device core acts as a consumer toward pinctrl */
// /*AFLA*/ #include <linux/pinctrl/consumer.h>
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct dev_pin_info - pin state container for devices
// /*AFLA*/  * @p: pinctrl handle for the containing device
// /*AFLA*/  * @default_state: the default state for the handle, if found
// /*AFLA*/  * @init_state: the state at probe time, if found
// /*AFLA*/  * @sleep_state: the state at suspend time, if found
// /*AFLA*/  * @idle_state: the state at idle (runtime suspend) time, if found
// /*AFLA*/  */
// /*AFLA*/ struct dev_pin_info {
// /*AFLA*/ 	struct pinctrl *p;
// /*AFLA*/ 	struct pinctrl_state *default_state;
// /*AFLA*/ 	struct pinctrl_state *init_state;
// /*AFLA*/ #ifdef CONFIG_PM
// /*AFLA*/ 	struct pinctrl_state *sleep_state;
// /*AFLA*/ 	struct pinctrl_state *idle_state;
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern int pinctrl_bind_pins(struct device *dev);
// /*AFLA*/ extern int pinctrl_init_done(struct device *dev);
// /*AFLA*/ 
// /*AFLA*/ #else
// /*AFLA*/ 
// /*AFLA*/ /* Stubs if we're not using pinctrl */
// /*AFLA*/ 
// /*AFLA*/ static inline int pinctrl_bind_pins(struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int pinctrl_init_done(struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_PINCTRL */
// /*AFLA*/ #endif /* PINCTRL_DEVINFO_H */
