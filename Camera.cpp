#include "Camera.hpp"

Camera::Camera()
	: Camera{ radians(90.0), 1.0, 0.1 }
{
}

Camera::Camera(double fov, double aspect, double nearClip)
	: Camera{ fov, aspect, nearClip, { 0.0, 0.0, 0.0 }, 0.0, 0.0 }
{
}

Camera::Camera(double fov, double aspect, double nearClip, Vector3 position, double yaw, double pitch)
	: fov{ fov }, perspective{ tan(fov / 2.0) }, aspect{ aspect }, nearClip{ nearClip }, position{ position }, yaw{ yaw }, pitch{ pitch },
	combinedTransform{ &positionTransform, &yawTransform, &pitchTransform }
{
    setPosition(position);
    setYaw(yaw);
    setPitch(pitch);
}

void Camera::setFov(double fov)
{
	this->fov = fov;
	perspective = tan(fov / 2.0);
}

void Camera::setAspect(double aspect)
{
	this->aspect = aspect;
}

void Camera::setNearClip(double nearClip)
{
	this->nearClip = nearClip;
}

void Camera::setPosition(Vector3 position)
{
	this->position = position;
	position.scl(-1.0);
	positionTransform = Translate{ position };
}

void Camera::setYaw(double yaw)
{
	this->yaw = yaw;
	yawTransform = Rotate{ Rotate::Axis::Y, -yaw };
}

void Camera::setPitch(double pitch)
{
	this->pitch = pitch;
	limitPitch();
	pitchTransform = Rotate{ Rotate::Axis::X, -pitch };
}

void Camera::translate(Vector3 translation)
{
	position.add(translation);
	Vector3 pos(position);
	pos.scl(-1.0);
	positionTransform = Translate{ pos };
}

void Camera::rotateYaw(double yaw)
{
	this->yaw += yaw;
	yawTransform = Rotate{ Rotate::Axis::Y, -this->yaw };
}

void Camera::rotatePitch(double pitch)
{
	this->pitch += pitch;
	limitPitch();
	pitchTransform = Rotate{ Rotate::Axis::X, -this->pitch };
}

double Camera::getFov() const
{
	return fov;
}

double Camera::getPerspective() const
{
	return perspective;
}

double Camera::getAspect() const
{
	return aspect;
}

double Camera::getNearClip() const
{
	return nearClip;
}

Vector3 Camera::getPosition() const
{
	return position;
}

double Camera::getYaw() const
{
	return yaw;
}

double Camera::getPitch() const
{
	return pitch;
}

const Transform& Camera::getTransform() const
{
	return combinedTransform;
}

Vector3 Camera::getForwardVec() const
{
	Rotate pitchTransform{ Rotate::Axis::X, pitch };
	Rotate yawTransform{ Rotate::Axis::Y, yaw };
	return yawTransform.apply(pitchTransform.apply(Vector3{ 0.0, 0.0, -1.0 }));
}

Vector3 Camera::getRightVec() const
{
    Vector3 topVec{ 0.0, 1.0, 0.0 };
    Vector3 rightVec = getForwardVec().cross(topVec);
    rightVec.norm();
    return rightVec;
}

Vector3 Camera::getUpVec() const
{
    Vector3 upVec = getRightVec().cross(getForwardVec());
    upVec.norm();
    return upVec;
}

Vector3 Camera::getFrontVec() const
{
    Vector3 frontVec = getForwardVec();
    frontVec.y = 0.0;
    frontVec.norm();
    return frontVec;
}

void Camera::limitPitch()
{
    double ninety = radians(89.9);
    if (pitch > ninety)
        pitch = ninety;
    if (pitch < -ninety)
        pitch = -ninety;
}
