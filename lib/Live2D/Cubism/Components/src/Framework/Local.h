/*
 * Copyright(c) Live2D Inc. All rights reserved.
 * 
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at http://live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */


#pragma once


#include <Live2DCubismCore.h>
#include <Live2DCubismFrameworkINTERNAL.h>


// -------- //
// REQUIRES //
// -------- //

/// Animation.
typedef struct csmAnimation csmAnimation;


/// Internal log function.
extern void Log(const char* message);


// ----- //
// TYPES //
// ----- //

/// Meta data of a serialized motion.
typedef struct MotionJsonMeta
{
  /// Duration of motion in seconds.
  float Duration;

  /// Frames per second.
  float Fps;

  /// Non-zero if motion should loop.
  int Loop;


  /// Number of curves serialized motion contains.
  int CurveCount;

  /// Total number of segments motion contains.
  int TotalSegmentCount;

  /// Total number of points motion contains.
  int TotalPointCount;


  /// Non-zero if béziers are restricted; '0' otherwise.
  int AreBeziersRestricted;
}
MotionJsonMeta;

// TODO Document
typedef struct PhysicsJsonEffectiveForces
{
  csmVector2 Gravity;

  csmVector2 Wind;
}
PhysicsJsonEffectiveForces;

// TODO Document
typedef struct PhysicsJsonMeta
{
  int SubRigCount;

  int TotalInputCount;

  int TotalOutputCount;

  int ParticleCount;

  PhysicsJsonEffectiveForces EffectiveForces;
}
PhysicsJsonMeta;


// ---------- //
// ASSERTIONS //
// ---------- //

/// Ensures expression is valid.
///
/// @param  expression  Expression to validate.
/// @param  message     Message to log if validation fails.
/// @param  body        Body to execute if validation fails.
#define Ensure(expression, message, body)       \
do                                              \
{                                               \
  if (!(expression))                            \
  {                                             \
    Log("[Live2D Cubism Components] " message); \
    body;                                       \
  }                                             \
}                                               \
while (0);


// ------ //
// STRING //
// ------ //

/// Reads a float from a string.
///
/// @param  string  String to read from.
/// @param  buffer  Buffer to write to.
void ReadFloatFromString(const char* string, float* buffer);

/// Reads an integer from a string.
///
/// @param  string  String to read from.
/// @param  buffer  Buffer to write to.
void ReadIntFromString(const char* string, int* buffer);


/// Checks whether a string begins with a given value.
///
/// @param  string    String to check.
/// @param  expected  Value to match against.
///
/// @return Non-zero if expectation met; '0' otherwise.
int DoesStringStartWith(const char* string, const char* expected);


// ----------- //
// MOTION JSON //
// ----------- //

/// Reads serialized motion meta.
///
/// @param  motionJson  Motion JSON string.
/// @param  buffer      Buffer to read into.
void ReadMotionJsonMeta(const char* motionJson, MotionJsonMeta* buffer);

/// Reads a serialized motion.
///
/// @param  motionJson  Motion JSON string.
/// @param  buffer      Buffer to read into.
void ReadMotionJson(const char* motionJson, csmAnimation* buffer);


// ------------ //
// PHYSICS JSON //
// ------------ //

// TODO Document
void ReadPhysicsJsonMeta(const char* physicsJson, PhysicsJsonMeta* buffer);

// TODO Document
void ReadPhysicsJson(const char* physicsJson, csmPhysicsRig* buffer);


// ------------ //
// PHYSICS MATH //
// ------------ //

// TODO Document
csmVector2 MakeVector2(float x, float y);

// TODO Document
csmVector2 AddVector2(csmVector2 a, csmVector2 b);

// TODO Document
csmVector2 SubVector2(csmVector2 a, csmVector2 b);

// TODO Document
csmVector2 MultiplyVector2(csmVector2 a, csmVector2 b);

// TODO Document
csmVector2 MultiplyVectoy2ByScalar(csmVector2 v, float s);

// TODO Document
csmVector2 DivideVector2(csmVector2 a, csmVector2 b);

// TODO Document
csmVector2 DivideVector2ByScalar(csmVector2 v, float s);

// TODO Document
float Distance(csmVector2 a, csmVector2 b);

// TODO Document
void Normalize(csmVector2* target);

// TODO Document
float DegreesToRadian(float degrees);

// TODO Document
float RadianToDegrees(float radian);

// TODO Document
float DirectionToRadian(csmVector2 from, csmVector2 to);

// TODO Document
float DirectionToDegrees(csmVector2 from, csmVector2 to);

// TODO Document
csmVector2 RadianToDirection(float totalAngle);

// TODO Document
float NormalizeParameterValue(
  float value,
  float parameterMinimum,
  float parameterMaximum,
  float parameterDefault,
  float NormalizedMinimum,
  float NormalizedMaximum,
  float NormalizedDefault,
  int isInverted
  );