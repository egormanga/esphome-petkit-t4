#include "../components/pkt4_mcu/pkt4_mcu.cpp"
#include <cassert>
#include <iostream>
using namespace esphome;
int main() {
 pkt4_mcu::PKT4MCUComponent m;
 m.motor(0,3,1,100,0,1000); assert(m.motors_idle());
 bool allowed=true; m.set_movement_allowed([&](){return allowed;});
 m.motor(0,3,1,100,0,1000); assert(!m.motors_idle());
 allowed=false; m.loop(); assert(m.motors_idle());
 allowed=true; m.motor(1,3,1,100,0,1000);
 test_now+=120001; m.loop(); assert(m.motors_idle());
 std::cout << "Motor interlock regression tests passed\n";
}
