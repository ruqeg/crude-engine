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
    CHECK(crude_engine::SysVector::getX(V1) == crude_engine::SysVector::getX(V2));\
    CHECK(crude_engine::SysVector::getY(V1) == crude_engine::SysVector::getY(V2));\
    CHECK(crude_engine::SysVector::getZ(V1) == crude_engine::SysVector::getZ(V2));\
    CHECK(crude_engine::SysVector::getW(V1) == crude_engine::SysVector::getW(V2));\
}

#define CHECK_VECTOR_EQUAL_VECTOR_INT(V1, V2)\
{\
    CHECK(crude_engine::SysVector::getIntX(V1) == crude_engine::SysVector::getIntX(V2));\
    CHECK(crude_engine::SysVector::getIntY(V1) == crude_engine::SysVector::getIntY(V2));\
    CHECK(crude_engine::SysVector::getIntZ(V1) == crude_engine::SysVector::getIntZ(V2));\
    CHECK(crude_engine::SysVector::getIntW(V1) == crude_engine::SysVector::getIntW(V2));\
}

#define CHECK_VECTOR_EQUAL_SCALAR(V, s)\
{\
    CHECK(crude_engine::SysVector::getX(V) == s);\
    CHECK(crude_engine::SysVector::getY(V) == s);\
    CHECK(crude_engine::SysVector::getZ(V) == s);\
    CHECK(crude_engine::SysVector::getW(V) == s);\
}

