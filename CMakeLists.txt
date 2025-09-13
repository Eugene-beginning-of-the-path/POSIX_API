#cmake -S . -B build -DENABLE_FILE=ON -DENABLE_PIPE=ON --log-level=DEBUG

cmake_minimum_required(VERSION 3.22)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED True)

project(POSIX_API)

option(ENABLE_FILE "build FILE example of POSIX API" OFF)
option(ENABLE_PIPE "build PIPE example of POSIX API" OFF)

set(SOURCE_LIST main.cpp)
set(INCLUDE_LIST)
set(MACROSES_DEFINE)

if (ENABLE_FILE)
    list(APPEND SOURCE_LIST file/w_w_file.cpp)
    list(APPEND INCLUDE_LIST ${CMAKE_SOURCE_DIR}/file)
    
    list(APPEND MACROSES_DEFINE ENABLE_FILE)
endif()

if (ENABLE_PIPE)
    list(APPEND SOURCE_LIST pipe/w_w_pipe.cpp)
    list(APPEND INCLUDE_LIST ${CMAKE_SOURCE_DIR}/pipe)

    list(APPEND MACROSES_DEFINE ENABLE_PIPE)
endif()

list(LENGTH SOURCE_LIST SOURCE_CNTR)
list(LENGTH INCLUDE_LIST INCLUDE_CNTR)
list(LENGTH MACROSES_DEFINE MACROSES_CNTR)

set(STATUS_MSG ".cpp: " ${SOURCE_CNTR})
list(APPEND STATUS_MSG " IncludeDir: " ${INCLUDE_CNTR})
if (INCLUDE_CNTR GREATER 0)
    string(JOIN " " INCLUDE_LIST_WITH_SPACES ${INCLUDE_LIST})
    list(APPEND STATUS_MSG "(" ${INCLUDE_LIST_WITH_SPACES} ")")
endif()
if (MACROSES_CNTR GREATER 0)
    list(APPEND STATUS_MSG " McrsDef: " ${MACROSES_CNTR})
endif()

message(DEBUG ${STATUS_MSG}) #--log-level=DEBUG

add_executable(${PROJECT_NAME} ${SOURCE_LIST})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${INCLUDE_LIST})

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        ${MACROSES_DEFINE})