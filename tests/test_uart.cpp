#include "../components/pkt4_mcu/pkt4_mcu.cpp"
#include <cassert>
#include <iostream>
using namespace esphome;
std::vector<uint8_t> frame(uint8_t pid, std::vector<uint8_t> payload) {
 std::vector<uint8_t> b{0x5a,0xa5,uint8_t(payload.size()+8),pid,0,0xff};
 b.insert(b.end(),payload.begin(),payload.end()); auto crc=crc16be(b.data(),b.size(),0xffff);
 b.push_back(crc&255);b.push_back(crc>>8);return b;
}
void feed(pkt4_mcu::PKT4MCUComponent &m, const std::vector<uint8_t> &b) {
 m.input.insert(m.input.end(),b.begin(),b.end());m.loop();
}
class ExposedMCU : public pkt4_mcu::PKT4MCUComponent {
 public: using PKT4MCUComponent::send_;
};
int main() {
 { ExposedMCU m; uint8_t data[249]{};
   m.send_(1,data,247); assert(m.output.size()==255);
   m.send_(1,data,248); assert(m.output.size()==255);
   m.send_(1,data,249); assert(m.output.size()==255);
 }

 pkt4_mcu::PKT4MCUComponent m; sensor::Sensor w; m.set_weight_sensor(&w);
 auto good=frame(7,{0,1,0x00,0x00,0x78,0x00});
 for(auto c:good) feed(m,{c});
 assert(w.count==1 && w.state==0x780000);
 auto bad=good;bad.back()^=1;feed(m,bad);assert(w.count==1);
 feed(m,frame(7,{0,255}));assert(w.count==1);
 for(int len=0;len<8;++len) feed(m,{0x5a,0xa5,uint8_t(len)});
 feed(m,good);assert(w.count==2);
 feed(m,{0x5a,0xa5,255,7});test_now+=201;feed(m,good);assert(w.count==3);
 for(auto pid:{1,7,9}) for(int n=0;n<14;++n) feed(m,frame(pid,std::vector<uint8_t>(n,0)));
 // Exercise arbitrary and truncated traffic under ASan/UBSan.
 uint32_t r=12345;
 for(int i=0;i<10000;++i) {r=r*1664525+1013904223;feed(m,{uint8_t(r>>24)});}
 test_now+=201;feed(m,good);assert(w.state==0x780000);
 std::cout<<"MCU framing regression tests passed\n";
}
