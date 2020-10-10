
file( GLOB pngs "*.png" )

install(FILES ${pngs} rgb.txt 
        DESTINATION share/stage/assets
)

install(FILES rgb.txt 
        DESTINATION share/stage
)

