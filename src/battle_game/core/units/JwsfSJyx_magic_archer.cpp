#include "JwsfSJyx_magic_archer.h"

#include "battle_game/core/bullets/bullets.h"
#include "battle_game/core/game_core.h"
#include "battle_game/graphics/graphics.h"

namespace battle_game::unit {

namespace {
uint32_t magic_archer_body_model_index = 0xffffffffu;
uint32_t magic_archer_arrow_model_index = 0xffffffffu;
uint32_t magic_archer_bow_model_index = 0xffffffffu;

}  // namespace

MagicArcher::MagicArcher(GameCore *game_core, uint32_t id, uint32_t player_id)
    : Unit(game_core, id, player_id) {
  if (!~magic_archer_body_model_index) {
    auto mgr = AssetsManager::GetInstance();
    // indice: every continuous 3 index indicate a triangle
    {
      // magic archer body

      std::vector<ObjectVertex> body_vertices;
      std::vector<uint32_t> body_indices;
      const int precision = 60;
      const float inv_precision = 1.0f / float(precision);
      for (int i = 0; i < precision; i++) {
        auto theta = (float(i) + 0.5f) * inv_precision;
        theta *= glm::pi<float>() * 2.0f;
        auto sin_theta = std::sin(theta);
        auto cos_theta = std::cos(theta);
        body_vertices.push_back({{sin_theta * 0.6f, cos_theta * 0.6f},
                                 {0.0f, 0.0f},
                                 {1.0f, 1.0f, 1.0f, 1.0f}});
        body_indices.push_back(i);
        body_indices.push_back((i + 1) % precision);
        body_indices.push_back(precision);
      }
      body_vertices.push_back(
          {{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}});
      body_vertices.push_back(
          {{0.5f, -0.6f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}});
      body_vertices.push_back(
          {{-0.5f, -0.6f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}});
      body_indices.push_back(precision / 4 - 1);
      body_indices.push_back(precision + 1);
      body_indices.push_back(precision + 2);
      body_indices.push_back(precision / 4);
      body_indices.push_back(precision * 3 / 4);
      body_indices.push_back(precision + 2);

      magic_archer_body_model_index =
          mgr->RegisterModel(body_vertices, body_indices);
    }
    {
      // magic archer arrow
      magic_archer_arrow_model_index = mgr->RegisterModel(
          {{{0.1f, -0.4f}, {0.0f, 0.0f}, {0.7f, 0.9f, 1.0f, 1.0f}},
           {{-0.1f, -0.4f}, {0.0f, 0.0f}, {0.7f, 0.9f, 1.0f, 1.0f}},
           {{0.1f, 0.8f}, {0.0f, 0.0f}, {0.7f, 0.9f, 1.0f, 1.0f}},
           {{-0.1f, 0.8f}, {0.0f, 0.0f}, {0.7f, 0.9f, 1.0f, 1.0f}},
           {{0.0f, 0.8f}, {0.0f, 0.0f}, {0.7f, 0.9f, 1.0f, 1.0f}},
           {{0.3f, 0.7f}, {0.0f, 0.0f}, {0.7f, 0.9f, 1.0f, 1.0f}},
           {{-0.3f, 0.7f}, {0.0f, 0.0f}, {0.7f, 0.9f, 1.0f, 1.0f}},
           {{0.0f, 1.0f}, {0.0f, 0.0f}, {0.7f, 0.9f, 1.0f, 1.0f}}},
          {0, 1, 2, 1, 2, 3, 4, 5, 7, 4, 6, 7});
    }
    {
      // magic archer bow
      magic_archer_bow_model_index = mgr->RegisterModel(
          {{{0.5f, -0.1f}, {0.0f, 0.0f}, {0.4f, 0.1f, 0.1f, 1.0f}},
           {{-0.5f, -0.1f}, {0.0f, 0.0f}, {0.4f, 0.1f, 0.1f, 1.0f}},
           {{0.5f, 0.0f}, {0.0f, 0.0f}, {0.4f, 0.1f, 0.1f, 1.0f}},
           {{-0.5f, 0.0f}, {0.0f, 0.0f}, {0.4f, 0.1f, 0.1f, 1.0f}},
           {{0.3f, 0.3f}, {0.0f, 0.0f}, {0.4f, 0.1f, 0.1f, 1.0f}},
           {{0.3f, 0.4f}, {0.0f, 0.0f}, {0.4f, 0.1f, 0.1f, 1.0f}},
           {{-0.3f, 0.3f}, {0.0f, 0.0f}, {0.4f, 0.1f, 0.1f, 1.0f}},
           {{-0.3f, 0.4f}, {0.0f, 0.0f}, {0.4f, 0.1f, 0.1f, 1.0f}}},
          {0, 1, 2, 1, 2, 3, 4, 5, 7, 4, 6, 7,
           2, 4, 5, 3, 6, 7, 0, 2, 4, 1, 3, 6});
    }
  }
}

void MagicArcher::Render() {
  battle_game::SetTransformation(position_, rotation_);
  battle_game::SetTexture(0);
  battle_game::SetColor(game_core_->GetPlayerColor(player_id_));
  battle_game::DrawModel(magic_archer_body_model_index);
  battle_game::SetRotation(bow_and_arrow_rotation_);
  battle_game::DrawModel(magic_archer_arrow_model_index);
  battle_game::DrawModel(magic_archer_bow_model_index);
}

void MagicArcher::Update() {
  MagicArcherMove(3.0f, glm::radians(180.0f));
  BowAndArrowRotate();
  Fire();
}

void MagicArcher::MagicArcherMove(float move_speed,
                                  float rotate_angular_speed) {
  auto player = game_core_->GetPlayer(player_id_);
  if (player) {
    auto &input_data = player->GetInputData();
    glm::vec2 offset{0.0f};
    if (input_data.key_down[GLFW_KEY_W]) {
      offset.y += 1.0f;
    }
    if (input_data.key_down[GLFW_KEY_S]) {
      offset.y -= 1.0f;
    }
    float speed = move_speed * GetSpeedScale();
    offset *= kSecondPerTick * speed;
    auto new_position =
        position_ + glm::vec2{glm::rotate(glm::mat4{1.0f}, rotation_,
                                          glm::vec3{0.0f, 0.0f, 1.0f}) *
                              glm::vec4{offset, 0.0f, 0.0f}};
    if (!game_core_->IsBlockedByObstacles(new_position)) {
      game_core_->PushEventMoveUnit(id_, new_position);
    }
    float rotation_offset = 0.0f;
    if (input_data.key_down[GLFW_KEY_A]) {
      rotation_offset += 1.0f;
    }
    if (input_data.key_down[GLFW_KEY_D]) {
      rotation_offset -= 1.0f;
    }
    rotation_offset *= kSecondPerTick * rotate_angular_speed * GetSpeedScale();
    game_core_->PushEventRotateUnit(id_, rotation_ + rotation_offset);
  }
}

void MagicArcher::BowAndArrowRotate() {
  auto player = game_core_->GetPlayer(player_id_);
  if (player) {
    auto &input_data = player->GetInputData();
    auto diff = input_data.mouse_cursor_position - position_;
    if (glm::length(diff) < 1e-4) {
      bow_and_arrow_rotation_ = rotation_;
    } else {
      bow_and_arrow_rotation_ =
          std::atan2(diff.y, diff.x) - glm::radians(90.0f);
    }
  }
}

void MagicArcher::Fire() {
  if (fire_count_down_ == 0) {
    auto player = game_core_->GetPlayer(player_id_);
    if (player) {
      auto &input_data = player->GetInputData();
      if (input_data.mouse_button_down[GLFW_MOUSE_BUTTON_LEFT]) {
        auto velocity = Rotate(glm::vec2{0.0f, 5.0f}, bow_and_arrow_rotation_);
        GenerateBullet<bullet::CannonBall>(
            position_ + Rotate({0.0f, 1.2f}, bow_and_arrow_rotation_),
            bow_and_arrow_rotation_, GetDamageScale(), velocity);
        GenerateBullet<bullet::CannonBall>(
            position_ + Rotate({0.0f, 1.2f},
                               bow_and_arrow_rotation_ ),
            bow_and_arrow_rotation_, GetDamageScale(), Rotate(velocity,glm::radians(20.0f)));
        GenerateBullet<bullet::CannonBall>(
            position_ + Rotate({0.0f, 1.2f},
                               bow_and_arrow_rotation_ ),
            bow_and_arrow_rotation_, GetDamageScale(), Rotate(velocity,glm::radians(-20.0f)));
        fire_count_down_ = kTickPerSecond;  // Fire interval 1 second.
      }
    }
  }
  if (fire_count_down_) {
    fire_count_down_--;
  }
}

bool MagicArcher::IsHit(glm::vec2 position) const {
  position = WorldToLocal(position);
  return position.x * position.x + position.y * position.y < 0.36f ||
         (position.y < 0 && position.y > -0.6f &&
          position.y - 3 * position.x > -1.8f &&
          position.y + 3 * position.x > -1.8f);
}

const char *MagicArcher::UnitName() const {
  return "Magic Archer";
}

const char *MagicArcher::Author() const {
  return "JwsfSJyx";
}
}  // namespace battle_game::unit
