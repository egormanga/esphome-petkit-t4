#include "../weight_autozero.h"
#include <cassert>
#include <iostream>

constexpr int baseline = 7975680;
constexpr float scale = -1.f / 65536;

bool run(petkit::AutoZero &z, float residual, bool quiet, int &zero,
         uint32_t start = 60000, uint32_t duration = 121000) {
  bool changed = false;
  const float raw = baseline + (2.f + residual) / scale;
  for (uint32_t t = 0; t <= duration; t += 100)
    changed |= z.update(start + t, raw, scale, zero, 2.f, quiet, 0.025f, zero);
  return changed;
}
int main() {
  { petkit::AutoZero z; int zero = baseline;
    assert(run(z, -.010f, true, zero));
    assert(std::fabs((baseline + 1.990f / scale - zero) * scale - 2.f) < .0001f);
  }
  for (float load : {0.1f, 4.f, .025f, .005f, -.1f}) {
    petkit::AutoZero z; int zero = baseline;
    assert(!run(z, load, true, zero)); assert(zero == baseline);
  }
  { petkit::AutoZero z; int zero = baseline;
    assert(!run(z, -.01f, false, zero));
    assert(!run(z, -.01f, true, zero, 200000, 119000));
  }
  { petkit::AutoZero z; int zero = baseline;
    assert(run(z, .003f, true, zero));
    assert(!run(z, .006f, true, zero, 181100)); // cumulative added load not erased
  }
  { petkit::AutoZero z; int zero = baseline;
    assert(!run(z, -.01f, true, zero, 60000, 119000));
    assert(!run(z, -.01f, true, zero, 190000, 119000)); // missing packets reset timer
  }
  { petkit::AutoZero z; int zero = baseline;
    for (uint32_t t=60000; t<400000; t+=100)
      assert(!z.update(t, baseline + (2.f + (t%200 ? -.01f : .001f))/scale,
                       scale, zero, 2.f, true, .025f, zero));
  }
  for (float gain : {0.f, NAN, INFINITY, 1.f}) {
    petkit::AutoZero z; int zero = baseline;
    for (uint32_t t=60000; t<200000; t+=100)
      assert(!z.update(t, baseline, gain, zero, 0, true, .025f, zero));
  }
  { petkit::AutoZero z; int zero = baseline;
    bool adjusted = false;
    // After litter removal, a signed reference must still allow drift correction.
    for (uint32_t t=60000; t<=181000; t+=100)
      adjusted |= z.update(t, baseline + (-2.010f)/scale, scale, zero,
                           -2.f, true, .025f, zero);
    assert(adjusted);
    assert(std::fabs((baseline + (-1.f)/scale - zero)*scale - (-2.f) - 1.010f) < .0001f);
  }
  std::cout << "Autozero regression tests passed\n";
}
