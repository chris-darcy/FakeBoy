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
add_subdirectory(deps/glfw)
target_include_directories(${PROJECT_NAME} PUBLIC deps/glfw/include)
target_link_directories(${PROJECT_NAME} PUBLIC deps/glfw/src)
target_link_libraries(${PROJECT_NAME} glfw)

# PORTAUDIO 
add_subdirectory(deps/portaudio)
target_include_directories(${PROJECT_NAME} PUBLIC deps/portaudio/include)
target_link_directories(${PROJECT_NAME} PUBLIC deps/portaudio/src/common)
target_link_libraries(${PROJECT_NAME} PortAudio)
