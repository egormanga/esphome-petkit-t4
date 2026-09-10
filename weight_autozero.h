#pragma once
#include <algorithm>
#include <cmath>
#include <cstdint>

namespace petkit {
// Only correct small, stable deviations from an already established empty load.
// Never bootstrap an arbitrary occupied load as empty, or change scale gain.
class AutoZero {
 public:
  bool update(uint32_t now, float raw, float scale, int zero, float load,
              bool quiet, float waste_threshold, int &result) {
    const bool gap = sampled_ && uint32_t(now - last_) > 2000;
    last_ = now;
    sampled_ = true;
    if (!anchored_ || zero != last_zero_ || scale != last_scale_ || load != last_load_) {
      if (!anchored_ || zero != last_zero_) anchor_ = zero;
      anchored_ = true; active_ = false;
      last_zero_ = zero; last_scale_ = scale; last_load_ = load;
    }
    const float residual = (raw - zero) * scale - load;
    const float positive_limit = std::min(0.004f, waste_threshold * 0.2f);
    if (gap || !quiet || now < 60000 || !std::isfinite(raw) ||
        !std::isfinite(scale) || scale >= -0.000001f || scale <= -0.001f ||
        !std::isfinite(load) || !std::isfinite(waste_threshold) ||
        waste_threshold <= 0 || residual < -0.020f || residual > positive_limit) {
      active_ = false;
      return false;
    }
    if (!active_) {
      active_ = true; since_ = now; low_ = high_ = raw; sum_ = 0; count_ = 0;
    }
    low_ = std::min(low_, raw); high_ = std::max(high_, raw);
    if ((high_ - low_) * std::fabs(scale) > 0.003f) {
      active_ = false;
      return false;
    }
    sum_ += raw; ++count_;
    if (uint32_t(now - since_) < 120000 || count_ < 60) return false;
    active_ = false;
    const int candidate = static_cast<int>(std::lround(sum_ / count_ - load / scale));
    // Total correction is bounded against boot baseline, preventing repeated
    // small additions (waste or a slowly applied load) from being zeroed away.
    const float total = (candidate - anchor_) * scale;
    if (total < -0.020f || total > positive_limit ||
        std::fabs((candidate - zero) * scale) < 0.001f) return false;
    last_zero_ = candidate;
    result = candidate;
    return true;
  }
 private:
  bool sampled_{false}, anchored_{false}, active_{false};
  uint32_t last_{0}, since_{0}, count_{0};
  int anchor_{0}, last_zero_{0};
  float last_scale_{0}, last_load_{0};
  float low_{0}, high_{0};
  double sum_{0};
};
}
