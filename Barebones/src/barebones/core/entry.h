#pragma once

#include "application.h"

application* create_application();

int main(int argc, int** argv)
{
	auto app = create_application();
	app->run();
	delete app;

	return 0;
}