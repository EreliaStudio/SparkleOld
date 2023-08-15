#pragma once

#include <cmath>

#include "math/spk_vector3.hpp"

namespace spk
{
	/**
     * @brief Represents a 4x4 matrix.
     */
	struct Matrix4x4
    {
		/// Default 4x4 identity matrix initialization.
        float data[4][4] = {
			{1, 0, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 1, 0},
			{0, 0, 0, 1}
		};

		/// Default constructor.
		Matrix4x4();

		/**
         * @brief Multiplies matrix with a Vector3.
         * @param v The Vector3 to multiply with.
         * @return The resulting Vector3 after multiplication.
         */
        Vector3 operator*(const Vector3& v) const;

		/**
         * @brief Multiplies this matrix with another matrix.
         * @param other The other matrix to multiply with.
         * @return The resulting Matrix4x4 after multiplication.
         */
        Matrix4x4 operator*(const Matrix4x4& other) const;

		/**
         * @brief Constructs a look-at matrix.
         * @param p_from Starting position of the view.
         * @param p_to Target position.
         * @param p_up Up direction (default is positive Y).
         * @return The look-at matrix.
         */
		static Matrix4x4 lookAt(const Vector3& p_from, const Vector3& p_to, const Vector3& p_up = Vector3(0, 1, 0));

		/**
         * @brief Constructs a translation matrix.
         * @param p_translation Translation vector.
         * @return The translation matrix.
         */
        static Matrix4x4 translationMatrix(const Vector3& p_translation);

		/**
         * @brief Constructs a scale matrix.
         * @param p_scale Scale vector.
         * @return The scale matrix.
         */
        static Matrix4x4 scaleMatrix(const Vector3& p_scale);

		/**
         * @brief Constructs a rotation matrix from Euler angles.
         * @param p_rotation Rotation angles in radians.
         * @return The rotation matrix.
         */
        static Matrix4x4 rotationMatrix(const Vector3& p_rotation);

		/**
         * @brief Constructs a matrix for rotation around an arbitrary axis.
         * @param p_axis The axis to rotate around.
         * @param p_rotationAngle The rotation angle in radians.
         * @return The rotation matrix.
         */
		static Matrix4x4 rotateAroundAxis(const Vector3& p_axis, const float& p_rotationAngle);

		/**
         * @brief Checks matrix equality.
         * @param other The other matrix.
         * @return True if matrices are equal, false otherwise.
         */
		bool operator==(const Matrix4x4& other) const;

		/// Check matrix inequality.
		bool operator!=(const Matrix4x4& other) const;

		/**
         * @brief Returns the transpose of the matrix.
         * @return Transposed matrix.
         */
		Matrix4x4 transpose() const;

		/**
         * @brief Computes determinant of a 2x2 matrix.
         * @param a Element at position (0,0).
         * @param b Element at position (0,1).
         * @param c Element at position (1,0).
         * @param d Element at position (1,1).
         * @return Determinant of the matrix.
         */
		float determinant2x2(float a, float b, float c, float d);

		/**
         * @brief Computes the inverse of the matrix.
         * @return Inverted matrix.
         */
		Matrix4x4 inverse() const;

		/**
         * @brief Returns a raw pointer to the matrix data.
         * @return Pointer to the matrix data.
         */
		float* ptr();

		/**
         * @brief Returns a constant raw pointer to the matrix data.
         * @return Constant pointer to the matrix data.
         */
		const float* c_ptr() const;
    };
}
