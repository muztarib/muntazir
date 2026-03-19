cmake_minimum_required(VERSION 3.16)

project(Muntazir)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Muntazir src/main.cpp)

find_package(CURL QUIET)

if(NOT CURL_FOUND)
    message(STATUS "CURL not found, fetching...")

    include(FetchContent)

    # this is an optimisation to reduce build size
    set(BUILD_CURL_EXE OFF CACHE BOOL "" FORCE)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(HTTP_ONLY ON CACHE BOOL "" FORCE)

    FetchContent_Declare(
            curl
            GIT_REPOSITORY https://github.com/curl/curl.git
            GIT_TAG curl-8_5_0
    )

    FetchContent_MakeAvailable(curl)

    target_link_libraries(Muntazir PRIVATE libcurl)

else()
    message(STATUS "Using system CURL")

    target_link_libraries(Muntazir PRIVATE CURL::libcurl)
endif()