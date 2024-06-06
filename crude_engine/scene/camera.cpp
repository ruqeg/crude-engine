module crude.scene.camera;

import crude.math.convert;

namespace crude::scene
{

Camera::Camera()
{
  m_positionFloat3 = math::Float3(0.0f, 0.0f, 0.0f);
  m_positionVector = math::loadFloat3(m_positionFloat3);
  m_rotationFloat3 = math::Float3(0.0f, 0.0f, 0.0f);
  m_rotationVector = math::loadFloat3(m_rotationFloat3);
  calculateMatrix();
}

void Camera::calculateProjectionMatrix(core::float32 fovRadians, core::float32 aspectRatio, core::float32 nearZ, core::float32 farZ)
{
  m_farZ = farZ;
  m_nearZ = nearZ;
  m_fovRadians = fovRadians;
  m_viewToClipMatrix = math::smatrix::perspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ);
  m_clipToViewMatrix = math::smatrix::inverse(m_viewToClipMatrix);
  math::storeFloat4x4(m_viewToClipFloat4x4, m_viewToClipMatrix);
  math::storeFloat4x4(m_clipToViewFloat4x4, m_clipToViewMatrix);
}

void Camera::calculateMatrix()
{
  const math::Matrix cameraRotationMatrix = math::smatrix::rotationRollPitchYawFromVector(m_rotationVector);
  const math::Vector cameraDirection = math::svector::transformCoord3(cDefaultCameraForwardVector, cameraRotationMatrix);
  const math::Vector cameraTarget = math::svector::add(m_positionVector, cameraDirection);
  const math::Vector upDir = math::svector::transformCoord3(cDefaultCameraTopVector, cameraRotationMatrix);
  m_worldToViewMatrix = math::smatrix::lookAtLH(m_positionVector, cameraTarget, upDir);
  m_viewToWorldMatrix = math::smatrix::inverse(m_worldToViewMatrix);
  math::storeFloat4x4(m_worldToViewFloat4x4, m_worldToViewMatrix);
  math::storeFloat4x4(m_viewToWorldFloat4x4, m_viewToWorldMatrix);

  m_forwardVector = math::svector::transformCoord3(cDefaultCameraForwardVector, cameraRotationMatrix);
  m_topVector = math::svector::transformCoord3(cDefaultCameraTopVector, cameraRotationMatrix);
  m_rightVector = math::svector::transformCoord3(cDefaultCameraRightVector, cameraRotationMatrix);

  m_worldToClipMatrix = math::smatrix::multiply(m_worldToViewMatrix, m_viewToClipMatrix);
  m_clipToWorldMatrix = math::smatrix::inverse(m_worldToClipMatrix);
  math::storeFloat4x4(m_worldToClipFloat4x4, m_worldToClipMatrix);
  math::storeFloat4x4(m_clipToWorldFloat4x4, m_clipToWorldMatrix);
}

void Camera::setPosition(const math::Vector& newPosition)
{
  math::storeFloat3(m_positionFloat3, newPosition);
  m_positionVector = newPosition;
}

void Camera::setPosition(core::float32 x, core::float32 y, core::float32 z)
{
  m_positionFloat3 = math::Float3(x, y, z);
  m_positionVector = math::loadFloat3(m_positionFloat3);
}

void Camera::addPosition(const math::Vector& addedPosition)
{
  m_positionVector = math::svector::add(m_positionVector, addedPosition);
  math::storeFloat3(m_positionFloat3, m_positionVector);
}

void Camera::addPosition(core::float32 x, core::float32 y, core::float32 z)
{
  m_positionFloat3.x += x;
  m_positionFloat3.y += y;
  m_positionFloat3.z += z;
  m_positionVector = math::loadFloat3(m_positionFloat3);
}

void Camera::setRotation(const math::Vector& newRotation)
{
  math::storeFloat3(m_rotationFloat3, newRotation);
  m_rotationVector = newRotation;
}

void Camera::setRotation(core::float32 x, core::float32 y, core::float32 z)
{
  m_rotationFloat3 = math::Float3(x, y, z);
  m_rotationVector = math::loadFloat3(m_rotationFloat3);
}

void Camera::addRotation(const math::Vector& addedRotation)
{
  m_rotationVector = math::svector::add(m_rotationVector, addedRotation);
  math::storeFloat3(m_rotationFloat3, m_rotationVector);
}

void Camera::addRotation(core::float32 x, core::float32 y, core::float32 z)
{
  m_rotationFloat3.x += x;
  m_rotationFloat3.y += y;
  m_rotationFloat3.z += z;
  m_rotationVector = math::loadFloat3(m_rotationFloat3);
}

}