message( STATUS "Configuring libstage" )

# Create the config.h file
# config.h belongs with the source (and not in CMAKE_CURRENT_BINARY_DIR as in Brian's original version)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/config.h.in 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/config.h @ONLY)

################################################################

## libstage
set(STAGE_LIB_SRC    
	src/block.cc
	src/blockgroup.cc
	src/camera.cc
	src/color.cc
	src/file_manager.cc
	src/file_manager.hh
	src/gl.cc
	src/logentry.cc
	src/model.cc
	src/model_actuator.cc
	src/model_blinkenlight.cc
	src/model_blobfinder.cc
	src/model_bumper.cc
	src/model_callbacks.cc
	src/model_camera.cc
	src/model_draw.cc
	src/model_fiducial.cc
	src/model_gripper.cc
	src/model_lightindicator.cc
	src/model_position.cc
	src/model_ranger.cc
	src/option.cc
	src/powerpack.cc
	src/region.cc
	src/stage.cc
	# stage.hh
	src/texture_manager.cc
	src/typetable.cc		
	src/world.cc			
	src/worldfile.cc		
	src/canvas.cc 
	src/options_dlg.cc
	src/options_dlg.hh
	src/vis_strip.cc			
	src/worldgui.cc 
	src/ancestor.cc
#	model_getset.cc
#	model_load.cc
)

#set_source_files_properties( ${STAGE_LIB_SRC} PROPERTIES COMPILE_FLAGS" )
add_library(stage SHARED ${STAGE_LIB_SRC})
# causes the shared library to have a version number
set_target_properties(stage PROPERTIES VERSION ${VERSION})
# set_target_properties(stage PROPERTIES VERSION ${VERSION} LINK_FLAGS "${FLTK_LDFLAGS}")
 # if fltk-config didn't bring along the OpenGL dependencies (eg. on Debian/Ubuntu), add them explicity 
if (NOT(${FLTK_LDFLAGS} MATCHES "-lGL"))
  target_link_libraries(stage ${OPENGL_LIBRARIES}) 
endif (NOT(${FLTK_LDFLAGS} MATCHES "-lGL"))
target_link_libraries(stage ${LTDL_LIB} ${JPEG_LIBRARIES} ${PNG_LIBRARIES} ${FLTK_LIBRARIES})
target_include_directories(stage PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/libstage>
)

################################################################

## Standalone stage
set(STAGE_APP_SRC src/main.cc)
set_source_files_properties(${STAGE_APP_SRC} PROPERTIES COMPILE_FLAGS "${FLTK_CFLAGS}")
add_executable(stagebinary ${STAGE_APP_SRC})
set_target_properties(stagebinary PROPERTIES OUTPUT_NAME stage)
# Apple seems to have trouble when libstage and stagebinary are linked against FLTK
# Newer Linux distributions won't allow stagebinary to inherit libstage's links to fltk, so we need
# to explicitly link on Linux
target_link_libraries(stagebinary stage)
if(PROJECT_OS_LINUX)
  target_link_libraries(stagebinary stage pthread)
endif(PROJECT_OS_LINUX)

# TODO: build an app bundle on OS X
#set(MACOSX_BUNDLE_INFO_STRING "${PROJECT_NAME}")
#set(MACOSX_BUNDLE_GUI_IDENTIFIER "org.playerstage.stage")
#set(MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_NAME} Version ${VERSION}")
#set(MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME})
#set(MACOSX_BUNDLE_SHORT_VERSION_STRING ${VERSION})
#set(MACOSX_BUNDLE_BUNDLE_VERSION ${VERSION})
#set(MACOSX_BUNDLE_COPYRIGHT "Copyright 2010. All Rights Reserved.")
# add_executable(stagebinary MACOSX_BUNDLE ${STAGE_APP_SRC})

################################################################

## Installation
install(TARGETS stagebinary stage
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION ${PROJECT_LIB_DIR}
)

# install(DIRECTORY include/libstage
#         DESTINATION include/${PROJECT_NAME}-${APIVERSION})

install(DIRECTORY include/libstage
        DESTINATION include)
