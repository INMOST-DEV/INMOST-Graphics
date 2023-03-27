project(Octree)
set(SOURCE main.cpp
           rotate.cpp
           rotate.h
	   octgrid.cpp
	   octgrid.h
           my_glut.h)

set(SOURCE_LIGHT main_light.cpp
	   octgrid.cpp
	   octgrid.h)

set(SOURCE_TEST main_test.cpp
	   octgrid.cpp
	   octgrid.h)


add_executable(Octree ${SOURCE})
add_executable(Octree_light ${SOURCE_LIGHT})
add_executable(Octree_test ${SOURCE_TEST})
set(TARGETS Octree Octree_light Octree_test)

foreach(vtarget ${TARGETS})
target_link_libraries(${vtarget} ${INMOST_LIBRARIES})
#target_link_libraries(${vtarget} lapack blas) 

cmake_policy(SET CMP0072 NEW) # Prefer GLVND to old GL when available
find_package(OpenGL)
if(NOT USE_FREEGLUT)
	find_package(GLUT)
endif(NOT USE_FREEGLUT)

if(OPENGL_FOUND)
	if(GLUT_FOUND OR USE_FREEGLUT)
		include_directories(${OPENGL_INCLUDE_DIR})
		target_link_libraries(${vtarget} ${OPENGL_LIBRARIES})
		if(USE_FREEGLUT)
			include_directories(${FREEGLUT_INCLUDE_DIR})
			target_link_libraries(${vtarget} ${FREEGLUT_LIBRARIES})
		else(USE_FREEGLUT)
			include_directories(${GLUT_INCLUDE_DIR})
			target_link_libraries(${vtarget} ${GLUT_LIBRARIES})
		endif(USE_FREEGLUT)
		add_definitions(-D__GRAPHICS__)
	else(GLUT_FOUND OR USE_FREEGLUT)
		message("GLUT not found for ${vtarget}")
	endif(GLUT_FOUND OR USE_FREEGLUT)
else(OPENGL_FOUND)
	message("OpenGL not found for ${vtarget}")
endif(OPENGL_FOUND)
endforeach(vtarget ${TARGETS})

#if(NOT GLUT_FOUND)
#	set_target_properties(Octree PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
#endif(NOT GLUT_FOUND)
