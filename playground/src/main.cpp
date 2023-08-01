#include "playground.hpp"

int main()
{
	srand(time(NULL));
	spk::Perlin1D _oneDimPerlin(rand());
	spk::Perlin2D _twoDimPerlin(rand());
	spk::Perlin3D _threeDimPerlin(rand());

	int count[3][100];

	for (size_t i = 0; i < 100; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			count[j][i] = 0;
		}
	}

	for (size_t i = 0; i < 1000000; i++)
	{
		float value1D = _oneDimPerlin.sample(i);
		count[0][static_cast<int>(value1D)]++;
	}

	for (size_t i = 0; i < 1000; i++)
	{
		for (size_t j = 0; j < 1000; j++)
		{
			float value2D = _twoDimPerlin.sample(i, j);
			count[1][static_cast<int>(value2D)]++;
		}	
	}

	for (size_t i = 0; i < 100; i++)
	{
		for (size_t j = 0; j < 100; j++)
		{
			for (size_t k = 0; k < 100; k++)
			{
				float value3D = _threeDimPerlin.sample(i, j, k);
				count[2][static_cast<int>(value3D)]++;
			}
		}	
	}

	for (size_t j = 0; j < 3; j++)
	{
		std::wfstream output;

		output.open(L"dim_" + std::to_wstring(j) + L".txt", std::ios_base::out);
		for (size_t i = 0; i < 100; i++)
		{
			output << L"Count [" << std::setw(3) << std::setfill(L' ') << i << L"] : " << std::wstring(count[j][i] / (300), L'*') << std::endl;
		}
		output.close();
	}

	return (0);
}