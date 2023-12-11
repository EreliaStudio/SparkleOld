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
         * @brief Overloads the << operator for outputting the Matrix4x4 value.
         * 
         * This function overloads the << operator to allow for easy outputting of the Matrix4x4
         * values as a wide string. This can be used for logging, debugging, or displaying the value to an end user.
         * 
         * @param p_os A reference to a std::wostream object (like spk::cout).
         * @param p_matrix The Matrix4x4 value to be outputted.
         * @return std::wostream& - The modified std::wostream object.
         */
        friend std::wostream& operator << (std::wostream& p_os, const Matrix4x4& p_matrix)
        {
            p_os << std::endl;
            for (size_t i = 0; i < 4; i++)
            {
                for (size_t j = 0; j < 4; j++)
                {
                    if (j != 0)
                        p_os << " ";
                    p_os << p_matrix.data[i][j];
                }
                p_os << std::endl;
            }
            
            return (p_os);
        }

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
         * @brief Creates a perspective projection matrix.
         * 
         * This function constructs a perspective transformation matrix defined by
         * a field of view, aspect ratio, and the near and far clipping planes.
         *
         * @param fov Field of view in radians.
         * @param aspectRatio Aspect ratio of the viewport (width / height).
         * @param nearPlane Distance to the near clipping plane.
         * @param farPlane Distance to the far clipping plane.
         * @return Matrix4x4 The perspective projection matrix.
         */
        static Matrix4x4 perspective(float fov, float aspectRatio, float nearPlane, float farPlane);

        /**
         * @brief Creates an orthographic projection matrix.
         * 
         * This function constructs an orthographic projection matrix defined by the
         * coordinates of the planes defining the viewing volume: left, right, bottom,
         * top, near, and far.
         *
         * @param left Coordinate for the left vertical clipping plane.
         * @param right Coordinate for the right vertical clipping plane.
         * @param bottom Coordinate for the bottom horizontal clipping plane.
         * @param top Coordinate for the top horizontal clipping plane.
         * @param nearPlane Distance to the near depth clipping plane.
         * @param farPlane Distance to the far depth clipping plane.
         * @return Matrix4x4 The orthographic projection matrix.
         */
        static Matrix4x4 ortho(float left, float right, float bottom, float top, float nearPlane, float farPlane);

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
