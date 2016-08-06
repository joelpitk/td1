#include "camera.hh"

Camera::Camera(CL_Vec2f position) : position(position)
{
}

const CL_Vec2f& Camera::getPosition() const
{
    return position;
}

void Camera::setPosition(CL_Vec2f position)
{
    this->position = position;
}

void Camera::move(float x, float y)
{
    this->position += CL_Vec2f(x, y);
}
