#pragma once
namespace esphome { namespace sensor { struct Sensor { float state=0; int count=0; void publish_state(float x) { state=x; ++count; } }; }}
