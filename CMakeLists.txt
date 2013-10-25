CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

IF(${CMAKE_SOURCE_DIR} MATCHES ${CMAKE_BINARY_DIR})
    MESSAGE(FATAL_ERROR "In-source builds are not supported. Please delete the CMakeCache.txt file and the CMakeFiles directory and try building out-of-source:\n\tmkdir build && cd build && cmake ..")
ENDIF(${CMAKE_SOURCE_DIR} MATCHES ${CMAKE_BINARY_DIR})

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules/")


PROJECT(Submarine)

find_package(SDL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenAL REQUIRED)
FIND_PACKAGE(SIEGE REQUIRED)

include_directories(${SIEGE_INCLUDE_DIR} ${SDL_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} ${OPENAL_INCLUDE_DIR})

FILE(GLOB_RECURSE ${PROJECT_NAME}_FILES src/*.c)

ADD_EXECUTABLE(${PROJECT_NAME} ${${PROJECT_NAME}_FILES})
target_link_libraries(${PROJECT_NAME} ${SIEGE_LIBRARY} ${SDL_LIBRARY} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${OPENAL_LIBRARY})
