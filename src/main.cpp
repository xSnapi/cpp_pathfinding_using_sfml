#include "stdafx.h"
#include "Application.hpp"

int main() {
	/* Creates application engine */
	Application application;

	/* Main loop */
	while (application.isRunning()) {

		application.update();

		application.render();

	}

	return EXIT_SUCCESS;
}