#pragma once

namespace spk
{
	struct Material
	{
		float specularIntensity;
		float specularPower;
		float padding[2];

		Material();
		bool operator==(const Material& p_other) const;
		bool operator!=(const Material& p_other) const;
	};
}