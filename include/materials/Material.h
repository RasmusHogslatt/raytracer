#pragma once

#include <glm/glm.hpp>
#include <string>

class Material {
public:
	Material();
	~Material();
	Material(const Material& old);
	virtual Material* clone() const = 0;
	virtual void GUI() = 0;
	virtual float getFr() = 0;
public:
	glm::vec3 color_;
	float reflectance_;
	float transmittance_;
	float ior_;
	std::string name_;
	int id_;
};

// ID = 0
class Lambertian : public Material {
public:
	Lambertian();
	~Lambertian();
	Lambertian(const Lambertian& old);
	Lambertian* clone() const;
	void GUI();
	float getFr();
};

// ID = 1
class Mirror : public Material {
public:
	Mirror();
	~Mirror();
	Mirror(const Mirror& old);
	Mirror* clone() const;
	void GUI();
	float getFr();
};

// ID = 2
class Glass : public Material {
public:
	Glass();
	~Glass();
	Glass(const Glass& old);
	Glass* clone() const;
	void GUI();
	float getFr();
};