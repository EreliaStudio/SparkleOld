#include "playground.hpp"

class MyClass
{
public:
	MyClass()
	{
		spk::cout << "Constructor" << std::endl;
	}

	~MyClass()
	{
		spk::cout << "Destructor" << std::endl;
	}
};

void test(spk::Pool<MyClass>& p_pool)
{
	DEBUG_LINE();
	spk::Pool<MyClass>::Object tmp = p_pool.obtain();
	DEBUG_LINE();
}

int main()
{
	spk::Pool<MyClass> pool;

	DEBUG_LINE();
	pool.resize(0);
	DEBUG_LINE();
	test(pool);
	DEBUG_LINE();
	return (0);
}