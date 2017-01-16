// /*AFLA*/ /*
// /*AFLA*/  * Consumer interface the pin control subsystem
// /*AFLA*/  *
// /*AFLA*/  * Copyright (C) 2012 ST-Ericsson SA
// /*AFLA*/  * Written on behalf of Linaro for ST-Ericsson
// /*AFLA*/  * Based on bits of regulator core, gpio core and clk core
// /*AFLA*/  *
// /*AFLA*/  * Author: Linus Walleij <linus.walleij@linaro.org>
// /*AFLA*/  *
// /*AFLA*/  * License terms: GNU General Public License (GPL) version 2
// /*AFLA*/  */
// /*AFLA*/ #ifndef __LINUX_PINCTRL_CONSUMER_H
// /*AFLA*/ #define __LINUX_PINCTRL_CONSUMER_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/err.h>
// /*AFLA*/ #include <linux/list.h>
// /*AFLA*/ #include <linux/seq_file.h>
// /*AFLA*/ #include <linux/pinctrl/pinctrl-state.h>
// /*AFLA*/ 
// /*AFLA*/ /* This struct is private to the core and should be regarded as a cookie */
// /*AFLA*/ struct pinctrl;
// /*AFLA*/ struct pinctrl_state;
// /*AFLA*/ struct device;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PINCTRL
// /*AFLA*/ 
// /*AFLA*/ /* External interface to pin control */
// /*AFLA*/ extern int pinctrl_request_gpio(unsigned gpio);
// /*AFLA*/ extern void pinctrl_free_gpio(unsigned gpio);
// /*AFLA*/ extern int pinctrl_gpio_direction_input(unsigned gpio);
// /*AFLA*/ extern int pinctrl_gpio_direction_output(unsigned gpio);
// /*AFLA*/ 
// /*AFLA*/ extern struct pinctrl * __must_check pinctrl_get(struct device *dev);
// /*AFLA*/ extern void pinctrl_put(struct pinctrl *p);
// /*AFLA*/ extern struct pinctrl_state * __must_check pinctrl_lookup_state(
// /*AFLA*/ 							struct pinctrl *p,
// /*AFLA*/ 							const char *name);
// /*AFLA*/ extern int pinctrl_select_state(struct pinctrl *p, struct pinctrl_state *s);
// /*AFLA*/ 
// /*AFLA*/ extern struct pinctrl * __must_check devm_pinctrl_get(struct device *dev);
// /*AFLA*/ extern void devm_pinctrl_put(struct pinctrl *p);
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PM
// /*AFLA*/ extern int pinctrl_pm_select_default_state(struct device *dev);
// /*AFLA*/ extern int pinctrl_pm_select_sleep_state(struct device *dev);
// /*AFLA*/ extern int pinctrl_pm_select_idle_state(struct device *dev);
// /*AFLA*/ #else
// /*AFLA*/ static inline int pinctrl_pm_select_default_state(struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline int pinctrl_pm_select_sleep_state(struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ static inline int pinctrl_pm_select_idle_state(struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #else /* !CONFIG_PINCTRL */
// /*AFLA*/ 
// /*AFLA*/ static inline int pinctrl_request_gpio(unsigned gpio)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void pinctrl_free_gpio(unsigned gpio)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int pinctrl_gpio_direction_input(unsigned gpio)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int pinctrl_gpio_direction_output(unsigned gpio)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct pinctrl * __must_check pinctrl_get(struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ 	return NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void pinctrl_put(struct pinctrl *p)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct pinctrl_state * __must_check pinctrl_lookup_state(
// /*AFLA*/ 							struct pinctrl *p,
// /*AFLA*/ 							const char *name)
// /*AFLA*/ {
// /*AFLA*/ 	return NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int pinctrl_select_state(struct pinctrl *p,
// /*AFLA*/ 				       struct pinctrl_state *s)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct pinctrl * __must_check devm_pinctrl_get(struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ 	return NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void devm_pinctrl_put(struct pinctrl *p)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int pinctrl_pm_select_default_state(struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int pinctrl_pm_select_sleep_state(struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int pinctrl_pm_select_idle_state(struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* CONFIG_PINCTRL */
// /*AFLA*/ 
// /*AFLA*/ static inline struct pinctrl * __must_check pinctrl_get_select(
// /*AFLA*/ 					struct device *dev, const char *name)
// /*AFLA*/ {
// /*AFLA*/ 	struct pinctrl *p;
// /*AFLA*/ 	struct pinctrl_state *s;
// /*AFLA*/ 	int ret;
// /*AFLA*/ 
// /*AFLA*/ 	p = pinctrl_get(dev);
// /*AFLA*/ 	if (IS_ERR(p))
// /*AFLA*/ 		return p;
// /*AFLA*/ 
// /*AFLA*/ 	s = pinctrl_lookup_state(p, name);
// /*AFLA*/ 	if (IS_ERR(s)) {
// /*AFLA*/ 		pinctrl_put(p);
// /*AFLA*/ 		return ERR_CAST(s);
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	ret = pinctrl_select_state(p, s);
// /*AFLA*/ 	if (ret < 0) {
// /*AFLA*/ 		pinctrl_put(p);
// /*AFLA*/ 		return ERR_PTR(ret);
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	return p;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct pinctrl * __must_check pinctrl_get_select_default(
// /*AFLA*/ 					struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ 	return pinctrl_get_select(dev, PINCTRL_STATE_DEFAULT);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct pinctrl * __must_check devm_pinctrl_get_select(
// /*AFLA*/ 					struct device *dev, const char *name)
// /*AFLA*/ {
// /*AFLA*/ 	struct pinctrl *p;
// /*AFLA*/ 	struct pinctrl_state *s;
// /*AFLA*/ 	int ret;
// /*AFLA*/ 
// /*AFLA*/ 	p = devm_pinctrl_get(dev);
// /*AFLA*/ 	if (IS_ERR(p))
// /*AFLA*/ 		return p;
// /*AFLA*/ 
// /*AFLA*/ 	s = pinctrl_lookup_state(p, name);
// /*AFLA*/ 	if (IS_ERR(s)) {
// /*AFLA*/ 		devm_pinctrl_put(p);
// /*AFLA*/ 		return ERR_CAST(s);
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	ret = pinctrl_select_state(p, s);
// /*AFLA*/ 	if (ret < 0) {
// /*AFLA*/ 		devm_pinctrl_put(p);
// /*AFLA*/ 		return ERR_PTR(ret);
// /*AFLA*/ 	}
// /*AFLA*/ 
// /*AFLA*/ 	return p;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline struct pinctrl * __must_check devm_pinctrl_get_select_default(
// /*AFLA*/ 					struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ 	return devm_pinctrl_get_select(dev, PINCTRL_STATE_DEFAULT);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* __LINUX_PINCTRL_CONSUMER_H */
