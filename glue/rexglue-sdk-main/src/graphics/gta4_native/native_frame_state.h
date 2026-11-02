#pragma once

#include "modern_shader_policy.h"
#include "native_cpu_profile.h"
#include "native_reflection_registry.h"

#include <string>

namespace rex::graphics::gta4_native {
// Assembly captures this once per guest frame, including all readback flushes.
struct NativeFrameSettings {
  ModernShaderSettings modern;
  std::string texture_filtering;
  std::string anisotropic_filtering;
  uint64_t guest_frame_serial = 0;
  bool modern_trace = false;
};

// Values copied at the packet boundary; recorder work never reads assembly state.
struct NativeFrameState {
  NativeFrameSettings settings;
  std::shared_ptr<const NativeReflectionRegistry> reflections;
  profile::TransportSummary transport;
  bool profiler_enabled = false;
  bool profile_autostart = false;
  bool profile_detailed_cpu = false;
};

// Recorder-owned carry across internal flushes, consumed by the real present.
class NativeRecorderTransport {
 public:
  profile::TransportSummary ForPacket(const profile::TransportSummary& assembly,
                                      bool title_present) {
    if (!title_present) return {};
    auto result = assembly;
    result.internal_flush_ticks = profile::AddSaturated(result.internal_flush_ticks, ticks_);
    result.internal_flushes = profile::AddSaturated(result.internal_flushes, flushes_);
    Reset();
    return result;
  }

  void ObserveInternalFlush(uint64_t ticks) {
    ticks_ = profile::AddSaturated(ticks_, ticks);
    flushes_ = profile::AddSaturated(flushes_, 1);
  }

  void Reset() { ticks_ = flushes_ = 0; }

 private:
  uint64_t ticks_ = 0, flushes_ = 0;
};
}  // namespace rex::graphics::gta4_native
