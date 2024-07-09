module;

#ifdef USE_LEGACY_HEADERS
#include <cmath>
#include <iostream>
#include <numbers>
#endif

export module potential_energy_polarization;

#ifndef USE_LEGACY_HEADERS
import <cmath>;
import <numbers>;
import <iostream>;
#endif

import double4;

import units;
import forcefield;
import energy_factor;

export [[clang::always_inline]] inline EnergyFactor potentialPolarization(
    const ForceField& forcefield, const bool& groupIdA, const bool& groupIdB, const double& scalingA,
    const double& scalingB, const double& r, const double& chargeA, const double& chargeB)
{
  double scaling = scalingA * scalingB;
  switch (forcefield.chargeMethod)
  {
    [[likely]] case ForceField::ChargeMethod::Ewald:
    {
      double alpha = forcefield.EwaldAlpha;
      double temp = -((std::erfc(alpha * r) + 2.0 * alpha * r * std::exp(-alpha * alpha * r * r) *
                     std::numbers::inv_sqrtpi_v<double>) / (r * r * r));
      EnergyFactor result = EnergyFactor(temp,
                                         (groupIdA ? scalingB * temp : 0.0) + (groupIdB ? scalingA * temp : 0.0));
      return result;
    }
    case ForceField::ChargeMethod::Coulomb:
    {
      return EnergyFactor(0.0, 0.0);
    }
    case ForceField::ChargeMethod::Wolf:
    {
      return EnergyFactor(0.0, 0.0);
    }
    case ForceField::ChargeMethod::ModifiedWolf:
    {
      return EnergyFactor(0.0, 0.0);
    }
    default:
      break;
  }
};
