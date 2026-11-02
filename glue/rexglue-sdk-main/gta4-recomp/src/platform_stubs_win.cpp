// Windows stand-ins for the macOS-only .mm sources (Game Center, microphone
// permission, Apple Music). ponytail: no-ops; wire Steam/XInput equivalents if wanted.
#include "achievement_bridge_gc.h"
#include "input/user_music_player.h"
#include "network/gta4_microphone_permission.h"

namespace gta4::game_center {
void Initialize() {}
void SubmitAchievement(uint32_t) {}
}  // namespace gta4::game_center

namespace gta4::voice {
MicrophonePermissionStatus GetMicrophonePermissionStatus() noexcept {
  return MicrophonePermissionStatus::kAuthorized;
}
void RequestMicrophonePermission(std::function<void()> completion) {
  if (completion) completion();
}
}  // namespace gta4::voice

namespace gta4::input {
struct UserMusicPlayer::Impl {};
UserMusicPlayer::UserMusicPlayer(std::filesystem::path) {}
UserMusicPlayer::~UserMusicPlayer() = default;
bool UserMusicPlayer::Next() { return false; }
bool UserMusicPlayer::Previous() { return false; }
bool UserMusicPlayer::Stop() { return false; }
bool UserMusicPlayer::HasTracks() const { return false; }
void PublishUserMusicPlayer(UserMusicPlayer*) {}
bool IsUserMusicAvailable() { return false; }
bool RequestUserMusicNext() { return false; }
bool RequestUserMusicPrevious() { return false; }
bool RequestUserMusicStop() { return false; }
}  // namespace gta4::input
