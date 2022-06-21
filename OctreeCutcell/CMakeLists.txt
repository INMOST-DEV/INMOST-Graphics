project(OctreeCutcell)
set(SOURCE main.cpp
           rotate.cpp
           rotate.h
	   tree.cpp
	   tree.h
	   triang.cpp
	   triang.h
	   gettype.hpp
	   obj.cpp
	   obj.h
	   octgrid.cpp
	   octgrid.h
	   point.h
	   proj.hpp
           my_glut.h)


add_executable(OctreeCutcell ${SOURCE})
target_link_libraries(OctreeCutcell ${INMOST_LIBRARIES})

find_package(OpenGL)
if(NOT USE_FREEGLUT)
	find_package(GLUT)
endif(NOT USE_FREEGLUT)


if(OPENGL_FOUND)
	if(GLUT_FOUND OR USE_FREEGLUT)
		message("linking OctreeCutcell with GLUT and OpenGL")
		include_directories(${OPENGL_INCLUDE_DIR})
		target_link_libraries(OctreeCutcell ${OPENGL_LIBRARIES})
		if(USE_FREEGLUT)
			include_directories(${FREEGLUT_INCLUDE_DIR})
			target_link_libraries(OctreeCutcell ${FREEGLUT_LIBRARIES})
		else(USE_FREEGLUT)
			include_directories(${GLUT_INCLUDE_DIR})
			target_link_libraries(OctreeCutcell ${GLUT_LIBRARIES})
		endif(USE_FREEGLUT)
		add_definitions(-D__GRAPHICS__)
	else(GLUT_FOUND OR USE_FREEGLUT)
		message("GLUT not found")
	endif(GLUT_FOUND OR USE_FREEGLUT)
else(OPENGL_FOUND)
	message("OpenGL not found")
endif(OPENGL_FOUND)

install(TARGETS OctreeCutcell EXPORT inmost-targets RUNTIME DESTINATION bin/OctreeCutcell)
install(DIRECTORY Obj oil_obj2 DESTINATION bin/OctreeCutcell)
