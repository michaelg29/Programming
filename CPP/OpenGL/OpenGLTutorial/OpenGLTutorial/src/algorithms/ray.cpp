#include "ray.h"

#include "../linalg.h"

Ray::Ray(glm::vec3 origin, glm::vec3 dir)
    : origin(origin), dir(dir), invdir(0.0f) {
    for (int i = 0; i < 3; i++) {
        invdir[i] = 1.0f / dir[i];
    }
}

bool Ray::intersectsBoundingRegion(BoundingRegion br, float& tmin, float& tmax) {
    if (br.type == BoundTypes::AABB) {
        // modified ray-AABB collision algorithm as described here: https://tavianator.com/2011/ray_box.html
        tmin = std::numeric_limits<float>::lowest();
        tmax = std::numeric_limits<float>::max();

        for (int i = 0; i < 3; i++) {
            float t1 = (br.min[i] - origin[i]) * invdir[i];
            float t2 = (br.max[i] - origin[i]) * invdir[i];

            tmin = std::fmaxf(tmin, std::fminf(t1, t2));
            tmax = std::fminf(tmax, std::fmaxf(t1, t2));
        }

        return (tmax >= tmin) && tmax >= 0.0f;
    }
    else {
        // ray-sphere collision
        // when plug in line equation of ray into equation of sphere, get a quadratic equation for t (the parameter of the line)

        glm::vec3 cp = origin - br.center; // vector from center of sphere to ray origin
        float dot = glm::dot(dir, cp); // dot product of ray direction and cp

        float a = magsq(dir);
        float b = 2.0f * glm::dot(dir, cp);
        float c = magsq(cp) - br.radius * br.radius;

        float D = b * b - 4.0f * a * c;
        if (D < 0) {
            return false;
        }
        D = sqrtf(D);

        float t1 = (-b + D) / (2.0f * a);
        float t2 = (-b - D) / (2.0f * a);

        if (t1 > t2) {
            tmin = t2;
            tmax = t1;
        }
        else {
            tmin = t1;
            tmax = t2;
        }

        return true;
    }
}

bool Ray::intersectsMesh(CollisionMesh* mesh, RigidBody* rb, float &t) {
    bool intersects = false;
    float tmp;
    for (Face &f : mesh->faces) {
        tmp = -1.0f;
        glm::vec3 P1 = mat4vec3mult(rb->model, mesh->points[f.i1]);
        glm::vec3 P2 = mat4vec3mult(rb->model, mesh->points[f.i2]) - P1;
        glm::vec3 P3 = mat4vec3mult(rb->model, mesh->points[f.i3]) - P1;
        glm::vec3 norm = rb->normalModel * f.norm;

        glm::vec3 U1 = origin - P1;

        LinePlaneIntCase intCase = linePlaneIntersection(glm::vec3(0.0f), norm, U1, dir, tmp);
        if ((char)intCase > 1) {
            // intersection with the infinite plane
            if (tmp < 0.0f || t < tmp) {
                continue;
            }

            // get point of intersection
            glm::vec3 intersection = U1 + tmp * dir;

            if (faceContainsPoint(P2, P3, norm, intersection)) {
                intersects = true;
                t = fminf(t, tmp);
            }
        }
    }

    return intersects;
}