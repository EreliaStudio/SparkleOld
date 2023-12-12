#pragma once

#include "profiler/spk_imetrics.hpp"

namespace spk
{
	class Counter : public IMetrics<size_t>
	{
	private:
		size_t _value;

	public:
		Counter();

		spk::JSON::Object emitReport() const;

		void save();

		void increment(const size_t& p_delta);

		Counter& operator+=(const size_t& p_delta);
		Counter& operator++();
		Counter operator++(int);
	};
}