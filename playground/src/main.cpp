#include "playground.hpp"

spk::Pool<int> pool;

void initiatePool()
{
    pool.reserve(10, 5);
}


spk::Pool<int>::Object functA()
{
    std::cout << "Initial size : " << pool.size() << std::endl;
    spk::Pool<int>::Object valueA = pool.obtain(10);
    spk::Pool<int>::Object valueB = pool.obtain(5);
    spk::Pool<int>::Object result = pool.obtain(*valueA + *valueB);

    std::cout << "After obtain size : " << pool.size() << std::endl;

    std::cout << "Operation : " << *valueA << " + " << *valueB << " = " << *result << std::endl;
    std::cout << "At end of function : " << pool.size() << std::endl;

    return (result);
}

int main()
{
    initiatePool();

    spk::Pool<int>::Object sum = functA();

    std::cout << "Outside of function : " << pool.size() << std::endl;
    return 0;
}
