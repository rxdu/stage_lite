file(GLOB worlds "*.world")
file(GLOB incs "*.inc")
install(FILES ${worlds} ${incs}
        DESTINATION share/stage/worlds/wifi
)

