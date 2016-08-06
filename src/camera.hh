#ifndef CAMERA_HH
#define CAMERA_HH

#include <ClanLib/core.h>

class Camera
{
public:
    Camera(CL_Vec2f position = CL_Vec2f());

    // Gets the position of the camera
    const CL_Vec2f& getPosition() const;

    // Sets the position of the camera
    void setPosition(CL_Vec2f position);

    // Moves the camera by the given offset
    void move(float x, float y);

private:
    CL_Vec2f position;
};

#endif
