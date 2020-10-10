cmake_minimum_required( VERSION 2.8.7 FATAL_ERROR )
project(Stage)

set( V_MAJOR 4 )
set( V_MINOR 3 )
set( V_BUGFIX 0 )

set( VERSION ${V_MAJOR}.${V_MINOR}.${V_BUGFIX} )
set( APIVERSION ${V_MAJOR}.${V_MINOR} )

# minimum version of Player to build the plugin
set( MIN_PLAYER 2.1.0 )

option (BUILD_PLAYER_PLUGIN "Build Player plugin" ON)
option (BUILD_LSPTEST "Build Player plugin tests" OFF)
option (CPACK_CFG "[release building] generate CPack configuration files" ON)

# todo - this doesn't work yet. Run Stage headless with -g.
# option (BUILD_GUI "Build FLTK-based GUI. If OFF, build a gui-less Stage useful e.g. for headless compute clusters." ON ) 

if (CMAKE_MAJOR_VERSION EQUAL 2 AND NOT CMAKE_MINOR_VERSION LESS 6)
	cmake_policy( set CMP0003 NEW )
	cmake_policy( set CMP0005 OLD )
endif (CMAKE_MAJOR_VERSION EQUAL 2 AND NOT CMAKE_MINOR_VERSION LESS 6)

message (STATUS "${PROJECT_NAME} version ${VERSION}")

# Set where to find our internal CMake scripts
set (PROJECT_CMAKE_DIR ${PROJECT_SOURCE_DIR}/cmake CACHE PATH "Location of CMake scripts")
# Determine the operating system in detail
include (${PROJECT_CMAKE_DIR}/internal/FindOS.cmake)
include (${PROJECT_CMAKE_DIR}/internal/UninstallTarget.cmake)

# Enable -Wall by default unless on Win or Solaris
if (NOT PROJECT_OS_WIN AND NOT PROJECT_OS_SOLARIS)
    # Using -Wall on Windows causes MSVC to produce thousands of warnings in its
    # own standard headers, dramatically slowing down the build.
    set (WALL "-Wall " )
endif (NOT PROJECT_OS_WIN AND NOT PROJECT_OS_SOLARIS)

if( PROJECT_OS_OSX )
  set(CMAKE_MACOSX_RPATH 1)
  # uncomment this to force 32bit on OS X, for example if you can't build FLTK 64bit
  #set( FORCE_ARCH "-arch i386" )

  # suppress warnings about deprecated stuff. Forgive an old project!
  set( WALL "-Wall -Wno-deprecated-declarations" )

endif( PROJECT_OS_OSX)

#####################################
# Build type cflags
set (OPTIMIZE "-O2")

set (CMAKE_CXX_FLAGS_RELEASE "${FORCE_ARCH} ${OPTIMIZE} -DNDEBUG ${WALL}" CACHE INTERNAL "CXX Flags for release" FORCE)
set (CMAKE_CXX_FLAGS_DEBUG "-ggdb  ${FORCE_ARCH} ${WALL} -DDEBUG" CACHE INTERNAL "CXX Flags for debug" FORCE)
set (CMAKE_CXX_FLAGS_PROFILE "-ggdb -pg  ${FORCE_ARCH} ${WALL}" CACHE INTERNAL "CXX Flags for profile" FORCE)

#####################################
# Set the default build type
if (NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE "release" CACHE STRING 
    "Choose the type of build, options are: release (default) debug profile" FORCE)
endif (NOT CMAKE_BUILD_TYPE)
string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)

message( STATUS "Build type ${CMAKE_BUILD_TYPE}" )

enable_testing()

set(RGBFILE ${CMAKE_INSTALL_PREFIX}/share/stage/rgb.txt )

# Create the config.h file
# config.h belongs with the source (and not in CMAKE_CURRENT_BINARY_DIR as in Brian's original version)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in 
					${CMAKE_CURRENT_SOURCE_DIR}/config.h @ONLY)

message( STATUS "Checking for libtool" )
find_path( LTDL_INCLUDE_DIR ltdl.h DOC "Libtool include dir" )
find_library( LTDL_LIB ltdl DOC "Libtool lib" )

include_directories( 
	${OPENGL_INCLUDE_DIR}
	${LTDL_INCLUDE_DIR}
)


message( STATUS "Checking for required libraries..." )

find_package( JPEG REQUIRED )

find_package( PNG REQUIRED )

# deal with new missing X11 on OS X 10.8 Mountain Lion
# todo: fix this properly
# set( PNG_LIBRARIES /opt/X11/lib/libpng.dylib )
# set( PNG_INCLUDE_DIR /opt/X11/include )

set (FLTK_SKIP_FLUID TRUE) 
find_package( FLTK REQUIRED )
find_package( OpenGL REQUIRED )

if( NOT OPENGL_GLU_FOUND )
  message( FATAL_ERROR "OpenGL GLU not found, aborting" )
endif( NOT OPENGL_GLU_FOUND )

