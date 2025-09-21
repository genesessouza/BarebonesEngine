#include "collider.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <array>
#include <algorithm>
#include <cstdio>

collider::~collider()
{
}

std::vector<glm::vec3> collider::extract_positions(const float* data, size_t vertex_count, size_t stride)
{
    std::vector<glm::vec3> positions;
    positions.reserve(vertex_count);

    for (size_t i = 0; i < vertex_count; i++) {
        size_t offset = i * stride;
        positions.emplace_back(data[offset], data[offset + 1], data[offset + 2]);
    }
    return positions;
}

void collider::compute_obb(const std::vector<glm::vec3>& vertices, const glm::mat4& modelMatrix)
{
    glm::vec3 minV = vertices[0];
    glm::vec3 maxV = vertices[0];

    for (auto& v : vertices) {
        minV = glm::min(minV, v);
        maxV = glm::max(maxV, v);
    }

    OBB box{};

    // centro no mundo
    glm::vec3 localCenter = (minV + maxV) * 0.5f;
    box.center = glm::vec3(modelMatrix * glm::vec4(localCenter, 1.0f));

    // halfSize no espaço local
    box.halfSize = (maxV - minV) * 0.5f;

    // eixos orientados pelo modelMatrix
    glm::mat3 rotation(modelMatrix);
    box.axis[0] = glm::normalize(rotation[0]);
    box.axis[1] = glm::normalize(rotation[1]);
    box.axis[2] = glm::normalize(rotation[2]);

    m_obb = box;
}

std::vector<glm::vec3> getCorners(const OBB& box) {
    std::vector<glm::vec3> corners;
    corners.reserve(8);

    glm::vec3 ex = box.axis[0] * box.halfSize.x;
    glm::vec3 ey = box.axis[1] * box.halfSize.y;
    glm::vec3 ez = box.axis[2] * box.halfSize.z;
    glm::vec3 c = box.center;

    corners.push_back(c - ex - ey - ez);
    corners.push_back(c + ex - ey - ez);
    corners.push_back(c - ex + ey - ez);
    corners.push_back(c + ex + ey - ez);
    corners.push_back(c - ex - ey + ez);
    corners.push_back(c + ex - ey + ez);
    corners.push_back(c - ex + ey + ez);
    corners.push_back(c + ex + ey + ez);

    return corners;
}

std::pair<float, float> collider::project_obb(const OBB& box, const glm::vec3& axis) {
    auto corners = getCorners(box);

    float minProj = glm::dot(corners[0], axis);
    float maxProj = minProj;

    for (size_t i = 1; i < corners.size(); i++) {
        float proj = glm::dot(corners[i], axis);
        minProj = std::min(minProj, proj);
        maxProj = std::max(maxProj, proj);
    }

    return { minProj, maxProj };
}

bool collider::overlap(const std::pair<float, float>& a, const std::pair<float, float>& b) {
    return !(a.second < b.first || b.second < a.first);
}

// ---------- SAT entre dois OBB ----------
bool collider::check_for_collision(const OBB& other) {
    std::vector<glm::vec3> axes;

    // 3 eixos de A
    axes.push_back(m_obb.axis[0]);
    axes.push_back(m_obb.axis[1]);
    axes.push_back(m_obb.axis[2]);

    // 3 eixos de B
    axes.push_back(other.axis[0]);
    axes.push_back(other.axis[1]);
    axes.push_back(other.axis[2]);

    // 9 eixos cruzados
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            glm::vec3 axis = glm::cross(m_obb.axis[i], m_obb.axis[j]);
            if (glm::length2(axis) > 1e-6f)
                axes.push_back(glm::normalize(axis));
        }
    }

    // Testa todos os eixos
    for (const auto& axis : axes) {
        auto projA = project_obb(m_obb, axis);
        auto projB = project_obb(other, axis);

        if (!overlap(projA, projB))
            return false; // separação encontrada
    }

    return true;
}