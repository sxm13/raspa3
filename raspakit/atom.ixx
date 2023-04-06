export module atom;

import double3;

import <cmath>;
import <cstddef>;

#if defined(_WIN32)
  import <cassert>;
#else
  #include <assert.h>
#endif

// Size is 2 times a __256d (double3 is padded and of size __256d)
// C++17 and higher: std::vector<T> is automatically properly aligned based on type T
export struct Atom
{
  double3 position;
  double3 velocity{};    
  double3 gradient{};    
  double charge;
  double scalingVDW{ 1.0 };
  double scalingCoulomb{ 1.0 };
  int moleculeId{ 0 };
  short type{ 0 };
  std::byte componentId{ 0 };
  std::byte groupId{ 0 };   // defaults to false

  Atom() noexcept = default;
  Atom(const Atom &a) noexcept = default;
  Atom& operator=(const Atom& a) noexcept = default;
  Atom(Atom&& a) noexcept = default;
  Atom& operator=(Atom&& a) noexcept = default;
  ~Atom() noexcept = default;

  Atom(double3 position, double charge, double lambda, short type, std::byte componentId, int moleculeId) :
      position(position), charge(charge), moleculeId(moleculeId), 
              type(type), componentId(componentId)
  {
      assert(lambda >= 0.0 && lambda <= 1.0);
      scalingVDW = lambda < 0.5 ? 2.0 * lambda : 1.0;
      scalingCoulomb = lambda < 0.5 ? 0.0 : 2.0 * (lambda - 0.5);
  };

  // scaling is linear and first switch LJ on in 0-0.5, then the electrostatics from 0.5 to 1.0
  void setScaling(double lambda)
  {
      assert(lambda >= 0.0 && lambda <= 1.0);
      scalingVDW = lambda < 0.5 ? 2.0 * lambda : 1.0;
      scalingCoulomb = lambda < 0.5 ? 0.0 : 2.0 * (lambda - 0.5);
  }

  void setScalingFullyOn()
  {
    scalingVDW = 1.0;
    scalingCoulomb = 1.0;
  }

  void setScalingFullyOff()
  {
    scalingVDW = 0.0;
    scalingCoulomb = 0.0;
  }

  void setScalingToInteger()
  {
    scalingVDW = 1.0;
    scalingCoulomb = 1.0;
    groupId = std::byte{ 0 };
  }
};

// should be 4 times double4 = 4x(8x4) = 4x32 = 128 bytes
static_assert(sizeof(Atom) == 128, "struct Atom size is not 128");
