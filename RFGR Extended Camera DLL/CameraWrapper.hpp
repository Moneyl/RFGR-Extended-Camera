#pragma once
//#include "Globals.hpp"
//#include "SnippetManager.hpp"
#include "Functions.hpp"

enum CameraDirection
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class CameraWrapper
{
public:
	CameraWrapper();
	~CameraWrapper();
	
	/*Tested using a getter/setter/adjuster model for this class. Meant to abstract away the camera variables
	for scripting. Not sure if it's really worth the effort of setting it up though. Might be more useful for 
	classes with more variables or more complex behavior.*/

	void Initialize(float InitialCameraSpeed, float InitialCameraRotationSpeed);
	void PrintCameraInfo();

	void MoveFreeCamera(CameraDirection Direction);
	void RecalculateOrientationMatrix();

	void ActivateFreeCamera();
	void ActivateRotationControl();

	void DeactivateFreeCamera();
	void DeactivateRotationControl();

	void ToggleFreeCamera();
	void ToggleRotationControls();

	bool IsFreeCameraActive();
	bool IsRotationControlActive();

	void EnforcePitchLimits(float Minimum, float Maximum);

	bool IsPitchLocked();
	bool IsYawLocked();
	bool IsRollLocked();

	void LockPitch();
	void LockYaw();
	void LockRoll();

	void UnlockPitch();
	void UnlockYaw();
	void UnlockRoll();

	bool TogglePitchLock();
	bool ToggleYawLock();
	bool ToggleRollLock();

	//Getters
	//Real position vector
	float GetRealX();
	float GetRealY();
	float GetRealZ();

	//Real orient matrix
	float GetRealRightX();
	float GetRealRightY();
	float GetRealRightZ();

	float GetRealUpX();
	float GetRealUpY();
	float GetRealUpZ();

	float GetRealDirectionX();
	float GetRealDirectionY();
	float GetRealDirectionZ();

	float GetRealFOV();
	float GetIdealFOV();

	float GetCustomFOV();
	float GetPitch();
	float GetYaw();
	float GetRoll();

	float GetCameraSpeed();
	float GetCameraRotationSpeed();

	float GetInitialPitch();
	float GetInitialYaw();
	float GetInitialRoll();

	float GetPreLockPitch();
	float GetPreLockYaw();
	float GetPreLockRoll();

	//Setters
	//Real position vector
	void SetRealX(float Value);
	void SetRealY(float Value);
	void SetRealZ(float Value);

	//Real orient matrix
	void SetRealRightX(float Value);
	void SetRealRightY(float Value);
	void SetRealRightZ(float Value);

	void SetRealUpX(float Value);
	void SetRealUpY(float Value);
	void SetRealUpZ(float Value);

	void SetRealDirectionX(float Value);
	void SetRealDirectionY(float Value);
	void SetRealDirectionZ(float Value);

	void SetRealFOV(float Value);
	void SetIdealFOV(float Value);

	void SetCustomFOV(float Value);
	void SetPitch(float Value);
	void SetYaw(float Value);
	void SetRoll(float Value);

	void SetCameraSpeed(float Value, bool LogValue);
	void SetCameraRotationSpeed(float Value, bool LogValue);

	void SetInitialPitch(float Value);
	void SetInitialYaw(float Value);
	void SetInitialRoll(float Value);
	
	void SetPreLockPitch(float Value);
	void SetPreLockYaw(float Value);
	void SetPreLockRoll(float Value);

	//Adjusters
	//Real position vector
	void AdjustRealX(float Value);
	void AdjustRealY(float Value);
	void AdjustRealZ(float Value);

	//Real orient matrix
	void AdjustRealRightX(float Value);
	void AdjustRealRightY(float Value);
	void AdjustRealRightZ(float Value);

	void AdjustRealUpX(float Value);
	void AdjustRealUpY(float Value);
	void AdjustRealUpZ(float Value);

	void AdjustRealDirectionX(float Value);
	void AdjustRealDirectionY(float Value);
	void AdjustRealDirectionZ(float Value);

	void AdjustRealFOV(float Value);
	void AdjustIdealFOV(float Value);

	void AdjustCustomFOV(float Value);
	void AdjustPitch(float Value);
	void AdjustYaw(float Value);
	void AdjustRoll(float Value);

	void AdjustCameraSpeed(float Value);
	void AdjustCameraRotationSpeed(float Value);

	void AdjustInitialPitch(float Value);
	void AdjustInitialYaw(float Value);
	void AdjustInitialRoll(float Value);

	void AdjustPreLockPitch(float Value);
	void AdjustPreLockYaw(float Value);
	void AdjustPreLockRoll(float Value);

private:
	glm::mat3x3 RealOrientationMatrix;
	glm::vec3 RealPositionVector;

	DWORD* RealX;
	DWORD* RealY;
	DWORD* RealZ;

	//matrix RealOrient; //36 Bytes
	//[50, 54, 58] Right
	//[5C, 60, 64] Up
	//[68, 6C, 70] Direction / Forward
	DWORD* RealRightX;
	DWORD* RealRightY;
	DWORD* RealRightZ;
					   
	DWORD* RealUpX;
	DWORD* RealUpY;
	DWORD* RealUpZ;

	DWORD* RealDirectionX;
	DWORD* RealDirectionY;
	DWORD* RealDirectionZ;

	DWORD* RealFOV;
	DWORD* IdealFOV;

	float CustomFOV = 50.0f;
	float Pitch = 0.0f;
	float Yaw = 0.0f;
	float Roll = 0.0f;

	float CameraSpeed = 0.7f;
	float CameraRotationSpeed = 5.0f;

	bool FreeCameraActive = false;
	bool RotationControlActive = false;
	bool SlewModeExplicityActivated = false;

	bool PitchLocked = false;
	bool YawLocked = false;
	bool RollLocked = false;

	float InitialPitch = 0.0f;
	float InitialYaw = 0.0f;
	float InitialRoll = 0.0f;

	float PreLockPitch = 0.0f;
	float PreLockYaw = 0.0f;
	float PreLockRoll = 0.0f;

	DWORD CameraYWriteAddress;
	DWORD CameraZWriteAddress;
								
	DWORD CameraRealOrientWrite1;
	DWORD CameraRealOrientWrite2;
	DWORD CameraRealOrientWrite3;
	DWORD CameraRealOrientWrite4;
	DWORD CameraRealOrientWrite5;
};