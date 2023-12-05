#include "blines_general.h"

#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

#include <iostream>
#include <fstream>

color ray_color(const ray& r, const hittable& world){
    hit_record rec;
    if(world.hit(r, interval(0, infinity), rec)){
        return 0.5 * (rec.normal + color(1, 1, 1));
    }

    // sky
    vec3 unit_direction = unit_vector(r.direction());
    double a = 0.5 * (unit_direction.y() + 1.0); // from [-1, 1] to [0, 1]
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

int main(){
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    int image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    double viewport_height = 2.0;
    double viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

    double focal_lenght = 1.0;
    point3 camera_center = point3(0, 0, 0);

    vec3 viewport_right = vec3(viewport_width, 0, 0);
    vec3 viewport_down = vec3(0, -viewport_height, 0);
    
    vec3 pixel_delta_right = viewport_right / image_width;
    vec3 pixel_delta_down = viewport_down / image_height;

    point3 viewport_upper_left = camera_center - vec3(0, 0, focal_lenght) - viewport_right / 2 - viewport_down / 2;
    point3 pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_right + pixel_delta_down);

    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    std::ofstream image_file("image.ppm");

    image_file << "P3\n" << image_width << " " << image_height << "\n255\n";
    for(int i = 0; i < image_height; ++i){
        std::clog << "\rScanlines remaining: " << (image_height - i);
        for(int j = 0; j < image_width; ++j){
            point3 pixel_center = pixel00_loc + (i * pixel_delta_down) + (j * pixel_delta_right);
            vec3 ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);
            
            vec3 pixel_color = ray_color(r, world);
            write_color(image_file, pixel_color);
        }
    }
    std::clog << "\rDone                                  \n";
    image_file.close();

    return 0;
}