message( STATUS "Looking for Avon" )

pkg_search_module( AVON avon )
if( AVON_FOUND )
  message( STATUS ${INDENT} "Avon version ${AVON_VERSION} detected at ${AVON_PREFIX}" )
  message( STATUS "    AVON_CFLAGS = ${AVON_CFLAGS}" )
  message( STATUS "    AVON_LDFLAGS = ${AVON_LDFLAGS}" )
  message( STATUS "    AVON_INCLUDE_DIRS = ${AVON_INCLUDE_DIRS}" )
  message( STATUS "    AVON_LIBRARY_DIRS = ${AVON_LIBRARY_DIRS}" )
  message( STATUS "Configuring avonstage" )
  include_directories( ${AVON_INCLUDE_DIRS} )
  link_directories( ${AVON_LIBRARY_DIRS} )
  add_executable( avonstage avonstage.cc )
  target_link_libraries( avonstage stage ${AVON_LIBRARIES} )

	set_target_properties( avonstage PROPERTIES LINK_FLAGS "${FLTK_LDFLAGS}" )

  install(TARGETS avonstage RUNTIME DESTINATION bin )

ELSE( AVON_FOUND )
  message( STATUS ${INDENT} "Avon not detected. If Avon is installed but not detected, check your PKG_CONFIG_PATH." )
endif( AVON_FOUND )




