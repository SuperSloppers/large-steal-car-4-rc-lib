#pragma once
#include <cstddef>
#include <cstdint>
#include <unordered_set>
#include <utility>
#include <vector>

#include "frame_constant_arena.h"

namespace rex::graphics::gta4_native {

// Include implicit host-pass inputs as well as title sampler bindings. A queued
// composite owns its half-scene generation even when the title never binds it.
template <typename Command, typename Visitor>
void VisitNativeCommandTextureGenerations(const Command& command, Visitor&& visit) {
  const auto texture = [&](const auto& resource) {
    if (!resource) return;
    visit(resource->generation);
    if (resource->packed_depth_source) visit(resource->packed_depth_source->generation);
  };
  texture(command.resolve_destination);
  texture(command.depth_handoff_source);
  texture(command.present_source);
  texture(command.postfx_half_scene);
  for (const auto& resource : command.textures) texture(resource);
}

// Published under the queue lock before commands leave the title queue. Keep
// the entire batch protected until it drains, including its active command.
// Readers never inspect the worker's mutable command owners or cursor.
class NativeWorkerTextureProtection {
 public:
  template <typename Command>
  void Capture(const Command& command) {
    VisitNativeCommandTextureGenerations(command, [&](uint64_t generation) {
      if (generation) generations_.insert(generation);
    });
  }
  void AppendTo(std::unordered_set<uint64_t>& generations) const {
    generations.insert(generations_.begin(), generations_.end());
  }
  void Reset() { generations_.clear(); }

 private:
  std::unordered_set<uint64_t> generations_;
};

// Queue lock owns this index. Counts are logical references, not shared_ptrs or
// GPU ownership. Zero counts are pruned in bulk and never reported. If an invariant is
// violated the renderer falls back to its original queue scan, never guessing
// that an image is safe to retire.
class NativeTextureProtectionIndex {
 public:
  bool Retain(uint64_t generation) {
    if (!generation) return true;
    const auto entry = counts_.Insert(generation, 0);
    if (!entry || *entry.value == UINT64_MAX) { valid_ = false; return false; }
    if (!(*entry.value)++) ++live_;
    return true;
  }
  bool Release(uint64_t generation) {
    if (!generation) return true;
    uint64_t* count = counts_.Find(generation);
    if (!count || !*count) { valid_ = false; return false; }
    // Keep zero counts: per-draw erase/insert churn dominated this index.
    if (--*count) return true;
    --live_;
    if (++zero_count_ > 8192) Prune();
    return true;
  }
  void AppendTo(std::unordered_set<uint64_t>& destination) const {
    destination.reserve(destination.size() + live_);
    counts_.ForEach([&](uint64_t generation, uint64_t count) { if (count) destination.insert(generation); });
  }
  bool Contains(uint64_t generation) const {
    const uint64_t* count = counts_.Find(generation);
    return count && *count;
  }
  size_t size() const { return live_; }
  bool valid() const { return valid_; }
  void Reset() { counts_ = {}; live_ = zero_count_ = 0; valid_ = true; }
 private:
  void Prune() {
    std::vector<std::pair<uint64_t, uint64_t>> live;
    live.reserve(live_);
    counts_.ForEach([&](uint64_t generation, uint64_t count) { if (count) live.emplace_back(generation, count); });
    if (!counts_.ResetGeneration()) counts_ = {};
    for (const auto& [generation, count] : live) if (!counts_.Insert(generation, count)) valid_ = false;
    zero_count_ = 0;
  }
  FrameGenerationMap<uint64_t, uint64_t> counts_;
  size_t live_ = 0;
  size_t zero_count_ = 0;
  bool valid_ = true;
};
}  // namespace rex::graphics::gta4_native
