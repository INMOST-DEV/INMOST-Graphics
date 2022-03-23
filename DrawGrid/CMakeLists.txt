project(DrawGrid)

option(USE_PNG "Use libpng to output png images (experimental)" OFF)

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
find_package(GLUT)


#if(WIN32)
#    # Prefix all shared libraries with 'lib'.
#    set(CMAKE_SHARED_LIBRARY_PREFIX "lib")
#    # Prefix all static libraries with 'lib'.
#    set(CMAKE_STATIC_LIBRARY_PREFIX "lib")
#endif()
if(MSVC)
	set(LIBPNG_APPEND "lib") # for libpng16
	set(LIBPNG_STATIC_NAME "_static")
	set(ZLIB_STATIC_NAME "static")
else(MSVC)
	set(LIBPNG_APPEND "")
	set(LIBPNG_STATIC_NAME "")
	set(ZLIB_STATIC_NAME "")
endif(MSVC)

if(USE_PNG)
	set(LIB_DOWNLOAD_PATH "${CMAKE_BINARY_DIR}/Libraries/download")
	include(ExternalProject)
	ExternalProject_Add(ZLIB
							GIT_REPOSITORY "https://github.com/madler/zlib.git"
							GIT_TAG "master"
							UPDATE_DISCONNECTED 1
							PREFIX "${LIB_DOWNLOAD_PATH}"
							#INSTALL_COMMAND true
							CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${LIB_DOWNLOAD_PATH} " #no install script!
									   "-DCMAKE_INSTALL_INCLUDEDIR=${LIB_DOWNLOAD_PATH}/include/"
									   "-DMPI_C_COMPILER=${CMAKE_C_COMPILER} "
									   "-DMPI_CXX_COMPILER=${CMAKE_CXX_COMPILER} "
									   "-DCMAKE_BUILD_TYPE=Release ")
	ExternalProject_Get_Property(ZLIB install_dir)
	add_library(zlib STATIC IMPORTED )
	message("location: ${install_dir}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}zlib${ZLIB_STATIC_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX}")
	set_target_properties(zlib PROPERTIES IMPORTED_LOCATION
		${install_dir}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}zlib${ZLIB_STATIC_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX}
	)
	add_dependencies(zlib ZLIB)
	set(ZLIB_LIBRARIES zlib) 
	set(ZLIB_DIR ${install_dir})
	set(ZLIB_INCLUDE_DIRS ${install_dir}/include)
	find_package_handle_standard_args(ZLIB DEFAULT_MSG ZLIB_LIBRARIES ZLIB_INCLUDE_DIRS)

	ExternalProject_Add(PNG
					GIT_REPOSITORY "git://git.code.sf.net/p/libpng/code"
					GIT_TAG "master"
					DEPENDS ZLIB
					#URL "http://prdownloads.sourceforge.net/libpng/libpng-1.6.37.tar.gz?download"
					UPDATE_DISCONNECTED 1
					PREFIX "${LIB_DOWNLOAD_PATH}"
					#INSTALL_COMMAND true
					CMAKE_ARGS  "-DCMAKE_INSTALL_PREFIX=${LIB_DOWNLOAD_PATH} "
								"-DCMAKE_INSTALL_INCLUDEDIR=${LIB_DOWNLOAD_PATH}/include/"
								"-DMPI_C_COMPILER=${CMAKE_C_COMPILER} "
								"-DMPI_CXX_COMPILER=${CMAKE_CXX_COMPILER} "
								"-DCMAKE_BUILD_TYPE=Release ")
	ExternalProject_Get_Property(PNG install_dir)
	add_library(png STATIC IMPORTED )
	message("location: ${install_dir}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}${LIBPNG_APPEND}png16${LIBPNG_STATIC_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX}")
	set_target_properties(png PROPERTIES IMPORTED_LOCATION
		${install_dir}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}${LIBPNG_APPEND}png16${LIBPNG_STATIC_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX}
	)
	add_dependencies(png PNG)
	set(PNG_LIBRARIES png ${ZLIB_LIBRARIES}) 
	set(PNG_DIR ${install_dir})
	set(PNG_INCLUDE_DIRS ${install_dir}/include)
	find_package_handle_standard_args(PNG DEFAULT_MSG PNG_LIBRARIES PNG_INCLUDE_DIRS)
	if(PNG_FOUND)
		#set(PNG_LIBRARIES_OPTIONAL ${PNG_LIBRARIES})
		message("png found")
		#message(${PNG_LIBRARIES})
		#message(${PNG_INCLUDE_DIRS})
	endif(PNG_FOUND)
	#add_dependencies(PNG ZLIB)
endif(USE_PNG)

if(OPENGL_FOUND)
  if(GLUT_FOUND)
    message("linking DrawGrid with GLUT and OpenGL")
    include_directories(${OPENGL_INCLUDE_DIR})
    include_directories(${GLUT_INCLUDE_DIR})
    add_executable(DrawGrid ${SOURCE})
    target_link_libraries(DrawGrid inmost ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
    target_link_libraries(DrawGrid ${INMOST_LIBRARIES})
	if(USE_PNG AND PNG_FOUND)
		include_directories(${PNG_INCLUDE_DIRS})
		add_definitions(-DUSE_PNG)
		target_link_libraries(DrawGrid ${PNG_LIBRARIES})
	endif(USE_PNG AND PNG_FOUND)
    install(TARGETS DrawGrid EXPORT inmost-targets RUNTIME DESTINATION bin)
  else(GLUT_FOUND)
    message("GLUT not found, not building DrawGrid")
  endif(GLUT_FOUND)
else(OPENGL_FOUND)
  message("OpenGL not found, not building DrawGrid")
endif(OPENGL_FOUND)
