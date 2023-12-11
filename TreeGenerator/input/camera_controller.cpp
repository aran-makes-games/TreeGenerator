#include "camera_controller.h"

#include <glm/glm.hpp>
#include <glm/gtx/common.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../graphics/common/renderer.h"

namespace tree_generator
{
	namespace
	{
		// Setting the horizontal angle range to strictly positive makes the
		// math a bit more elegant when the camera loops around.
		constexpr float kMaxHorizontalAngle = 360.0f;

		// We stop just below 90 degrees to keep the camera from flipping.
		constexpr float kMaxVerticalAngle = 89.0f;

		// Only update the camera position if it has at least this much
		// movement along any axis. This ensures we only send data to the GPU
		// when there has been a change.
		constexpr float kMinUpdateSpeed = 0.001f;
	}

	CameraController::CameraController(Renderer* renderer) :
		CameraController(renderer, MovementSettings())
	{
	}
	
	CameraController::CameraController(
		Renderer* renderer, MovementSettings movementSettings) :
		renderer_(renderer),
		movementSettings_(movementSettings)
	{
		UpdateCameraPosition();
	}

	void CameraController::Update(double elapsedTime)
	{
		bool wasPositionUpdated = false;
		if (glm::abs(currentMovement_.horizontalRotationVelocity) > kMinUpdateSpeed)
		{
			currentPosition_.horizontalAngle += (
				currentMovement_.horizontalRotationVelocity * elapsedTime);
			currentPosition_.horizontalAngle = glm::fmod(
				currentPosition_.horizontalAngle, kMaxHorizontalAngle);
			if (currentPosition_.horizontalAngle < -kMaxHorizontalAngle)
			{
				currentPosition_.horizontalAngle += kMaxHorizontalAngle;
			}
			wasPositionUpdated = true;
		}

		if (glm::abs(currentMovement_.verticalRotationVelocity) > kMinUpdateSpeed)
		{
			currentPosition_.verticalAngle += (
				currentMovement_.verticalRotationVelocity * elapsedTime);
			if (currentPosition_.verticalAngle > kMaxVerticalAngle)
			{
				currentPosition_.verticalAngle = kMaxVerticalAngle;
			}
			else if (currentPosition_.verticalAngle < -kMaxVerticalAngle)
			{
				currentPosition_.verticalAngle = -kMaxVerticalAngle;
			}
			wasPositionUpdated = true;
		}

		if (currentMovement_.remainingDistanceChange > kMinUpdateSpeed)
		{
			float diff = movementSettings_.maxDistanceChangePerSecond * elapsedTime;
			float newRemainingDistance = currentMovement_.remainingDistanceChange - diff;
			if (newRemainingDistance - diff < 0.0f)
			{
				diff = currentMovement_.remainingDistanceChange;
				currentMovement_.remainingDistanceChange = 0;
			}
			else
			{
				currentMovement_.remainingDistanceChange = newRemainingDistance;
			}
			currentPosition_.distance += diff;
			if (currentPosition_.distance > movementSettings_.maxDistance)
			{
				currentPosition_.distance = movementSettings_.maxDistance;
			}
			wasPositionUpdated = true;
		}
		else if (currentMovement_.remainingDistanceChange < -kMinUpdateSpeed)
		{
			float diff = -movementSettings_.maxDistanceChangePerSecond * elapsedTime;
			float newRemainingDistance = currentMovement_.remainingDistanceChange - diff;
			if (newRemainingDistance - diff > 0.0f)
			{
				diff = currentMovement_.remainingDistanceChange;
				currentMovement_.remainingDistanceChange = 0;
			}
			else
			{
				currentMovement_.remainingDistanceChange = newRemainingDistance;
			}
			currentPosition_.distance += diff;
			if (currentPosition_.distance < movementSettings_.minDistance)
			{
				currentPosition_.distance = movementSettings_.minDistance;
			}
			wasPositionUpdated = true;
		}

		if (wasPositionUpdated)
		{
			UpdateCameraPosition();
		}
	}

	void CameraController::UpdateCameraPosition()
	{
		float verticalMultiplier = glm::cos(glm::radians(currentPosition_.verticalAngle));

		float x = verticalMultiplier * glm::sin(
			glm::radians(currentPosition_.horizontalAngle));
		float y = glm::sin(glm::radians(currentPosition_.verticalAngle));
		float z = verticalMultiplier * glm::cos(
			glm::radians(currentPosition_.horizontalAngle));

		glm::vec3 newPosition = glm::vec3(x, y, z ) * currentPosition_.distance;

		glm::mat4 view =
			glm::lookAt(newPosition, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		renderer_->SetCameraView(view);
	}
}
