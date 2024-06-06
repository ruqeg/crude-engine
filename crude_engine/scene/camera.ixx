module;

export module crude.scene.camera;

export import crude.core.alias;
export import crude.math.matrix;
export import crude.math.fuicont;

export namespace crude::scene
{

class Camera
{
public:
  Camera();
  ~Camera() = default;
public:
  void calculateViewToClipMatrix(core::float32 fovRadians, core::float32 aspectRatio, core::float32 nearZ, core::float32 farZ);
  void calculateWorldToViewMatrix();
public:
  void setPosition(const math::Vector& newPosition);
  void setPosition(core::float32 x, core::float32 y, core::float32 z);
  void addPosition(const math::Vector& newPosition);
  void addPosition(core::float32 x, core::float32 y, core::float32 z);
  void setRotation(const math::Vector& newRotation);
  void setRotation(core::float32 x, core::float32 y, core::float32 z);
  void addRotation(const math::Vector& addedRotation);
  void addRotation(core::float32 x, core::float32 y, core::float32 z);
public:
  const math::Matrix& getWorldToClipMatrix() const { return m_worldToClipMatrix; }
  const math::Matrix& getClipToWorldMatrix() const { return m_clipToWorldMatrix; }
  const math::Matrix& getWorldToViewMatrix() const { return m_worldToViewMatrix; }
  const math::Matrix& getViewToWorldMatrix() const { return m_viewToWorldMatrix; }
  const math::Matrix& getViewToClipMatrix() const { return m_viewToClipMatrix; }
  const math::Matrix& getClipToViewMatrix() const { return m_clipToViewMatrix; }
  const math::Float4x4& getWorldToClipFloat4x4() const { return m_worldToClipFloat4x4; }
  const math::Float4x4& getClipToWorldFloat4x4() const { return m_clipToWorldFloat4x4; }
  const math::Float4x4& getWorldToViewFloat4x4() const { return m_worldToViewFloat4x4; }
  const math::Float4x4& getViewToWorldFloat4x4() const { return m_viewToWorldFloat4x4; }
  const math::Float4x4& getViewToClipFloat4x4() const { return m_viewToClipFloat4x4; }
  const math::Float4x4& getClipToViewFloat4x4() const { return m_clipToViewFloat4x4; }
  const math::Vector& getPositionVector() const { return m_positionVector; }
  const math::Float3& getPositionFloat3() const { return m_positionFloat3; }
  const math::Vector& getRotationVector() const { return m_rotationVector; }
  const math::Float3& getRotationFloat3() const { return m_rotationFloat3; }
  const math::Vector& getRightVector() const { return m_rightVector; }
  const math::Vector& getTopVector() const { return m_topVector; }
  const math::Vector& getForwardVector() const { return m_forwardVector; }
  core::float32 getFovRadians() const { return m_fovRadians; }
  core::float32 getNearZ() const { return m_nearZ;  }
  core::float32 getFarZ() const { return m_farZ; }
private:
  math::Vector m_positionVector;
  math::Vector m_rotationVector;
  math::Float3 m_positionFloat3;
  math::Float3 m_rotationFloat3;
  math::Vector m_forwardVector;
  math::Vector m_rightVector;
  math::Vector m_topVector;
private:
  math::Matrix m_worldToClipMatrix;
  math::Matrix m_clipToWorldMatrix;
  math::Matrix m_worldToViewMatrix;
  math::Matrix m_viewToWorldMatrix;
  math::Matrix m_viewToClipMatrix;
  math::Matrix m_clipToViewMatrix;
  math::Float4x4 m_worldToClipFloat4x4;
  math::Float4x4 m_clipToWorldFloat4x4;
  math::Float4x4 m_worldToViewFloat4x4;
  math::Float4x4 m_viewToWorldFloat4x4;
  math::Float4x4 m_viewToClipFloat4x4;
  math::Float4x4 m_clipToViewFloat4x4;
private:
  core::float32 m_fovRadians;
  core::float32 m_nearZ;
  core::float32 m_farZ;
private:
  const math::Vector cDefaultCameraForwardVector = math::svector::set(0.0f, 0.0f, 1.0f, 0.0f);
  const math::Vector cDefaultCameraTopVector = math::svector::set(0.0f, 1.0f, 0.0f, 0.0f);
  const math::Vector cDefaultCameraRightVector = math::svector::set(1.0f, 0.0f, 0.0f, 0.0f);

};
}
