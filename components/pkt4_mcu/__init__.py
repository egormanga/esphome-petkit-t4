import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import uart
from esphome.const import (
	CONF_DIRECTION,
	CONF_DURATION,
	CONF_ID,
	CONF_MODE,
	CONF_SPEED,
	CONF_TIMEOUT,
)


DEPENDENCIES = ('uart',)

pkt4_mcu_ns = cg.esphome_ns.namespace('pkt4_mcu')
PKT4MCUComponent = pkt4_mcu_ns.class_('PKT4MCUComponent', cg.Component, uart.UARTDevice)
InitAction = pkt4_mcu_ns.class_('InitAction', automation.Action)
DeinitAction = pkt4_mcu_ns.class_('DeinitAction', automation.Action)
MotorAction = pkt4_mcu_ns.class_('MotorAction', automation.Action)

CONF_MOTOR = 'motor'

CONFIG_SCHEMA = (
	cv.Schema({
		cv.GenerateID(): cv.declare_id(PKT4MCUComponent),
	})
	.extend(cv.COMPONENT_SCHEMA)
	.extend(uart.UART_DEVICE_SCHEMA)
)


async def to_code(config):
	var = cg.new_Pvariable(config[CONF_ID])
	await cg.register_component(var, config)
	await uart.register_uart_device(var, config)


@automation.register_action('pkt4_mcu.init', InitAction, cv.Schema({
	cv.GenerateID(): cv.use_id(PKT4MCUComponent),
}))
async def pkt4_mcu_init_to_code(config, action_id, template_arg, args):
	var = cg.new_Pvariable(action_id, template_arg)
	await cg.register_parented(var, config[CONF_ID])

	return var


@automation.register_action('pkt4_mcu.deinit', DeinitAction, cv.Schema({
	cv.GenerateID(): cv.use_id(PKT4MCUComponent),
}))
async def pkt4_mcu_deinit_to_code(config, action_id, template_arg, args):
	var = cg.new_Pvariable(action_id, template_arg)
	await cg.register_parented(var, config[CONF_ID])

	return var


@automation.register_action('pkt4_mcu.motor', MotorAction, cv.All(cv.Schema({
	cv.GenerateID(): cv.use_id(PKT4MCUComponent),
	cv.Required(CONF_MOTOR): cv.enum({'drum': 0, 'tray': 1}),
	cv.Required(CONF_MODE): cv.enum({'sense': 1, 'stop': 2, 'time': 3}),
	cv.Optional(CONF_DIRECTION, default='stop'): cv.enum({'stop': 0, 'down': 1, 'up': 2, 'open': 1, 'close': 2}),
	cv.Optional(CONF_SPEED): cv.positive_not_null_int,
	cv.Optional(CONF_DURATION): cv.positive_int,
	cv.Optional(CONF_TIMEOUT): cv.positive_int,
}), cv.vol.truth(lambda conf: all(bool((i in conf) ^ (conf[CONF_MODE] == 'stop')) for i in (CONF_SPEED, CONF_TIMEOUT)) and (conf[CONF_MODE] != 'sense' or CONF_DURATION in conf) and (conf[CONF_MODE] != 'stop' or CONF_DIRECTION not in conf or conf[CONF_DIRECTION] == 'stop')))) #, msg="This mode doesn't support the passed combination of parameters."))
async def pkt4_mcu_motor_to_code(config, action_id, template_arg, args):
	var = cg.new_Pvariable(action_id, template_arg)
	await cg.register_parented(var, config[CONF_ID])

	cg.add(var.set_motor(config[CONF_MOTOR]))
	cg.add(var.set_mode(config[CONF_MODE]))
	cg.add(var.set_direction(config[CONF_DIRECTION]))
	if (CONF_SPEED in config): cg.add(var.set_speed(config[CONF_SPEED]))
	if (CONF_DURATION in config): cg.add(var.set_duration(config[CONF_DURATION]))
	if (CONF_TIMEOUT in config): cg.add(var.set_timeout(config[CONF_TIMEOUT]))

	return var
