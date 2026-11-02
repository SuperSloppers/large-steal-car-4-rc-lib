#pragma once

#include "native_owned_commands.h"

#include <cstdint>
#include <unordered_set>

namespace rex::graphics::gta4_native {
// Own commands and their eviction protection together from publish through
// recording/readback. This boundary is synchronous; it adds no synchronization.
struct NativeEmptyFrameState {};

template <typename Command, typename FrameState = NativeEmptyFrameState>
class NativeFramePacket {
 public:
  bool TrySeal(NativeOwnedCommands<Command>& assembled_commands,
               std::unordered_set<uint64_t>& assembled_protection,
               const FrameState& assembled_state = {}) {
    if (sealed_) return false;
    assert(commands.empty() && texture_generations.empty());
    // Copy before transferring owners: a failed state copy leaves assembly intact.
    state = assembled_state;
    std::swap(commands, assembled_commands);
    texture_generations.swap(assembled_protection);
    sealed_ = true;
    return true;
  }

  // The caller retires command owners (possibly to its reaper) before reset.
  void ResetAfterRetirement() {
    assert(commands.empty());
    texture_generations.clear();
    state = {};
    sealed_ = false;
  }

  NativeOwnedCommands<Command> commands;
  std::unordered_set<uint64_t> texture_generations;
  FrameState state{};

 private:
  bool sealed_ = false;
};
}  // namespace rex::graphics::gta4_native
