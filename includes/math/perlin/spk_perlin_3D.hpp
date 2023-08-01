#pragma once

#include "math/perlin/spk_perlin_2D.hpp"

#include "math/spk_vector3.hpp"

namespace spk
{
	/**
	 * @class Perlin2D
	 * @brief A Perlin noise generator for three-dimensional input.
	 * 
	 * This class extends the base class Perlin2D to generate Perlin noise 
	 * for three-dimensional input. It uses a seed for noise generation that 
	 * can be provided either directly or through a JSON object. 
	 * The sample method computes a noise value based on a position on the X axis.
	 * 
	 * @see spk::Perlin2D
	 */
	class Perlin3D : public Perlin2D
	{
	private:
		/**
		 * @brief Compute a wavelenght using a 3D coordinate and a frequency.
		 * @param p_x the coordinate in the X axis to compute wavelenght from.
		 * @param p_y the coordinate in the Y axis to compute wavelenght from.
		 * @param p_z the coordinate in the Z axis to compute wavelenght from.
		 * @param p_frequency the frequency of the wave to compute.
 		 */
		float _computeWaveLength(const float& p_x, const float& p_y, const float& p_z, const float& p_frequency) const;

	public:
		/**
		 * @brief Default constructor for the Perlin3D class.
		 */
		Perlin3D();

		/**
		 * @brief Overloaded constructor for the Perlin3D class that takes a seed value.
		 * @param p_seed Seed value for Perlin noise generation.
		 */
		Perlin3D(const long long& p_seed);

		/**
		 * @brief Overloaded constructor for the Perlin3D class that takes a JSON object.
		 * @param p_object JSON object containing configuration for Perlin noise generation.
		 */
		Perlin3D(const spk::JSON::Object& p_object);

		/**
		 * @brief Launch the computation of a value based of a position on the X, Y and Z axis.
		 * @param p_x the coordinate in the X axis to compute wavelenght from.
		 * @param p_y the coordinate in the X axis to compute wavelenght from.
		 * @param p_z the coordinate in the X axis to compute wavelenght from.
		 */
		float sample(const float& p_x, const float& p_y, const float& p_z) const;


		/**
		 * @brief Launch the computation of a value based of a position on the X, Y and Z axis.
		 * @param p_position the vector representing the position to compute wavelenght from.
		 */
		template<typename TVectorType>
		float sample(const spk::IVector3<TVectorType>& p_position) const
		{
			return (sample(p_position.x, p_position.y, p_position.z));
		}
	};
}