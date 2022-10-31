module;

#include <numbers>
#define sqr( x ) (( x )*( x ))
#define SIGN(a,b) ((b)>=0.0?fabs(a):-fabs(a))

module double3x3;

/* DSYEV prototype */
extern "C"
{
    void dsyev_(char* jobz, char* uplo, int* n, double* a, int* lda,
        double* w, double* work, int* lwork, int* info);
}

import <cmath>;
import <numbers>;
import <ostream>;
import <vector>;

import int3x3;
import simd_quatd;
import double3;
import randomnumbers;

// dlambda_limit, below which two lambdas are relatively equal
double dlambda_limit = 1.0E-3;
double iszero_limit = 1.0E-20;

double3x3::double3x3(simd_quatd q)
{
    double sqw = q.r * q.r;
    double sqx = q.ix * q.ix;
    double sqy = q.iy * q.iy;
    double sqz = q.iz * q.iz;

    // invs (inverse square length) is only required if quaternion is not already normalised
    double invs = 1.0 / (sqx + sqy + sqz + sqw);
    m11 = (sqx - sqy - sqz + sqw) * invs; // since sqw + sqx + sqy + sqz =1/invs*invs
    m22 = (-sqx + sqy - sqz + sqw) * invs;
    m33 = (-sqx - sqy + sqz + sqw) * invs;

    double tmp1 = q.ix * q.iy;
    double tmp2 = q.iz * q.r;
    m21 = 2.0 * (tmp1 + tmp2) * invs;
    m12 = 2.0 * (tmp1 - tmp2) * invs;

    tmp1 = q.ix * q.iz;
    tmp2 = q.iy * q.r;
    m31 = 2.0 * (tmp1 - tmp2) * invs;
    m13 = 2.0 * (tmp1 + tmp2) * invs;
    tmp1 = q.iy * q.iz;
    tmp2 = q.ix * q.r;
    m32 = 2.0 * (tmp1 + tmp2) * invs;
    m23 = 2.0 * (tmp1 - tmp2) * invs;
}

double3x3::double3x3(const int3x3 &m)
{
    m11 = m.m11; m21 = m.m21; m31 = m.m31;
    m12 = m.m12; m22 = m.m22; m32 = m.m32;
    m13 = m.m13; m23 = m.m23; m33 = m.m33;
}
double double3x3::determinant(void)
{
    double determinant =
        +m11 * (m22 * m33 - m23 * m32)
        - m12 * (m21 * m33 - m23 * m31)
        + m13 * (m21 * m32 - m22 * m31);

    return determinant;
}

double3x3::double3x3(double lattice[3][3])
{
    m11 = lattice[0][0]; m21 = lattice[1][0]; m31 = lattice[2][0];
    m12 = lattice[0][1]; m22 = lattice[1][1]; m32 = lattice[2][1];
    m13 = lattice[0][2]; m23 = lattice[1][2]; m33 = lattice[2][2];
}

double3x3 double3x3::identity()
{
    return double3x3(double3(1.0, 0.0, 0.0), double3(0.0, 1.0, 0.0), double3(0.0, 0.0, 1.0));
}


double double3x3::trace(void) const
{
    return m11 + m22 + m33;
}

const double3x3 double3x3::inverse()
{
    double determinant =
        +m11 * (m22 * m33 - m23 * m32)
        - m12 * (m21 * m33 - m23 * m31)
        + m13 * (m21 * m32 - m22 * m31);

    double3x3 inverse;
    inverse.m11 = +(m22 * m33 - m32 * m23) / determinant;
    inverse.m21 = -(m21 * m33 - m31 * m23) / determinant;
    inverse.m31 = +(m21 * m32 - m31 * m22) / determinant;
    inverse.m12 = -(m12 * m33 - m32 * m13) / determinant;
    inverse.m22 = +(m11 * m33 - m31 * m13) / determinant;
    inverse.m32 = -(m11 * m32 - m31 * m12) / determinant;
    inverse.m13 = +(m12 * m23 - m22 * m13) / determinant;
    inverse.m23 = -(m11 * m23 - m21 * m13) / determinant;
    inverse.m33 = +(m11 * m22 - m21 * m12) / determinant;

    return inverse;
}

double3x3 double3x3::inverse(const double3x3& a)
{
    double determinant =
        +a.m11 * (a.m22 * a.m33 - a.m23 * a.m32)
        - a.m12 * (a.m21 * a.m33 - a.m23 * a.m31)
        + a.m13 * (a.m21 * a.m32 - a.m22 * a.m31);

    double3x3 inverse;
    inverse.m11 = +(a.m22 * a.m33 - a.m32 * a.m23) / determinant;
    inverse.m21 = -(a.m21 * a.m33 - a.m31 * a.m23) / determinant;
    inverse.m31 = +(a.m21 * a.m32 - a.m31 * a.m22) / determinant;
    inverse.m12 = -(a.m12 * a.m33 - a.m32 * a.m13) / determinant;
    inverse.m22 = +(a.m11 * a.m33 - a.m31 * a.m13) / determinant;
    inverse.m32 = -(a.m11 * a.m32 - a.m31 * a.m12) / determinant;
    inverse.m13 = +(a.m12 * a.m23 - a.m22 * a.m13) / determinant;
    inverse.m23 = -(a.m11 * a.m23 - a.m21 * a.m13) / determinant;
    inverse.m33 = +(a.m11 * a.m22 - a.m21 * a.m12) / determinant;

    return inverse;
}

double3x3 double3x3::transpose(const double3x3& right)
{
    double3x3 res;

    res.m11 = right.m11; res.m12 = right.m21; res.m13 = right.m31;
    res.m21 = right.m12; res.m22 = right.m22; res.m23 = right.m32;
    res.m31 = right.m13; res.m32 = right.m23; res.m33 = right.m33;

    return res;
}


