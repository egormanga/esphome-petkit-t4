import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import (
	CONF_ID,
	DEVICE_CLASS_MOTION,
	DEVICE_CLASS_MOVING,
	DEVICE_CLASS_OCCUPANCY,
	DEVICE_CLASS_OPENING,
	DEVICE_CLASS_PROBLEM,
	DEVICE_CLASS_SAFETY,
)
from . import PKT4MCUComponent


DEPENDENCIES = ('pkt4_mcu',)

CONF_APPROACH = 'approach'
CONF_BIN = 'bin'
CONF_COVER = 'cover'
CONF_DRUM_UP = 'drum_up'
CONF_DRUM_DOWN = 'drum_down'
CONF_DRUM_LEVEL = 'drum_level'
CONF_TRAY = 'tray'

CONFIG_SCHEMA = cv.Schema({
	cv.GenerateID(): cv.use_id(PKT4MCUComponent),
	cv.Optional(CONF_APPROACH): binary_sensor.binary_sensor_schema(
		device_class=DEVICE_CLASS_MOTION,
	),
	cv.Optional(CONF_BIN): binary_sensor.binary_sensor_schema(
		device_class=DEVICE_CLASS_PROBLEM,
	),
	cv.Optional(CONF_COVER): binary_sensor.binary_sensor_schema(
		device_class=DEVICE_CLASS_PROBLEM,
	),
	cv.Optional(CONF_DRUM_UP): binary_sensor.binary_sensor_schema(
		device_class=DEVICE_CLASS_PROBLEM,
	),
	cv.Optional(CONF_DRUM_DOWN): binary_sensor.binary_sensor_schema(
		device_class=DEVICE_CLASS_PROBLEM,
	),
	cv.Optional(CONF_DRUM_LEVEL): binary_sensor.binary_sensor_schema(
		device_class=DEVICE_CLASS_PROBLEM,
	),
	cv.Optional(CONF_TRAY): binary_sensor.binary_sensor_schema(
		device_class=DEVICE_CLASS_OPENING,
	),
})


async def to_code(config):
    var = await cg.get_variable(config[CONF_ID])

    # Allocate every entity before building automations: callbacks may reference
    # other sensors in this same platform, regardless of declaration order.
    sensors = []
    for key in (CONF_TRAY, CONF_COVER, CONF_DRUM_UP, CONF_DRUM_DOWN,
                CONF_BIN, CONF_DRUM_LEVEL, CONF_APPROACH):
        if sensor_config := config.get(key):
            sens = cg.new_Pvariable(sensor_config[CONF_ID])
            cg.add(getattr(var, f"set_{key}_sensor")(sens))
            sensors.append((sens, sensor_config))
    for sens, sensor_config in sensors:
        await binary_sensor.register_binary_sensor(sens, sensor_config)
