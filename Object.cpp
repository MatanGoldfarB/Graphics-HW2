#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <limits>
#include "Object.h"
#include "Vector.h"
#include "Scene.h"
#include "Intersection.h"


Object :: ~Object() {}

//plane
Plane :: ~Plane() {}
Plane::Plane(const Vector& n, float dist, const Vector& color, float s, bool t, bool r)
    : colors(color), shininess(s), reflective(r), transparent(t) {
    
    float magnitude = n.magnitude();
    if (magnitude > 1e-6) {
        normal = n / magnitude; // Normalize the normal vector
        d = dist / magnitude;   // Adjust 
    } else {
        // Handle the edge case where the normal vector's magnitude is zero
        normal = Vector(0, 0, 0);
        d = 0;
        std::cerr << "Warning: Plane normal vector magnitude is zero. Plane may be invalid." << std::endl;
    }
}
Intersection Plane::intersect(const Ray& ray) {
    float denominator = normal.dot(ray.direction);

    // Check if the ray is parallel to the plane
    if (std::abs(denominator) < 1e-6) {
        return Intersection(); 
    }
    float t = -(normal.dot(ray.origin) + d) / denominator;
    // Check if the intersection is behind the ray's origin
    if (t < 1e-6) {
        return Intersection();
    }

    Vector intersectionPoint = ray.origin + ray.direction * t;
    Vector baseColor = colors;
    Vector color = checkerboardColor(baseColor, intersectionPoint);
    return Intersection(true, t, intersectionPoint, normal.normalize(), color, shininess, reflective, transparent);
}


void Plane :: setColor(const Vector& newColors, const float newShiness) {
    if (!reflective && !transparent )
    {
        colors = newColors;
        shininess = newShiness;
    }
    
}

Vector Plane :: checkerboardColor(const Vector& baseColor, const Vector& hitPoint)  {
    float scaleParameter = 0.5f;
    float checkerboard = 0;

    if (hitPoint.x < 0) {
        checkerboard += std::floor((0.5f - hitPoint.x) / scaleParameter);
    } else {
        checkerboard += std::floor(hitPoint.x / scaleParameter);
    }
    if (hitPoint.y < 0) {
        checkerboard += std::floor((0.5f - hitPoint.y) / scaleParameter);
    } else {
        checkerboard += std::floor(hitPoint.y / scaleParameter);
    }

    checkerboard = (checkerboard * 0.5f) - static_cast<int>(checkerboard * 0.5f);
    checkerboard *= 2;

    if (checkerboard > 0.5f) {
        return baseColor * 0.5f; 
    }
    return baseColor; 
    }



// Sphere 
Sphere :: ~Sphere(){}

Sphere :: Sphere(const Vector& c, float radius, const Vector& color, float s, bool t, bool reflective)
    : center(c), radius(radius), colors(color), shininess(s), reflective(reflective), transparent(t) {}

Intersection Sphere::intersect(const Ray& ray) {
    Vector rayOrigin = ray.origin; 
    Vector rayDirection = ray.direction; 
    Vector sphereCenter = center; 
    Vector toCenter = sphereCenter - rayOrigin; 

    float projectionLength = toCenter.dot(rayDirection); 
    float perpendicularDist2 = toCenter.dot(toCenter) - projectionLength * projectionLength; 

    if (perpendicularDist2 > radius * radius) {
        return Intersection(); 
    }

    float halfChord = std::sqrt(radius * radius - perpendicularDist2); 
    float t0 = projectionLength - halfChord;
    float t1 = projectionLength + halfChord;

    // Find the closest positive t
    float t = std::numeric_limits<float>::infinity();
    if (t0 > 1e-6 && t1 > 1e-6) {
        t = std::min(t0, t1);
    } else if (t1 > 1e-6) {
        t = t1;
    } else if (t0 > 1e-6) {
        t = t0;
    }

    if (t == std::numeric_limits<float>::infinity()) {
        return Intersection(); 
    }

    Vector intersectionPoint = rayOrigin + rayDirection * t; 
    Vector normal = (intersectionPoint - sphereCenter).normalize(); 

    return Intersection(true, t, intersectionPoint, normal, colors, shininess, reflective, transparent);
}


void Sphere :: setColor(const Vector& newColors, const float newShiness) {
    if (!reflective && !transparent )
    {
        colors = newColors;
        shininess = newShiness;
    }
    
}




