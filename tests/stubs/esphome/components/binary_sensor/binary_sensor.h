#pragma once
namespace esphome { namespace binary_sensor { struct BinarySensor { bool state=false; void publish_state(bool x) { state=x; } void invalidate_state() {} }; }}
