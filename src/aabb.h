#ifndef AABB_H
#define AABB_H

#include "blines_general.h"

class aabb{
public:
    interval x, y, z;

    aabb() {}

    aabb(const interval& ix, const interval& iy, const interval& iz)
        : x(ix), y(iy), z(iz) {}

    aabb(const point3& a, const point3& b){
        x = interval(fmin(a[0], b[0]), fmax(a[0], b[0]));
        y = interval(fmin(a[1], b[1]), fmax(a[1], b[1]));
        z = interval(fmin(a[2], b[2]), fmax(a[2], b[2]));
    }

    aabb(const aabb& box1, const aabb& box2){
        x = interval(box1.x, box2.x);
        y = interval(box1.y, box2.y);
        z = interval(box1.z, box2.z);
    }

    const interval& axis(int n) const {
        if(n == 1) return y;
        if(n == 2) return z;
        return x;
    }

    bool hit(const ray& r, interval& ray_t) const {
        for(int a = 0; a < 3; ++a){
            double invd = 1 / r.direction()[a];
            double orig = r.origin()[a];

            double t0 = (axis(a).min - orig) * invd;
            double t1 = (axis(a).max - orig) * invd;

            if(invd < 0)
                std::swap(t0, t1);

            if(t0 > ray_t.min) ray_t.min = t0;
            if(t1 < ray_t.max) ray_t.max = t1;

            if(ray_t.max <= ray_t.min)
                return false;
        }
        return true;
    }
};

#endif