project(DrawGridTests)
set(SOURCE main.cpp
		rotate.cpp
		rotate.h
		my_glut.h)

cmake_policy(SET CMP0072 NEW) # Prefer GLVND to old GL when available
find_package(OpenGL)
if(NOT USE_FREEGLUT)
	find_package(GLUT)
endif(NOT USE_FREEGLUT)

if(OPENGL_FOUND)
	if(GLUT_FOUND OR USE_FREEGLUT)
		message("linking DrawGridTests with GLUT and OpenGL")
		add_executable(DrawGridTests ${SOURCE})
		include_directories(${OPENGL_INCLUDE_DIR})
		target_link_libraries(DrawGridTests ${OPENGL_LIBRARIES})
		if(USE_FREEGLUT)
			include_directories(${FREEGLUT_INCLUDE_DIR})
			target_link_libraries(DrawGridTests ${FREEGLUT_LIBRARIES})
		else(USE_FREEGLUT)
			include_directories(${GLUT_INCLUDE_DIR})
			target_link_libraries(DrawGridTests ${GLUT_LIBRARIES})
		endif(USE_FREEGLUT)
		target_link_libraries(DrawGridTests ${INMOST_LIBRARIES})
		install(TARGETS DrawGridTests EXPORT inmost-targets RUNTIME DESTINATION bin)
	else(GLUT_FOUND OR USE_FREEGLUT)
		message("GLUT not found, not building DrawGridTests")
	endif(GLUT_FOUND OR USE_FREEGLUT)
else(OPENGL_FOUND)
	message("OpenGL not found, not building DrawGridTests")
endif(OPENGL_FOUND)
