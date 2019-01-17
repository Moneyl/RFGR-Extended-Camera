#include "Globals.hpp"

float pi = 3.14159265f;

json MainConfig;
float DefaultFreeCameraSpeed = 0.3f;
float DefaultRotationSpeed = 0.7f;
bool UseDebugConsole = false;

int KeyDecreaseCameraSpeed = 0;
int KeyIncreaseCameraSpeed = 0;
int KeyToggleFreecamera = 0;
int KeyToggleHud = 0;
int KeyToggleFog = 0;
int KeyCameraForward = 0;
int KeyCameraBackward = 0;
int KeyCameraLeft = 0;
int KeyCameraRight = 0;
int KeyCameraUp = 0;
int KeyCameraDown = 0;

int KeyDecreaseRotationSpeed = 0;
int KeyIncreaseRotationSpeed = 0;
int KeyToggleRotationControl = 0;
int KeyResetRotation = 0;
int KeyLockPitch = 0;
int KeyLockYaw = 0;
int KeyLockRoll = 0;
int KeyDecreasePitch = 0;
int KeyIncreasePitch = 0;
int KeyDecreaseYaw = 0;
int KeyIncreaseYaw = 0;
int KeyDecreaseRoll = 0;
int KeyIncreaseRoll = 0;

HANDLE ConsoleHandle = { 0 };