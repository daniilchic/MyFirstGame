#pragma once
#include <glm/vec2.hpp>
inline glm::vec2 worldPosition(int x, int y) {
    return glm::vec2((x - 1) * 64.0f + 32.0f, (y - 1) * 64.0f + 32.0f);
}
