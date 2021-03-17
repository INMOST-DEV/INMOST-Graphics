project(DrawGridTests)
set(SOURCE main.cpp
		rotate.cpp
		rotate.h
		my_glut.h)
find_package(OpenGL)
find_package(GLUT)

if(OPENGL_FOUND)
	if(GLUT_FOUND)
		message("linking DrawGridTests with GLUT and OpenGL")
		include_directories(${OPENGL_INCLUDE_DIR})
		include_directories(${GLUT_INCLUDE_DIR})
		add_executable(DrawGridTests ${SOURCE})
		target_link_libraries(DrawGridTests inmost ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
		target_link_libraries(DrawGridTests ${INMOST_LIBRARIES})
		install(TARGETS DrawGridTests EXPORT inmost-targets RUNTIME DESTINATION bin)
	else(GLUT_FOUND)
		message("GLUT not found, not building DrawGridTests")
	endif(GLUT_FOUND)
else(OPENGL_FOUND)
	message("OpenGL not found, not building DrawGridTests")
endif(OPENGL_FOUND)
