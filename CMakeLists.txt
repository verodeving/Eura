cmake_minimum_required(VERSION 4.1.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_CXX_EXTENSIONS off)
message(STATUS "Building on ${CMAKE_BUILD_TYPE} mode...")
project(Eura VERSION 0.0.1 LANGUAGES CXX)
add_executable(Eura)
target_compile_definitions(Eura PRIVATE eura_major=0 eura_middle=0 eura_minor=1)
target_include_directories(Eura PRIVATE Source)
target_compile_features(Eura PRIVATE cxx_std_26)
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_definitions(Eura PRIVATE eura_windows= _CRT_SECURE_NO_WARNINGS=)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_compile_definitions(Eura PRIVATE eura_linux=)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_compile_definitions(Eura PRIVATE eura_darwin=)
else()
    message(FATAL_ERROR "Unsupported system: ${CMAKE_SYSTEM_NAME}. Eura currently supports Windows, Linux, and macOS.")
endif()
get_property(EuraMulti GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|AppleClang")
    target_compile_options(Eura PRIVATE -Werror -Wall -Wextra -Wpedantic -fno-rtti -fno-exceptions)
    if(EuraMulti)
        target_compile_options(Eura PRIVATE $<$<CONFIG:Debug>:-g -O0> $<$<CONFIG:Release>:-O3 -flto=full>)
        target_link_options(Eura PRIVATE $<$<CONFIG:Debug>:-g> $<$<CONFIG:Release>:-flto=full>)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(Eura PRIVATE -g -O0)
        target_link_options(Eura PRIVATE -g)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(Eura PRIVATE -O3 -flto=full)
        target_link_options(Eura PRIVATE -flto=full)
    else()
        message(FATAL_ERROR "Unsupported configuration. Eura currently supports Debug and Release.")
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
        target_compile_options(Eura PRIVATE -Werror -Wall -Wextra -Wpedantic -fno-rtti -fno-exceptions)
        if(EuraMulti)
            target_compile_options(Eura PRIVATE $<$<CONFIG:Debug>:-g -O0> $<$<CONFIG:Release>:-O3 -flto=full>)
            target_link_options(Eura PRIVATE $<$<CONFIG:Debug>:-g> $<$<CONFIG:Release>:-flto=full>)
        elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
            target_compile_options(Eura PRIVATE -g -O0)
            target_link_options(Eura PRIVATE -g)
        elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
            target_compile_options(Eura PRIVATE -O3 -flto=full)
            target_link_options(Eura PRIVATE -flto=full)
        else()
            message(FATAL_ERROR "Unsupported configuration. Eura currently supports Debug and Release.")
        endif()
    elseif(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
        target_compile_options(Eura PRIVATE /WX /W4 /permissive- /GR- /EHsc-)
        if(EuraMulti)
            target_compile_options(Eura PRIVATE $<$<CONFIG:Debug>:/Zi /Od /MDd> $<$<CONFIG:Release>:/Ox /GL>)
            target_link_options(Eura PRIVATE $<$<CONFIG:Debug>:/DEBUG> $<$<CONFIG:Release>:/LTCG>)
        elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
            target_compile_options(Eura PRIVATE /Zi /Od /MDd)
            target_link_options(Eura PRIVATE /DEBUG)
        elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
            target_compile_options(Eura PRIVATE /Ox /GL)
            target_link_options(Eura PRIVATE /LTCG)
        else()
            message(FATAL_ERROR "Unsupported configuration. Eura currently supports Debug and Release.")
        endif()
    else()
        message(FATAL_ERROR "Unsupported Clang's front-end variant. Eura currently supports GNU and MSVC.")
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(Eura PRIVATE /WX /W4 /permissive- /GR- /EHsc-)
    if(EuraMulti)
        target_compile_options(Eura PRIVATE $<$<CONFIG:Debug>:/Zi /Od /MDd> $<$<CONFIG:Release>:/Ox /GL>)
        target_link_options(Eura PRIVATE $<$<CONFIG:Debug>:/DEBUG> $<$<CONFIG:Release>:/LTCG>)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(Eura PRIVATE /Zi /Od /MDd)
        target_link_options(Eura PRIVATE /DEBUG)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(Eura PRIVATE /Ox /GL)
        target_link_options(Eura PRIVATE /LTCG)
    else()
        message(FATAL_ERROR "Unsupported configuration. Eura currently supports Debug and Release.")
    endif()
else()
    message(FATAL_ERROR "Unsupported C++ compiler. Eura currently supports GNU GCC, Clang, and MSVC.")
endif()
file(GLOB_RECURSE EURA_SOURCES "Source/*.cpp")
target_sources(Eura PRIVATE ${EURA_SOURCES})
if(EURA_TEST)
    file(GLOB_RECURSE EURA_TESTS "Test/*.cpp")
    set(EURA_TEST_DEPENDENCIES "")
    enable_testing()
    foreach(EURA_TEST_FILE ${EURA_TESTS})
        file(RELATIVE_PATH REL_PATH "${CMAKE_SOURCE_DIR}/Test" "${EURA_TEST_FILE}")
        string(REPLACE "/" "_" TARGET_NAME "${REL_PATH}")
        string(REPLACE "." "_" TARGET_NAME "${TARGET_NAME}")
        set(EXEC_NAME "EuraTest_${TARGET_NAME}")
        add_executable(${EXEC_NAME} ${EURA_TEST_FILE} ${EURA_SOURCES})
        add_test(NAME ${EURA_TEST_FILE} COMMAND ${EXEC_NAME})
        list(APPEND EURA_TEST_DEPENDENCIES ${EXEC_NAME})
    endforeach()
    add_custom_target(RunEuraTests ALL COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    add_dependencies(RunEuraTests ${EURA_TEST_DEPENDENCIES})
endif()