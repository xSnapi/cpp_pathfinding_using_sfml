#pragma once

#include <iostream>

template<typename ...Args>
static void Log(Args&&... args) {
	(std::cerr << ... << args);
}

#ifdef DEBUG

#define LOG(...)		Log(__VA_ARGS__)
#define ASSERT(VAR)		if(!(VAR))	__debugbreak();
#define DB_ASSERT(VAR)	if(!(VAR))	__debugbreak();

#else

#define LOG(...)
#define ASSERT(VAR)		VAR
#define DB_ASSERT(VAR)


#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")

#endif
