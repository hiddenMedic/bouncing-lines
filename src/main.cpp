#include "blines_general.h"

#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

#include <iostream>
#include <fstream>

int main(){
    hittable_list world;

    shared_ptr<material> material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    shared_ptr<material> material_center = make_shared<lambertian>(color(0.7, 0.3, 0.3));
    shared_ptr<material> material_left = make_shared<metal>(color(0.8, 0.8, 0.8));
    shared_ptr<material> material_right = make_shared<metal>(color(0.8, 0.6, 0.2));

    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

    camera cam("image.ppm");
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.render(world);

    return 0;
}