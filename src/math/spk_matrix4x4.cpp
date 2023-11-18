#include "math/spk_matrix4x4.hpp"

namespace spk
{
	Matrix4x4::Matrix4x4()
	{
	}

	Vector3 Matrix4x4::operator*(const Vector3 &v) const
	{
		float w = data[3][0] * v.x + data[3][1] * v.y + data[3][2] * v.z + data[3][3];
		return Vector3(
				(data[0][0] * v.x + data[0][1] * v.y + data[0][2] * v.z + data[0][3]) / w,
				(data[1][0] * v.x + data[1][1] * v.y + data[1][2] * v.z + data[1][3]) / w,
				(data[2][0] * v.x + data[2][1] * v.y + data[2][2] * v.z + data[2][3]) / w
			);
	}

	Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &other) const
	{
		Matrix4x4 result;

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				result.data[i][j] =
					other.data[i][0] * data[0][j] +
					other.data[i][1] * data[1][j] +
					other.data[i][2] * data[2][j] +
					other.data[i][3] * data[3][j];
			}
		}

		return result;
	}

	Matrix4x4 Matrix4x4::lookAt(const Vector3 &p_from, const Vector3 &p_to, const Vector3 &p_up)
	{
		//spk::cout << "From : " << p_from << " / to : " << p_to << " / up : " << p_up << std::endl;
		const spk::Vector3 forward((p_to - p_from).normalize());
		//spk::cout << "Forward : " << forward << std::endl;
		const spk::Vector3 right((forward.cross(p_up)).normalize());
		//spk::cout << "Right : " << right << std::endl;
		const spk::Vector3 up(right.cross(forward));
		//spk::cout << "Up : " << up << std::endl;

		Matrix4x4 result;

		result.data[0][0] = right.x;
		result.data[1][0] = right.y;
		result.data[2][0] = right.z;
		result.data[0][1] = up.x;
		result.data[1][1] = up.y;
		result.data[2][1] = up.z;
		result.data[0][2] =-forward.x;
		result.data[1][2] =-forward.y;
		result.data[2][2] =-forward.z;
		result.data[3][0] =-right.dot(p_from);
		result.data[3][1] =-up.dot(p_from);
		result.data[3][2] = forward.dot(p_from);

		return result;
	}

	Matrix4x4 Matrix4x4::translationMatrix(const Vector3 &p_translation)
	{
		Matrix4x4 result;
		result.data[0][3] = p_translation.x;
		result.data[1][3] = p_translation.y;
		result.data[2][3] = p_translation.z;
		return result;
	}

	Matrix4x4 Matrix4x4::scaleMatrix(const Vector3 &p_scale)
	{
		Matrix4x4 result;
		result.data[0][0] = p_scale.x;
		result.data[1][1] = p_scale.y;
		result.data[2][2] = p_scale.z;
		return result;
	}

	Matrix4x4 Matrix4x4::rotationMatrix(const Vector3 &p_rotation)
	{
		Matrix4x4 rx, ry, rz;

		float cosX = cos(p_rotation.x);
		float sinX = sin(p_rotation.x);

		rx.data[1][1] = cosX;
		rx.data[1][2] = -sinX;
		rx.data[2][1] = sinX;
		rx.data[2][2] = cosX;

		float cosY = cos(p_rotation.y);
		float sinY = sin(p_rotation.y);

		ry.data[0][0] = cosY;
		ry.data[0][2] = sinY;
		ry.data[2][0] = -sinY;
		ry.data[2][2] = cosY;

		float cosZ = cos(p_rotation.z);
		float sinZ = sin(p_rotation.z);

		rz.data[0][0] = cosZ;
		rz.data[0][1] = -sinZ;
		rz.data[1][0] = sinZ;
		rz.data[1][1] = cosZ;

		return rz * ry * rx;
	}

	Matrix4x4 Matrix4x4::rotateAroundAxis(const Vector3 &p_axis, const float &p_rotationAngle)
	{
		Matrix4x4 result;

		Vector3 v = p_axis.normalize();

		float c = cos(p_rotationAngle);
		float s = sin(p_rotationAngle);

		result.data[0][0] = c + v.x * v.x * (1 - c);
		result.data[0][1] = v.x * v.y * (1 - c) - v.z * s;
		result.data[0][2] = v.x * v.z * (1 - c) + v.y * s;

		result.data[1][0] = v.y * v.x * (1 - c) + v.z * s;
		result.data[1][1] = c + v.y * v.y * (1 - c);
		result.data[1][2] = v.y * v.z * (1 - c) - v.x * s;

		result.data[2][0] = v.z * v.x * (1 - c) - v.y * s;
		result.data[2][1] = v.z * v.y * (1 - c) + v.x * s;
		result.data[2][2] = c + v.z * v.z * (1 - c);

		result.data[3][0] = 0.0f;
		result.data[3][1] = 0.0f;
		result.data[3][2] = 0.0f;
		result.data[3][3] = 1.0f;

		return result;
	}

	Matrix4x4 Matrix4x4::perspective(float p_fov, float p_aspectRatio, float p_nearPlane, float p_farPlane)
	{
		Matrix4x4 result;

		const float tanHalfFovY = tan(spk::degreeToRadian(p_fov) / 2.0f);

		result.data[0][0] = 1.0f / (p_aspectRatio * tanHalfFovY);
		result.data[1][1] = 1.0f / tanHalfFovY;
		result.data[2][2] = -(p_farPlane + p_nearPlane) / (p_farPlane - p_nearPlane);
		result.data[2][3] = -1.0f;
		result.data[3][2] = -(2 * p_farPlane * p_nearPlane) / (p_farPlane - p_nearPlane);
		result.data[3][3] = 0.0f;

		return result;
	}

	Matrix4x4 Matrix4x4::ortho(float p_left, float p_right, float p_bottom, float p_top, float p_nearPlane, float p_farPlane)
	{
		Matrix4x4 result;

		result.data[0][0] = 2.0f / (p_right - p_left);
		result.data[1][1] = 2.0f / (p_top - p_bottom);
		result.data[2][2] = -2.0f / (p_farPlane - p_nearPlane);
		result.data[3][0] = -(p_right + p_left) / (p_right - p_left);
		result.data[3][1] = -(p_top + p_bottom) / (p_top - p_bottom);
		result.data[3][2] = -(p_farPlane + p_nearPlane) / (p_farPlane - p_nearPlane);

		return result;
	}

	bool Matrix4x4::operator==(const Matrix4x4 &other) const
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				if (data[i][j] != other.data[i][j])
				{
					return false;
				}
			}
		}
		return true;
	}

	bool Matrix4x4::operator!=(const Matrix4x4 &other) const
	{
		return !(*this == other);
	}

	Matrix4x4 Matrix4x4::transpose() const
	{
		Matrix4x4 result;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				result.data[j][i] = data[i][j];
			}
		}
		return result;
	}

	float Matrix4x4::determinant2x2(float a, float b, float c, float d)
	{
		return a * d - b * c;
	}

	Matrix4x4 Matrix4x4::inverse() const
	{
		Matrix4x4 adjugate;
		float det = 0;

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				float sub[3][3] = {0};
				for (int x = 0; x < 3; ++x)
				{
					for (int y = 0; y < 3; ++y)
					{
						int xi = (x < i) ? x : x + 1;
						int yj = (y < j) ? y : y + 1;
						sub[x][y] = data[xi][yj];
					}
				}

				float det3x3 = sub[0][0] * (sub[1][1] * sub[2][2] - sub[1][2] * sub[2][1]) - sub[0][1] * (sub[1][0] * sub[2][2] - sub[1][2] * sub[2][0]) + sub[0][2] * (sub[1][0] * sub[2][1] - sub[1][1] * sub[2][0]);

				adjugate.data[j][i] = ((i + j) % 2 == 0 ? 1 : -1) * det3x3;

				if (i == 0)
					det += data[i][j] * adjugate.data[j][i];
			}
		}

		if (det == 0)
		{
			return Matrix4x4();
		}

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				adjugate.data[i][j] /= det;
			}
		}

		return adjugate;
	}

	float *Matrix4x4::ptr()
	{
		return (&(data[0][0]));
	}

	const float *Matrix4x4::c_ptr() const
	{
		return (&(data[0][0]));
	}
}
