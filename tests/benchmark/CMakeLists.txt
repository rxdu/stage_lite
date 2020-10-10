set( expand_swarmSrcs expand_swarm.cc )
add_library( expand_swarm MODULE ${expand_swarmSrcs} )
target_link_libraries( expand_swarm stage )
set_source_files_properties( ${expand_swarmSrcs} PROPERTIES COMPILE_FLAGS "${FLTK_CFLAGS}" )
set_target_properties( expand_swarm PROPERTIES PREFIX "" )

set( expand_pioneerSrcs expand_pioneer.cc )
add_library( expand_pioneer MODULE ${expand_pioneerSrcs} )
target_link_libraries( expand_pioneer stage )
set_source_files_properties( ${expand_pioneerSrcs} PROPERTIES COMPILE_FLAGS "${FLTK_CFLAGS}" )
set_target_properties( expand_pioneer PROPERTIES PREFIX "" )

install( TARGETS expand_swarm expand_pioneer DESTINATION ${PROJECT_PLUGIN_DIR})