double3x3 const double3x3::transpose(void) const
{
    double3x3 res;

    res.m11 = m11; res.m12 = m21; res.m13 = m31;
    res.m21 = m12; res.m22 = m22; res.m23 = m32;
    res.m31 = m13; res.m32 = m23; res.m33 = m33;

    return res;
}

double3x3 double3x3::inversetranpose()
{
    double determinant =
        +m11 * (m22 * m33 - m23 * m32)
        - m12 * (m21 * m33 - m23 * m31)
        + m13 * (m21 * m32 - m22 * m31);

    double3x3 inverse;
    inverse.m11 = +(m22 * m33 - m32 * m23) / determinant;
    inverse.m12 = -(m21 * m33 - m31 * m23) / determinant;
    inverse.m13 = +(m21 * m32 - m31 * m22) / determinant;
    inverse.m21 = -(m12 * m33 - m32 * m13) / determinant;
    inverse.m22 = +(m11 * m33 - m31 * m13) / determinant;
    inverse.m23 = -(m11 * m32 - m31 * m12) / determinant;
    inverse.m31 = +(m12 * m23 - m22 * m13) / determinant;
    inverse.m32 = -(m11 * m23 - m21 * m13) / determinant;
    inverse.m33 = +(m11 * m22 - m21 * m12) / determinant;

    return inverse;
}


double trunc_sqrt(double x)
{
    return(x <= 0.0 ? 0.0 : sqrt(x));
}

double trunc_acos(double x)
{
    if (x >= 1.0)
        return 0.0;
    if (x <= -1.0)
        return std::numbers::pi;
    return acos(x);
}

double sign(double x)
{
    return(x < 0.0 ? -1.0 : 1.0);
}

double angle(double x, double y)
{
    if (x == 0.0)
        return(y == 0.0 ? 0.0 : 0.5 * std::numbers::pi * sign(y));
    return(x < 0.0 ? std::atan(y / x) + std::numbers::pi * sign(y)
        : std::atan(y / x));
}


void double3x3::EigenSystemSymmetric(double3& eigenvalues, double3x3& eigenvectors)
{
    char decompositionJobV = 'V';
    char upload = 'U';
    std::vector<double> matrix = std::vector<double>{ ax, ay, az, bx, by, bz, cx, cy, cz };
    std::vector<double> work(9 * 3);
    int lwork = 9 * 3;
    std::vector<double> e = std::vector<double>(3);
    int error = 0;
    int N = 3;
    int M = 3;

    dsyev_(&decompositionJobV, &upload, &M, matrix.data(), &N, e.data(), work.data(), &lwork, &error);

    eigenvalues = double3(e[2], e[1], e[0]);
    double3 v1, v2, v3;
    v1 = double3(matrix[0], matrix[1], matrix[2]);
    v2 = double3(matrix[3], matrix[4], matrix[5]);
    v3 = double3(matrix[6], matrix[7], matrix[8]);
    eigenvectors = double3x3(v1, v2, v3);
    if (eigenvectors.determinant() < 0)
    {
        eigenvectors = double3x3(v1, v3, v2);
    }
}



std::ostream& operator<<(std::ostream& out, const double3x3& vec) // output
{
    out << vec.m[0] << vec.m[1] << vec.m[2];
    out << vec.m[3] << vec.m[4] << vec.m[5];
    out << vec.m[6] << vec.m[7] << vec.m[8];
    return out;
}

double3x3 double3x3::randomRotationMatrix()
{
    //simd_quatd randomQuaternion = simd_quatd::random();
    //return double3x3(randomQuaternion);

    double X0 = RandomNumber::Uniform();
    double Y1 = 2.0 * std::numbers::pi * RandomNumber::Uniform();
    double Y2 = 2.0 * std::numbers::pi * RandomNumber::Uniform();
    double R1 = sqrt(1.0 - X0);
    double R2 = sqrt(X0);
    double U0 = cos(Y2) * R2;
    double U1 = sin(Y1) * R1;
    double U2 = cos(Y1) * R1;
    double U3 = sin(Y2) * R2;
    double COEFI = 2.0 * U0 * U0 - 1.0;
    double COEFUU = 2.0;
    double COEFE = 2.0 * U0;
    double3x3 R;
    R[0][0] = COEFI + COEFUU * U1 * U1;
    R[1][1] = COEFI + COEFUU * U2 * U2;
    R[2][2] = COEFI + COEFUU * U3 * U3;
    R[1][2] = COEFUU * U2 * U3 - COEFE * U1;
    R[2][0] = COEFUU * U3 * U1 - COEFE * U2;
    R[0][1] = COEFUU * U1 * U2 - COEFE * U3;
    R[2][1] = COEFUU * U3 * U2 + COEFE * U1;
    R[0][2] = COEFUU * U1 * U3 + COEFE * U2;
    R[1][0] = COEFUU * U2 * U1 + COEFE * U3;
    return R;
}

double3x3 double3x3::randomRotationAroundX(double angle)
{
    double c = std::cos(angle);
    double s = std::sin(angle);
    return double3x3(double3(1.0,0.0,0.0), double3(0.0,c,s), double3(0.0,-s,c));
}

double3x3 double3x3::randomRotationAroundY(double angle)
{
    double c = std::cos(angle);
    double s = std::sin(angle);
    return double3x3(double3(c, 0.0, -s), double3(0.0, 1.0, 0.0), double3(s, 0.0, c));
}

double3x3 double3x3::randomRotationAroundZ(double angle)
{
    double c = std::cos(angle);
    double s = std::sin(angle);
    return double3x3(double3(c, s, 0.0), double3(-s, c, 0.0), double3(0.0, 0.0, 1.0));
}
