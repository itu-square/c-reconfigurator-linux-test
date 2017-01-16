// /*AFLA*/ /*
// /*AFLA*/  *  pm_wakeup.h - Power management wakeup interface
// /*AFLA*/  *
// /*AFLA*/  *  Copyright (C) 2008 Alan Stern
// /*AFLA*/  *  Copyright (C) 2010 Rafael J. Wysocki, Novell Inc.
// /*AFLA*/  *
// /*AFLA*/  *  This program is free software; you can redistribute it and/or modify
// /*AFLA*/  *  it under the terms of the GNU General Public License as published by
// /*AFLA*/  *  the Free Software Foundation; either version 2 of the License, or
// /*AFLA*/  *  (at your option) any later version.
// /*AFLA*/  *
// /*AFLA*/  *  This program is distributed in the hope that it will be useful,
// /*AFLA*/  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
// /*AFLA*/  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// /*AFLA*/  *  GNU General Public License for more details.
// /*AFLA*/  *
// /*AFLA*/  *  You should have received a copy of the GNU General Public License
// /*AFLA*/  *  along with this program; if not, write to the Free Software
// /*AFLA*/  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _LINUX_PM_WAKEUP_H
// /*AFLA*/ #define _LINUX_PM_WAKEUP_H
// /*AFLA*/ 
// /*AFLA*/ #ifndef _DEVICE_H_
// /*AFLA*/ # error "please don't include this file directly"
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #include <linux/types.h>
// /*AFLA*/ 
// /*AFLA*/ struct wake_irq;
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct wakeup_source - Representation of wakeup sources
// /*AFLA*/  *
// /*AFLA*/  * @name: Name of the wakeup source
// /*AFLA*/  * @entry: Wakeup source list entry
// /*AFLA*/  * @lock: Wakeup source lock
// /*AFLA*/  * @wakeirq: Optional device specific wakeirq
// /*AFLA*/  * @timer: Wakeup timer list
// /*AFLA*/  * @timer_expires: Wakeup timer expiration
// /*AFLA*/  * @total_time: Total time this wakeup source has been active.
// /*AFLA*/  * @max_time: Maximum time this wakeup source has been continuously active.
// /*AFLA*/  * @last_time: Monotonic clock when the wakeup source's was touched last time.
// /*AFLA*/  * @prevent_sleep_time: Total time this source has been preventing autosleep.
// /*AFLA*/  * @event_count: Number of signaled wakeup events.
// /*AFLA*/  * @active_count: Number of times the wakeup source was activated.
// /*AFLA*/  * @relax_count: Number of times the wakeup source was deactivated.
// /*AFLA*/  * @expire_count: Number of times the wakeup source's timeout has expired.
// /*AFLA*/  * @wakeup_count: Number of times the wakeup source might abort suspend.
// /*AFLA*/  * @active: Status of the wakeup source.
// /*AFLA*/  * @has_timeout: The wakeup source has been activated with a timeout.
// /*AFLA*/  */
// /*AFLA*/ struct wakeup_source {
// /*AFLA*/ 	const char 		*name;
// /*AFLA*/ 	struct list_head	entry;
// /*AFLA*/ 	spinlock_t		lock;
// /*AFLA*/ 	struct wake_irq		*wakeirq;
// /*AFLA*/ 	struct timer_list	timer;
// /*AFLA*/ 	unsigned long		timer_expires;
// /*AFLA*/ 	ktime_t total_time;
// /*AFLA*/ 	ktime_t max_time;
// /*AFLA*/ 	ktime_t last_time;
// /*AFLA*/ 	ktime_t start_prevent_time;
// /*AFLA*/ 	ktime_t prevent_sleep_time;
// /*AFLA*/ 	unsigned long		event_count;
// /*AFLA*/ 	unsigned long		active_count;
// /*AFLA*/ 	unsigned long		relax_count;
// /*AFLA*/ 	unsigned long		expire_count;
// /*AFLA*/ 	unsigned long		wakeup_count;
// /*AFLA*/ 	bool			active:1;
// /*AFLA*/ 	bool			autosleep_enabled:1;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PM_SLEEP
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Changes to device_may_wakeup take effect on the next pm state change.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ static inline bool device_can_wakeup(struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ 	return dev->power.can_wakeup;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool device_may_wakeup(struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ 	return dev->power.can_wakeup && !!dev->power.wakeup;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /* drivers/base/power/wakeup.c */
// /*AFLA*/ extern void wakeup_source_prepare(struct wakeup_source *ws, const char *name);
// /*AFLA*/ extern struct wakeup_source *wakeup_source_create(const char *name);
// /*AFLA*/ extern void wakeup_source_drop(struct wakeup_source *ws);
// /*AFLA*/ extern void wakeup_source_destroy(struct wakeup_source *ws);
// /*AFLA*/ extern void wakeup_source_add(struct wakeup_source *ws);
// /*AFLA*/ extern void wakeup_source_remove(struct wakeup_source *ws);
// /*AFLA*/ extern struct wakeup_source *wakeup_source_register(const char *name);
// /*AFLA*/ extern void wakeup_source_unregister(struct wakeup_source *ws);
// /*AFLA*/ extern int device_wakeup_enable(struct device *dev);
// /*AFLA*/ extern int device_wakeup_disable(struct device *dev);
// /*AFLA*/ extern void device_set_wakeup_capable(struct device *dev, bool capable);
// /*AFLA*/ extern int device_init_wakeup(struct device *dev, bool val);
// /*AFLA*/ extern int device_set_wakeup_enable(struct device *dev, bool enable);
// /*AFLA*/ extern void __pm_stay_awake(struct wakeup_source *ws);
// /*AFLA*/ extern void pm_stay_awake(struct device *dev);
// /*AFLA*/ extern void __pm_relax(struct wakeup_source *ws);
// /*AFLA*/ extern void pm_relax(struct device *dev);
// /*AFLA*/ extern void __pm_wakeup_event(struct wakeup_source *ws, unsigned int msec);
// /*AFLA*/ extern void pm_wakeup_event(struct device *dev, unsigned int msec);
// /*AFLA*/ 
// /*AFLA*/ #else /* !CONFIG_PM_SLEEP */
// /*AFLA*/ 
// /*AFLA*/ static inline void device_set_wakeup_capable(struct device *dev, bool capable)
// /*AFLA*/ {
// /*AFLA*/ 	dev->power.can_wakeup = capable;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool device_can_wakeup(struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ 	return dev->power.can_wakeup;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void wakeup_source_prepare(struct wakeup_source *ws,
// /*AFLA*/ 					 const char *name) {}
// /*AFLA*/ 
// /*AFLA*/ static inline struct wakeup_source *wakeup_source_create(const char *name)
// /*AFLA*/ {
// /*AFLA*/ 	return NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void wakeup_source_drop(struct wakeup_source *ws) {}
// /*AFLA*/ 
// /*AFLA*/ static inline void wakeup_source_destroy(struct wakeup_source *ws) {}
// /*AFLA*/ 
// /*AFLA*/ static inline void wakeup_source_add(struct wakeup_source *ws) {}
// /*AFLA*/ 
// /*AFLA*/ static inline void wakeup_source_remove(struct wakeup_source *ws) {}
// /*AFLA*/ 
// /*AFLA*/ static inline struct wakeup_source *wakeup_source_register(const char *name)
// /*AFLA*/ {
// /*AFLA*/ 	return NULL;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void wakeup_source_unregister(struct wakeup_source *ws) {}
// /*AFLA*/ 
// /*AFLA*/ static inline int device_wakeup_enable(struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ 	dev->power.should_wakeup = true;
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int device_wakeup_disable(struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ 	dev->power.should_wakeup = false;
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int device_set_wakeup_enable(struct device *dev, bool enable)
// /*AFLA*/ {
// /*AFLA*/ 	dev->power.should_wakeup = enable;
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline int device_init_wakeup(struct device *dev, bool val)
// /*AFLA*/ {
// /*AFLA*/ 	device_set_wakeup_capable(dev, val);
// /*AFLA*/ 	device_set_wakeup_enable(dev, val);
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline bool device_may_wakeup(struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ 	return dev->power.can_wakeup && dev->power.should_wakeup;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void __pm_stay_awake(struct wakeup_source *ws) {}
// /*AFLA*/ 
// /*AFLA*/ static inline void pm_stay_awake(struct device *dev) {}
// /*AFLA*/ 
// /*AFLA*/ static inline void __pm_relax(struct wakeup_source *ws) {}
// /*AFLA*/ 
// /*AFLA*/ static inline void pm_relax(struct device *dev) {}
// /*AFLA*/ 
// /*AFLA*/ static inline void __pm_wakeup_event(struct wakeup_source *ws, unsigned int msec) {}
// /*AFLA*/ 
// /*AFLA*/ static inline void pm_wakeup_event(struct device *dev, unsigned int msec) {}
// /*AFLA*/ 
// /*AFLA*/ #endif /* !CONFIG_PM_SLEEP */
// /*AFLA*/ 
// /*AFLA*/ static inline void wakeup_source_init(struct wakeup_source *ws,
// /*AFLA*/ 				      const char *name)
// /*AFLA*/ {
// /*AFLA*/ 	wakeup_source_prepare(ws, name);
// /*AFLA*/ 	wakeup_source_add(ws);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void wakeup_source_trash(struct wakeup_source *ws)
// /*AFLA*/ {
// /*AFLA*/ 	wakeup_source_remove(ws);
// /*AFLA*/ 	wakeup_source_drop(ws);
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_PM_WAKEUP_H */
