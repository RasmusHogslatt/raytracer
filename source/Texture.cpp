#pragma once

#include "Texture.h"
#include <iostream>

Texture::Texture() : data_{ nullptr }, width_{ 0 }, height_{ 0 }, format_{ GL_RGBA } {
	glGenTextures(1, &ID_);
}

Texture::~Texture() {
	delete[]data_;
}

void Texture::setPixelColor(size_t x, size_t y, ImVec4 color) {
	size_t pos = y * width_ + x;
	pos <<= 2;
	data_[pos] = (unsigned char)(color.x * 255);
	data_[pos + 1] = (unsigned char)(color.y * 255);
	data_[pos + 2] = (unsigned char)(color.z * 255);
	data_[pos + 3] = (unsigned char)(color.w * 255);
}

void Texture::clearTextureColor(ImVec4 color) {
	for (uint32_t y = 0; y < height_; ++y) {
		for (uint32_t x = 0; x < width_; ++x) {
			setPixelColor(x, y, color);
		}
	}
}

void Texture::createTexture(size_t width, size_t height) {
	glBindTexture(GL_TEXTURE_2D, ID_);

	width_ = width;
	height_ = height;
	glBindTexture(GL_TEXTURE_2D, ID_);

	// Set texture interpolation methods for minification and magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Set texture clamping method
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	data_ = new unsigned char[(size_t)width_ * height_ * 4];
	clearTextureColor();
	updateTextureData();

	// Create the texture
	glTexImage2D(GL_TEXTURE_2D,
		0,
		format_,
		static_cast<GLsizei>(width_),
		static_cast<GLsizei>(height_),
		0,
		format_,
		GL_UNSIGNED_BYTE,
		data_);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::updateTextureData() {
	glBindTexture(GL_TEXTURE_2D, ID_);
	// Create the texture
	glTexImage2D(GL_TEXTURE_2D,
		0,
		format_,
		static_cast<GLsizei>(width_),
		static_cast<GLsizei>(height_),
		0,
		format_,
		GL_UNSIGNED_BYTE,
		data_);

	glBindTexture(GL_TEXTURE_2D, 0);
}

size_t Texture::getWidth() const {
	return width_;
}

ImVec2 Texture::getDimensions() const {
	return ImVec2(static_cast<float>(width_), static_cast<float>(height_));
}

size_t Texture::getHeight() const {
	return height_;
}

void Texture::setData(unsigned char* newData) {
	data_ = newData;
}