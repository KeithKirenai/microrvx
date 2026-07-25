cmake_minimum_required(VERSION 3.20)

project(MicroRVX LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(MicroRVX WIN32
    src/main.cpp
    src/Window.cpp
    src/AudioCallback.cpp
)

target_link_libraries(MicroRVX
    user32
    gdi32
    ole32
)

target_compile_definitions(MicroRVX PRIVATE
    UNICODE
    _UNICODE
)

if(MSVC)
    target_compile_options(MicroRVX PRIVATE
        /O2
        /GL
        /GR-
    )

    target_link_options(MicroRVX PRIVATE
        /LTCG
        /OPT:REF
        /OPT:ICF
        /INCREMENTAL:NO
    )
endif()