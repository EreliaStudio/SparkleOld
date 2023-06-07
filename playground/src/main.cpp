#include "playground.hpp"

class MyApplication : public spk::AbstractApplication
{
	protected:
		void setupJobs()
		{
			addJob(L"Worker1", [](){
				spk::cout << "WJob1" << std::endl;
			});
			addJob(L"Worker1", [](){
				spk::cout << "WJob2" << std::endl;
			});
			addJob([](){
				spk::cout << "Job1" << std::endl;
			});
			addJob([](){
				spk::cout << "Job2" << std::endl;
			});
		}

	public:
};

int main()
{
	MyApplication app;

	return app.run();
}