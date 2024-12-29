#pragma once
#include "battle_game/core/object.h"
#include "battle_game/graphics/assets_manager.h"
#include "cstdint"

namespace battle_game {
class GameCore;
class Bullet : public Object {
 public:
  Bullet(GameCore *core,
         uint32_t id,
         uint32_t unit_id,
         uint32_t player_id,
         glm::vec2 position,
         float rotation,
         float damage_scale
         );
Bullet(GameCore *core,
         uint32_t id,
         uint32_t unit_id,
         uint32_t player_id,
         glm::vec2 position,
         float rotation,
         float damage_scale,
         float heal_scale
         );
  ~Bullet() override;

 protected:
  uint32_t unit_id_{};
  uint32_t player_id_{};
  float damage_scale_{1.0f};
  float heal_scale_{0.0f};  // 吸血, 命中回复heal_scale*造成伤害(需手动编写)
};
}  // namespace battle_game
