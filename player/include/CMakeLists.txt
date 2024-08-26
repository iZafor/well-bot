add_library(libplayer player.c)

find_package(PkgConfig REQUIRED)

pkg_check_modules(SDL REQUIRED sdl2)
pkg_check_modules(FFMPEG REQUIRED libavformat libavcodec libavutil)

target_include_directories(
    libplayer 
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SDL_INCLUDE_DIRS} 
    ${FFMPEG_INCLUDE_DIRS}
)

target_link_libraries(
    libplayer  
    ${SDL_LIBRARIES} 
    ${FFMPEG_LIBRARIES}
)