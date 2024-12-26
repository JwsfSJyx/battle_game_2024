#pragma once
#include "battle_game/core/unit.h"

// features:
// 1. 3 split bullet.
// 2. lower health, higher damage.

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
  [[nodiscard]] const char *UnitName() const override;
  [[nodiscard]] const char *Author() const override;
  float GetDamageScale() const override {
    return static_cast<float>(3 - 2 * GetHealth());
  }
  float bow_and_arrow_rotation_{0.0f};
  uint32_t fire_count_down_{0};
  uint32_t mine_count_down_{0};
};
}  // namespace battle_game::unit