#define CHECK_VECTOR_EQUAL_SCALAR_INT(V, s)\
{\
    CHECK(crude_engine::SysVector::getIntX(V) == s);\
    CHECK(crude_engine::SysVector::getIntY(V) == s);\
    CHECK(crude_engine::SysVector::getIntZ(V) == s);\
    CHECK(crude_engine::SysVector::getIntW(V) == s);\
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

  SUBCASE("crude_engine::SysVector::zero")
  {
    const crude_engine::Vector vZero = crude_engine::SysVector::zero();
    CHECK(crude_engine::SysVector::getX(vZero) == 0.0f);
    CHECK(crude_engine::SysVector::getY(vZero) == 0.0f);
    CHECK(crude_engine::SysVector::getZ(vZero) == 0.0f);
    CHECK(crude_engine::SysVector::getW(vZero) == 0.0f);
  }
  SUBCASE("crude_engine::SysVector::set")
  {
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::float32 x = randomFloat(min, max);
      const crude_engine::float32 y = randomFloat(min, max);
      const crude_engine::float32 z = randomFloat(min, max);
      const crude_engine::float32 w = randomFloat(min, max);
      const crude_engine::Vector vRandom = crude_engine::SysVector::set(x, y, z, w);
      CHECK(crude_engine::SysVector::getX(vRandom) == x);
      CHECK(crude_engine::SysVector::getY(vRandom) == y);
      CHECK(crude_engine::SysVector::getZ(vRandom) == z);
      CHECK(crude_engine::SysVector::getW(vRandom) == w);
    }
  }
  SUBCASE("crude_engine::SysVector::setInt")
  {
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::uint32 x = randomInt(0, max);
      const crude_engine::uint32 y = randomFloat(0, max);
      const crude_engine::uint32 z = randomFloat(0, max);
      const crude_engine::uint32 w = randomFloat(0, max);
      const crude_engine::Vector vRandom = crude_engine::SysVector::setInt(x, y, z, w);
      CHECK(crude_engine::SysVector::getIntX(vRandom) == x);
      CHECK(crude_engine::SysVector::getIntY(vRandom) == y);
      CHECK(crude_engine::SysVector::getIntZ(vRandom) == z);
      CHECK(crude_engine::SysVector::getIntW(vRandom) == w);
    }
  }
  SUBCASE("crude_engine::SysVector::fill")
  {
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::float32 random = randomFloat(min, max);
      CHECK_VECTOR_EQUAL_SCALAR(crude_engine::SysVector::fill(random), random);
    }
  }
  SUBCASE("crude_engine::SysVector::fillInt")
  {
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::uint32 random = randomInt(0, max);
      CHECK_VECTOR_EQUAL_SCALAR_INT(crude_engine::SysVector::fillInt(random), random);
    }
  }
  SUBCASE("crude_engine::SysVector::trueInt")
  {
    crude_engine::Vector v = crude_engine::SysVector::trueInt();
    CHECK_VECTOR_EQUAL_SCALAR_INT(v, 0xFFFFFFFFU);
  }
  SUBCASE("crude_engine::SysVector::falseInt")
  {
    crude_engine::Vector v = crude_engine::SysVector::falseInt();
    CHECK_VECTOR_EQUAL_SCALAR_INT(v, 0x00000000U);
  }
  SUBCASE("crude_engine::SysVector::splat")
  {
    CHECK_VECTOR_EQUAL_SCALAR(crude_engine::SysVector::splatOne(), 1.0f);
    CHECK_VECTOR_EQUAL_SCALAR(crude_engine::SysVector::splatInfinity(), 0x7F800000);
    CHECK_VECTOR_EQUAL_SCALAR(crude_engine::SysVector::splatQNaN(), 0x7FC00000);
    CHECK_VECTOR_EQUAL_SCALAR(crude_engine::SysVector::splatEpsilon(), 0x34000000);
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom = randomVector(min, max);
      CHECK_VECTOR_EQUAL_SCALAR(crude_engine::SysVector::splatX(vRandom), crude_engine::SysVector::getX(vRandom));
      CHECK_VECTOR_EQUAL_SCALAR(crude_engine::SysVector::splatY(vRandom), crude_engine::SysVector::getY(vRandom));
      CHECK_VECTOR_EQUAL_SCALAR(crude_engine::SysVector::splatZ(vRandom), crude_engine::SysVector::getZ(vRandom));
      CHECK_VECTOR_EQUAL_SCALAR(crude_engine::SysVector::splatW(vRandom), crude_engine::SysVector::getW(vRandom));
    }
  }
  SUBCASE("crude_engine::SysVector::get[X/Y/Z/W/ByIndex]")
  {
    const crude_engine::Vector vRandom = randomVector(min, max);
    VectorF32 vCopy = { { {
           crude_engine::SysVector::getX(vRandom),
           crude_engine::SysVector::getY(vRandom),
           crude_engine::SysVector::getZ(vRandom),
           crude_engine::SysVector::getW(vRandom)
    } } };
    CHECK_VECTOR_EQUAL_VECTOR(vRandom, vCopy.v);
  }
  SUBCASE("crude_engine::SysVector::getInt[X/Y/Z/W/ByIndex]")
  {
    const crude_engine::Vector vRandom = randomVectorInt(0, max);
    VectorU32 vCopy = { { {
           crude_engine::SysVector::getIntX(vRandom),
           crude_engine::SysVector::getIntY(vRandom),
           crude_engine::SysVector::getIntZ(vRandom),
           crude_engine::SysVector::getIntW(vRandom)
    } } };
    CHECK_VECTOR_EQUAL_VECTOR(vRandom, vCopy.v);
  }
  SUBCASE("crude_engine::SysVector::set[X/Y/Z/W/ByIndex]")
  {
    const crude_engine::Vector vRandom = randomVector(min, max);
    crude_engine::Vector vCopy;
    vCopy = crude_engine::SysVector::setX(vRandom, crude_engine::SysVector::getX(vRandom));
    vCopy = crude_engine::SysVector::setY(vRandom, crude_engine::SysVector::getY(vRandom));
    vCopy = crude_engine::SysVector::setZ(vRandom, crude_engine::SysVector::getZ(vRandom));
    vCopy = crude_engine::SysVector::setW(vRandom, crude_engine::SysVector::getW(vRandom));
    CHECK_VECTOR_EQUAL_VECTOR(vRandom, vCopy);
  }
  SUBCASE("crude_engine::SysVector::setInt[X/Y/Z/W/ByIndex]")
  {
    const crude_engine::Vector vRandom = randomVectorInt(0, max);
    crude_engine::Vector vCopy;
    vCopy = crude_engine::SysVector::setIntX(vRandom, crude_engine::SysVector::getIntX(vRandom));
    vCopy = crude_engine::SysVector::setIntY(vRandom, crude_engine::SysVector::getIntY(vRandom));
    vCopy = crude_engine::SysVector::setIntZ(vRandom, crude_engine::SysVector::getIntZ(vRandom));
    vCopy = crude_engine::SysVector::setIntW(vRandom, crude_engine::SysVector::getIntW(vRandom));
    CHECK_VECTOR_EQUAL_VECTOR(vRandom, vCopy);
  }
  SUBCASE("crude_engine::SysVector::equal")
  {
    const crude_engine::Vector vRandom = randomVector(min, max);
    const crude_engine::Vector vRandomInt = randomVectorInt(0, max);
    const crude_engine::Vector vCopy = vRandom;
    const crude_engine::Vector vCopyInt = vRandomInt;
    CHECK_VECTOR_EQUAL_VECTOR_INT(crude_engine::SysVector::equal(vCopy, vRandom), crude_engine::SysVector::trueInt());
    CHECK_VECTOR_EQUAL_VECTOR_INT(crude_engine::SysVector::equalInt(vCopyInt, vRandomInt), crude_engine::SysVector::trueInt());
    CHECK_VECTOR_EQUAL_VECTOR_INT(crude_engine::SysVector::notEqual(vCopy, vRandom), crude_engine::SysVector::falseInt());
    CHECK_VECTOR_EQUAL_VECTOR_INT(crude_engine::SysVector::notEqualInt(vCopyInt, vRandomInt), crude_engine::SysVector::falseInt());
  }
  SUBCASE("crude_engien::SysVector::[greater/less/inBounds]")
  {
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::float32 bounds = randomFloat(min, max);
      const crude_engine::float32 absbounds = bounds > 0 ? bounds : -bounds;
      const crude_engine::Vector vRandom = randomVector(min, max);
      const crude_engine::Vector vLess = crude_engine::SysVector::set(
        crude_engine::SysVector::getX(vRandom) - absbounds,
        crude_engine::SysVector::getY(vRandom) - absbounds,
        crude_engine::SysVector::getZ(vRandom) - absbounds,
        crude_engine::SysVector::getW(vRandom) - absbounds);
      const crude_engine::Vector vGreater = crude_engine::SysVector::set(
        crude_engine::SysVector::getX(vRandom) + absbounds,
        crude_engine::SysVector::getY(vRandom) + absbounds,
        crude_engine::SysVector::getZ(vRandom) + absbounds,
        crude_engine::SysVector::getW(vRandom) + absbounds);
      const crude_engine::Vector vBounds = crude_engine::SysVector::set(
        crude_engine::SysVector::getX(vRandom) + (crude_engine::SysVector::getX(vRandom) > 0 ? absbounds : -absbounds),
        crude_engine::SysVector::getY(vRandom) + (crude_engine::SysVector::getY(vRandom) > 0 ? absbounds : -absbounds),
        crude_engine::SysVector::getZ(vRandom) + (crude_engine::SysVector::getZ(vRandom) > 0 ? absbounds : -absbounds),
        crude_engine::SysVector::getW(vRandom) + (crude_engine::SysVector::getW(vRandom) > 0 ? absbounds : -absbounds));
      CHECK_VECTOR_EQUAL_VECTOR_INT(crude_engine::SysVector::greater(vGreater, vRandom), crude_engine::SysVector::trueInt());
      CHECK_VECTOR_EQUAL_VECTOR_INT(crude_engine::SysVector::greaterOrEqual(vRandom, vRandom), crude_engine::SysVector::trueInt());
      CHECK_VECTOR_EQUAL_VECTOR_INT(crude_engine::SysVector::less(vLess, vRandom), crude_engine::SysVector::trueInt());
      CHECK_VECTOR_EQUAL_VECTOR_INT(crude_engine::SysVector::lessOrEqual(vRandom, vRandom), crude_engine::SysVector::trueInt());
      CHECK_VECTOR_EQUAL_VECTOR_INT(crude_engine::SysVector::inBounds(vRandom, vBounds), crude_engine::SysVector::trueInt());
    }
  }
  SUBCASE("crude_engine::SysVector::is[NaN/Infinite]")
  {
    CHECK_VECTOR_EQUAL_VECTOR_INT(crude_engine::SysVector::isNaN(crude_engine::SysVector::splatQNaN()), crude_engine::SysVector::trueInt());
    CHECK_VECTOR_EQUAL_VECTOR_INT(crude_engine::SysVector::isInfinite(crude_engine::SysVector::splatInfinity()), crude_engine::SysVector::trueInt());
  }
  SUBCASE("crude_engine::Vector bitOperations")
  {
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom1 = randomVector(min, max);
      const crude_engine::Vector vRandom2 = randomVector(min, max);
      CHECK_VECTOR_EQUAL_VECTOR(crude_engine::SysVector::andInt(vRandom1, vRandom2), crude_engine::SysVector::setInt(
        crude_engine::SysVector::getIntX(vRandom1) & crude_engine::SysVector::getIntX(vRandom2),
        crude_engine::SysVector::getIntY(vRandom1) & crude_engine::SysVector::getIntY(vRandom2),
        crude_engine::SysVector::getIntZ(vRandom1) & crude_engine::SysVector::getIntZ(vRandom2),
        crude_engine::SysVector::getIntW(vRandom1) & crude_engine::SysVector::getIntW(vRandom2)
      ));
      CHECK_VECTOR_EQUAL_VECTOR(crude_engine::SysVector::orInt(vRandom1, vRandom2), crude_engine::SysVector::setInt(
        crude_engine::SysVector::getIntX(vRandom1) | crude_engine::SysVector::getIntX(vRandom2),
        crude_engine::SysVector::getIntY(vRandom1) | crude_engine::SysVector::getIntY(vRandom2),
        crude_engine::SysVector::getIntZ(vRandom1) | crude_engine::SysVector::getIntZ(vRandom2),
        crude_engine::SysVector::getIntW(vRandom1) | crude_engine::SysVector::getIntW(vRandom2)
      ));
      CHECK_VECTOR_EQUAL_VECTOR(crude_engine::SysVector::norInt(vRandom1, vRandom2), crude_engine::SysVector::setInt(
        ~(crude_engine::SysVector::getIntX(vRandom1) | crude_engine::SysVector::getIntX(vRandom2)),
        ~(crude_engine::SysVector::getIntY(vRandom1) | crude_engine::SysVector::getIntY(vRandom2)),
        ~(crude_engine::SysVector::getIntZ(vRandom1) | crude_engine::SysVector::getIntZ(vRandom2)),
        ~(crude_engine::SysVector::getIntW(vRandom1) | crude_engine::SysVector::getIntW(vRandom2))
      ));
      CHECK_VECTOR_EQUAL_VECTOR(crude_engine::SysVector::xorInt(vRandom1, vRandom2), crude_engine::SysVector::setInt(
        crude_engine::SysVector::getIntX(vRandom1) ^ crude_engine::SysVector::getIntX(vRandom2),
        crude_engine::SysVector::getIntY(vRandom1) ^ crude_engine::SysVector::getIntY(vRandom2),
        crude_engine::SysVector::getIntZ(vRandom1) ^ crude_engine::SysVector::getIntZ(vRandom2),
        crude_engine::SysVector::getIntW(vRandom1) ^ crude_engine::SysVector::getIntW(vRandom2)
      ));
    }
  }
  SUBCASE("crude_engine::SysVector::dot")
  {
    //Vector1
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom1 = randomVector(min, max);
      const crude_engine::Vector vRandom2 = randomVector(min, max);
      crude_engine::Vector vDot = crude_engine::SysVector::fill(crude_engine::SysVector::getX(vRandom1) * crude_engine::SysVector::getX(vRandom2));
      CHECK_VECTOR_EQUAL_VECTOR(vDot, crude_engine::SysVector::dot1(vRandom1, vRandom2));
    }
    //Vector2
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom1 = randomVector(min, max);
      const crude_engine::Vector vRandom2 = randomVector(min, max);
      crude_engine::Vector vDot = crude_engine::SysVector::fill(
        crude_engine::SysVector::getX(vRandom1) * crude_engine::SysVector::getX(vRandom2) + 
        crude_engine::SysVector::getY(vRandom1) * crude_engine::SysVector::getY(vRandom2));
      CHECK_VECTOR_EQUAL_VECTOR(vDot, crude_engine::SysVector::dot2(vRandom1, vRandom2));
    }
    //Vector3
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom1 = randomVector(min, max);
      const crude_engine::Vector vRandom2 = randomVector(min, max);
      crude_engine::Vector vDot = crude_engine::SysVector::fill(
        crude_engine::SysVector::getX(vRandom1) * crude_engine::SysVector::getX(vRandom2) + 
        crude_engine::SysVector::getY(vRandom1) * crude_engine::SysVector::getY(vRandom2) + 
        crude_engine::SysVector::getZ(vRandom1) * crude_engine::SysVector::getZ(vRandom2));
      CHECK_VECTOR_EQUAL_VECTOR(vDot, crude_engine::SysVector::dot3(vRandom1, vRandom2));
    }
    //Vector4
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom1 = randomVector(min, max);
      const crude_engine::Vector vRandom2 = randomVector(min, max);
      crude_engine::Vector vDot = crude_engine::SysVector::fill(
        crude_engine::SysVector::getX(vRandom1) * crude_engine::SysVector::getX(vRandom2) + 
        crude_engine::SysVector::getY(vRandom1) * crude_engine::SysVector::getY(vRandom2) + 
        crude_engine::SysVector::getZ(vRandom1) * crude_engine::SysVector::getZ(vRandom2) + 
        crude_engine::SysVector::getW(vRandom1) * crude_engine::SysVector::getW(vRandom2));

      CHECK_VECTOR_EQUAL_VECTOR(vDot, crude_engine::SysVector::dot4(vRandom1, vRandom2));
    }
  }
  SUBCASE("crude_engine::SysVector::covector")
  {
    //Vector1
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom = randomVector(min, max);
      const crude_engine::Vector e1Random = randomVector(min, max);
      crude_engine::Vector covector = crude_engine::SysVector::set(
        crude_engine::SysVector::getX(crude_engine::SysVector::dot1(vRandom, e1Random)), 
        1.f, 
        1.f, 
        1.f);
      CHECK_VECTOR_EQUAL_VECTOR(covector, crude_engine::SysVector::covector1(vRandom, e1Random));
    }
    //Vector2
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom = randomVector(min, max);
      const crude_engine::Vector e1Random = randomVector(min, max);
      const crude_engine::Vector e2Random = randomVector(min, max);
      crude_engine::Vector covector = crude_engine::SysVector::set(
        crude_engine::SysVector::getX(crude_engine::SysVector::dot2(vRandom, e1Random)),
        crude_engine::SysVector::getX(crude_engine::SysVector::dot2(vRandom, e2Random)),
        1.f,
        1.f);
      CHECK_VECTOR_EQUAL_VECTOR(covector, crude_engine::SysVector::covector2(vRandom, e1Random, e2Random));
    }
    //Vector3
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom = randomVector(min, max);
      const crude_engine::Vector e1Random = randomVector(min, max);
      const crude_engine::Vector e2Random = randomVector(min, max);
      const crude_engine::Vector e3Random = randomVector(min, max);
      crude_engine::Vector covector = crude_engine::SysVector::set(
        crude_engine::SysVector::getX(crude_engine::SysVector::dot3(vRandom, e1Random)),
        crude_engine::SysVector::getX(crude_engine::SysVector::dot3(vRandom, e2Random)),
        crude_engine::SysVector::getX(crude_engine::SysVector::dot3(vRandom, e3Random)),
        1.f);
      CHECK_VECTOR_EQUAL_VECTOR(covector, crude_engine::SysVector::covector3(vRandom, e1Random, e2Random, e3Random));
    }
    //Vector4
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom = randomVector(min, max);
      const crude_engine::Vector e1Random = randomVector(min, max);
      const crude_engine::Vector e2Random = randomVector(min, max);
      const crude_engine::Vector e3Random = randomVector(min, max);
      const crude_engine::Vector e4Random = randomVector(min, max);
      crude_engine::Vector covector = crude_engine::SysVector::set(
        crude_engine::SysVector::getX(crude_engine::SysVector::dot4(vRandom, e1Random)),
        crude_engine::SysVector::getX(crude_engine::SysVector::dot4(vRandom, e2Random)),
        crude_engine::SysVector::getX(crude_engine::SysVector::dot4(vRandom, e3Random)),
        crude_engine::SysVector::getX(crude_engine::SysVector::dot4(vRandom, e4Random)));
      CHECK_VECTOR_EQUAL_VECTOR(covector, crude_engine::SysVector::covector4(vRandom, e1Random, e2Random, e3Random, e4Random));
    }
  }
  SUBCASE("crude_engine::SysVector::length")
  {
    //Vector1
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom = randomVector(min, max);
      crude_engine::Vector vOrthoNormLength = crude_engine::SysVector::sqrt(crude_engine::SysVector::dot1(vRandom, vRandom));
      CHECK_VECTOR_EQUAL_VECTOR(vOrthoNormLength, crude_engine::SysVector::length1(vRandom));
    }
    //Vector2
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom = randomVector(min, max);
      crude_engine::Vector vOrthoNormLength = crude_engine::SysVector::sqrt(crude_engine::SysVector::dot2(vRandom, vRandom));
      CHECK_VECTOR_EQUAL_VECTOR(vOrthoNormLength, crude_engine::SysVector::length2(vRandom));
    }
    //Vector3
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom = randomVector(min, max);
      crude_engine::Vector vOrthoNormLength = crude_engine::SysVector::sqrt(crude_engine::SysVector::dot3(vRandom, vRandom));
      CHECK_VECTOR_EQUAL_VECTOR(vOrthoNormLength, crude_engine::SysVector::length3(vRandom));
    }
    //Vector4
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom = randomVector(min, max);
      crude_engine::Vector vOrthoNormLength = crude_engine::SysVector::sqrt(crude_engine::SysVector::dot4(vRandom, vRandom));
      CHECK_VECTOR_EQUAL_VECTOR(vOrthoNormLength, crude_engine::SysVector::length4(vRandom));
    }
  }
  SUBCASE("crude_engine::SysVector::normalize")
  {
    //Vector1
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom = randomVector(min, max);
      crude_engine::Vector vOrthoNormNormalize;
      {
        crude_engine::Vector vLength = crude_engine::SysVector::lengthSq1(vRandom);
        crude_engine::float32 fLength = crude_engine::SysVector::getX(vLength);
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

        vOrthoNormNormalize = crude_engine::SysVector::set(
          crude_engine::SysVector::getX(vRandom) * fInvLength,
          crude_engine::SysVector::getY(vRandom),
          crude_engine::SysVector::getZ(vRandom),
          crude_engine::SysVector::getW(vRandom));
      }
      CHECK_VECTOR_EQUAL_VECTOR(vOrthoNormNormalize, crude_engine::SysVector::normalize1(vRandom));
    }
    //Vector2
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom = randomVector(min, max);
      crude_engine::Vector vOrthoNormNormalize;
      {
        crude_engine::Vector vLength = crude_engine::SysVector::lengthSq2(vRandom);
        crude_engine::float32 fLength = crude_engine::SysVector::getX(vLength);
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

        vOrthoNormNormalize = crude_engine::SysVector::set(
          crude_engine::SysVector::getX(vRandom) * fInvLength,
          crude_engine::SysVector::getY(vRandom) * fInvLength,
          crude_engine::SysVector::getZ(vRandom),
          crude_engine::SysVector::getW(vRandom));
      }
      CHECK_VECTOR_EQUAL_VECTOR(vOrthoNormNormalize, crude_engine::SysVector::normalize2(vRandom));
    }
    //Vector3
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom = randomVector(min, max);
      crude_engine::Vector vOrthoNormNormalize;
      {
        crude_engine::Vector vLength = crude_engine::SysVector::lengthSq3(vRandom);
        crude_engine::float32 fLength = crude_engine::SysVector::getX(vLength);
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

        vOrthoNormNormalize = crude_engine::SysVector::set(
          crude_engine::SysVector::getX(vRandom) * fInvLength,
          crude_engine::SysVector::getY(vRandom) * fInvLength,
          crude_engine::SysVector::getZ(vRandom) * fInvLength,
          crude_engine::SysVector::getW(vRandom));
      }
      CHECK_VECTOR_EQUAL_VECTOR(vOrthoNormNormalize, crude_engine::SysVector::normalize3(vRandom));
    }
    //Vector4
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom = randomVector(min, max);
      crude_engine::Vector vOrthoNormNormalize;
      {
        crude_engine::Vector vLength = crude_engine::SysVector::lengthSq4(vRandom);
        crude_engine::float32 fLength = crude_engine::SysVector::getX(vLength);
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

        vOrthoNormNormalize = crude_engine::SysVector::set(
          crude_engine::SysVector::getX(vRandom) * fInvLength,
          crude_engine::SysVector::getY(vRandom) * fInvLength,
          crude_engine::SysVector::getZ(vRandom) * fInvLength,
          crude_engine::SysVector::getW(vRandom) * fInvLength);
      }
      CHECK_VECTOR_EQUAL_VECTOR(vOrthoNormNormalize, crude_engine::SysVector::normalize4(vRandom));
    }
  }
  SUBCASE("crude_engine::SysVector::tripleProduct")
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
          x = crude_engine::SysVector::getX(vRandom1) * (crude_engine::SysVector::getY(vRandom2) * crude_engine::SysVector::getZ(vRandom3) - crude_engine::SysVector::getY(vRandom3) * crude_engine::SysVector::getZ(vRandom2));
          x -= crude_engine::SysVector::getY(vRandom1) * (crude_engine::SysVector::getX(vRandom2) * crude_engine::SysVector::getZ(vRandom3) - crude_engine::SysVector::getX(vRandom3) * crude_engine::SysVector::getZ(vRandom2));
          x += crude_engine::SysVector::getZ(vRandom1) * (crude_engine::SysVector::getX(vRandom2) * crude_engine::SysVector::getY(vRandom3) - crude_engine::SysVector::getX(vRandom3) * crude_engine::SysVector::getY(vRandom2));
        }

        vOrthoNormTripleProduct = crude_engine::SysVector::fill(x);
      }
      CHECK_VECTOR_EQUAL_VECTOR(vOrthoNormTripleProduct, crude_engine::SysVector::tripleProduct3(vRandom1, vRandom2, vRandom3));
    }
  }
  SUBCASE("crude_engine::SysVector::cross")
  {
    //Vector3
    for (size_t i = 0; i < count; ++i)
    {
      const crude_engine::Vector vRandom1 = randomVector(min, max);
      const crude_engine::Vector vRandom2 = randomVector(min, max);
      crude_engine::Vector vOrthoNormOrtho = crude_engine::SysVector::set(
        crude_engine::SysVector::getY(vRandom1) * crude_engine::SysVector::getZ(vRandom2) - crude_engine::SysVector::getY(vRandom2) * crude_engine::SysVector::getZ(vRandom1),
        crude_engine::SysVector::getX(vRandom2) * crude_engine::SysVector::getZ(vRandom1) - crude_engine::SysVector::getX(vRandom1) * crude_engine::SysVector::getZ(vRandom2),
        crude_engine::SysVector::getX(vRandom1) * crude_engine::SysVector::getY(vRandom2) - crude_engine::SysVector::getX(vRandom2) * crude_engine::SysVector::getY(vRandom1),
        0.0f);
      CHECK_VECTOR_EQUAL_VECTOR(vOrthoNormOrtho, crude_engine::SysVector::cross3(vRandom1, vRandom2));
    }
  }
}