cmake_minimum_required(VERSION 2.8)
project(Graphics)

option(USE_FREEGLUT "Compile with freeglut instead of glut" ON)
option(USE_PNG "Use libpng to output png images" ON)

find_package(inmost REQUIRED)

link_directories(${INMOST_LIBRARY_DIRS})
include_directories(${INMOST_INCLUDE_DIRS})
add_definitions(${INMOST_DEFINITIONS})

if(USE_FREEGLUT)
	set(LIB_DOWNLOAD_PATH "${CMAKE_BINARY_DIR}/Libraries/download")
	include(ExternalProject)
	ExternalProject_Add(FREEGLUT
							GIT_REPOSITORY "https://github.com/FreeGLUTProject/freeglut.git"
							GIT_TAG "master"
							UPDATE_DISCONNECTED 1
							PREFIX "${LIB_DOWNLOAD_PATH}"
							CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${LIB_DOWNLOAD_PATH} "
									   "-DCMAKE_INSTALL_INCLUDEDIR=${LIB_DOWNLOAD_PATH}/include/"
									   "-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} "
									   "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} "
									   "-DFREEGLUT_BUILD_DEMOS=OFF "
									   "-DFREEGLUT_BUILD_SHARED_LIBS=OFF "
									   )
	ExternalProject_Get_Property(FREEGLUT install_dir)
	add_library(freeglut_static STATIC IMPORTED)
	message("location: ${install_dir}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}freeglut_static${CMAKE_STATIC_LIBRARY_SUFFIX}")
	set_target_properties(freeglut_static PROPERTIES IMPORTED_LOCATION
		${install_dir}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}freeglut_static${CMAKE_STATIC_LIBRARY_SUFFIX})
	set_target_properties(freeglut_static PROPERTIES IMPORTED_LOCATION_DEBUG
		${install_dir}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}freeglut_staticd${CMAKE_STATIC_LIBRARY_SUFFIX})
	add_dependencies(freeglut_static FREEGLUT)
	set(FREEGLUT_LIBRARIES freeglut_static) 
	set(FREEGLUT_DIR ${install_dir})
	set(FREEGLUT_INCLUDE_DIR ${install_dir}/include)
	find_package_handle_standard_args(FREEGLUT DEFAULT_MSG FREEGLUT_LIBRARIES FREEGLUT_INCLUDE_DIR)

	if(FREEGLUT_FOUND)
		message("freeglut found")
		message(${FREEGLUT_LIBRARIES})
		message(${FREEGLUT_INCLUDE_DIR})
		add_definitions(-DUSE_FREEGLUT)
		add_definitions(-DFREEGLUT_STATIC)
	endif(FREEGLUT_FOUND)
endif(USE_FREEGLUT)


if(USE_PNG)
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
	set_target_properties(zlib PROPERTIES IMPORTED_LOCATION_DEBUG
		${install_dir}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}zlib${ZLIB_STATIC_NAME}d${CMAKE_STATIC_LIBRARY_SUFFIX}
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
	set_target_properties(png PROPERTIES IMPORTED_LOCATION_DEBUG
		${install_dir}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}${LIBPNG_APPEND}png16${LIBPNG_STATIC_NAME}d${CMAKE_STATIC_LIBRARY_SUFFIX}
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

add_subdirectory(DrawMatrix)
add_subdirectory(DrawGrid)
add_subdirectory(DrawGridTests)
if( USE_MPI )
	add_subdirectory(Octree)
endif( USE_MPI )
add_subdirectory(OctreeCutcell)
