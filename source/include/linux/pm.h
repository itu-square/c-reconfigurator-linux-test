// /*AFLA*/ /*
// /*AFLA*/  *  pm.h - Power management interface
// /*AFLA*/  *
// /*AFLA*/  *  Copyright (C) 2000 Andrew Henroid
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
// /*AFLA*/ #ifndef _LINUX_PM_H
// /*AFLA*/ #define _LINUX_PM_H
// /*AFLA*/ 
// /*AFLA*/ #include <linux/list.h>
// /*AFLA*/ #include <linux/workqueue.h>
// /*AFLA*/ #include <linux/spinlock.h>
// /*AFLA*/ #include <linux/wait.h>
// /*AFLA*/ #include <linux/timer.h>
// /*AFLA*/ #include <linux/completion.h>
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Callbacks for platform drivers to implement.
// /*AFLA*/  */
// /*AFLA*/ extern void (*pm_power_off)(void);
// /*AFLA*/ extern void (*pm_power_off_prepare)(void);
// /*AFLA*/ 
// /*AFLA*/ struct device; /* we have a circular dep with device.h */
// /*AFLA*/ #ifdef CONFIG_VT_CONSOLE_SLEEP
// /*AFLA*/ extern void pm_vt_switch_required(struct device *dev, bool required);
// /*AFLA*/ extern void pm_vt_switch_unregister(struct device *dev);
// /*AFLA*/ #else
// /*AFLA*/ static inline void pm_vt_switch_required(struct device *dev, bool required)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ static inline void pm_vt_switch_unregister(struct device *dev)
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ #endif /* CONFIG_VT_CONSOLE_SLEEP */
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Device power management
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ struct device;
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PM
// /*AFLA*/ extern const char power_group_name[];		/* = "power" */
// /*AFLA*/ #else
// /*AFLA*/ #define power_group_name	NULL
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ typedef struct pm_message {
// /*AFLA*/ 	int event;
// /*AFLA*/ } pm_message_t;
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * struct dev_pm_ops - device PM callbacks
// /*AFLA*/  *
// /*AFLA*/  * Several device power state transitions are externally visible, affecting
// /*AFLA*/  * the state of pending I/O queues and (for drivers that touch hardware)
// /*AFLA*/  * interrupts, wakeups, DMA, and other hardware state.  There may also be
// /*AFLA*/  * internal transitions to various low-power modes which are transparent
// /*AFLA*/  * to the rest of the driver stack (such as a driver that's ON gating off
// /*AFLA*/  * clocks which are not in active use).
// /*AFLA*/  *
// /*AFLA*/  * The externally visible transitions are handled with the help of callbacks
// /*AFLA*/  * included in this structure in such a way that two levels of callbacks are
// /*AFLA*/  * involved.  First, the PM core executes callbacks provided by PM domains,
// /*AFLA*/  * device types, classes and bus types.  They are the subsystem-level callbacks
// /*AFLA*/  * supposed to execute callbacks provided by device drivers, although they may
// /*AFLA*/  * choose not to do that.  If the driver callbacks are executed, they have to
// /*AFLA*/  * collaborate with the subsystem-level callbacks to achieve the goals
// /*AFLA*/  * appropriate for the given system transition, given transition phase and the
// /*AFLA*/  * subsystem the device belongs to.
// /*AFLA*/  *
// /*AFLA*/  * @prepare: The principal role of this callback is to prevent new children of
// /*AFLA*/  *	the device from being registered after it has returned (the driver's
// /*AFLA*/  *	subsystem and generally the rest of the kernel is supposed to prevent
// /*AFLA*/  *	new calls to the probe method from being made too once @prepare() has
// /*AFLA*/  *	succeeded).  If @prepare() detects a situation it cannot handle (e.g.
// /*AFLA*/  *	registration of a child already in progress), it may return -EAGAIN, so
// /*AFLA*/  *	that the PM core can execute it once again (e.g. after a new child has
// /*AFLA*/  *	been registered) to recover from the race condition.
// /*AFLA*/  *	This method is executed for all kinds of suspend transitions and is
// /*AFLA*/  *	followed by one of the suspend callbacks: @suspend(), @freeze(), or
// /*AFLA*/  *	@poweroff().  If the transition is a suspend to memory or standby (that
// /*AFLA*/  *	is, not related to hibernation), the return value of @prepare() may be
// /*AFLA*/  *	used to indicate to the PM core to leave the device in runtime suspend
// /*AFLA*/  *	if applicable.  Namely, if @prepare() returns a positive number, the PM
// /*AFLA*/  *	core will understand that as a declaration that the device appears to be
// /*AFLA*/  *	runtime-suspended and it may be left in that state during the entire
// /*AFLA*/  *	transition and during the subsequent resume if all of its descendants
// /*AFLA*/  *	are left in runtime suspend too.  If that happens, @complete() will be
// /*AFLA*/  *	executed directly after @prepare() and it must ensure the proper
// /*AFLA*/  *	functioning of the device after the system resume.
// /*AFLA*/  *	The PM core executes subsystem-level @prepare() for all devices before
// /*AFLA*/  *	starting to invoke suspend callbacks for any of them, so generally
// /*AFLA*/  *	devices may be assumed to be functional or to respond to runtime resume
// /*AFLA*/  *	requests while @prepare() is being executed.  However, device drivers
// /*AFLA*/  *	may NOT assume anything about the availability of user space at that
// /*AFLA*/  *	time and it is NOT valid to request firmware from within @prepare()
// /*AFLA*/  *	(it's too late to do that).  It also is NOT valid to allocate
// /*AFLA*/  *	substantial amounts of memory from @prepare() in the GFP_KERNEL mode.
// /*AFLA*/  *	[To work around these limitations, drivers may register suspend and
// /*AFLA*/  *	hibernation notifiers to be executed before the freezing of tasks.]
// /*AFLA*/  *
// /*AFLA*/  * @complete: Undo the changes made by @prepare().  This method is executed for
// /*AFLA*/  *	all kinds of resume transitions, following one of the resume callbacks:
// /*AFLA*/  *	@resume(), @thaw(), @restore().  Also called if the state transition
// /*AFLA*/  *	fails before the driver's suspend callback: @suspend(), @freeze() or
// /*AFLA*/  *	@poweroff(), can be executed (e.g. if the suspend callback fails for one
// /*AFLA*/  *	of the other devices that the PM core has unsuccessfully attempted to
// /*AFLA*/  *	suspend earlier).
// /*AFLA*/  *	The PM core executes subsystem-level @complete() after it has executed
// /*AFLA*/  *	the appropriate resume callbacks for all devices.  If the corresponding
// /*AFLA*/  *	@prepare() at the beginning of the suspend transition returned a
// /*AFLA*/  *	positive number and the device was left in runtime suspend (without
// /*AFLA*/  *	executing any suspend and resume callbacks for it), @complete() will be
// /*AFLA*/  *	the only callback executed for the device during resume.  In that case,
// /*AFLA*/  *	@complete() must be prepared to do whatever is necessary to ensure the
// /*AFLA*/  *	proper functioning of the device after the system resume.  To this end,
// /*AFLA*/  *	@complete() can check the power.direct_complete flag of the device to
// /*AFLA*/  *	learn whether (unset) or not (set) the previous suspend and resume
// /*AFLA*/  *	callbacks have been executed for it.
// /*AFLA*/  *
// /*AFLA*/  * @suspend: Executed before putting the system into a sleep state in which the
// /*AFLA*/  *	contents of main memory are preserved.  The exact action to perform
// /*AFLA*/  *	depends on the device's subsystem (PM domain, device type, class or bus
// /*AFLA*/  *	type), but generally the device must be quiescent after subsystem-level
// /*AFLA*/  *	@suspend() has returned, so that it doesn't do any I/O or DMA.
// /*AFLA*/  *	Subsystem-level @suspend() is executed for all devices after invoking
// /*AFLA*/  *	subsystem-level @prepare() for all of them.
// /*AFLA*/  *
// /*AFLA*/  * @suspend_late: Continue operations started by @suspend().  For a number of
// /*AFLA*/  *	devices @suspend_late() may point to the same callback routine as the
// /*AFLA*/  *	runtime suspend callback.
// /*AFLA*/  *
// /*AFLA*/  * @resume: Executed after waking the system up from a sleep state in which the
// /*AFLA*/  *	contents of main memory were preserved.  The exact action to perform
// /*AFLA*/  *	depends on the device's subsystem, but generally the driver is expected
// /*AFLA*/  *	to start working again, responding to hardware events and software
// /*AFLA*/  *	requests (the device itself may be left in a low-power state, waiting
// /*AFLA*/  *	for a runtime resume to occur).  The state of the device at the time its
// /*AFLA*/  *	driver's @resume() callback is run depends on the platform and subsystem
// /*AFLA*/  *	the device belongs to.  On most platforms, there are no restrictions on
// /*AFLA*/  *	availability of resources like clocks during @resume().
// /*AFLA*/  *	Subsystem-level @resume() is executed for all devices after invoking
// /*AFLA*/  *	subsystem-level @resume_noirq() for all of them.
// /*AFLA*/  *
// /*AFLA*/  * @resume_early: Prepare to execute @resume().  For a number of devices
// /*AFLA*/  *	@resume_early() may point to the same callback routine as the runtime
// /*AFLA*/  *	resume callback.
// /*AFLA*/  *
// /*AFLA*/  * @freeze: Hibernation-specific, executed before creating a hibernation image.
// /*AFLA*/  *	Analogous to @suspend(), but it should not enable the device to signal
// /*AFLA*/  *	wakeup events or change its power state.  The majority of subsystems
// /*AFLA*/  *	(with the notable exception of the PCI bus type) expect the driver-level
// /*AFLA*/  *	@freeze() to save the device settings in memory to be used by @restore()
// /*AFLA*/  *	during the subsequent resume from hibernation.
// /*AFLA*/  *	Subsystem-level @freeze() is executed for all devices after invoking
// /*AFLA*/  *	subsystem-level @prepare() for all of them.
// /*AFLA*/  *
// /*AFLA*/  * @freeze_late: Continue operations started by @freeze().  Analogous to
// /*AFLA*/  *	@suspend_late(), but it should not enable the device to signal wakeup
// /*AFLA*/  *	events or change its power state.
// /*AFLA*/  *
// /*AFLA*/  * @thaw: Hibernation-specific, executed after creating a hibernation image OR
// /*AFLA*/  *	if the creation of an image has failed.  Also executed after a failing
// /*AFLA*/  *	attempt to restore the contents of main memory from such an image.
// /*AFLA*/  *	Undo the changes made by the preceding @freeze(), so the device can be
// /*AFLA*/  *	operated in the same way as immediately before the call to @freeze().
// /*AFLA*/  *	Subsystem-level @thaw() is executed for all devices after invoking
// /*AFLA*/  *	subsystem-level @thaw_noirq() for all of them.  It also may be executed
// /*AFLA*/  *	directly after @freeze() in case of a transition error.
// /*AFLA*/  *
// /*AFLA*/  * @thaw_early: Prepare to execute @thaw().  Undo the changes made by the
// /*AFLA*/  *	preceding @freeze_late().
// /*AFLA*/  *
// /*AFLA*/  * @poweroff: Hibernation-specific, executed after saving a hibernation image.
// /*AFLA*/  *	Analogous to @suspend(), but it need not save the device's settings in
// /*AFLA*/  *	memory.
// /*AFLA*/  *	Subsystem-level @poweroff() is executed for all devices after invoking
// /*AFLA*/  *	subsystem-level @prepare() for all of them.
// /*AFLA*/  *
// /*AFLA*/  * @poweroff_late: Continue operations started by @poweroff().  Analogous to
// /*AFLA*/  *	@suspend_late(), but it need not save the device's settings in memory.
// /*AFLA*/  *
// /*AFLA*/  * @restore: Hibernation-specific, executed after restoring the contents of main
// /*AFLA*/  *	memory from a hibernation image, analogous to @resume().
// /*AFLA*/  *
// /*AFLA*/  * @restore_early: Prepare to execute @restore(), analogous to @resume_early().
// /*AFLA*/  *
// /*AFLA*/  * @suspend_noirq: Complete the actions started by @suspend().  Carry out any
// /*AFLA*/  *	additional operations required for suspending the device that might be
// /*AFLA*/  *	racing with its driver's interrupt handler, which is guaranteed not to
// /*AFLA*/  *	run while @suspend_noirq() is being executed.
// /*AFLA*/  *	It generally is expected that the device will be in a low-power state
// /*AFLA*/  *	(appropriate for the target system sleep state) after subsystem-level
// /*AFLA*/  *	@suspend_noirq() has returned successfully.  If the device can generate
// /*AFLA*/  *	system wakeup signals and is enabled to wake up the system, it should be
// /*AFLA*/  *	configured to do so at that time.  However, depending on the platform
// /*AFLA*/  *	and device's subsystem, @suspend() or @suspend_late() may be allowed to
// /*AFLA*/  *	put the device into the low-power state and configure it to generate
// /*AFLA*/  *	wakeup signals, in which case it generally is not necessary to define
// /*AFLA*/  *	@suspend_noirq().
// /*AFLA*/  *
// /*AFLA*/  * @resume_noirq: Prepare for the execution of @resume() by carrying out any
// /*AFLA*/  *	operations required for resuming the device that might be racing with
// /*AFLA*/  *	its driver's interrupt handler, which is guaranteed not to run while
// /*AFLA*/  *	@resume_noirq() is being executed.
// /*AFLA*/  *
// /*AFLA*/  * @freeze_noirq: Complete the actions started by @freeze().  Carry out any
// /*AFLA*/  *	additional operations required for freezing the device that might be
// /*AFLA*/  *	racing with its driver's interrupt handler, which is guaranteed not to
// /*AFLA*/  *	run while @freeze_noirq() is being executed.
// /*AFLA*/  *	The power state of the device should not be changed by either @freeze(),
// /*AFLA*/  *	or @freeze_late(), or @freeze_noirq() and it should not be configured to
// /*AFLA*/  *	signal system wakeup by any of these callbacks.
// /*AFLA*/  *
// /*AFLA*/  * @thaw_noirq: Prepare for the execution of @thaw() by carrying out any
// /*AFLA*/  *	operations required for thawing the device that might be racing with its
// /*AFLA*/  *	driver's interrupt handler, which is guaranteed not to run while
// /*AFLA*/  *	@thaw_noirq() is being executed.
// /*AFLA*/  *
// /*AFLA*/  * @poweroff_noirq: Complete the actions started by @poweroff().  Analogous to
// /*AFLA*/  *	@suspend_noirq(), but it need not save the device's settings in memory.
// /*AFLA*/  *
// /*AFLA*/  * @restore_noirq: Prepare for the execution of @restore() by carrying out any
// /*AFLA*/  *	operations required for thawing the device that might be racing with its
// /*AFLA*/  *	driver's interrupt handler, which is guaranteed not to run while
// /*AFLA*/  *	@restore_noirq() is being executed.  Analogous to @resume_noirq().
// /*AFLA*/  *
// /*AFLA*/  * All of the above callbacks, except for @complete(), return error codes.
// /*AFLA*/  * However, the error codes returned by the resume operations, @resume(),
// /*AFLA*/  * @thaw(), @restore(), @resume_noirq(), @thaw_noirq(), and @restore_noirq(), do
// /*AFLA*/  * not cause the PM core to abort the resume transition during which they are
// /*AFLA*/  * returned.  The error codes returned in those cases are only printed by the PM
// /*AFLA*/  * core to the system logs for debugging purposes.  Still, it is recommended
// /*AFLA*/  * that drivers only return error codes from their resume methods in case of an
// /*AFLA*/  * unrecoverable failure (i.e. when the device being handled refuses to resume
// /*AFLA*/  * and becomes unusable) to allow us to modify the PM core in the future, so
// /*AFLA*/  * that it can avoid attempting to handle devices that failed to resume and
// /*AFLA*/  * their children.
// /*AFLA*/  *
// /*AFLA*/  * It is allowed to unregister devices while the above callbacks are being
// /*AFLA*/  * executed.  However, a callback routine must NOT try to unregister the device
// /*AFLA*/  * it was called for, although it may unregister children of that device (for
// /*AFLA*/  * example, if it detects that a child was unplugged while the system was
// /*AFLA*/  * asleep).
// /*AFLA*/  *
// /*AFLA*/  * Refer to Documentation/power/devices.txt for more information about the role
// /*AFLA*/  * of the above callbacks in the system suspend process.
// /*AFLA*/  *
// /*AFLA*/  * There also are callbacks related to runtime power management of devices.
// /*AFLA*/  * Again, these callbacks are executed by the PM core only for subsystems
// /*AFLA*/  * (PM domains, device types, classes and bus types) and the subsystem-level
// /*AFLA*/  * callbacks are supposed to invoke the driver callbacks.  Moreover, the exact
// /*AFLA*/  * actions to be performed by a device driver's callbacks generally depend on
// /*AFLA*/  * the platform and subsystem the device belongs to.
// /*AFLA*/  *
// /*AFLA*/  * @runtime_suspend: Prepare the device for a condition in which it won't be
// /*AFLA*/  *	able to communicate with the CPU(s) and RAM due to power management.
// /*AFLA*/  *	This need not mean that the device should be put into a low-power state.
// /*AFLA*/  *	For example, if the device is behind a link which is about to be turned
// /*AFLA*/  *	off, the device may remain at full power.  If the device does go to low
// /*AFLA*/  *	power and is capable of generating runtime wakeup events, remote wakeup
// /*AFLA*/  *	(i.e., a hardware mechanism allowing the device to request a change of
// /*AFLA*/  *	its power state via an interrupt) should be enabled for it.
// /*AFLA*/  *
// /*AFLA*/  * @runtime_resume: Put the device into the fully active state in response to a
// /*AFLA*/  *	wakeup event generated by hardware or at the request of software.  If
// /*AFLA*/  *	necessary, put the device into the full-power state and restore its
// /*AFLA*/  *	registers, so that it is fully operational.
// /*AFLA*/  *
// /*AFLA*/  * @runtime_idle: Device appears to be inactive and it might be put into a
// /*AFLA*/  *	low-power state if all of the necessary conditions are satisfied.
// /*AFLA*/  *	Check these conditions, and return 0 if it's appropriate to let the PM
// /*AFLA*/  *	core queue a suspend request for the device.
// /*AFLA*/  *
// /*AFLA*/  * Refer to Documentation/power/runtime_pm.txt for more information about the
// /*AFLA*/  * role of the above callbacks in device runtime power management.
// /*AFLA*/  *
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ struct dev_pm_ops {
// /*AFLA*/ 	int (*prepare)(struct device *dev);
// /*AFLA*/ 	void (*complete)(struct device *dev);
// /*AFLA*/ 	int (*suspend)(struct device *dev);
// /*AFLA*/ 	int (*resume)(struct device *dev);
// /*AFLA*/ 	int (*freeze)(struct device *dev);
// /*AFLA*/ 	int (*thaw)(struct device *dev);
// /*AFLA*/ 	int (*poweroff)(struct device *dev);
// /*AFLA*/ 	int (*restore)(struct device *dev);
// /*AFLA*/ 	int (*suspend_late)(struct device *dev);
// /*AFLA*/ 	int (*resume_early)(struct device *dev);
// /*AFLA*/ 	int (*freeze_late)(struct device *dev);
// /*AFLA*/ 	int (*thaw_early)(struct device *dev);
// /*AFLA*/ 	int (*poweroff_late)(struct device *dev);
// /*AFLA*/ 	int (*restore_early)(struct device *dev);
// /*AFLA*/ 	int (*suspend_noirq)(struct device *dev);
// /*AFLA*/ 	int (*resume_noirq)(struct device *dev);
// /*AFLA*/ 	int (*freeze_noirq)(struct device *dev);
// /*AFLA*/ 	int (*thaw_noirq)(struct device *dev);
// /*AFLA*/ 	int (*poweroff_noirq)(struct device *dev);
// /*AFLA*/ 	int (*restore_noirq)(struct device *dev);
// /*AFLA*/ 	int (*runtime_suspend)(struct device *dev);
// /*AFLA*/ 	int (*runtime_resume)(struct device *dev);
// /*AFLA*/ 	int (*runtime_idle)(struct device *dev);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PM_SLEEP
// /*AFLA*/ #define SET_SYSTEM_SLEEP_PM_OPS(suspend_fn, resume_fn) \
// /*AFLA*/ 	.suspend = suspend_fn, \
// /*AFLA*/ 	.resume = resume_fn, \
// /*AFLA*/ 	.freeze = suspend_fn, \
// /*AFLA*/ 	.thaw = resume_fn, \
// /*AFLA*/ 	.poweroff = suspend_fn, \
// /*AFLA*/ 	.restore = resume_fn,
// /*AFLA*/ #else
// /*AFLA*/ #define SET_SYSTEM_SLEEP_PM_OPS(suspend_fn, resume_fn)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PM_SLEEP
// /*AFLA*/ #define SET_LATE_SYSTEM_SLEEP_PM_OPS(suspend_fn, resume_fn) \
// /*AFLA*/ 	.suspend_late = suspend_fn, \
// /*AFLA*/ 	.resume_early = resume_fn, \
// /*AFLA*/ 	.freeze_late = suspend_fn, \
// /*AFLA*/ 	.thaw_early = resume_fn, \
// /*AFLA*/ 	.poweroff_late = suspend_fn, \
// /*AFLA*/ 	.restore_early = resume_fn,
// /*AFLA*/ #else
// /*AFLA*/ #define SET_LATE_SYSTEM_SLEEP_PM_OPS(suspend_fn, resume_fn)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PM_SLEEP
// /*AFLA*/ #define SET_NOIRQ_SYSTEM_SLEEP_PM_OPS(suspend_fn, resume_fn) \
// /*AFLA*/ 	.suspend_noirq = suspend_fn, \
// /*AFLA*/ 	.resume_noirq = resume_fn, \
// /*AFLA*/ 	.freeze_noirq = suspend_fn, \
// /*AFLA*/ 	.thaw_noirq = resume_fn, \
// /*AFLA*/ 	.poweroff_noirq = suspend_fn, \
// /*AFLA*/ 	.restore_noirq = resume_fn,
// /*AFLA*/ #else
// /*AFLA*/ #define SET_NOIRQ_SYSTEM_SLEEP_PM_OPS(suspend_fn, resume_fn)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PM
// /*AFLA*/ #define SET_RUNTIME_PM_OPS(suspend_fn, resume_fn, idle_fn) \
// /*AFLA*/ 	.runtime_suspend = suspend_fn, \
// /*AFLA*/ 	.runtime_resume = resume_fn, \
// /*AFLA*/ 	.runtime_idle = idle_fn,
// /*AFLA*/ #else
// /*AFLA*/ #define SET_RUNTIME_PM_OPS(suspend_fn, resume_fn, idle_fn)
// /*AFLA*/ #endif
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Use this if you want to use the same suspend and resume callbacks for suspend
// /*AFLA*/  * to RAM and hibernation.
// /*AFLA*/  */
// /*AFLA*/ #define SIMPLE_DEV_PM_OPS(name, suspend_fn, resume_fn) \
// /*AFLA*/ const struct dev_pm_ops name = { \
// /*AFLA*/ 	SET_SYSTEM_SLEEP_PM_OPS(suspend_fn, resume_fn) \
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Use this for defining a set of PM operations to be used in all situations
// /*AFLA*/  * (system suspend, hibernation or runtime PM).
// /*AFLA*/  * NOTE: In general, system suspend callbacks, .suspend() and .resume(), should
// /*AFLA*/  * be different from the corresponding runtime PM callbacks, .runtime_suspend(),
// /*AFLA*/  * and .runtime_resume(), because .runtime_suspend() always works on an already
// /*AFLA*/  * quiescent device, while .suspend() should assume that the device may be doing
// /*AFLA*/  * something when it is called (it should ensure that the device will be
// /*AFLA*/  * quiescent after it has returned).  Therefore it's better to point the "late"
// /*AFLA*/  * suspend and "early" resume callback pointers, .suspend_late() and
// /*AFLA*/  * .resume_early(), to the same routines as .runtime_suspend() and
// /*AFLA*/  * .runtime_resume(), respectively (and analogously for hibernation).
// /*AFLA*/  */
// /*AFLA*/ #define UNIVERSAL_DEV_PM_OPS(name, suspend_fn, resume_fn, idle_fn) \
// /*AFLA*/ const struct dev_pm_ops name = { \
// /*AFLA*/ 	SET_SYSTEM_SLEEP_PM_OPS(suspend_fn, resume_fn) \
// /*AFLA*/ 	SET_RUNTIME_PM_OPS(suspend_fn, resume_fn, idle_fn) \
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * PM_EVENT_ messages
// /*AFLA*/  *
// /*AFLA*/  * The following PM_EVENT_ messages are defined for the internal use of the PM
// /*AFLA*/  * core, in order to provide a mechanism allowing the high level suspend and
// /*AFLA*/  * hibernation code to convey the necessary information to the device PM core
// /*AFLA*/  * code:
// /*AFLA*/  *
// /*AFLA*/  * ON		No transition.
// /*AFLA*/  *
// /*AFLA*/  * FREEZE	System is going to hibernate, call ->prepare() and ->freeze()
// /*AFLA*/  *		for all devices.
// /*AFLA*/  *
// /*AFLA*/  * SUSPEND	System is going to suspend, call ->prepare() and ->suspend()
// /*AFLA*/  *		for all devices.
// /*AFLA*/  *
// /*AFLA*/  * HIBERNATE	Hibernation image has been saved, call ->prepare() and
// /*AFLA*/  *		->poweroff() for all devices.
// /*AFLA*/  *
// /*AFLA*/  * QUIESCE	Contents of main memory are going to be restored from a (loaded)
// /*AFLA*/  *		hibernation image, call ->prepare() and ->freeze() for all
// /*AFLA*/  *		devices.
// /*AFLA*/  *
// /*AFLA*/  * RESUME	System is resuming, call ->resume() and ->complete() for all
// /*AFLA*/  *		devices.
// /*AFLA*/  *
// /*AFLA*/  * THAW		Hibernation image has been created, call ->thaw() and
// /*AFLA*/  *		->complete() for all devices.
// /*AFLA*/  *
// /*AFLA*/  * RESTORE	Contents of main memory have been restored from a hibernation
// /*AFLA*/  *		image, call ->restore() and ->complete() for all devices.
// /*AFLA*/  *
// /*AFLA*/  * RECOVER	Creation of a hibernation image or restoration of the main
// /*AFLA*/  *		memory contents from a hibernation image has failed, call
// /*AFLA*/  *		->thaw() and ->complete() for all devices.
// /*AFLA*/  *
// /*AFLA*/  * The following PM_EVENT_ messages are defined for internal use by
// /*AFLA*/  * kernel subsystems.  They are never issued by the PM core.
// /*AFLA*/  *
// /*AFLA*/  * USER_SUSPEND		Manual selective suspend was issued by userspace.
// /*AFLA*/  *
// /*AFLA*/  * USER_RESUME		Manual selective resume was issued by userspace.
// /*AFLA*/  *
// /*AFLA*/  * REMOTE_WAKEUP	Remote-wakeup request was received from the device.
// /*AFLA*/  *
// /*AFLA*/  * AUTO_SUSPEND		Automatic (device idle) runtime suspend was
// /*AFLA*/  *			initiated by the subsystem.
// /*AFLA*/  *
// /*AFLA*/  * AUTO_RESUME		Automatic (device needed) runtime resume was
// /*AFLA*/  *			requested by a driver.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #define PM_EVENT_INVALID	(-1)
// /*AFLA*/ #define PM_EVENT_ON		0x0000
// /*AFLA*/ #define PM_EVENT_FREEZE		0x0001
// /*AFLA*/ #define PM_EVENT_SUSPEND	0x0002
// /*AFLA*/ #define PM_EVENT_HIBERNATE	0x0004
// /*AFLA*/ #define PM_EVENT_QUIESCE	0x0008
// /*AFLA*/ #define PM_EVENT_RESUME		0x0010
// /*AFLA*/ #define PM_EVENT_THAW		0x0020
// /*AFLA*/ #define PM_EVENT_RESTORE	0x0040
// /*AFLA*/ #define PM_EVENT_RECOVER	0x0080
// /*AFLA*/ #define PM_EVENT_USER		0x0100
// /*AFLA*/ #define PM_EVENT_REMOTE		0x0200
// /*AFLA*/ #define PM_EVENT_AUTO		0x0400
// /*AFLA*/ 
// /*AFLA*/ #define PM_EVENT_SLEEP		(PM_EVENT_SUSPEND | PM_EVENT_HIBERNATE)
// /*AFLA*/ #define PM_EVENT_USER_SUSPEND	(PM_EVENT_USER | PM_EVENT_SUSPEND)
// /*AFLA*/ #define PM_EVENT_USER_RESUME	(PM_EVENT_USER | PM_EVENT_RESUME)
// /*AFLA*/ #define PM_EVENT_REMOTE_RESUME	(PM_EVENT_REMOTE | PM_EVENT_RESUME)
// /*AFLA*/ #define PM_EVENT_AUTO_SUSPEND	(PM_EVENT_AUTO | PM_EVENT_SUSPEND)
// /*AFLA*/ #define PM_EVENT_AUTO_RESUME	(PM_EVENT_AUTO | PM_EVENT_RESUME)
// /*AFLA*/ 
// /*AFLA*/ #define PMSG_INVALID	((struct pm_message){ .event = PM_EVENT_INVALID, })
// /*AFLA*/ #define PMSG_ON		((struct pm_message){ .event = PM_EVENT_ON, })
// /*AFLA*/ #define PMSG_FREEZE	((struct pm_message){ .event = PM_EVENT_FREEZE, })
// /*AFLA*/ #define PMSG_QUIESCE	((struct pm_message){ .event = PM_EVENT_QUIESCE, })
// /*AFLA*/ #define PMSG_SUSPEND	((struct pm_message){ .event = PM_EVENT_SUSPEND, })
// /*AFLA*/ #define PMSG_HIBERNATE	((struct pm_message){ .event = PM_EVENT_HIBERNATE, })
// /*AFLA*/ #define PMSG_RESUME	((struct pm_message){ .event = PM_EVENT_RESUME, })
// /*AFLA*/ #define PMSG_THAW	((struct pm_message){ .event = PM_EVENT_THAW, })
// /*AFLA*/ #define PMSG_RESTORE	((struct pm_message){ .event = PM_EVENT_RESTORE, })
// /*AFLA*/ #define PMSG_RECOVER	((struct pm_message){ .event = PM_EVENT_RECOVER, })
// /*AFLA*/ #define PMSG_USER_SUSPEND	((struct pm_message) \
// /*AFLA*/ 					{ .event = PM_EVENT_USER_SUSPEND, })
// /*AFLA*/ #define PMSG_USER_RESUME	((struct pm_message) \
// /*AFLA*/ 					{ .event = PM_EVENT_USER_RESUME, })
// /*AFLA*/ #define PMSG_REMOTE_RESUME	((struct pm_message) \
// /*AFLA*/ 					{ .event = PM_EVENT_REMOTE_RESUME, })
// /*AFLA*/ #define PMSG_AUTO_SUSPEND	((struct pm_message) \
// /*AFLA*/ 					{ .event = PM_EVENT_AUTO_SUSPEND, })
// /*AFLA*/ #define PMSG_AUTO_RESUME	((struct pm_message) \
// /*AFLA*/ 					{ .event = PM_EVENT_AUTO_RESUME, })
// /*AFLA*/ 
// /*AFLA*/ #define PMSG_IS_AUTO(msg)	(((msg).event & PM_EVENT_AUTO) != 0)
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * Device run-time power management status.
// /*AFLA*/  *
// /*AFLA*/  * These status labels are used internally by the PM core to indicate the
// /*AFLA*/  * current status of a device with respect to the PM core operations.  They do
// /*AFLA*/  * not reflect the actual power state of the device or its status as seen by the
// /*AFLA*/  * driver.
// /*AFLA*/  *
// /*AFLA*/  * RPM_ACTIVE		Device is fully operational.  Indicates that the device
// /*AFLA*/  *			bus type's ->runtime_resume() callback has completed
// /*AFLA*/  *			successfully.
// /*AFLA*/  *
// /*AFLA*/  * RPM_SUSPENDED	Device bus type's ->runtime_suspend() callback has
// /*AFLA*/  *			completed successfully.  The device is regarded as
// /*AFLA*/  *			suspended.
// /*AFLA*/  *
// /*AFLA*/  * RPM_RESUMING		Device bus type's ->runtime_resume() callback is being
// /*AFLA*/  *			executed.
// /*AFLA*/  *
// /*AFLA*/  * RPM_SUSPENDING	Device bus type's ->runtime_suspend() callback is being
// /*AFLA*/  *			executed.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ enum rpm_status {
// /*AFLA*/ 	RPM_ACTIVE = 0,
// /*AFLA*/ 	RPM_RESUMING,
// /*AFLA*/ 	RPM_SUSPENDED,
// /*AFLA*/ 	RPM_SUSPENDING,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * Device run-time power management request types.
// /*AFLA*/  *
// /*AFLA*/  * RPM_REQ_NONE		Do nothing.
// /*AFLA*/  *
// /*AFLA*/  * RPM_REQ_IDLE		Run the device bus type's ->runtime_idle() callback
// /*AFLA*/  *
// /*AFLA*/  * RPM_REQ_SUSPEND	Run the device bus type's ->runtime_suspend() callback
// /*AFLA*/  *
// /*AFLA*/  * RPM_REQ_AUTOSUSPEND	Same as RPM_REQ_SUSPEND, but not until the device has
// /*AFLA*/  *			been inactive for as long as power.autosuspend_delay
// /*AFLA*/  *
// /*AFLA*/  * RPM_REQ_RESUME	Run the device bus type's ->runtime_resume() callback
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ enum rpm_request {
// /*AFLA*/ 	RPM_REQ_NONE = 0,
// /*AFLA*/ 	RPM_REQ_IDLE,
// /*AFLA*/ 	RPM_REQ_SUSPEND,
// /*AFLA*/ 	RPM_REQ_AUTOSUSPEND,
// /*AFLA*/ 	RPM_REQ_RESUME,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct wakeup_source;
// /*AFLA*/ struct wake_irq;
// /*AFLA*/ struct pm_domain_data;
// /*AFLA*/ 
// /*AFLA*/ struct pm_subsys_data {
// /*AFLA*/ 	spinlock_t lock;
// /*AFLA*/ 	unsigned int refcount;
// /*AFLA*/ #ifdef CONFIG_PM_CLK
// /*AFLA*/ 	struct list_head clock_list;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_PM_GENERIC_DOMAINS
// /*AFLA*/ 	struct pm_domain_data *domain_data;
// /*AFLA*/ #endif
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ struct dev_pm_info {
// /*AFLA*/ 	pm_message_t		power_state;
// /*AFLA*/ 	unsigned int		can_wakeup:1;
// /*AFLA*/ 	unsigned int		async_suspend:1;
// /*AFLA*/ 	bool			is_prepared:1;	/* Owned by the PM core */
// /*AFLA*/ 	bool			is_suspended:1;	/* Ditto */
// /*AFLA*/ 	bool			is_noirq_suspended:1;
// /*AFLA*/ 	bool			is_late_suspended:1;
// /*AFLA*/ 	bool			early_init:1;	/* Owned by the PM core */
// /*AFLA*/ 	bool			direct_complete:1;	/* Owned by the PM core */
// /*AFLA*/ 	spinlock_t		lock;
// /*AFLA*/ #ifdef CONFIG_PM_SLEEP
// /*AFLA*/ 	struct list_head	entry;
// /*AFLA*/ 	struct completion	completion;
// /*AFLA*/ 	struct wakeup_source	*wakeup;
// /*AFLA*/ 	bool			wakeup_path:1;
// /*AFLA*/ 	bool			syscore:1;
// /*AFLA*/ 	bool			no_pm_callbacks:1;	/* Owned by the PM core */
// /*AFLA*/ #else
// /*AFLA*/ 	unsigned int		should_wakeup:1;
// /*AFLA*/ #endif
// /*AFLA*/ #ifdef CONFIG_PM
// /*AFLA*/ 	struct timer_list	suspend_timer;
// /*AFLA*/ 	unsigned long		timer_expires;
// /*AFLA*/ 	struct work_struct	work;
// /*AFLA*/ 	wait_queue_head_t	wait_queue;
// /*AFLA*/ 	struct wake_irq		*wakeirq;
// /*AFLA*/ 	atomic_t		usage_count;
// /*AFLA*/ 	atomic_t		child_count;
// /*AFLA*/ 	unsigned int		disable_depth:3;
// /*AFLA*/ 	unsigned int		idle_notification:1;
// /*AFLA*/ 	unsigned int		request_pending:1;
// /*AFLA*/ 	unsigned int		deferred_resume:1;
// /*AFLA*/ 	unsigned int		run_wake:1;
// /*AFLA*/ 	unsigned int		runtime_auto:1;
// /*AFLA*/ 	bool			ignore_children:1;
// /*AFLA*/ 	unsigned int		no_callbacks:1;
// /*AFLA*/ 	unsigned int		irq_safe:1;
// /*AFLA*/ 	unsigned int		use_autosuspend:1;
// /*AFLA*/ 	unsigned int		timer_autosuspends:1;
// /*AFLA*/ 	unsigned int		memalloc_noio:1;
// /*AFLA*/ 	enum rpm_request	request;
// /*AFLA*/ 	enum rpm_status		runtime_status;
// /*AFLA*/ 	int			runtime_error;
// /*AFLA*/ 	int			autosuspend_delay;
// /*AFLA*/ 	unsigned long		last_busy;
// /*AFLA*/ 	unsigned long		active_jiffies;
// /*AFLA*/ 	unsigned long		suspended_jiffies;
// /*AFLA*/ 	unsigned long		accounting_timestamp;
// /*AFLA*/ #endif
// /*AFLA*/ 	struct pm_subsys_data	*subsys_data;  /* Owned by the subsystem. */
// /*AFLA*/ 	void (*set_latency_tolerance)(struct device *, s32);
// /*AFLA*/ 	struct dev_pm_qos	*qos;
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ extern void update_pm_runtime_accounting(struct device *dev);
// /*AFLA*/ extern int dev_pm_get_subsys_data(struct device *dev);
// /*AFLA*/ extern void dev_pm_put_subsys_data(struct device *dev);
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * Power domains provide callbacks that are executed during system suspend,
// /*AFLA*/  * hibernation, system resume and during runtime PM transitions along with
// /*AFLA*/  * subsystem-level and driver-level callbacks.
// /*AFLA*/  *
// /*AFLA*/  * @detach: Called when removing a device from the domain.
// /*AFLA*/  * @activate: Called before executing probe routines for bus types and drivers.
// /*AFLA*/  * @sync: Called after successful driver probe.
// /*AFLA*/  * @dismiss: Called after unsuccessful driver probe and after driver removal.
// /*AFLA*/  */
// /*AFLA*/ struct dev_pm_domain {
// /*AFLA*/ 	struct dev_pm_ops	ops;
// /*AFLA*/ 	void (*detach)(struct device *dev, bool power_off);
// /*AFLA*/ 	int (*activate)(struct device *dev);
// /*AFLA*/ 	void (*sync)(struct device *dev);
// /*AFLA*/ 	void (*dismiss)(struct device *dev);
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * The PM_EVENT_ messages are also used by drivers implementing the legacy
// /*AFLA*/  * suspend framework, based on the ->suspend() and ->resume() callbacks common
// /*AFLA*/  * for suspend and hibernation transitions, according to the rules below.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /* Necessary, because several drivers use PM_EVENT_PRETHAW */
// /*AFLA*/ #define PM_EVENT_PRETHAW PM_EVENT_QUIESCE
// /*AFLA*/ 
// /*AFLA*/ /*
// /*AFLA*/  * One transition is triggered by resume(), after a suspend() call; the
// /*AFLA*/  * message is implicit:
// /*AFLA*/  *
// /*AFLA*/  * ON		Driver starts working again, responding to hardware events
// /*AFLA*/  *		and software requests.  The hardware may have gone through
// /*AFLA*/  *		a power-off reset, or it may have maintained state from the
// /*AFLA*/  *		previous suspend() which the driver will rely on while
// /*AFLA*/  *		resuming.  On most platforms, there are no restrictions on
// /*AFLA*/  *		availability of resources like clocks during resume().
// /*AFLA*/  *
// /*AFLA*/  * Other transitions are triggered by messages sent using suspend().  All
// /*AFLA*/  * these transitions quiesce the driver, so that I/O queues are inactive.
// /*AFLA*/  * That commonly entails turning off IRQs and DMA; there may be rules
// /*AFLA*/  * about how to quiesce that are specific to the bus or the device's type.
// /*AFLA*/  * (For example, network drivers mark the link state.)  Other details may
// /*AFLA*/  * differ according to the message:
// /*AFLA*/  *
// /*AFLA*/  * SUSPEND	Quiesce, enter a low power device state appropriate for
// /*AFLA*/  *		the upcoming system state (such as PCI_D3hot), and enable
// /*AFLA*/  *		wakeup events as appropriate.
// /*AFLA*/  *
// /*AFLA*/  * HIBERNATE	Enter a low power device state appropriate for the hibernation
// /*AFLA*/  *		state (eg. ACPI S4) and enable wakeup events as appropriate.
// /*AFLA*/  *
// /*AFLA*/  * FREEZE	Quiesce operations so that a consistent image can be saved;
// /*AFLA*/  *		but do NOT otherwise enter a low power device state, and do
// /*AFLA*/  *		NOT emit system wakeup events.
// /*AFLA*/  *
// /*AFLA*/  * PRETHAW	Quiesce as if for FREEZE; additionally, prepare for restoring
// /*AFLA*/  *		the system from a snapshot taken after an earlier FREEZE.
// /*AFLA*/  *		Some drivers will need to reset their hardware state instead
// /*AFLA*/  *		of preserving it, to ensure that it's never mistaken for the
// /*AFLA*/  *		state which that earlier snapshot had set up.
// /*AFLA*/  *
// /*AFLA*/  * A minimally power-aware driver treats all messages as SUSPEND, fully
// /*AFLA*/  * reinitializes its device during resume() -- whether or not it was reset
// /*AFLA*/  * during the suspend/resume cycle -- and can't issue wakeup events.
// /*AFLA*/  *
// /*AFLA*/  * More power-aware drivers may also use low power states at runtime as
// /*AFLA*/  * well as during system sleep states like PM_SUSPEND_STANDBY.  They may
// /*AFLA*/  * be able to use wakeup events to exit from runtime low-power states,
// /*AFLA*/  * or from system low-power states such as standby or suspend-to-RAM.
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifdef CONFIG_PM_SLEEP
// /*AFLA*/ extern void device_pm_lock(void);
// /*AFLA*/ extern void dpm_resume_start(pm_message_t state);
// /*AFLA*/ extern void dpm_resume_end(pm_message_t state);
// /*AFLA*/ extern void dpm_resume_noirq(pm_message_t state);
// /*AFLA*/ extern void dpm_resume_early(pm_message_t state);
// /*AFLA*/ extern void dpm_resume(pm_message_t state);
// /*AFLA*/ extern void dpm_complete(pm_message_t state);
// /*AFLA*/ 
// /*AFLA*/ extern void device_pm_unlock(void);
// /*AFLA*/ extern int dpm_suspend_end(pm_message_t state);
// /*AFLA*/ extern int dpm_suspend_start(pm_message_t state);
// /*AFLA*/ extern int dpm_suspend_noirq(pm_message_t state);
// /*AFLA*/ extern int dpm_suspend_late(pm_message_t state);
// /*AFLA*/ extern int dpm_suspend(pm_message_t state);
// /*AFLA*/ extern int dpm_prepare(pm_message_t state);
// /*AFLA*/ 
// /*AFLA*/ extern void __suspend_report_result(const char *function, void *fn, int ret);
// /*AFLA*/ 
// /*AFLA*/ #define suspend_report_result(fn, ret)					\
// /*AFLA*/ 	do {								\
// /*AFLA*/ 		__suspend_report_result(__func__, fn, ret);		\
// /*AFLA*/ 	} while (0)
// /*AFLA*/ 
// /*AFLA*/ extern int device_pm_wait_for_dev(struct device *sub, struct device *dev);
// /*AFLA*/ extern void dpm_for_each_dev(void *data, void (*fn)(struct device *, void *));
// /*AFLA*/ 
// /*AFLA*/ extern int pm_generic_prepare(struct device *dev);
// /*AFLA*/ extern int pm_generic_suspend_late(struct device *dev);
// /*AFLA*/ extern int pm_generic_suspend_noirq(struct device *dev);
// /*AFLA*/ extern int pm_generic_suspend(struct device *dev);
// /*AFLA*/ extern int pm_generic_resume_early(struct device *dev);
// /*AFLA*/ extern int pm_generic_resume_noirq(struct device *dev);
// /*AFLA*/ extern int pm_generic_resume(struct device *dev);
// /*AFLA*/ extern int pm_generic_freeze_noirq(struct device *dev);
// /*AFLA*/ extern int pm_generic_freeze_late(struct device *dev);
// /*AFLA*/ extern int pm_generic_freeze(struct device *dev);
// /*AFLA*/ extern int pm_generic_thaw_noirq(struct device *dev);
// /*AFLA*/ extern int pm_generic_thaw_early(struct device *dev);
// /*AFLA*/ extern int pm_generic_thaw(struct device *dev);
// /*AFLA*/ extern int pm_generic_restore_noirq(struct device *dev);
// /*AFLA*/ extern int pm_generic_restore_early(struct device *dev);
// /*AFLA*/ extern int pm_generic_restore(struct device *dev);
// /*AFLA*/ extern int pm_generic_poweroff_noirq(struct device *dev);
// /*AFLA*/ extern int pm_generic_poweroff_late(struct device *dev);
// /*AFLA*/ extern int pm_generic_poweroff(struct device *dev);
// /*AFLA*/ extern void pm_generic_complete(struct device *dev);
// /*AFLA*/ extern void pm_complete_with_resume_check(struct device *dev);
// /*AFLA*/ 
// /*AFLA*/ #else /* !CONFIG_PM_SLEEP */
// /*AFLA*/ 
// /*AFLA*/ #define device_pm_lock() do {} while (0)
// /*AFLA*/ #define device_pm_unlock() do {} while (0)
// /*AFLA*/ 
// /*AFLA*/ static inline int dpm_suspend_start(pm_message_t state)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define suspend_report_result(fn, ret)		do {} while (0)
// /*AFLA*/ 
// /*AFLA*/ static inline int device_pm_wait_for_dev(struct device *a, struct device *b)
// /*AFLA*/ {
// /*AFLA*/ 	return 0;
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ static inline void dpm_for_each_dev(void *data, void (*fn)(struct device *, void *))
// /*AFLA*/ {
// /*AFLA*/ }
// /*AFLA*/ 
// /*AFLA*/ #define pm_generic_prepare		NULL
// /*AFLA*/ #define pm_generic_suspend_late		NULL
// /*AFLA*/ #define pm_generic_suspend_noirq	NULL
// /*AFLA*/ #define pm_generic_suspend		NULL
// /*AFLA*/ #define pm_generic_resume_early		NULL
// /*AFLA*/ #define pm_generic_resume_noirq		NULL
// /*AFLA*/ #define pm_generic_resume		NULL
// /*AFLA*/ #define pm_generic_freeze_noirq		NULL
// /*AFLA*/ #define pm_generic_freeze_late		NULL
// /*AFLA*/ #define pm_generic_freeze		NULL
// /*AFLA*/ #define pm_generic_thaw_noirq		NULL
// /*AFLA*/ #define pm_generic_thaw_early		NULL
// /*AFLA*/ #define pm_generic_thaw			NULL
// /*AFLA*/ #define pm_generic_restore_noirq	NULL
// /*AFLA*/ #define pm_generic_restore_early	NULL
// /*AFLA*/ #define pm_generic_restore		NULL
// /*AFLA*/ #define pm_generic_poweroff_noirq	NULL
// /*AFLA*/ #define pm_generic_poweroff_late	NULL
// /*AFLA*/ #define pm_generic_poweroff		NULL
// /*AFLA*/ #define pm_generic_complete		NULL
// /*AFLA*/ #endif /* !CONFIG_PM_SLEEP */
// /*AFLA*/ 
// /*AFLA*/ /* How to reorder dpm_list after device_move() */
// /*AFLA*/ enum dpm_order {
// /*AFLA*/ 	DPM_ORDER_NONE,
// /*AFLA*/ 	DPM_ORDER_DEV_AFTER_PARENT,
// /*AFLA*/ 	DPM_ORDER_PARENT_BEFORE_DEV,
// /*AFLA*/ 	DPM_ORDER_DEV_LAST,
// /*AFLA*/ };
// /*AFLA*/ 
// /*AFLA*/ #endif /* _LINUX_PM_H */
