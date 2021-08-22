set (CMAKE_CXX_STANDARD 17)

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

if (${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "AMD64")
	set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin/glslangValidator.exe")
else()
	set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin32/glslangValidator.exe")
endif()
  
file(GLOB_RECURSE GLSL_SOURCE_FILES
	"shaders/*.frag"
	"shaders/*.vert"
)
  
foreach(GLSL ${GLSL_SOURCE_FILES})
	get_filename_component(FILE_NAME ${GLSL} NAME)
	set(SPIRV "${PROJECT_BINARY_DIR}/shaders/${FILE_NAME}.spv")
	add_custom_command(
		OUTPUT ${SPIRV}
		COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/shaders/"
		COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
		DEPENDS ${GLSL})
	list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)
  
add_custom_target(
	Shaders 
	DEPENDS ${SPIRV_BINARY_FILES}
)
  
add_dependencies(vge Shaders)
  
add_custom_command(TARGET vge POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:vge>/shaders/"
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${PROJECT_BINARY_DIR}/shaders"
		"$<TARGET_FILE_DIR:vge>/shaders"
	)

