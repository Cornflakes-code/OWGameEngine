#pragma once

#include "../OWEngine/OWEngine.h"

/*
//#include <Core/CoreDLL.h>
//#include <Core/World/CoordinateSystem.h>
//#include <Foundation/Math/Mat4.h>
//#include <Foundation/Reflection/Reflection.h>
//#include <Foundation/Types/SharedPtr.h>
//#include <Foundation/Types/UniquePtr.h>

/// \brief Specifies in which mode this camera is configured.
struct OWENGINE_API ezCameraMode
{
  enum Enum : unsigned char
  {
    None,                 ///< Not initialized
    PerspectiveFixedFovX, ///< Perspective camera, the fov for X is fixed, Y depends on the aspect ratio
    PerspectiveFixedFovY, ///< Perspective camera, the fov for Y is fixed, X depends on the aspect ratio
    OrthoFixedWidth,      ///< Orthographic camera, the width is fixed, the height depends on the aspect ratio
    OrthoFixedHeight,     ///< Orthographic camera, the height is fixed, the width depends on the aspect ratio
    Stereo,               ///< A stereo camera with view/projection matrices provided by an HMD.
    Default = PerspectiveFixedFovY
  };
};

struct ezBasisAxis
{
    /// \brief An enum that allows to select on of the six main axis (positive / negative)
    enum Enum : unsigned char
    {
        PositiveX,
        PositiveY,
        PositiveZ,
        NegativeX,
        NegativeY,
        NegativeZ,

        Default = PositiveX
    };


/// \brief Determines left or right eye of a stereo camera.
///
/// As a general rule, this parameter does not matter for mono-scopic cameras and will always return the same value.
enum class ezCameraEye
{
  Left,
  Right,
  // Two eyes should be enough for everyone.
};

/// \brief A camera class that stores the orientation and some basic camera settings.
class OWENGINE_API ezCamera
{
public:
  ezCamera();

  /// \brief Allows to specify a different coordinate system in which the camera input and output coordinates are given.
  ///
  /// The default in z is forward = PositiveX, right = PositiveY, Up = PositiveZ.
  void SetCoordinateSystem(ezBasisAxis::Enum forwardAxis, ezBasisAxis::Enum rightAxis, ezBasisAxis::Enum axis);

  /// \brief Returns the position of the camera that should be used for rendering etc.
  glm::vec3 GetPosition(ezCameraEye eye = ezCameraEye::Left) const;

  /// \brief Returns the forwards vector that should be used for rendering etc.
  glm::vec3 GetDirForwards(ezCameraEye eye = ezCameraEye::Left) const;

  /// \brief Returns the up vector that should be used for rendering etc.
  glm::vec3 GetDirUp(ezCameraEye eye = ezCameraEye::Left) const;

  /// \brief Returns the right vector that should be used for rendering etc.
  glm::vec3 GetDirRight(ezCameraEye eye = ezCameraEye::Left) const;

  /// \brief Returns the horizontal FOV.
  ///
  /// Works only with ezCameraMode::PerspectiveFixedFovX and ezCameraMode::PerspectiveFixedFovY
  float GetFovX(float fAspectRatioWidthDivHeight) const;

  /// \brief Returns the vertical FOV.
  ///
  /// Works only with ezCameraMode::PerspectiveFixedFovX and ezCameraMode::PerspectiveFixedFovY
  float GetFovY(float fAspectRatioWidthDivHeight) const;

  /// \brief Returns the horizontal dimension for an orthographic view.
  ///
  /// Works only with ezCameraMode::OrthoFixedWidth and ezCameraMode::OrthoFixedWidth
  float GetDimensionX(float fAspectRatioWidthDivHeight) const;

  /// \brief Returns the vertical dimension for an orthographic view.
  ///
  /// Works only with ezCameraMode::OrthoFixedWidth and ezCameraMode::OrthoFixedWidth
  float GetDimensionY(float fAspectRatioWidthDivHeight) const;

  /// \brief Returns the average camera position.
  ///
  /// For all cameras execpt Stereo cameras this is identical to GetPosition()
  inline glm::vec3 GetCenterPosition() const
  {
      if (m_Mode == ezCameraMode::Stereo)
          return (GetPosition(ezCameraEye::Left) + GetPosition(ezCameraEye::Right)) * 0.5f;
      else
          return GetPosition();
  }

  /// \brief Returns the average forwards vector.
  ///
  /// For all cameras execpt Stereo cameras this is identical to GetDirForwards()
  inline glm::vec3 GetCenterDirForwards() const
  {
      if (m_Mode == ezCameraMode::Stereo)
          return glm::normalize(GetDirForwards(ezCameraEye::Left) + GetDirForwards(ezCameraEye::Right));
      else
          return GetDirForwards();
  }


  /// \brief Returns the average up vector.
  ///
  /// For all cameras execpt Stereo cameras this is identical to GetDirUp()
  inline glm::vec3 GetCenterDirUp() const
  {
      if (m_Mode == ezCameraMode::Stereo)
          return glm::normalize(GetDirUp(ezCameraEye::Left) + GetDirUp(ezCameraEye::Right));
      else
          return GetDirUp();
  }

  /// \brief Returns the average right vector.
  ///
  /// For all cameras execpt Stereo cameras this is identical to GetDirRight()
  inline glm::vec3 GetCenterDirRight() const
  {
      if (m_Mode == ezCameraMode::Stereo)
          return glm::normalize(GetDirRight(ezCameraEye::Left) + GetDirRight(ezCameraEye::Right));
      else
          return GetDirRight();
  }

  /// \brief Returns the near plane distance that was passed to SetCameraProjectionAndMode().
  float GetNearPlane() const
  {
      return m_fNearPlane;
  }

  /// \brief Returns the far plane distance that was passed to SetCameraProjectionAndMode().
  float GetFarPlane() const
  {
      return m_fFarPlane;
  }

  /// \brief Specifies the mode and the projection settings that this camera uses.
  ///
  /// \param fFovOrDim
  ///   Fov X/Y in degree or width/height (depending on Mode).
  void SetCameraMode(ezCameraMode::Enum mode, float fFovOrDim, float fNearPlane, float fFarPlane);

  /// Sets the camera mode to stereo and specifies projection matrices directly.
  ///
  /// \param fAspectRatio
  ///   These stereo projection matrices will only be returned by getProjectionMatrix for the given aspectRatio.
  void SetStereoProjection(const glm::mat4& mProjectionLeftEye, const glm::mat4& mProjectionRightEye, float fAspectRatioWidthDivHeight);

  /// \brief Returns the fFovOrDim parameter that was passed to SetCameraProjectionAndMode().
  float GetFovOrDim() const
  {
      return m_fFovOrDim;
  }

  /// \brief Returns the current camera mode.
  ezCameraMode::Enum GetCameraMode() const
  {
      return m_Mode;
  }


  bool IsPerspective() const
  {
      return m_Mode == ezCameraMode::PerspectiveFixedFovX || m_Mode == ezCameraMode::PerspectiveFixedFovY ||
          m_Mode == ezCameraMode::Stereo; // All HMD stereo cameras are perspective!
  }

  bool IsOrthographic() const
  {
      return m_Mode == ezCameraMode::OrthoFixedWidth || m_Mode == ezCameraMode::OrthoFixedHeight;
  }

  /// \brief Whether this is a stereoscopic camera.
  bool IsStereoscopic() const
  {
      return m_Mode == ezCameraMode::Stereo;
  }

  /// \brief Sets the view matrix directly.
  ///
  /// Works with all camera types. Position- and direction- getter/setter will work as usual.
  void SetViewMatrix(const glm::mat4& mLookAtMatrix, ezCameraEye eye = ezCameraEye::Left);

  /// \brief Repositions the camera such that it looks at the given target position.
  ///
  /// Not supported for stereo cameras.
  void LookAt(const glm::vec3& vCameraPos, const glm::vec3& vTargetPos, const glm::vec3& vUp);

  /// \brief Moves the camera in its local space along the forward/right/up directions of the coordinate system.
  ///
  /// Not supported for stereo cameras.
  void MoveLocally(float fForward, float fRight, float fUp);

  /// \brief Moves the camera in global space along the forward/right/up directions of the coordinate system.
  ///
  /// Not supported for stereo cameras.
  void MoveGlobally(float fForward, float fRight, float fUp);

  /// \brief Rotates the camera around the forward, right and up axis in its own local space.
  ///
  /// Rotate around \a rightAxis for looking up/down. \forwardAxis is roll. For turning left/right use RotateGlobally().
  /// Not supported for stereo cameras.
  void RotateLocally(float forwardAxis, float rightAxis, float axis);

  /// \brief Rotates the camera around the forward, right and up axis of the coordinate system in global space.
  ///
  /// Rotate around Z for turning the camera left/right.
  /// Not supported for stereo cameras.
  void RotateGlobally(float forwardAxis, float rightAxis, float axis);

  /// \brief Returns the view matrix for the given eye.
  ///
  /// \note The view matrix is given in OpenGL convention.
  const glm::mat4& GetViewMatrix(ezCameraEye eye) const
  {
      return m_mViewMatrix[static_cast<int>(eye)];
  }

  /// \brief Calculates the projection matrix from the current camera properties and stores it in out_projectionMatrix.
  ///
  /// If the camera is stereo and the given aspect ratio is close to the aspect ratio passed in SetStereoProjection,
  /// the matrix set in SetStereoProjection will be used.
  void GetProjectionMatrix(float fAspectRatioWidthDivHeight, glm::mat4& out_mProjectionMatrix, ezCameraEye eye = ezCameraEye::Left) const;

  float GetExposure() const
  {
      return m_fExposure;
  }

  void SetExposure(float fExposure)
  {
      m_fExposure = fExposure;
  }


  /// \brief Returns a counter that is increased every time the camera settings are modified.
  ///
  /// The camera settings are used to compute the projection matrix. This counter can be used to determine whether the projection matrix
  /// has changed and thus whether cached values need to be updated.
  int GetSettingsModificationCounter() const { return m_uiSettingsModificationCounter; }

  /// \brief Returns a counter that is increased every time the camera orientation is modified.
  ///
  /// The camera orientation is used to compute the view matrix. This counter can be used to determine whether the view matrix
  /// has changed and thus whether cached values need to be updated.
  int GetOrientationModificationCounter() const { return m_uiOrientationModificationCounter; }

private:
  /// \brief This function is called whenever the camera position or rotation changed.
  void CameraOrientationChanged() { ++m_uiOrientationModificationCounter; }

  /// \brief This function is called when the camera mode or projection changes (e.g. SetCameraProjectionAndMode was called).
  void CameraSettingsChanged();

  /// \brief This function is called by RotateLocally() and RotateGlobally() BEFORE the values are applied,
  /// and allows to adjust them (e.g. for limiting how far the camera can rotate).
  void ClampRotationAngles(bool bLocalSpace, float& forwardAxis, float& rightAxis, float& upAxis);

  glm::vec3 InternalGetPosition(ezCameraEye eye = ezCameraEye::Left) const;
  glm::vec3 InternalGetDirForwards(ezCameraEye eye = ezCameraEye::Left) const;
  glm::vec3 InternalGetDirUp(ezCameraEye eye = ezCameraEye::Left) const;
  glm::vec3 InternalGetDirRight(ezCameraEye eye = ezCameraEye::Left) const;

  float m_fNearPlane = 0.1f;
  float m_fFarPlane = 1000.0f;

  ezCameraMode::Enum m_Mode = ezCameraMode::None;

  float m_fFovOrDim = 90.0f;

  float m_fExposure = 1.0f;

  glm::vec3 m_vCameraPosition[2];
  glm::mat4 m_mViewMatrix[2];

  /// If the camera mode is stereo and the aspect ratio given in getProjectio is close to this value, one of the stereo projection matrices
  /// is returned.
  float m_fAspectOfPrecomputedStereoProjection = -1.0;
  glm::mat4 m_mStereoProjectionMatrix[2];

  int m_uiSettingsModificationCounter = 0;
  int m_uiOrientationModificationCounter = 0;

  
  glm::vec3 MapExternalToInternal(const glm::vec3& v) const;
  glm::vec3 MapInternalToExternal(const glm::vec3& v) const;
}
*/
