#include "Core/Application.h"

using namespace Fidelia;

int main()
{
	Application app;

	app.Init();
	app.Run();
	app.Shutdown();

	return 0;
}