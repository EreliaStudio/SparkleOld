#include "playground.hpp"

struct Foo
{
    int x;
  
    Foo(int p_val) : x(p_val)
    {
        spk::cout << "Foo constructor. x = " << x << std::endl;
    }

    Foo(const Foo &p_other) : x(p_other.x)
    {
        spk::cout << "Foo copy constructor. x = " << x << std::endl;
    }

    Foo &operator=(const Foo &p_other)
    {
        spk::cout << "Foo copy assignment operator. x = " << p_other.x << std::endl;
        if (this != &p_other)
        {
            x = p_other.x;
        }
        return *this;
    }

    ~Foo()
    {
        spk::cout << "Foo destructor. x = " << x << std::endl;
    }
};

void testObjectDestruction(spk::Pool<Foo> &p_pool)
{
    spk::cout << "Entering testObjectDestruction." << std::endl;
	spk::cout << "Pool size: " << p_pool.size() << std::endl;
    auto obj = p_pool.obtain(3);
    spk::cout << "Inside testObjectDestruction, obj x: " << (*obj).x << std::endl;
	spk::cout << "Pool size: " << p_pool.size() << std::endl;
    spk::cout << "Exiting testObjectDestruction." << std::endl;
	spk::cout << "Pool size: " << p_pool.size() << std::endl;
}

int main()
{
    spk::Pool<Foo> myPool;

    myPool.reserve(3, 0);

    spk::cout << "After reserve, pool size: " << myPool.size() << std::endl;

    auto obj1 = myPool.obtain(1);
    auto obj2 = myPool.obtain(2);

    spk::cout << "After obtaining two objects, pool size: " << myPool.size() << std::endl;
    spk::cout << "obj1 x: " << (*obj1).x << std::endl;
    spk::cout << "obj2 x: " << (*obj2).x << std::endl;

    testObjectDestruction(myPool);

	spk::cout << "Outside function pool size: " << myPool.size() << std::endl;

    spk::cout << "Exiting main function." << std::endl;

    return 0;
}