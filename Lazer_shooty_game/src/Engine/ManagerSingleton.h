#include "ResourceManager.h"

static class ManagerSingleton
{
public:
	static ManagerSingleton& getInstance()
	{
		static ManagerSingleton instance;
		return instance;
	}

	ResourceManager* res_man = nullptr;
private:
	ManagerSingleton() {};
	ManagerSingleton(ManagerSingleton const&);              // Don't Implement.
	void operator=(ManagerSingleton const&); // Don't implement
};