cmake_minimum_required(VERSION "3.21.0")
project("Grpahics" VERSION 1.0.0)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/inc") 


add_executable("${PROJECT_NAME}" "src/main.cpp")

install(TARGETS "${PROJECT_NAME}" DESTINATION bin)
install(FILES "main.cpp" DESTINATION src)