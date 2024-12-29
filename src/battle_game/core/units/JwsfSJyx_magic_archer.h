#pragma once
#include <chrono>

#include "battle_game/core/unit.h"
// features:
// 1. 3 split bullet.
// 2. lower health, higher damage.
// 3. (skill c) bullet in 24 average directions. CD: 10s
// 4. (闪现) CD: 5s, 并3s内的子弹获得heal_scale_=0.5的吸血(回复)
namespace battle_game::unit {
class MagicArcher : public Unit {
 public:
  MagicArcher(GameCore *game_core, uint32_t id, uint32_t player_id);
  void Render() override;
  void Update() override;
  [[nodiscard]] bool IsHit(glm::vec2 position) const override;

 protected:
  void MagicArcherMove(float move_speed, float rotate_angular_speed);
  void BowAndArrowRotate();
  void Fire();
  void SkillC();         // press C
  void ImmediateMove();  // press V
  void ReduceHealth();   // press O
  [[nodiscard]] const char *UnitName() const override;
  [[nodiscard]] const char *Author() const override;
  float GetDamageScale() const override {
    return static_cast<float>(3 - 2 * GetHealth());
  }

  float bow_and_arrow_rotation_{0.0f};
  uint32_t fire_count_down_{0};
  uint32_t mine_count_down_{0};
  std::chrono::time_point<std::chrono::high_resolution_clock> skill_c_cd_time_;
  bool is_c_available_{true};
  std::chrono::time_point<std::chrono::high_resolution_clock> im_move_cd_time_;
  bool is_im_move_available_{true};
  float heal_scale_{0.0f};  // 吸血比例
  std::chrono::time_point<std::chrono::high_resolution_clock> reduce_health_cd_time_;
  bool is_reduce_health_available_{true};
};
}  // namespace battle_game::unit
