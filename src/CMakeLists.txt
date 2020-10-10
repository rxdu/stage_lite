# all targets need these include directories
include_directories( . 
		     libstage 
		     replace 
		     ${FLTK_INCLUDE_DIR}
                     ${PNG_INCLUDE_DIR}
                     ${JPEG_INCLUDE_DIR}
             ${CMAKE_INCLUDE_PATH}
            #  ${CMAKE_SOURCE_DIR}
)

add_subdirectory(libstage)
add_subdirectory(examples)
#add_subdirectory(avonstage)	