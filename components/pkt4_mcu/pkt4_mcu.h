#pragma once

#include <esphome/core/component.h>
#include <esphome/components/binary_sensor/binary_sensor.h>
#include <esphome/components/sensor/sensor.h>
#include <esphome/components/uart/uart.h>

namespace esphome {
namespace pkt4_mcu {

static const uint16_t MAGIC = 0xA55A;

struct __attribute__((packed)) MCUPacket {
	uint16_t magic;
	uint8_t len,
	        pid,
	        seq,
	        err;
	uint8_t payload[0xFF - 6];
};

class PKT4MCUComponent: public Component, public uart::UARTDevice {
	public:
		void setup() override;
		//void update() override;
		void loop() override;
		void dump_config() override;
		float get_setup_priority() const { return setup_priority::LATE; }

		void set_distance_sensor(sensor::Sensor *distance_sensor) { this->distance_sensor_ = distance_sensor; }
		void set_weight_sensor(sensor::Sensor *weight_sensor) { this->weight_sensor_ = weight_sensor; }
		void set_approach_sensor(binary_sensor::BinarySensor *approach_sensor) { this->approach_sensor_ = approach_sensor; }
		void set_bin_sensor(binary_sensor::BinarySensor *bin_sensor) { this->bin_sensor_ = bin_sensor; }
		void set_cover_sensor(binary_sensor::BinarySensor *cover_sensor) { this->cover_sensor_ = cover_sensor; }
		void set_drum_up_sensor(binary_sensor::BinarySensor *drum_up_sensor) { this->drum_up_sensor_ = drum_up_sensor; }
		void set_drum_down_sensor(binary_sensor::BinarySensor *drum_down_sensor) { this->drum_down_sensor_ = drum_down_sensor; }
		void set_drum_level_sensor(binary_sensor::BinarySensor *drum_level_sensor) { this->drum_level_sensor_ = drum_level_sensor; }
		void set_tray_sensor(binary_sensor::BinarySensor *tray_sensor) { this->tray_sensor_ = tray_sensor; }

		void init(void);
		void deinit(void);
		void motor(uint8_t motor, uint8_t mode, uint8_t direction, uint8_t speed, uint16_t duration, uint16_t timeout);

	protected:
		uint8_t hw_ver_,
		        sw_ver_;
		bool inited_;
		MCUPacket packet_;
		uint8_t seq_{0};

		sensor::Sensor *distance_sensor_{nullptr},
		               *weight_sensor_{nullptr};
		binary_sensor::BinarySensor *approach_sensor_{nullptr},
		                            *bin_sensor_{nullptr},
		                            *cover_sensor_{nullptr},
		                            *drum_up_sensor_{nullptr},
		                            *drum_down_sensor_{nullptr},
		                            *drum_level_sensor_{nullptr},
		                            *tray_sensor_{nullptr};

		void send_(uint8_t pid, uint8_t payload[], uint8_t len);
};

template<typename... Ts> class InitAction: public Action<Ts...>, public Parented<PKT4MCUComponent> {
	public:
		void play(Ts... x) override { this->parent_->init(); }
};

template<typename... Ts> class DeinitAction: public Action<Ts...>, public Parented<PKT4MCUComponent> {
	public:
		void play(Ts... x) override { this->parent_->deinit(); }
};

template<typename... Ts> class MotorAction: public Action<Ts...>, public Parented<PKT4MCUComponent> {
	public:
		void set_motor(uint8_t motor) { this->motor_ = motor; }
		void set_mode(uint8_t mode) { this->mode_ = mode; }
		void set_direction(uint8_t direction) { this->direction_ = direction; }
		void set_speed(uint8_t speed) { this->speed_ = speed; }
		void set_duration(uint16_t duration) { this->duration_ = duration; }
		void set_timeout(uint16_t timeout) { this->timeout_ = timeout; }

		void play(Ts... x) override { this->parent_->motor(this->motor_, this->mode_, this->direction_, this->speed_, this->duration_, this->timeout_); }

	protected:
		uint8_t motor_, mode_, direction_, speed_{0};
		uint16_t duration_{0}, timeout_{0};
};

}  // namespace pkt4_mcu
}  // namespace esphome
