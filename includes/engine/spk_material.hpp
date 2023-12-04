#pragma once

namespace spk
{
	struct Material
	{
		float specularIntensity;
		float specularPower;

		Material();
		bool operator==(const Material& p_other) const;
		bool operator!=(const Material& p_other) const;
	};
}