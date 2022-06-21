project(DrawGrid)



set(SOURCE main.cpp
           rotate.cpp
           clipboard.cpp
           rotate.h
           inc_glut.h
           clipboard.h
           color.h
           coord.h
           coord.cpp
           octree.h
           streamline.h
           streamline.cpp
           svg_line.h
           svg_line.cpp
           face2gl.h
           face2gl.cpp
		   color_bar.h
		   color_bar.cpp
		   printtext.cpp
		   tga.h
		   tga.cpp
		   screenshot.h
		   screenshot.cpp
		   volumetric.h
		   volumetric.cpp
		   input.h
		   input.cpp
		   picker.h
		   picker.cpp
		   clipper.h
		   clipper.cpp
		   vector.h
		   vector.cpp)
find_package(OpenGL)
if(NOT USE_FREEGLUT)
	find_package(GLUT)
endif(NOT USE_FREEGLUT)




if(OPENGL_FOUND)
	if(GLUT_FOUND OR USE_FREEGLUT)
		message("linking DrawGrid with GLUT and OpenGL")
		add_executable(DrawGrid ${SOURCE})
		include_directories(${OPENGL_INCLUDE_DIR})
		target_link_libraries(DrawGrid ${OPENGL_LIBRARIES})
		if(USE_FREEGLUT)
			include_directories(${FREEGLUT_INCLUDE_DIR})
			target_link_libraries(DrawGrid ${FREEGLUT_LIBRARIES})
		else(USE_FREEGLUT)
			include_directories(${GLUT_INCLUDE_DIR})
			target_link_libraries(DrawGrid ${GLUT_LIBRARIES})
		endif(USE_FREEGLUT)
		target_link_libraries(DrawGrid ${INMOST_LIBRARIES})
		if(USE_PNG AND PNG_FOUND)
			include_directories(${PNG_INCLUDE_DIRS})
			add_definitions(-DUSE_PNG)
			target_link_libraries(DrawGrid ${PNG_LIBRARIES})
		endif(USE_PNG AND PNG_FOUND)
		install(TARGETS DrawGrid EXPORT inmost-targets RUNTIME DESTINATION bin)
	else(GLUT_FOUND OR USE_FREEGLUT)
		message("GLUT not found, not building DrawGrid")
	endif(GLUT_FOUND OR USE_FREEGLUT)
else(OPENGL_FOUND)
	message("OpenGL not found, not building DrawGrid")
endif(OPENGL_FOUND)
