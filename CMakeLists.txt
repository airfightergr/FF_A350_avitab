# Useful command line flags:
#   * -DCMAKE_BUILD_TYPE=XXX, where XXX is one of Debug, Release,
#     RelWithDebInfo and MinSizeRel. Default is Release.
#   * -DCREATECOMPLETEEDITION=XXX, where XXX is true or false. Default is true.

cmake_minimum_required(VERSION 3.20)
project(ffa350_avitab VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable all X-Plane SDK APIs up to the newest version.
add_definitions(-DXPLM200=1 -DXPLM210=1 -DXPLM300=1 -DXPLM301=1 -DXPLM302=1 -DXPLM303=1 -DXPLM400=1)

if (APPLE)
    #    set(CMAKE_OSX_DEPLOYMENT_TARGET "11.3" CACHE STRING "Minimum macOS version" FORCE)
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "Build architectures for Mac OS X" FORCE) # We will enable this when we have a SDK supporting arm
    set(ARCH "mac64")
    add_definitions(-DAPL=1 -DIBM=0 -DLIN=0)

elseif (WIN32)
    set(ARCH "mingw64")
    add_definitions(-DAPL=0 -DIBM=1 -DLIN=0)
    add_definitions(-D_WIN32_WINNT=0x0600)

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
    set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_C_FLAGS} -static-libgcc -s")
    set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS}  -static-libgcc -static-libstdc++ -s")

else ()
    set(ARCH "lin64")
    add_definitions(-DAPL=0 -DIBM=0 -DLIN=1)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused -Wall -Wno-missing-field-initializers -Wshadow -Wextra -Wno-error")
endif ()


set(SDK_ROOT "${CMAKE_SOURCE_DIR}/SDK")

# Set include directories used by our code and dependencies.
include_directories(
        "SDK/CHeaders/Widgets"
        "SDK/CHeaders/Wrappers"
        "SDK/CHeaders/XPLM"
)


# Specify library search locations.
if (WIN32)
    list(APPEND CMAKE_LIBRARY_PATH "${CMAKE_SOURCE_DIR}/SDK/Libraries/Win")
#elseif (APPLE)
#    list(APPEND CMAKE_FRAMEWORK_PATH "${CMAKE_SOURCE_DIR}/SDK/Libraries/Mac")
#elseif (UNIX)

endif ()


find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)

add_library(ffa350_avitab SHARED src/main.cpp )

target_compile_options(ffa350_avitab PUBLIC -Wall -Wextra )
target_link_libraries(ffa350_avitab PUBLIC ${LIBACFUTILS_LIBS} ${OPENGL_LIBRARIES})

# X-Plane XPLM stuff
if (APPLE)
    find_library(XPLM_LIBRARY XPLM ${SDK_ROOT}/Libraries/Mac/)
    find_library(XPWIDGETS_LIBRARY XPWidgets ${SDK_ROOT}/Libraries/Mac/)
    find_library(CocoaLib Cocoa)

    target_link_libraries(ffa350_avitab PUBLIC ${XPLM_LIBRARY})
    target_link_libraries(ffa350_avitab PUBLIC ${XPWIDGETS_LIBRARY})
    target_link_libraries(ffa350_avitab PUBLIC ${CocoaLib})

elseif (WIN32)
    set(CMAKE_LIBRARY_PATH ${SDK_ROOT}/Libraries/Win/)
    find_library(XPLM_LIBRARY
            NAMES XPLM_64
            PATHS ${SDK_ROOT}/Libraries/Win/
            NO_DEFAULT_PATH
            NO_CMAKE_FIND_ROOT_PATH
    )
    #    find_library(XPWIDGETS_LIBRARY
    #            NAMES XPLMWidgets_64
    #            PATHS SDK/Libraries/Win/
    #            NO_DEFAULT_PATH
    #            NO_CMAKE_FIND_ROOT_PATH
    #    )
    target_link_libraries(ffa350_avitab PUBLIC ${XPLM_LIBRARY} ${XPWIDGETS_LIBRARY})

endif ()

set_target_properties(ffa350_avitab PROPERTIES LINK_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fno-stack-protector -fPIC -fvisibility=hidden -Wno-unused")

add_compile_options(-fexceptions)
add_compile_options(-fvisibility=hidden)
add_compile_options(-Wall -Wpedantic -Wshadow -Wfloat-equal -Wextra)
add_compile_options(-Wno-unused)


set(PLUGIN_BUILD_DIR "${CMAKE_SOURCE_DIR}/ffa350_avitab")

set_target_properties(ffa350_avitab PROPERTIES PREFIX "")
set_target_properties(ffa350_avitab PROPERTIES SUFFIX "")
#set_target_properties(ffa350_avitab PROPERTIES SUFFIX ".xpl")      # Enable to build locally, disable before committing
#set_target_properties(ffa350_avitab PROPERTIES OUTPUT_NAME "ffa350_avitab")    # Enable to build locally, disable before committing


if (WIN32)
    set_target_properties(ffa350_avitab PROPERTIES OUTPUT_NAME "win.xpl")
#    set_target_properties(ffa350_avitab PROPERTIES RUNTIME_OUTPUT_DIRECTORY "ffa350_avitab/win_x64")
#    set_target_properties(ffa350_avitab PROPERTIES LIBRARY_OUTPUT_DIRECTORY "ffa350_avitab/win_x64")
elseif (APPLE)
    set_target_properties(ffa350_avitab PROPERTIES OUTPUT_NAME "mac.xpl")
#    set_target_properties(ffa350_avitab PROPERTIES RUNTIME_OUTPUT_DIRECTORY "ffa350_avitab/mac_x64")
#    set_target_properties(ffa350_avitab PROPERTIES LIBRARY_OUTPUT_DIRECTORY "ffa350_avitab/mac_x64")
elseif (UNIX)
    set_target_properties(ffa350_avitab PROPERTIES OUTPUT_NAME "lin.xpl")
#    set_target_properties(ffa350_avitab PROPERTIES RUNTIME_OUTPUT_DIRECTORY "ffa350_avitab/lin_x64")    # Enable to build locally, disable before committing
#    set_target_properties(ffa350_avitab PROPERTIES LIBRARY_OUTPUT_DIRECTORY "ffa350_avitab/lin_x64")    # Enable to build locally, disable before committing
endif ()

