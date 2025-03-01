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

	# Dependency order

	if tray_config := config.get(CONF_TRAY):
		sens = await binary_sensor.new_binary_sensor(tray_config)
		cg.add(var.set_tray_sensor(sens))

	if cover_config := config.get(CONF_COVER):
		sens = await binary_sensor.new_binary_sensor(cover_config)
		cg.add(var.set_cover_sensor(sens))

	if drum_up_config := config.get(CONF_DRUM_UP):
		sens = await binary_sensor.new_binary_sensor(drum_up_config)
		cg.add(var.set_drum_up_sensor(sens))

	if drum_down_config := config.get(CONF_DRUM_DOWN):
		sens = await binary_sensor.new_binary_sensor(drum_down_config)
		cg.add(var.set_drum_down_sensor(sens))

	if bin_config := config.get(CONF_BIN):
		sens = await binary_sensor.new_binary_sensor(bin_config)
		cg.add(var.set_bin_sensor(sens))

	if drum_level_config := config.get(CONF_DRUM_LEVEL):
		sens = await binary_sensor.new_binary_sensor(drum_level_config)
		cg.add(var.set_drum_level_sensor(sens))

	if approach_config := config.get(CONF_APPROACH):
		sens = await binary_sensor.new_binary_sensor(approach_config)
		cg.add(var.set_approach_sensor(sens))
