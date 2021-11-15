cmake_minimum_required(VERSION 3.21.0)

project(GBE)

# Add source files
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)
	
# Add header files
file(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}/src/*.hpp)

# Define the executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

# OPENGL
target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})

# GLFW
add_subdirectory(deps/glfw-3.3.5)
target_include_directories(${PROJECT_NAME} PUBLIC deps/glfw-3.3.5/include)
target_link_directories(${PROJECT_NAME} PUBLIC deps/glfw-3.3.5/src)
target_link_libraries(${PROJECT_NAME} glfw)

