add_library(replace
    basename.c
    dirname.c
    dummy.c
    poll.c
    scandir.c
)
# target_link_libraries(replace Threads::Threads)
target_include_directories(replace PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:include>)