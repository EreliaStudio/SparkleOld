#include "engine/spk_material.hpp"

namespace spk
{
	Material::Material() :
		specularIntensity(1.0f),
		specularPower(32.0f)
	{
		
	}

	Material::Material(const float& p_specularIntensity, const float& p_specularPower) :
		specularIntensity(p_specularIntensity),
		specularPower(p_specularPower)
	{

	}

	bool Material::operator==(const Material& p_other) const
	{
		return ((specularIntensity == p_other.specularIntensity) && (specularPower == p_other.specularPower));
	}
	
	bool Material::operator!=(const Material& p_other) const
	{
		return (!(*this == p_other));
	}
}