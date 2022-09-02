#pragma once

#include <integrators/MonteCarloIntegrator.h>

MonteCarloIntegrator::MonteCarloIntegrator(Scene* scene) : Integrator(scene), maxRecursiveDepth_{ 1 }, samplerPerPixel_{ 1 }, bias_{ 0.001f } {}

void MonteCarloIntegrator::Integrate(Texture& renderTexture) {
	glm::ivec2 resolution = scene_->cameras_[scene_->activeCamera_]->resolution_;

	Ray ray;
	glm::vec3 color;
	glm::vec2 samplePosition = glm::vec2(0);
	int currentSample;

	for (int y = 0; y < resolution.y; ++y) {
		for (int x = 0; x < resolution.x; ++x) {
			color = glm::vec3(0);
			currentSample = 0;
			for (currentSample; currentSample < samplerPerPixel_; ++currentSample) {
				samplePosition = scene_->samplers_[scene_->activeSampler_]->getSamplePosition(x, y, *scene_->cameras_[scene_->activeCamera_], currentSample);
				ray = scene_->cameras_[scene_->activeCamera_]->generateRay(samplePosition);

				//trace
				
				// monte carlo
				/*
					1 - If hit object
						--- GATHERLIGHT START ---
						for(x amountOfRaySamples)
							randomDirection = getrandomdirection(normalvectorOfHitPointOnObject)
							Ray ray;
							ray.origin = intersectionPoint
							ray.direction = randomDirection
							indirectLight += traceRay(ray) // traceRay first calculates direct illumination, then use gather light after and returns sum of both
						end
						indirectLight /= amountOfRaySamples
						--- GATHERLIGHT END ---
				*/
			}

			renderTexture.setPixelColor(x, y, ImVec4(color.r, color.g, color.b, 1.0f));
		}
	}
	renderTexture.updateTextureData();
}

glm::vec3 MonteCarloIntegrator::traceRay(Ray& r) {
	/*
	vec3f castRay(Vec2f P, Vec2f N, uin32_t depth) { 
    if (depth > scene->options.maxDepth) return 0; 
    Vec2f N = ..., P = ...;  //normal and position of the shaded point 
    Vec3f directLightContrib = 0, indirectLightContrib = 0; 
    // compute direct illumination
    for (uint32_t i = 0; i < scene->nlights; ++i) { 
        Vec2f L = scene->lights[i].getLightDirection(P); 
        Vec3f L_i = scene->lights[i]->intensity * scene->lights[i]->color; 
        // we assume the surface at P is diffuse
        directLightContrib += shadowRay(P, -L) * std::max(0.f, N.dotProduct(L)) * L_i; 
    } 
    // compute indirect illumination
    float rotMat[2][2] = {{N.y, -N.x}, {N.x, N.y}};  //compute a rotation matrix 
    uint32_t N = 16; 
    for (uint32_t n = 0; n < N; ++n) { 
        // step 1: draw a random sample in the half-disk
        float theta = drand48() * M_PI; 
        float cosTheta = cos(theta); 
        float sinTheta = sin(theta); 
        // step 2: rotate the sample direction
        float sx = cosTheta  * rotMat[0][0] + sinTheta  * rotMat[0][1]; 
        float sy = cosTheta  * rotMat[1][0] + sinTheta  * rotMat[1][1]; 
        // step 3: cast the ray into the scene
        Vec3f sampleColor = castRay(P, Vec2f(sx, sy), depth + 1);  //trace a ray from P in random direction 
        // step 4 and 5: treat the return color as if it was a light (we assume our shaded surface is diffuse)
        IndirectLightContrib += sampleColor * cosTheta;  //diffuse shading = L_i * cos(N.L) 
    } 
    // step 6: divide the result of indirectLightContrib by the number of samples N (Monte Carlo integration)
    indirectLightContrib /= N; 
 
    // final result is diffuse from direct and indirect lighting multiplied by the object color at P
    return (indirectLightContrib + directLightContrib) * objectAlbedo / M_PI; 
} 
	*/
}

void MonteCarloIntegrator::GUI() {
	ImGui::Text("Monte Carlo Integrator");
}