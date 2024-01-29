#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>
#include <core/math/vector.hpp>

#include <algorithm>
#include <stdlib.h>
#include <time.h>

struct VectorF32
{
  union
  {
    crude_engine::float32  f[4];
    crude_engine::Vector   v;
  };

  operator crude_engine::Vector() const noexcept
  {
    return v;
  }
};

struct VectorU32
{
  union
  {
    crude_engine::uint32  u[4];
    crude_engine::Vector  v;
  };

  operator crude_engine::Vector() const noexcept
  {
    return v;
  }
};

float randomFloat()
{
  return (float)(rand()) / (float)(RAND_MAX);
}

int randomInt(int a, int b)
{
  if (a > b)
    return randomInt(b, a);
  if (a == b)
    return a;
  return a + (rand() % (b - a));
}

float randomFloat(int a, int b)
{
  if (a > b)
    return randomFloat(b, a);
  if (a == b)
    return a;

  return (float)randomInt(a, b) + randomFloat();
}

crude_engine::Vector randomVector(int a, int b)
{
  VectorF32 vResult = { { {
      randomFloat(a, b),
      randomFloat(a, b),
      randomFloat(a, b),
      randomFloat(a, b)
  } } };
  return vResult.v;
}

crude_engine::Vector randomVectorInt(int a, int b)
{
  VectorU32 vResult = { { {
      randomInt(a, b),
      randomInt(a, b),
      randomInt(a, b),
      randomInt(a, b)
  } } };
  return vResult.v;
}

#define CHECK_VECTOR_EQUAL_VECTOR(V1, V2)\
{\
    CHECK(crude_engine::Vector::getX(V1) == crude_engine::Vector::getX(V2));\
    CHECK(crude_engine::Vector::getY(V1) == crude_engine::Vector::getY(V2));\
    CHECK(crude_engine::Vector::getZ(V1) == crude_engine::Vector::getZ(V2));\
    CHECK(crude_engine::Vector::getW(V1) == crude_engine::Vector::getW(V2));\
}

#define CHECK_VECTOR_EQUAL_VECTOR_INT(V1, V2)\
{\
    CHECK(crude_engine::Vector::getIntX(V1) == crude_engine::Vector::getIntX(V2));\
    CHECK(crude_engine::Vector::getIntY(V1) == crude_engine::Vector::getIntY(V2));\
    CHECK(crude_engine::Vector::getIntZ(V1) == crude_engine::Vector::getIntZ(V2));\
    CHECK(crude_engine::Vector::getIntW(V1) == crude_engine::Vector::getIntW(V2));\
}

#define CHECK_VECTOR_EQUAL_SCALAR(V, s)\
{\
    CHECK(crude_engine::Vector::getX(V) == s);\
    CHECK(crude_engine::Vector::getY(V) == s);\
    CHECK(crude_engine::Vector::getZ(V) == s);\
    CHECK(crude_engine::Vector::getW(V) == s);\
}

#define CHECK_VECTOR_EQUAL_SCALAR_INT(V, s)\
{\
    CHECK(crude_engine::Vector::getIntX(V) == s);\
    CHECK(crude_engine::Vector::getIntY(V) == s);\
    CHECK(crude_engine::Vector::getIntZ(V) == s);\
    CHECK(crude_engine::Vector::getIntW(V) == s);\
}


