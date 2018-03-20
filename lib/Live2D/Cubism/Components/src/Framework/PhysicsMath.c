#include <Live2DCubismCore.h>
#include <math.h>

#include "Local.h"

// TODO Document
const float PI = 3.14159f;


// TODO Document
static float Length(csmVector2 vector)
{
  return (float)sqrt((vector.X * vector.X) + (vector.Y * vector.Y));
}

// TODO Document
static float Dot(csmVector2 vector1, csmVector2 vector2)
{
  return (vector1.X * vector2.X) + (vector1.Y * vector2.Y);
}

// TODO Document
static float Max(float l, float r)
{
  return (l > r) ? l : r;
}

// TODO Document
static float Min(float l, float r)
{
  return (l > r) ? r : l;
}

// TODO Document
static int Sign(float value)
{
  int ret;

  ret = 0;

  if (value > 0.0f)
  {
    ret = 1;
  }
  else if (value < 0.0f)
  {
    ret = -1;
  }

  return ret;
}

// TODO Document
csmVector2 MakeVector2(float x, float y)
{
  csmVector2 ret;

  ret.X = x;
  ret.Y = y;

  return ret;
}

// TODO Document
csmVector2 AddVector2(csmVector2 a, csmVector2 b)
{
  return MakeVector2(a.X + b.X, a.Y + b.Y);
}

// TODO Document
csmVector2 SubVector2(csmVector2 a, csmVector2 b)
{
  return MakeVector2(a.X - b.X, a.Y - b.Y);
}

// TODO Document
csmVector2 MultiplyVector2(csmVector2 a, csmVector2 b)
{
  return MakeVector2(a.X * b.X, a.Y * b.Y);
}

// TODO Document
csmVector2 MultiplyVectoy2ByScalar(csmVector2 v, float s)
{
  return MakeVector2(v.X * s, v.Y * s);
}

// TODO Document
csmVector2 DivideVector2(csmVector2 a, csmVector2 b)
{
  return MakeVector2(a.X / b.X, a.Y / b.Y);
}

// TODO Document
csmVector2 DivideVector2ByScalar(csmVector2 v, float s)
{
  return MakeVector2(v.X / s, v.Y / s);
}

// TODO Document
float Distance(csmVector2 a, csmVector2 b)
{
  return Length(SubVector2(a, b));
}

// TODO Document
void Normalize(csmVector2* target)
{
  float length;

  length = powf((target->X * target->X) + (target->Y * target->Y), 0.5f);

  target->X = target->X / length;
  target->Y = target->Y / length;
}

// TODO Document
float DegreesToRadian(float degrees)
{
  return (degrees / 180.0f) * PI;
}

// TODO Document
float RadianToDegrees(float radian)
{
  return (radian * 180.0f) / PI;
}

// TODO Document
float DirectionToRadian(csmVector2 from, csmVector2 to)
{
  float dotProduct;
  float magnitude;


  dotProduct = Dot(from, to);
  magnitude = Length(from) * Length(to);

  if (magnitude == 0.0f)
  {
    return 0.0f;
  }


  float cosTheta = (dotProduct / magnitude);

  if (fabs(cosTheta) > 1.0)
  {
    return 0.0f;
  }


  float theta = (float)acos(cosTheta);

  return theta;
}

// TODO Document
float DirectionToDegrees(csmVector2 from, csmVector2 to)
{
  float radian;
  float degree;


  radian = DirectionToRadian(from, to);
  degree = (float)RadianToDegrees(radian);

  if ((to.X - from.X) > 0.0f)
  {
    degree = -degree;
  }


  return degree;
}

// TODO Document
csmVector2 RadianToDirection(float totalAngle)
{
  csmVector2 ret;


  ret.X = (float)sin(totalAngle);
  ret.Y = (float)cos(totalAngle);


  return ret;
}

// TODO Document
float NormalizeParameterValue(
  float value,
  float parameterMinimum,
  float parameterMaximum,
  float parameterDefault,
  float NormalizedMinimum,
  float NormalizedMaximum,
  float NormalizedDefault,
  int isInverted)
{

  float result;
  float minimumValue;
  float maximumValue;
  float defaultValue;
  float parameterValue;
  float parameterRange;
  float normalizedRange;

  result = 0.0f;
  maximumValue = Max(parameterMaximum, parameterMinimum);
  minimumValue = Min(parameterMaximum, parameterMinimum);
  defaultValue = parameterDefault;
  parameterValue = value - defaultValue;

  switch (Sign(parameterValue))
  {
    case 1:
    {
      parameterRange = maximumValue - defaultValue;

      if (parameterRange == 0.0f)
      {
        return NormalizedDefault;
      }

      normalizedRange = NormalizedMaximum - NormalizedDefault;

      if (normalizedRange == 0.0f)
      {
        return NormalizedMaximum;
      }


      result = value * (float)fabs(normalizedRange / parameterRange);
    }
    break;
    case -1:
    {
      parameterRange = defaultValue - minimumValue;

      if (parameterRange == 0.0f)
      {
        return NormalizedDefault;
      }

      normalizedRange = NormalizedDefault - NormalizedMinimum;

      if (normalizedRange == 0.0f)
      {
        return NormalizedMinimum;
      }


      result = value * (float)fabs(normalizedRange / parameterRange);
    }
    break;
    case 0:
    {
      result = NormalizedDefault;
    }
    break;
  }


  return (isInverted)
    ? result
    : (result * -1.0f);
}

