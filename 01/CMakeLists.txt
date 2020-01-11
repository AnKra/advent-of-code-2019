cmake_minimum_required(VERSION 3.10)

project(day01)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# generate executable
add_executable(${PROJECT_NAME} 
    src/main.cpp 
)

target_include_directories (${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../common/include
)

target_compile_options(${PROJECT_NAME}  PRIVATE -Werror -Wall -Wextra)
