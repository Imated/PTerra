cmake_minimum_required(VERSION 3.5)

option(PRODUCTION_BUILD "Make this a production build" ON)

# Emit compile_commands.json
# This allows certain IDE tools to better understand the project.
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")

if(MSVC) 
add_compile_options(/arch:AVX2) #make sure SIMD optimizations take place
endif()

project(terra)
set(CMAKE_CXX_STANDARD 20)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++ -static-libgcc")
set(THREADS_PREFER_PTHREAD_FLAG OFF CACHE BOOL "" FORCE)
set(CMAKE_THREAD_LIBS_INIT "-lwinpthread" CACHE STRING "" FORCE)
set(Threads_FOUND TRUE CACHE BOOL "" FORCE)
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_X11     OFF CACHE BOOL "" FORCE)

add_subdirectory(libs/glfw)
add_subdirectory(libs/glad)
add_subdirectory(libs/glm)

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable("${CMAKE_PROJECT_NAME}" ${MY_SOURCES} "${CMAKE_CURRENT_SOURCE_DIR}/resources/resources.rc"
		src/audio/Audio.cpp
		src/audio/Audio.h
		src/audio/AudioFile.h
		src/audio/AudioBuffer.cpp
		src/audio/AudioBuffer.h
		src/audio/AudioSource.cpp
		src/audio/AudioSource.h)

set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY CXX_STANDARD 20)

target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC GLFW_INCLUDE_NONE=1) 

if(PRODUCTION_BUILD)
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=1) 
	set(CMAKE_CXX_FLAGS "-O2 -march=native -std=c++20 -Wall")# release mode
	add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_SOURCE_DIR}/resources
			$<TARGET_FILE_DIR:terra>/resources

			COMMAND ${CMAKE_COMMAND} -E copy_if_different
			${CMAKE_SOURCE_DIR}/libs/OpenAL/OpenAL32.dll
			$<TARGET_FILE_DIR:terra>

			COMMAND ${CMAKE_COMMAND} -E copy_if_different
			${CMAKE_SOURCE_DIR}/libs/OpenAL/OpenAL32.lib
			$<TARGET_FILE_DIR:terra>

			COMMAND ${CMAKE_COMMAND} -E copy_if_different
			${CMAKE_SOURCE_DIR}/libs/sndfile.dll
			$<TARGET_FILE_DIR:terra>
	)
else()
	# This is useful to get an ASSETS_PATH in your IDE during development
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=0) 
	set(CMAKE_CXX_FLAGS "-g")# Debug mode
	add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_SOURCE_DIR}/resources
			$<TARGET_FILE_DIR:terra>/resources

			COMMAND ${CMAKE_COMMAND} -E copy_if_different
			${CMAKE_SOURCE_DIR}/libs/OpenAL/OpenAL32.dll
			$<TARGET_FILE_DIR:terra>

			COMMAND ${CMAKE_COMMAND} -E copy_if_different
			${CMAKE_SOURCE_DIR}/libs/OpenAL/OpenAL32.lib
			$<TARGET_FILE_DIR:terra>

			COMMAND ${CMAKE_COMMAND} -E copy_if_different
			${CMAKE_SOURCE_DIR}/libs/sndfile.dll
			$<TARGET_FILE_DIR:terra>
	)
endif()

target_sources("${CMAKE_PROJECT_NAME}" PRIVATE ${MY_SOURCES} )

if(MSVC) # If using the VS compiler...
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC _CRT_SECURE_NO_WARNINGS)

	#add this line if you want to remove the console!
	set_target_properties("${CMAKE_PROJECT_NAME}" PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup") #no console
	
	#set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDebug<$<CONFIG:Debug>:Debug>")
	#set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
endif()

target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/")

link_directories("${CMAKE_SOURCE_DIR}/libs" "${CMAKE_SOURCE_DIR}/libs/OpenAL")

target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE glfw glad glm "${CMAKE_SOURCE_DIR}/libs/OpenAL/OpenAL32.lib" "${CMAKE_SOURCE_DIR}/libs/sndfile.lib")