TEST_CASE("crude_engine::Scalar")
{
  constexpr int min = -10000;
  constexpr int max = 10000;
  constexpr size_t count = 5;

  srand(time(NULL));

  SUBCASE("crude_engine::Scalar::round")
  {
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::float32 s = randomFloat(min, max);
      CHECK(crude_engine::Scalar::round(s) == std::roundf(s));
    }
  }
  SUBCASE("crude_engine::Scalar::floor")
  {
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::float32 s = randomFloat(min, max);
      CHECK(crude_engine::Scalar::floor(s) == std::floorf(s));
    }
  }
  SUBCASE("crude_engine::Scalar::ceil")
  {
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::float32 s = randomFloat(min, max);
      CHECK(crude_engine::Scalar::ceil(s) == std::ceilf(s));
    }
  }
  SUBCASE("crude_engine::Scalar::trunc")
  {
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::float32 s = randomFloat(min, max);
      CHECK(crude_engine::Scalar::trunc(s) == std::truncf(s));
    }
  }
  SUBCASE("crude_engine::Scalar::clamp")
  {
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::float32 minS = randomFloat(min, max);
      const crude_engine::float32 maxS = randomFloat(minS, max);
      const crude_engine::float32 s = randomFloat(min, max);
      CHECK(crude_engine::Scalar::clamp(s, minS, maxS) == std::clamp(s, minS, maxS));
    }
  }
}
TEST_CASE("crude_engine::Vector")
{
  constexpr int min = -10000;
  constexpr int max = 10000;
  constexpr size_t count = 5;

  srand(time(NULL));

  SUBCASE("crude_engine::Vector::zero")
  {
    const crude_engine::Vector vZero = crude_engine::Vector::zero();
    CHECK(crude_engine::Vector::getX(vZero) == 0.0f);
    CHECK(crude_engine::Vector::getY(vZero) == 0.0f);
    CHECK(crude_engine::Vector::getZ(vZero) == 0.0f);
    CHECK(crude_engine::Vector::getW(vZero) == 0.0f);
  }
  SUBCASE("crude_engine::Vector::set")
  {
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::float32 x = randomFloat(min, max);
      const crude_engine::float32 y = randomFloat(min, max);
      const crude_engine::float32 z = randomFloat(min, max);
      const crude_engine::float32 w = randomFloat(min, max);
      const crude_engine::Vector vRandom = crude_engine::Vector::set(x, y, z, w);
      CHECK(crude_engine::Vector::getX(vRandom) == x);
      CHECK(crude_engine::Vector::getY(vRandom) == y);
      CHECK(crude_engine::Vector::getZ(vRandom) == z);
      CHECK(crude_engine::Vector::getW(vRandom) == w);
    }
  }
  SUBCASE("crude_engine::Vector::setInt")
  {
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::uint32 x = randomInt(0, max);
      const crude_engine::uint32 y = randomFloat(0, max);
      const crude_engine::uint32 z = randomFloat(0, max);
      const crude_engine::uint32 w = randomFloat(0, max);
      const crude_engine::Vector vRandom = crude_engine::Vector::setInt(x, y, z, w);
      CHECK(crude_engine::Vector::getIntX(vRandom) == x);
      CHECK(crude_engine::Vector::getIntY(vRandom) == y);
      CHECK(crude_engine::Vector::getIntZ(vRandom) == z);
      CHECK(crude_engine::Vector::getIntW(vRandom) == w);
    }
  }
  SUBCASE("crude_engine::Vector::fill")
  {
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::float32 random = randomFloat(min, max);
      CHECK_VECTOR_EQUAL_SCALAR(crude_engine::Vector::fill(random), random);
    }
  }
  SUBCASE("crude_engine::Vector::fillInt")
  {
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::uint32 random = randomInt(0, max);
      CHECK_VECTOR_EQUAL_SCALAR_INT(crude_engine::Vector::fillInt(random), random);
    }
  }
  SUBCASE("crude_engine::Vector::trueInt")
  {
    crude_engine::Vector v = crude_engine::Vector::trueInt();
    CHECK_VECTOR_EQUAL_SCALAR_INT(v, 0xFFFFFFFFU);
  }
  SUBCASE("crude_engine::Vector::falseInt")
  {
    crude_engine::Vector v = crude_engine::Vector::falseInt();
    CHECK_VECTOR_EQUAL_SCALAR_INT(v, 0x00000000U);
  }
  SUBCASE("crude_engine::Vector::splat")
  {
    CHECK_VECTOR_EQUAL_SCALAR(crude_engine::Vector::splatOne(), 1.0f);
    CHECK_VECTOR_EQUAL_SCALAR(crude_engine::Vector::splatInfinity(), 0x7F800000);
    CHECK_VECTOR_EQUAL_SCALAR(crude_engine::Vector::splatQNaN(), 0x7FC00000);
    CHECK_VECTOR_EQUAL_SCALAR(crude_engine::Vector::splatEpsilon(), 0x34000000);
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom = randomVector(min, max);
      CHECK_VECTOR_EQUAL_SCALAR(crude_engine::Vector::splatX(vRandom), crude_engine::Vector::getX(vRandom));
      CHECK_VECTOR_EQUAL_SCALAR(crude_engine::Vector::splatY(vRandom), crude_engine::Vector::getY(vRandom));
      CHECK_VECTOR_EQUAL_SCALAR(crude_engine::Vector::splatZ(vRandom), crude_engine::Vector::getZ(vRandom));
      CHECK_VECTOR_EQUAL_SCALAR(crude_engine::Vector::splatW(vRandom), crude_engine::Vector::getW(vRandom));
    }
  }
  SUBCASE("crude_engine::Vector::get[X/Y/Z/W/ByIndex]")
  {
    const crude_engine::Vector vRandom = randomVector(min, max);
    VectorF32 vCopy = { { {
           crude_engine::Vector::getX(vRandom),
           crude_engine::Vector::getY(vRandom),
           crude_engine::Vector::getZ(vRandom),
           crude_engine::Vector::getW(vRandom)
    } } };
    CHECK_VECTOR_EQUAL_VECTOR(vRandom, vCopy.v);
  }
  SUBCASE("crude_engine::Vector::getInt[X/Y/Z/W/ByIndex]")
  {
    const crude_engine::Vector vRandom = randomVectorInt(0, max);
    VectorU32 vCopy = { { {
           crude_engine::Vector::getIntX(vRandom),
           crude_engine::Vector::getIntY(vRandom),
           crude_engine::Vector::getIntZ(vRandom),
           crude_engine::Vector::getIntW(vRandom)
    } } };
    CHECK_VECTOR_EQUAL_VECTOR(vRandom, vCopy.v);
  }
  SUBCASE("crude_engine::Vector::set[X/Y/Z/W/ByIndex]")
  {
    const crude_engine::Vector vRandom = randomVector(min, max);
    crude_engine::Vector vCopy;
    vCopy = crude_engine::Vector::setX(vRandom, crude_engine::Vector::getX(vRandom));
    vCopy = crude_engine::Vector::setY(vRandom, crude_engine::Vector::getY(vRandom));
    vCopy = crude_engine::Vector::setZ(vRandom, crude_engine::Vector::getZ(vRandom));
    vCopy = crude_engine::Vector::setW(vRandom, crude_engine::Vector::getW(vRandom));
    CHECK_VECTOR_EQUAL_VECTOR(vRandom, vCopy);
  }
  SUBCASE("crude_engine::Vector::setInt[X/Y/Z/W/ByIndex]")
  {
    const crude_engine::Vector vRandom = randomVectorInt(0, max);
    crude_engine::Vector vCopy;
    vCopy = crude_engine::Vector::setIntX(vRandom, crude_engine::Vector::getIntX(vRandom));
    vCopy = crude_engine::Vector::setIntY(vRandom, crude_engine::Vector::getIntY(vRandom));
    vCopy = crude_engine::Vector::setIntZ(vRandom, crude_engine::Vector::getIntZ(vRandom));
    vCopy = crude_engine::Vector::setIntW(vRandom, crude_engine::Vector::getIntW(vRandom));
    CHECK_VECTOR_EQUAL_VECTOR(vRandom, vCopy);
  }
  SUBCASE("crude_engine::Vector::equal")
  {
    const crude_engine::Vector vRandom = randomVector(min, max);
    const crude_engine::Vector vRandomInt = randomVectorInt(0, max);
    const crude_engine::Vector vCopy = vRandom;
    const crude_engine::Vector vCopyInt = vRandomInt;
    CHECK_VECTOR_EQUAL_VECTOR_INT(crude_engine::Vector::equal(vCopy, vRandom), crude_engine::Vector::trueInt());
    CHECK_VECTOR_EQUAL_VECTOR_INT(crude_engine::Vector::equalInt(vCopyInt, vRandomInt), crude_engine::Vector::trueInt());
    CHECK_VECTOR_EQUAL_VECTOR_INT(crude_engine::Vector::notEqual(vCopy, vRandom), crude_engine::Vector::falseInt());
    CHECK_VECTOR_EQUAL_VECTOR_INT(crude_engine::Vector::notEqualInt(vCopyInt, vRandomInt), crude_engine::Vector::falseInt());
  }
  SUBCASE("crude_engien::Vector::[greater/less/inBounds]")
  {
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::float32 bounds = randomFloat(min, max);
      const crude_engine::float32 absbounds = bounds > 0 ? bounds : -bounds;
      const crude_engine::Vector vRandom = randomVector(min, max);
      const crude_engine::Vector vLess = crude_engine::Vector::set(
        crude_engine::Vector::getX(vRandom) - absbounds,
        crude_engine::Vector::getY(vRandom) - absbounds,
        crude_engine::Vector::getZ(vRandom) - absbounds,
        crude_engine::Vector::getW(vRandom) - absbounds);
      const crude_engine::Vector vGreater = crude_engine::Vector::set(
        crude_engine::Vector::getX(vRandom) + absbounds,
        crude_engine::Vector::getY(vRandom) + absbounds,
        crude_engine::Vector::getZ(vRandom) + absbounds,
        crude_engine::Vector::getW(vRandom) + absbounds);
      const crude_engine::Vector vBounds = crude_engine::Vector::set(
        crude_engine::Vector::getX(vRandom) + (crude_engine::Vector::getX(vRandom) > 0 ? absbounds : -absbounds),
        crude_engine::Vector::getY(vRandom) + (crude_engine::Vector::getY(vRandom) > 0 ? absbounds : -absbounds),
        crude_engine::Vector::getZ(vRandom) + (crude_engine::Vector::getZ(vRandom) > 0 ? absbounds : -absbounds),
        crude_engine::Vector::getW(vRandom) + (crude_engine::Vector::getW(vRandom) > 0 ? absbounds : -absbounds));
      CHECK_VECTOR_EQUAL_VECTOR_INT(crude_engine::Vector::greater(vGreater, vRandom), crude_engine::Vector::trueInt());
      CHECK_VECTOR_EQUAL_VECTOR_INT(crude_engine::Vector::greaterOrEqual(vRandom, vRandom), crude_engine::Vector::trueInt());
      CHECK_VECTOR_EQUAL_VECTOR_INT(crude_engine::Vector::less(vLess, vRandom), crude_engine::Vector::trueInt());
      CHECK_VECTOR_EQUAL_VECTOR_INT(crude_engine::Vector::lessOrEqual(vRandom, vRandom), crude_engine::Vector::trueInt());
      CHECK_VECTOR_EQUAL_VECTOR_INT(crude_engine::Vector::inBounds(vRandom, vBounds), crude_engine::Vector::trueInt());
    }
  }
  SUBCASE("crude_engine::Vector::is[NaN/Infinite]")
  {
    CHECK_VECTOR_EQUAL_VECTOR_INT(crude_engine::Vector::isNaN(crude_engine::Vector::splatQNaN()), crude_engine::Vector::trueInt());
    CHECK_VECTOR_EQUAL_VECTOR_INT(crude_engine::Vector::isInfinite(crude_engine::Vector::splatInfinity()), crude_engine::Vector::trueInt());
  }
  SUBCASE("crude_engine::Vector bitOperations")
  {
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom1 = randomVector(min, max);
      const crude_engine::Vector vRandom2 = randomVector(min, max);
      CHECK_VECTOR_EQUAL_VECTOR(crude_engine::Vector::andInt(vRandom1, vRandom2), crude_engine::Vector::setInt(
        crude_engine::Vector::getIntX(vRandom1) & crude_engine::Vector::getIntX(vRandom2),
        crude_engine::Vector::getIntY(vRandom1) & crude_engine::Vector::getIntY(vRandom2),
        crude_engine::Vector::getIntZ(vRandom1) & crude_engine::Vector::getIntZ(vRandom2),
        crude_engine::Vector::getIntW(vRandom1) & crude_engine::Vector::getIntW(vRandom2)
      ));
      CHECK_VECTOR_EQUAL_VECTOR(crude_engine::Vector::orInt(vRandom1, vRandom2), crude_engine::Vector::setInt(
        crude_engine::Vector::getIntX(vRandom1) | crude_engine::Vector::getIntX(vRandom2),
        crude_engine::Vector::getIntY(vRandom1) | crude_engine::Vector::getIntY(vRandom2),
        crude_engine::Vector::getIntZ(vRandom1) | crude_engine::Vector::getIntZ(vRandom2),
        crude_engine::Vector::getIntW(vRandom1) | crude_engine::Vector::getIntW(vRandom2)
      ));
      CHECK_VECTOR_EQUAL_VECTOR(crude_engine::Vector::norInt(vRandom1, vRandom2), crude_engine::Vector::setInt(
        ~(crude_engine::Vector::getIntX(vRandom1) | crude_engine::Vector::getIntX(vRandom2)),
        ~(crude_engine::Vector::getIntY(vRandom1) | crude_engine::Vector::getIntY(vRandom2)),
        ~(crude_engine::Vector::getIntZ(vRandom1) | crude_engine::Vector::getIntZ(vRandom2)),
        ~(crude_engine::Vector::getIntW(vRandom1) | crude_engine::Vector::getIntW(vRandom2))
      ));
      CHECK_VECTOR_EQUAL_VECTOR(crude_engine::Vector::xorInt(vRandom1, vRandom2), crude_engine::Vector::setInt(
        crude_engine::Vector::getIntX(vRandom1) ^ crude_engine::Vector::getIntX(vRandom2),
        crude_engine::Vector::getIntY(vRandom1) ^ crude_engine::Vector::getIntY(vRandom2),
        crude_engine::Vector::getIntZ(vRandom1) ^ crude_engine::Vector::getIntZ(vRandom2),
        crude_engine::Vector::getIntW(vRandom1) ^ crude_engine::Vector::getIntW(vRandom2)
      ));
    }
  }
  SUBCASE("crude_engine::Vector::dot")
  {
    //Vector1
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom1 = randomVector(min, max);
      const crude_engine::Vector vRandom2 = randomVector(min, max);
      crude_engine::Vector vDot = crude_engine::Vector::fill(crude_engine::Vector::getX(vRandom1) * crude_engine::Vector::getX(vRandom2));
      CHECK_VECTOR_EQUAL_VECTOR(vDot, crude_engine::Vector::dot1(vRandom1, vRandom2));
    }
    //Vector2
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom1 = randomVector(min, max);
      const crude_engine::Vector vRandom2 = randomVector(min, max);
      crude_engine::Vector vDot = crude_engine::Vector::fill(
        crude_engine::Vector::getX(vRandom1) * crude_engine::Vector::getX(vRandom2) + 
        crude_engine::Vector::getY(vRandom1) * crude_engine::Vector::getY(vRandom2));
      CHECK_VECTOR_EQUAL_VECTOR(vDot, crude_engine::Vector::dot2(vRandom1, vRandom2));
    }
    //Vector3
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom1 = randomVector(min, max);
      const crude_engine::Vector vRandom2 = randomVector(min, max);
      crude_engine::Vector vDot = crude_engine::Vector::fill(
        crude_engine::Vector::getX(vRandom1) * crude_engine::Vector::getX(vRandom2) + 
        crude_engine::Vector::getY(vRandom1) * crude_engine::Vector::getY(vRandom2) + 
        crude_engine::Vector::getZ(vRandom1) * crude_engine::Vector::getZ(vRandom2));
      CHECK_VECTOR_EQUAL_VECTOR(vDot, crude_engine::Vector::dot3(vRandom1, vRandom2));
    }
    //Vector4
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom1 = randomVector(min, max);
      const crude_engine::Vector vRandom2 = randomVector(min, max);
      crude_engine::Vector vDot = crude_engine::Vector::fill(
        crude_engine::Vector::getX(vRandom1) * crude_engine::Vector::getX(vRandom2) + 
        crude_engine::Vector::getY(vRandom1) * crude_engine::Vector::getY(vRandom2) + 
        crude_engine::Vector::getZ(vRandom1) * crude_engine::Vector::getZ(vRandom2) + 
        crude_engine::Vector::getW(vRandom1) * crude_engine::Vector::getW(vRandom2));

      CHECK_VECTOR_EQUAL_VECTOR(vDot, crude_engine::Vector::dot4(vRandom1, vRandom2));
    }
  }
  SUBCASE("crude_engine::Vector::covector")
  {
    //Vector1
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom = randomVector(min, max);
      const crude_engine::Vector e1Random = randomVector(min, max);
      crude_engine::Vector covector = crude_engine::Vector::set(
        crude_engine::Vector::getX(crude_engine::Vector::dot1(vRandom, e1Random)), 
        1.f, 
        1.f, 
        1.f);
      CHECK_VECTOR_EQUAL_VECTOR(covector, crude_engine::Vector::covector1(vRandom, e1Random));
    }
    //Vector2
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom = randomVector(min, max);
      const crude_engine::Vector e1Random = randomVector(min, max);
      const crude_engine::Vector e2Random = randomVector(min, max);
      crude_engine::Vector covector = crude_engine::Vector::set(
        crude_engine::Vector::getX(crude_engine::Vector::dot2(vRandom, e1Random)),
        crude_engine::Vector::getX(crude_engine::Vector::dot2(vRandom, e2Random)),
        1.f,
        1.f);
      CHECK_VECTOR_EQUAL_VECTOR(covector, crude_engine::Vector::covector2(vRandom, e1Random, e2Random));
    }
    //Vector3
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom = randomVector(min, max);
      const crude_engine::Vector e1Random = randomVector(min, max);
      const crude_engine::Vector e2Random = randomVector(min, max);
      const crude_engine::Vector e3Random = randomVector(min, max);
      crude_engine::Vector covector = crude_engine::Vector::set(
        crude_engine::Vector::getX(crude_engine::Vector::dot3(vRandom, e1Random)),
        crude_engine::Vector::getX(crude_engine::Vector::dot3(vRandom, e2Random)),
        crude_engine::Vector::getX(crude_engine::Vector::dot3(vRandom, e3Random)),
        1.f);
      CHECK_VECTOR_EQUAL_VECTOR(covector, crude_engine::Vector::covector3(vRandom, e1Random, e2Random, e3Random));
    }
    //Vector4
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom = randomVector(min, max);
      const crude_engine::Vector e1Random = randomVector(min, max);
      const crude_engine::Vector e2Random = randomVector(min, max);
      const crude_engine::Vector e3Random = randomVector(min, max);
      const crude_engine::Vector e4Random = randomVector(min, max);
      crude_engine::Vector covector = crude_engine::Vector::set(
        crude_engine::Vector::getX(crude_engine::Vector::dot4(vRandom, e1Random)),
        crude_engine::Vector::getX(crude_engine::Vector::dot4(vRandom, e2Random)),
        crude_engine::Vector::getX(crude_engine::Vector::dot4(vRandom, e3Random)),
        crude_engine::Vector::getX(crude_engine::Vector::dot4(vRandom, e4Random)));
      CHECK_VECTOR_EQUAL_VECTOR(covector, crude_engine::Vector::covector4(vRandom, e1Random, e2Random, e3Random, e4Random));
    }
  }
  SUBCASE("crude_engine::Vector::length")
  {
    //Vector1
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom = randomVector(min, max);
      crude_engine::Vector vOrthoNormLength = crude_engine::Vector::sqrt(crude_engine::Vector::dot1(vRandom, vRandom));
      CHECK_VECTOR_EQUAL_VECTOR(vOrthoNormLength, crude_engine::Vector::length1(vRandom));
    }
    //Vector2
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom = randomVector(min, max);
      crude_engine::Vector vOrthoNormLength = crude_engine::Vector::sqrt(crude_engine::Vector::dot2(vRandom, vRandom));
      CHECK_VECTOR_EQUAL_VECTOR(vOrthoNormLength, crude_engine::Vector::length2(vRandom));
    }
    //Vector3
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom = randomVector(min, max);
      crude_engine::Vector vOrthoNormLength = crude_engine::Vector::sqrt(crude_engine::Vector::dot3(vRandom, vRandom));
      CHECK_VECTOR_EQUAL_VECTOR(vOrthoNormLength, crude_engine::Vector::length3(vRandom));
    }
    //Vector4
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom = randomVector(min, max);
      crude_engine::Vector vOrthoNormLength = crude_engine::Vector::sqrt(crude_engine::Vector::dot4(vRandom, vRandom));
      CHECK_VECTOR_EQUAL_VECTOR(vOrthoNormLength, crude_engine::Vector::length4(vRandom));
    }
  }
  SUBCASE("crude_engine::Vector::normalize")
  {
    //Vector1
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom = randomVector(min, max);
      crude_engine::Vector vOrthoNormNormalize;
      {
        crude_engine::Vector vLength = crude_engine::Vector::lengthSq1(vRandom);
        crude_engine::float32 fLength = crude_engine::Vector::getX(vLength);
        crude_engine::float32 fInvLength;
        {
          if (fLength > 0)
          {
            fInvLength = 1.0f / fLength;
          }
          else
          {
            fInvLength = crude_engine::Constans::CMAXF32;
          }
        }

        vOrthoNormNormalize = crude_engine::Vector::set(
          crude_engine::Vector::getX(vRandom) * fInvLength,
          crude_engine::Vector::getY(vRandom),
          crude_engine::Vector::getZ(vRandom),
          crude_engine::Vector::getW(vRandom));
      }
      CHECK_VECTOR_EQUAL_VECTOR(vOrthoNormNormalize, crude_engine::Vector::normalize1(vRandom));
    }
    //Vector2
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom = randomVector(min, max);
      crude_engine::Vector vOrthoNormNormalize;
      {
        crude_engine::Vector vLength = crude_engine::Vector::lengthSq2(vRandom);
        crude_engine::float32 fLength = crude_engine::Vector::getX(vLength);
        crude_engine::float32 fInvLength;
        {
          if (fLength > 0)
          {
            fInvLength = 1.0f / fLength;
          }
          else
          {
            fInvLength = crude_engine::Constans::CMAXF32;
          }
        }

        vOrthoNormNormalize = crude_engine::Vector::set(
          crude_engine::Vector::getX(vRandom) * fInvLength,
          crude_engine::Vector::getY(vRandom) * fInvLength,
          crude_engine::Vector::getZ(vRandom),
          crude_engine::Vector::getW(vRandom));
      }
      CHECK_VECTOR_EQUAL_VECTOR(vOrthoNormNormalize, crude_engine::Vector::normalize2(vRandom));
    }
    //Vector3
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom = randomVector(min, max);
      crude_engine::Vector vOrthoNormNormalize;
      {
        crude_engine::Vector vLength = crude_engine::Vector::lengthSq3(vRandom);
        crude_engine::float32 fLength = crude_engine::Vector::getX(vLength);
        crude_engine::float32 fInvLength;
        {
          if (fLength > 0)
          {
            fInvLength = 1.0f / fLength;
          }
          else
          {
            fInvLength = crude_engine::Constans::CMAXF32;
          }
        }

        vOrthoNormNormalize = crude_engine::Vector::set(
          crude_engine::Vector::getX(vRandom) * fInvLength,
          crude_engine::Vector::getY(vRandom) * fInvLength,
          crude_engine::Vector::getZ(vRandom) * fInvLength,
          crude_engine::Vector::getW(vRandom));
      }
      CHECK_VECTOR_EQUAL_VECTOR(vOrthoNormNormalize, crude_engine::Vector::normalize3(vRandom));
    }
    //Vector4
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom = randomVector(min, max);
      crude_engine::Vector vOrthoNormNormalize;
      {
        crude_engine::Vector vLength = crude_engine::Vector::lengthSq4(vRandom);
        crude_engine::float32 fLength = crude_engine::Vector::getX(vLength);
        crude_engine::float32 fInvLength;
        {
          if (fLength > 0)
          {
            fInvLength = 1.0f / fLength;
          }
          else
          {
            fInvLength = crude_engine::Constans::CMAXF32;
          }
        }

        vOrthoNormNormalize = crude_engine::Vector::set(
          crude_engine::Vector::getX(vRandom) * fInvLength,
          crude_engine::Vector::getY(vRandom) * fInvLength,
          crude_engine::Vector::getZ(vRandom) * fInvLength,
          crude_engine::Vector::getW(vRandom) * fInvLength);
      }
      CHECK_VECTOR_EQUAL_VECTOR(vOrthoNormNormalize, crude_engine::Vector::normalize4(vRandom));
    }
  }
  SUBCASE("crude_engine::tripleProduct")
  {
    //Vector3
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom1 = randomVector(min, max);
      const crude_engine::Vector vRandom2 = randomVector(min, max);
      const crude_engine::Vector vRandom3 = randomVector(min, max);
      crude_engine::Vector vOrthoNormTripleProduct;
      {
        crude_engine::float32 x;
        {
          x = crude_engine::Vector::getX(vRandom1) * (crude_engine::Vector::getY(vRandom2) * crude_engine::Vector::getZ(vRandom3) - crude_engine::Vector::getY(vRandom3) * crude_engine::Vector::getZ(vRandom2));
          x -= crude_engine::Vector::getY(vRandom1) * (crude_engine::Vector::getX(vRandom2) * crude_engine::Vector::getZ(vRandom3) - crude_engine::Vector::getX(vRandom3) * crude_engine::Vector::getZ(vRandom2));
          x += crude_engine::Vector::getZ(vRandom1) * (crude_engine::Vector::getX(vRandom2) * crude_engine::Vector::getY(vRandom3) - crude_engine::Vector::getX(vRandom3) * crude_engine::Vector::getY(vRandom2));
        }

        vOrthoNormTripleProduct = crude_engine::Vector::fill(x);
      }
      CHECK_VECTOR_EQUAL_VECTOR(vOrthoNormTripleProduct, crude_engine::Vector::tripleProduct3(vRandom1, vRandom2, vRandom3));
    }
  }
  SUBCASE("MorphineMath::VectorOrthoNormCross")
  {
    //Vector3
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom1 = randomVector(min, max);
      const crude_engine::Vector vRandom2 = randomVector(min, max);
      crude_engine::Vector vOrthoNormOrtho = crude_engine::Vector::set(
        crude_engine::Vector::getY(vRandom1) * crude_engine::Vector::getZ(vRandom2) - crude_engine::Vector::getY(vRandom2) * crude_engine::Vector::getZ(vRandom1),
        crude_engine::Vector::getX(vRandom2) * crude_engine::Vector::getZ(vRandom1) - crude_engine::Vector::getX(vRandom1) * crude_engine::Vector::getZ(vRandom2),
        crude_engine::Vector::getX(vRandom1) * crude_engine::Vector::getY(vRandom2) - crude_engine::Vector::getX(vRandom2) * crude_engine::Vector::getY(vRandom1),
        0.0f);
      CHECK_VECTOR_EQUAL_VECTOR(vOrthoNormOrtho, crude_engine::Vector::cross3(vRandom1, vRandom2));
    }
  }
}