// /*AFLA*/ /*
// /*AFLA*/  * Standard pin control state definitions
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ /**
// /*AFLA*/  * @PINCTRL_STATE_DEFAULT: the state the pinctrl handle shall be put
// /*AFLA*/  *	into as default, usually this means the pins are up and ready to
// /*AFLA*/  *	be used by the device driver. This state is commonly used by
// /*AFLA*/  *	hogs to configure muxing and pins at boot, and also as a state
// /*AFLA*/  *	to go into when returning from sleep and idle in
// /*AFLA*/  *	.pm_runtime_resume() or ordinary .resume() for example.
// /*AFLA*/  * @PINCTRL_STATE_INIT: normally the pinctrl will be set to "default"
// /*AFLA*/  *	before the driver's probe() function is called.  There are some
// /*AFLA*/  *	drivers where that is not appropriate becausing doing so would
// /*AFLA*/  *	glitch the pins.  In those cases you can add an "init" pinctrl
// /*AFLA*/  *	which is the state of the pins before drive probe.  After probe
// /*AFLA*/  *	if the pins are still in "init" state they'll be moved to
// /*AFLA*/  *	"default".
// /*AFLA*/  * @PINCTRL_STATE_IDLE: the state the pinctrl handle shall be put into
// /*AFLA*/  *	when the pins are idle. This is a state where the system is relaxed
// /*AFLA*/  *	but not fully sleeping - some power may be on but clocks gated for
// /*AFLA*/  *	example. Could typically be set from a pm_runtime_suspend() or
// /*AFLA*/  *	pm_runtime_idle() operation.
// /*AFLA*/  * @PINCTRL_STATE_SLEEP: the state the pinctrl handle shall be put into
// /*AFLA*/  *	when the pins are sleeping. This is a state where the system is in
// /*AFLA*/  *	its lowest sleep state. Could typically be set from an
// /*AFLA*/  *	ordinary .suspend() function.
// /*AFLA*/  */
// /*AFLA*/ #define PINCTRL_STATE_DEFAULT "default"
// /*AFLA*/ #define PINCTRL_STATE_INIT "init"
// /*AFLA*/ #define PINCTRL_STATE_IDLE "idle"
// /*AFLA*/ #define PINCTRL_STATE_SLEEP "sleep"
