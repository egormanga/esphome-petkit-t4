import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
	CONF_DISTANCE,
	CONF_ID,
	CONF_WEIGHT,
	DEVICE_CLASS_DISTANCE,
	DEVICE_CLASS_WEIGHT,
	STATE_CLASS_MEASUREMENT,
	UNIT_CENTIMETER,
	UNIT_KILOGRAM,
)
from . import PKT4MCUComponent


DEPENDENCIES = ('pkt4_mcu',)

CONFIG_SCHEMA = cv.Schema({
	cv.GenerateID(): cv.use_id(PKT4MCUComponent),
	cv.Optional(CONF_DISTANCE): sensor.sensor_schema(
		unit_of_measurement=UNIT_CENTIMETER,
		accuracy_decimals=0,
		device_class=DEVICE_CLASS_DISTANCE,
		state_class=STATE_CLASS_MEASUREMENT,
	),
	cv.Optional(CONF_WEIGHT): sensor.sensor_schema(
		unit_of_measurement=UNIT_KILOGRAM,
		accuracy_decimals=2,
		device_class=DEVICE_CLASS_WEIGHT,
		state_class=STATE_CLASS_MEASUREMENT,
	),
})


async def to_code(config):
	var = await cg.get_variable(config[CONF_ID])

	if distance_config := config.get(CONF_DISTANCE):
		sens = await sensor.new_sensor(distance_config)
		cg.add(var.set_distance_sensor(sens))

	if weight_config := config.get(CONF_WEIGHT):
		sens = await sensor.new_sensor(weight_config)
		cg.add(var.set_weight_sensor(sens))
