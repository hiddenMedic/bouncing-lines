#ifndef HITTABLE_H
#define HITTABLE_H

#include "blines.h"
#include "aabb.h"

class material;

class hit_record{
public:
    point3 p;
    vec3 normal;
    shared_ptr<material> mat;
    double t;
    double u, v;
    bool front_face;

    // outward normal has to have unit lenght
    void set_face_normal(const ray& r, const vec3& outward_normal){
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }

};

class hittable {
public:
    virtual ~hittable() = default;

    // ray_tmin and ray_tmax are exclusive
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;

    virtual aabb bounding_box() const = 0;
};

class translate : public hittable {
public:
    translate(shared_ptr<hittable> p, const vec3& displacement)
        : object(p), offset(displacement)
    {
        bbox = object->bounding_box() + offset;
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        ray offset_r(r.origin() - offset, r.direction(), r.time());

        if(!object->hit(offset_r, ray_t, rec)){
            return false;
        }

        rec.p += offset;
        
        return true;
    }

    aabb bounding_box() const override {
        return bbox;
    }
private:
    shared_ptr<hittable> object;
    vec3 offset;
    aabb bbox;
};

class rotate_y : public hittable {
public:
    rotate_y(shared_ptr<hittable> obj, double angle) : object(obj) {
        double radians = degrees_to_radians(angle);
        sin_theta = sin(radians);
        cos_theta = cos(radians);
        bbox = object->bounding_box();

        point3 min(infinity, infinity, infinity);
        point3 max(-infinity, -infinity, -infinity);

        for(int i = 0; i < 2; ++i){
            for(int j = 0; j < 2; ++j){
                for(int k = 0; k < 2; ++k){
                    // go through all the boxes points
                    double x = i * bbox.x.max + (1 - i) * bbox.x.min;
                    double y = j * bbox.y.max + (1 - j) * bbox.y.min;
                    double z = k * bbox.z.max + (1 - k) * bbox.y.min;

                    double newx = cos_theta * x + sin_theta * z;
                    double newz = -sin_theta * x + cos_theta * z;

                    vec3 tester(newx, y, newz);

                    for(int c = 0; c < 3; ++c){
                        min[c] = fmin(min[c], tester[c]);
                        max[c] = fmax(max[c], tester[c]);
                    }
                }
            }
        }

        bbox = aabb(min, max);
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        // put ray into object space
        point3 origin = r.origin();
        vec3 direction = r.direction();

        origin[0] = cos_theta * r.origin()[0] - sin_theta * r.origin()[2];
        origin[2] = sin_theta * r.origin()[0] + cos_theta * r.origin()[2];

        direction[0] = cos_theta * r.direction()[0] - sin_theta * r.direction()[2];
        direction[2] = sin_theta * r.direction()[0] + cos_theta * r.direction()[2];

        ray rotated_r(origin, direction, r.time());

        if(!object->hit(rotated_r, ray_t, rec)){
            return false;
        }

        // put intersection point back to world space
        point3 p = rec.p;
        p[0] = cos_theta * rec.p[0] + sin_theta * rec.p[2];
        p[2] = -sin_theta * rec.p[0] + cos_theta * rec.p[2];

        // put normal back into world space
        vec3 normal = rec.normal;
        normal[0] = cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
        normal[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];

        rec.p = p;
        rec.normal = normal;

        return true;
    }

    aabb bounding_box() const override {
        return bbox;
    }
private:
    shared_ptr<hittable> object;
    double sin_theta;
    double cos_theta;
    aabb bbox;
};
#endif