cmake_minimum_required(VERSION 3.8)
project(clion)

set(CMAKE_CXX_STANDARD 11)

include_directories(
    ../../
    ../../lib/CoreFS
)

set(SOURCE_FILES
        ../../CoreDir/CoreDir.h
        ../../CoreDir/include/CoreDir.h
        ../../CoreDir/include/CoreDir_Utils.h

        ../../CoreDir/src/CoreDir.cpp
        ../../CoreDir/src/os_functions.h
        ../../CoreDir/src/gnu_linux/os_functions.cpp
        ../../CoreDir/src/win32/os_functions.cpp

        ../../lib/CoreFS/CoreFS/CoreFS.h
        ../../lib/CoreFS/CoreFS/include/CoreFS.h
        ../../lib/CoreFS/CoreFS/include/CoreFS_Utils.h
        ../../lib/CoreFS/CoreFS/src/CoreFS.cpp
        ../../lib/CoreFS/CoreFS/src/CoreFS_GNU_Linux.cpp
        ../../lib/CoreFS/CoreFS/src/CoreFS_Win32.cpp

        ../main.cpp
)

add_executable(clion ${SOURCE_FILES})
