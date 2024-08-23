module;


export module scaling;

export namespace Scaling
{
inline double scalingVDW(double lambda)
{
  // assert(lambda >= 0.0 && lambda <= 1.0);
  double temp = 2.0 * lambda;
  double inv_temp = 1.0 - temp;
  double temp2 = temp * temp;
  return lambda < 0.5 ? temp2 / (temp2 + inv_temp * inv_temp) : 1.0;
}

inline double scalingCoulomb(double lambda)
{
  // assert(lambda >= 0.0 && lambda <= 1.0);
  double temp = 2.0 * (lambda - 0.5);
  double inv_temp = 1.0 - temp;
  double temp2 = temp * temp;
  return lambda > 0.5 ? temp2 / (temp2 + inv_temp * inv_temp) : 0.0;
}

inline double scalingVDWDerivative(double lambda)
{
  // assert(lambda >= 0.0 && lambda <= 1.0);
  double temp = 1.0 - 4.0 * lambda + 8.0 * lambda * lambda;
  return lambda < 0.5 ? (8.0 * (1.0 - 2.0 * lambda) * lambda) / (temp * temp) : 0.0;
}

inline double scalingCoulombDerivative(double lambda)
{
  // assert(lambda >= 0.0 && lambda <= 1.0);
  double temp = 5.0 + 4.0 * lambda * (2.0 * lambda - 3.0);
  return lambda > 0.5 ? -(8.0 * (lambda - 1.0) * (2.0 * lambda - 1.0)) / (temp * temp) : 0.0;
}
}  // namespace Scaling
