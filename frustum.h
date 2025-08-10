#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "camera.h"
#include "glm-1.0.1/glm/vec3.hpp"
#include <array>

struct Plane {
    glm::vec3 normal = {0.f, 1.f, 0.f}; // нормаль плоскости
    float distance = 0.f;               // смещение от начала координат

    Plane() = default;
    Plane(const glm::vec3& p, const glm::vec3& n)
    {
        normal = glm::normalize(n);
        distance = -glm::dot(normal, p);
    }

    // проверка точки на стороне плоскости
    float getSignedDistanceToPoint(const glm::vec3& point) const {
        return glm::dot(normal, point) + distance;
    }
};

struct Frustum {
    Plane topFace;
    Plane bottomFace;

    Plane leftFace;
    Plane rightFace;

    Plane nearFace;
    Plane farFace;

    // Проверка пересечения фрустума с AABB
    bool isBoxVisible(const glm::vec3& min, const glm::vec3& max) const {
        for (const Plane& plane : { topFace, bottomFace, leftFace, rightFace, nearFace, farFace }) {
            glm::vec3 positiveVertex = min;

            if (plane.normal.x >= 0) positiveVertex.x = max.x;
            if (plane.normal.y >= 0) positiveVertex.y = max.y;
            if (plane.normal.z >= 0) positiveVertex.z = max.z;

            if (plane.getSignedDistanceToPoint(positiveVertex) < 0)
                return false; // весь AABB вне плоскости
        }
        return true;
    }
};

inline Frustum createFrustumFromCamera(const Camera& camera, float aspect, float fovY,
                                       float zNear, float zFar)
{
    Frustum frustum;
    const float halfVSide = zFar * tanf(fovY * 0.5f);
    const float halfHSide = halfVSide * aspect;
    const glm::vec3 frontMultFar = zFar * camera.front;

    frustum.nearFace   = { camera.position + zNear * camera.front,  camera.front };
    frustum.farFace    = { camera.position + frontMultFar,         -camera.front };
    frustum.rightFace  = { camera.position,
                           glm::cross(frontMultFar - camera.right * halfHSide, camera.up) };
    frustum.leftFace   = { camera.position,
                           glm::cross(camera.up, frontMultFar + camera.right * halfHSide) };
    frustum.topFace    = { camera.position,
                           glm::cross(camera.right, frontMultFar - camera.up * halfVSide) };
    frustum.bottomFace = { camera.position,
                           glm::cross(frontMultFar + camera.up * halfVSide, camera.right) };

    return frustum;
}

#endif //FRUSTUM_H
