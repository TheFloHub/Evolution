#pragma once

#include <Eigen/Geometry>
#include <cstdint>

namespace math
{

// fix vectors
using Vector2f = Eigen::Matrix<float, 2, 1, Eigen::DontAlign>;
using Vector3f = Eigen::Matrix<float, 3, 1, Eigen::DontAlign>;
using Vector4f = Eigen::Matrix<float, 4, 1, Eigen::DontAlign>;

using Vector2d = Eigen::Matrix<double, 2, 1, Eigen::DontAlign>;
using Vector3d = Eigen::Matrix<double, 3, 1, Eigen::DontAlign>;
using Vector4d = Eigen::Matrix<double, 4, 1, Eigen::DontAlign>;

using Vector2ui = Eigen::Matrix<uint32_t, 2, 1, Eigen::DontAlign>;
using Vector3ui = Eigen::Matrix<uint32_t, 3, 1, Eigen::DontAlign>;

using RowVector2f = Eigen::Matrix<float, 1, 2, Eigen::DontAlign>;
using RowVector3f = Eigen::Matrix<float, 1, 3, Eigen::DontAlign>;
using RowVector4f = Eigen::Matrix<float, 1, 4, Eigen::DontAlign>;

using RowVector2d = Eigen::Matrix<double, 1, 2, Eigen::DontAlign>;
using RowVector3d = Eigen::Matrix<double, 1, 3, Eigen::DontAlign>;
using RowVector4d = Eigen::Matrix<double, 1, 4, Eigen::DontAlign>;

// dynamic vectors
using VectorXf = Eigen::Matrix<float, Eigen::Dynamic, 1, Eigen::DontAlign>;
using VectorXd = Eigen::Matrix<double, Eigen::Dynamic, 1, Eigen::DontAlign>;
using RowVectorXf = Eigen::Matrix<float, 1, Eigen::Dynamic, Eigen::DontAlign>;
using RowVectorXd = Eigen::Matrix<double, 1, Eigen::Dynamic, Eigen::DontAlign>;

// fix matrices
using Matrix2f = Eigen::Matrix<float, 2, 2, Eigen::DontAlign>;
using Matrix3f = Eigen::Matrix<float, 3, 3, Eigen::DontAlign>;
using Matrix4f = Eigen::Matrix<float, 4, 4, Eigen::DontAlign>;
using Matrix34f = Eigen::Matrix<float, 3, 4, Eigen::DontAlign>;

using Matrix2d = Eigen::Matrix<double, 2, 2, Eigen::DontAlign>;
using Matrix3d = Eigen::Matrix<double, 3, 3, Eigen::DontAlign>;
using Matrix4d = Eigen::Matrix<double, 4, 4, Eigen::DontAlign>;
using Matrix34d = Eigen::Matrix<double, 3, 4, Eigen::DontAlign>;

// half dynamic matrices
using Matrix2Xf = Eigen::Matrix<float, 2, Eigen::Dynamic, Eigen::DontAlign>;
using Matrix3Xf = Eigen::Matrix<float, 3, Eigen::Dynamic, Eigen::DontAlign>;
using Matrix4Xf = Eigen::Matrix<float, 4, Eigen::Dynamic, Eigen::DontAlign>;

using Matrix2Xd = Eigen::Matrix<double, 2, Eigen::Dynamic, Eigen::DontAlign>;
using Matrix3Xd = Eigen::Matrix<double, 3, Eigen::Dynamic, Eigen::DontAlign>;
using Matrix4Xd = Eigen::Matrix<double, 4, Eigen::Dynamic, Eigen::DontAlign>;

using MatrixX2f = Eigen::Matrix<float, Eigen::Dynamic, 2, Eigen::DontAlign>;
using MatrixX3f = Eigen::Matrix<float, Eigen::Dynamic, 3, Eigen::DontAlign>;
using MatrixX4f = Eigen::Matrix<float, Eigen::Dynamic, 4, Eigen::DontAlign>;

using MatrixX2d = Eigen::Matrix<double, Eigen::Dynamic, 2, Eigen::DontAlign>;
using MatrixX3d = Eigen::Matrix<double, Eigen::Dynamic, 3, Eigen::DontAlign>;
using MatrixX4d = Eigen::Matrix<double, Eigen::Dynamic, 4, Eigen::DontAlign>;

// dynamic matrices
using MatrixXf =
    Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::DontAlign>;
using MatrixXd =
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::DontAlign>;
using MatrixXi =
    Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic, Eigen::DontAlign>;

// fixed arrays
using Array2f = Eigen::Array<float, 2, 1, Eigen::DontAlign>;
using Array3f = Eigen::Array<float, 3, 1, Eigen::DontAlign>;
using Array4f = Eigen::Array<float, 4, 1, Eigen::DontAlign>;

using Array2d = Eigen::Array<double, 2, 1, Eigen::DontAlign>;
using Array3d = Eigen::Array<double, 3, 1, Eigen::DontAlign>;
using Array4d = Eigen::Array<double, 4, 1, Eigen::DontAlign>;

using Array2i = Eigen::Array<int, 2, 1, Eigen::DontAlign>;
using Array3i = Eigen::Array<int, 3, 1, Eigen::DontAlign>;
using Array4i = Eigen::Array<int, 4, 1, Eigen::DontAlign>;

// half dynamic arrays
using Array1Xf = Eigen::Array<float, 1, Eigen::Dynamic>;
using Array2Xf = Eigen::Array<float, 2, Eigen::Dynamic, Eigen::DontAlign>;
using Array3Xf = Eigen::Array<float, 3, Eigen::Dynamic, Eigen::DontAlign>;

using Array1Xd = Eigen::Array<double, 1, Eigen::Dynamic, Eigen::DontAlign>;
using Array2Xd = Eigen::Array<double, 2, Eigen::Dynamic, Eigen::DontAlign>;
using Array3Xd = Eigen::Array<double, 3, Eigen::Dynamic, Eigen::DontAlign>;

using Array1Xi = Eigen::Array<int, 1, Eigen::Dynamic, Eigen::DontAlign>;
using Array2Xi = Eigen::Array<int, 2, Eigen::Dynamic, Eigen::DontAlign>;
using Array3Xi = Eigen::Array<int, 3, Eigen::Dynamic, Eigen::DontAlign>;

// dynamic arrays
using ArrayXf =
    Eigen::Array<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::DontAlign>;
using ArrayXd =
    Eigen::Array<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::DontAlign>;

// special transformation matrices
using Transform3f =
    Eigen::Transform<float, 3, Eigen::Isometry, Eigen::DontAlign>;
using Transform3d =
    Eigen::Transform<double, 3, Eigen::Isometry, Eigen::DontAlign>;

using Projective3f =
    Eigen::Transform<float, 3, Eigen::Projective, Eigen::DontAlign>;
using Projective3d =
    Eigen::Transform<double, 3, Eigen::Projective, Eigen::DontAlign>;

} // namespace math
