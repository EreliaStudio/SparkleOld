#include "profiler/spk_profiler.hpp"

namespace spk
{
	Counter::Counter() : 
		_value(0)
	{

	}

	bool Counter::needEmition() const
	{
		if (cardinal() == 0)
			return (false);
		return (true);
	}

	spk::JSON::Object Counter::emitReport() const
	{
		spk::JSON::Object result;

		result.addAttribute(L"Min").set<long>(min());
		result.addAttribute(L"Max").set<long>(max());
		result.addAttribute(L"Average").set<long>(average());
		result.addAttribute(L"Cardinal").set<long>(cardinal());

		return (result);
	}

	void Counter::save()
	{
		_insertValue(_value);
		_value = 0;
	}

	void Counter::increment(const size_t& p_delta)
	{
		_value += p_delta;
	}

	Counter& Counter::operator+=(const size_t& p_delta)
	{
		increment(p_delta);
		return *this;
	}

	Counter& Counter::operator++()
	{
		++_value;
		return *this;
	}

	Counter Counter::operator++(int)
	{
		Counter temp = *this;
		++(*this);
		return temp;
	}
}