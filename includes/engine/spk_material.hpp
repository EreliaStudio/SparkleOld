#pragma once

namespace spk
{
	struct Material
	{
		float specularIntensity;
		float specularPower;

		Material();
		Material(const float& p_specularIntensity, const float& p_specularPower);
		bool operator==(const Material& p_other) const;
		bool operator!=(const Material& p_other) const;
	};
}