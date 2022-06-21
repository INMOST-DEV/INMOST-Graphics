#pragma once
#if defined(__GRAPHICS__)
#if defined(USE_FREEGLUT)
#include <GL/freeglut.h>
#else //USE_FREEGLUT
#if defined (__APPLE__) || defined(MAXOSX)
#include <GLUT/glut.h>
#endif
#if defined(_WIN32)
//#include <GL/glut.h>
#include <windows.h>
#include <GL/glut.h>
//#include "glut.h"
#pragma comment(lib,"glut32.lib")
#endif
#if defined(__linux__)
#include <GL/glut.h>
#endif
#endif //USE_FREEGLUT
#endif //__GRAPHICS__
