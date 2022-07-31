#pragma once

#include <Renderer.h>
#include <glm/vec2.hpp>
#include <glm/common.hpp>
#include <glm/glm.hpp>

Renderer::Renderer(Texture& texture) : image_{ texture }, width_{ texture.getWidth() }, height_{ texture.getHeight() }
{
	//scene_.push_back(Sphere());
}

//glm::vec4 Renderer::trace(glm::vec3& rayOrigo, glm::vec3& rayDirection, const int& depth)
//{
//    float tnear = INFINITY;
//    const Sphere* sphere = NULL;
//    // find intersection of this ray with the sphere in the scene
//    for (unsigned i = 0; i < scene_.size(); ++i) {
//        float t0 = INFINITY, t1 = INFINITY;
//        if (scene_[i].intersect(rayOrigo, rayDirection, t0, t1)) {
//            if (t0 < 0) t0 = t1;
//            if (t0 < tnear) {
//                tnear = t0;
//                sphere = &spheres[i];
//            }
//        }
//    }
//    // if there's no intersection return black or background color
//    if (!sphere) return Vec3f(2);
//    Vec3f surfaceColor = 0;  //color of the ray/surfaceof the object intersected by the ray 
//    Vec3f phit = rayorig + raydir * tnear;  //point of intersection 
//    Vec3f nhit = phit - sphere->center;  //normal at the intersection point 
//    nhit.normalize();  //normalize normal direction 
//    // If the normal and the view direction are not opposite to each other
//    // reverse the normal direction. That also means we are inside the sphere so set
//    // the inside bool to true. Finally reverse the sign of IdotN which we want
//    // positive.
//    float bias = 1e-4;  //add some bias to the point from which we will be tracing 
//    bool inside = false;
//    if (raydir.dot(nhit) > 0) nhit = -nhit, inside = true;
//    if ((sphere->transparency > 0 || sphere->reflection > 0) && depth < MAX_RAY_DEPTH) {
//        float facingratio = -raydir.dot(nhit);
//        // change the mix value to tweak the effect
//        float fresneleffect = mix(pow(1 - facingratio, 3), 1, 0.1);
//        // compute reflection direction (not need to normalize because all vectors
//        // are already normalized)
//        Vec3f refldir = raydir - nhit * 2 * raydir.dot(nhit);
//        refldir.normalize();
//        Vec3f reflection = trace(phit + nhit * bias, refldir, spheres, depth + 1);
//        Vec3f refraction = 0;
//        // if the sphere is also transparent compute refraction ray (transmission)
//        if (sphere->transparency) {
//            float ior = 1.1, eta = (inside) ? ior : 1 / ior;  //are we inside or outside the surface? 
//            float cosi = -nhit.dot(raydir);
//            float k = 1 - eta * eta * (1 - cosi * cosi);
//            Vec3f refrdir = raydir * eta + nhit * (eta * cosi - sqrt(k));
//            refrdir.normalize();
//            refraction = trace(phit - nhit * bias, refrdir, spheres, depth + 1);
//        }
//        // the result is a mix of reflection and refraction (if the sphere is transparent)
//        surfaceColor = (
//            reflection * fresneleffect +
//            refraction * (1 - fresneleffect) * sphere->transparency) * sphere->surfaceColor;
//    }
//    else {
//        // it's a diffuse object, no need to raytrace any further
//        for (unsigned i = 0; i < spheres.size(); ++i) {
//            if (spheres[i].emissionColor.x > 0) {
//                // this is a light
//                Vec3f transmission = 1;
//                Vec3f lightDirection = spheres[i].center - phit;
//                lightDirection.normalize();
//                for (unsigned j = 0; j < spheres.size(); ++j) {
//                    if (i != j) {
//                        float t0, t1;
//                        if (spheres[j].intersect(phit + nhit * bias, lightDirection, t0, t1)) {
//                            transmission = 0;
//                            break;
//                        }
//                    }
//                }
//                surfaceColor += sphere->surfaceColor * transmission *
//                    std::max(float(0), nhit.dot(lightDirection)) * spheres[i].emissionColor;
//            }
//        }
//    }
//
//	return glm::vec4();
//}

void Renderer::Render()
{
	float aspectRatio = (float)width_ / (float)height_;
	for (uint32_t y = 0; y < height_; ++y)
	{
		for (uint32_t x = 0; x < width_; ++x)
		{
			glm::vec2 coord = { (2.0f * ((float)x + 0.5) * (1.0 / (float)width_) - 1.0f) * aspectRatio, (1.0f - 2.0f * (((float)y + 0.5f) * (1.0f / (float)height_))) };
			glm::vec3 rayDirection = glm::vec3(coord, -1.0f);
			rayDirection = normalize(rayDirection);
			//glm::vec4 result = trace(cameraPosition_, rayDirection, 0);

			image_.setPixelColor(x, y, ImVec4(x, y, x, 1));
			//image_.data_[x + y * width_] = 0.5;
		}
	}
}

glm::vec3 Renderer::getInitialRay()
{
	return glm::vec3(0);
}



unsigned char* Renderer::getImageTextureData() const
{
	return image_.data_;
}