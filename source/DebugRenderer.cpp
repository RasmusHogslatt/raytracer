#pragma once

#include "DebugRenderer.h"
#include <Ray.h>

#include <iostream>

DebugRenderer::DebugRenderer(Parameters& params) : Integrator(&params.scene), p{ params }, samples{ 1 } {}

void DebugRenderer::Render() {
	size_t width = p.renderTexture_.getWidth();
	size_t height = p.renderTexture_.getHeight();
	glm::vec3 color = glm::vec3(0);
	glm::vec2 xyPos = glm::vec2(0);
	Ray ray;
	// Raster space [0, width] && [0, height]
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			color = glm::vec4(0); // Reset color at next pixel in pixel space
			for (int currentSample = 1; currentSample <= scene->cameras_[p.scene.activeCamera]->samples; ++currentSample) {
				// Generate a ray from pixel space
				xyPos = p.samplers_[p.activeSampler]->getSamplePosition(x, y, *p.scene.cameras_[p.scene.activeCamera], currentSample);
				ray = p.scene.cameras_[p.scene.activeCamera]->generateRay(xyPos);

				// Trace ray through scene and accumulate color
				currentRecurseDepth = maxRecursiveDepth;
				color += trace(ray);
			}
			// Average color
			color /= scene->cameras_[p.scene.activeCamera]->samples;
			color = glm::clamp(color, 0.0f, 1.0f);
			// Assign color to internal texture data
			p.renderTexture_.setPixelColor(x, y, ImVec4(color.r, color.g, color.b, 1.0f));
		}
	}
	// Update texture's data
	p.renderTexture_.updateTextureData();
}

glm::vec3 DebugRenderer::trace(Ray& ray)
{
	if (currentRecurseDepth == 0) {
		return glm::vec3(0);
	}
	--currentRecurseDepth;
	glm::vec3 localColor = glm::vec3(0); // Accumulated color
	glm::vec3 reflectedColor = glm::vec3(0); // Accumulated color
	glm::vec3 color = glm::vec3(0); // Accumulated color

	int closestObject = getClosestObjectIndex(ray);

	// Nearest intersection found
	if (closestObject != -1) {

		// Get current object material
		Material m = getMaterial(scene->primitives[closestObject]);
		float reflectionConstant = m.getSpecularStrength();
		
		float localShadingConstant = (1.0f - reflectionConstant);

		// Update ray.origin to intersection point
		ray.origin = ray.getPositionAlongRay(scene->primitives[closestObject]->t);
		// Get normal
		glm::vec3 normal = glm::normalize(ray.origin - scene->primitives[closestObject]->position_);

		// Local shading
		localColor = localShadingConstant * phongShading(scene->primitives[closestObject], normal);


		Ray reflectionRay = getReflectionRay(ray, normal);
		reflectedColor = reflectionConstant * trace(reflectionRay);
		return localColor + reflectedColor;
	}
	return localColor + reflectedColor;
	// No intersection found
}

void DebugRenderer::GUISettings()
{
	ImGui::SameLine();
	ImGui::Text(": DebugRenderer");
	Integrator::GUISettings();
}
//double for loop
// for each sample: generate ray -> trace ray += add color -> average color