set( INDENT "  * " )
# message( STATUS ${INDENT} "JPEG_INCLUDE_DIR = ${JPEG_INCLUDE_DIR}" )
# message( STATUS ${INDENT} "JPEG_LIBRARIES = ${JPEG_LIBRARIES}" ) 
# message( STATUS ${INDENT} "PNG_INCLUDE_DIR = ${PNG_INCLUDE_DIR}" )
# message( STATUS ${INDENT} "PNG_LIBRARIES = ${PNG_LIBRARIES}" ) 
# message( STATUS ${INDENT} "FLTK_LIBRARIES=${FLTK_LIBRARIES}" )
# message( STATUS ${INDENT} "FLTK_INCLUDE_DIR=${FLTK_INCLUDE_DIR}" )
# message( STATUS ${INDENT} "OpenGL GLU found at ${OPENGL_INCLUDE_DIR}" )
# message( STATUS "    OPENGL_INCLUDE_DIR = ${OPENGL_INCLUDE_DIR}")
# message( STATUS "    OPENGL_glu_LIBRARY = ${OPENGL_glu_LIBRARY}")

message( STATUS "Checking for optional libraries..." )

# Format the fltk libraries for inclusion in stage.pc
set(PC_LIBRARIES ${FLTK_LIBRARIES} ${OPENGL_LIBRARIES})
set(PC_INCLUDE_DIRS ${FLTK_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})

set(PC_LINK_FLAGS "")
foreach(LIB ${PC_LIBRARIES})
  get_filename_component(LIBNAME ${LIB} NAME_WE)
  string(REGEX REPLACE "^lib" "" LINKLIB ${LIBNAME})
  set(PC_LINK_FLAGS "${PC_LINK_FLAGS} -l${LINKLIB}")
endforeach(LIB ${FLTK_LIBRARIES})

set(PC_INCLUDE_FLAGS "")
foreach(INC ${PC_INCLUDE_DIRS})
  set(PC_INCLUDE_FLAGS "${PC_INCLUDE_FLAGS} -I${INC}")
endforeach(INC ${PC_INCLUDE_DIRS})

# Create the pkgconfig file
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/stage.pc.in ${CMAKE_CURRENT_BINARY_DIR}/stage.pc @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/stage.pc DESTINATION ${PROJECT_LIB_DIR}/pkgconfig/)

message( STATUS "Installation path CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}" )

# all targets need these include directories
include_directories( . 
		     libstage 
		     replace 
		     ${FLTK_INCLUDE_DIR}
                     ${PNG_INCLUDE_DIR}
                     ${JPEG_INCLUDE_DIR}
		     ${CMAKE_INCLUDE_PATH}
)


# work through these subdirs
add_subdirectory(libstage)
add_subdirectory(examples)
add_subdirectory(assets)
add_subdirectory(worlds)
#add_subdirectory(avonstage)		 

if ( BUILD_PLAYER_PLUGIN )
  # Player does not have a CMake package, but does provide pkgconfig info
  include(FindPkgConfig)
  pkg_search_module( PLAYER playercore>=2.1.0 ) 
  if( PLAYER_FOUND )
    message( STATUS ${INDENT} "Player version ${PLAYER_VERSION} detected at ${PLAYER_PREFIX}" )
    message( STATUS "    PLAYER_CFLAGS: ${PLAYER_CFLAGS}" )
    message( STATUS "    PLAYER_LDFLAGS: ${PLAYER_LDFLAGS}"  )
  ELSE( PLAYER_FOUND )
    message(STATUS ${INDENT} "Player not detected. If Player is installed but not detected, check your PKG_CONFIG_PATH." )
  endif( PLAYER_FOUND )

  if ( PLAYER_FOUND )
    add_subdirectory(libstageplugin)
  endif ( PLAYER_FOUND )
endif ( BUILD_PLAYER_PLUGIN )

# Create the CMake module files (needs to be run after the stage target was created)
if (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION} GREATER 2.8.11)
  # Use a generator expression if the version of cmake allows it.
  set(STAGE_TARGET_NAME "$<TARGET_FILE_NAME:stage>")
else()
  # Otherwise use the LOCATION property of the target (this will produce a warning on newer versions of cmake)
  get_property(location_ TARGET stage PROPERTY LOCATION)
  get_filename_component(STAGE_TARGET_NAME "${location_}" NAME)
  unset(location_)
endif()
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/stage-config.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/stage-config.cmake @ONLY)
if (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION} GREATER 2.8.11)
  # Also run it through file(GENERATE ...) to expand generator expressions (if the version of cmake supports it).
  file (GENERATE
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/stage-config.cmake
    INPUT ${CMAKE_CURRENT_BINARY_DIR}/stage-config.cmake)
endif()
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/stage-config-version.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/stage-config-version.cmake @ONLY)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/stage-config.cmake ${CMAKE_CURRENT_BINARY_DIR}/stage-config-version.cmake DESTINATION ${PROJECT_LIB_DIR}/cmake/${PROJECT_NAME})

# generate a cpack config file used to create packaged tarballs
if ( CPACK_CFG )
  include(InstallRequiredSystemLibraries)
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_NAME}: A Multiple Robot Simulator")
  set(CPACK_PACKAGE_VENDOR "The Player Project")
  set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
  set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
  set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING.txt")
  set(CPACK_PACKAGE_VERSION_MAJOR "${V_MAJOR}")
  set(CPACK_PACKAGE_VERSION_MINOR "${V_MINOR}")
  set(CPACK_PACKAGE_VERSION_PATCH "${V_BUGFIX}")
  set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}-${V_MAJOR}.${V_MINOR}")
  include(CPack)
endif ( CPACK_CFG )
