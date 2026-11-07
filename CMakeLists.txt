cmake_minimum_required(VERSION 3.10.0)

project(STM32_Gimbal_Project VERSION 0.1.0 LANGUAGES C ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/user
    ${CMAKE_CURRENT_SOURCE_DIR}/Bsp
    ${CMAKE_CURRENT_SOURCE_DIR}/my_lib
    ${CMAKE_CURRENT_SOURCE_DIR}/std_periph_driver/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/startup
)

file(GLOB_RECURSE ALL_SOURCES "*.c" "*.s")

list(REMOVE_ITEM ALL_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/Objects/*" 
    "${CMAKE_CURRENT_SOURCE_DIR}/Listings/*" 
    "${CMAKE_CURRENT_SOURCE_DIR}/DebugConfig/*"
)

add_executable(${PROJECT_NAME} ${ALL_SOURCES})