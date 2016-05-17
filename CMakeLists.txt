project(visualization.nastyfft)

cmake_minimum_required(VERSION 2.6)

find_package(Kodi REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${OpenGL_INCLUDE_DIR}
                    ${KODI_INCLUDE_DIR})

set(NASTY_SOURCES src/nastyfft.cpp
                  src/plugin.cpp)

set(DEPLIBS ${OPENGL_LIBRARIES})

build_addon(visualization.nastyfft NASTY DEPLIBS)

include(CPack)
