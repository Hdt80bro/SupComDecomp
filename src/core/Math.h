#include "gpggal/Math.h"
#include "Wm3Vector3.h"

namespace Moho {

struct Vector4f
{
    float x, y, z, w;

    Vector4f(); // MohoEngine.dll
    Vector4f(const Moho::Vector4f &); // MohoEngine.dll
    Vector4f(float, float, float, float); // MohoEngine.dll
    Vector4f(Wm3::Vector3f, float); // MohoEngine.dll
    Vector4f(const float *); // MohoEngine.dll

    float *Raw(); // MohoEngine.dll
    const float *Raw() const; // MohoEngine.dll
    float &operator[](unsigned int); // MohoEngine.dll
    float operator[](unsigned int) const; // MohoEngine.dll
    float &X(); // MohoEngine.dll
    float &Y(); // MohoEngine.dll
    float &Z(); // MohoEngine.dll
    float &W(); // MohoEngine.dll
    float X() const; // MohoEngine.dll
    float Y() const; // MohoEngine.dll
    float Z() const; // MohoEngine.dll
    float W() const; // MohoEngine.dll

    Moho::Vector4f &operator=(const Moho::Vector4f &); // MohoEngine.dll
    Moho::Vector4f &operator+=(const Moho::Vector4f &); // MohoEngine.dll
    Moho::Vector4f &operator-=(const Moho::Vector4f &); // MohoEngine.dll
    Moho::Vector4f &operator*=(const Moho::Vector4f &); // MohoEngine.dll
    Moho::Vector4f &operator/=(const Moho::Vector4f &); // MohoEngine.dll
    Moho::Vector4f &operator*=(float); // MohoEngine.dll
    Moho::Vector4f &operator/=(float); // MohoEngine.dll

    float Length() const; // MohoEngine.dll
    float SquaredLength() const; // MohoEngine.dll
    float Normalize(); // MohoEngine.dll
};

struct VEuler3
{

};

struct VAxes3
{
    Wm3::Vector3f mAxes[3];
};

struct VMatrix4
{
    Moho::Vector4f d[4];
};

struct VTransform
{
    Wm3::Quaternionf orient;
    Wm3::Vector3f pos;
};

struct SCoordsVec2
{
    float x, z;
};

template<class T>
const T& Zeroed() {
    
}

template<class T>
const T& Invalid() {
    
}

std::string ToString(const Wm3::IVector2i &) {
    
}

std::string ToString(const Wm3::IVector3i &) {
    
}

std::string ToString(const Wm3::Vector2f &) {
    
}

std::string ToString(const Wm3::Vector3f &) {
    
}

std::string ToString(const Moho::Vector4f &) {
    
}

std::string ToString(const Wm3::Quaternionf &) {
    
}

std::string ToString(const Moho::VEuler3 &) {
    
}

std::string ToString(const Moho::VAxes3 &) {
    
}

std::string ToString(const Moho::VTransform &) {
    
}

}
