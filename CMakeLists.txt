set (CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE Debug)

cmake_minimum_required(VERSION "3.21.0")
project("Vulkan Graphics Engine" VERSION 1.0.0)

add_subdirectory(glfw-3.3.4)
add_subdirectory(glm-0.9.9.8)

find_package(Vulkan REQUIRED)

# Add all source files
file(GLOB_RECURSE SOURCE_FILES 
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp)
	
# Add all header files
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_SOURCE_DIR}/inc/*.h
	${CMAKE_SOURCE_DIR}/inc/*.hpp)


add_executable(vge src/main.cpp)

target_link_libraries(vge glfw ${GLFW_LIBRARIES})
target_link_libraries(vge glm::glm)
target_link_libraries(vge Vulkan::Vulkan)

install(TARGETS vge DESTINATION bin)
install(FILES main.cpp DESTINATION src)