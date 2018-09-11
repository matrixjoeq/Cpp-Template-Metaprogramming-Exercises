
project(chapter_2_tests)

file(GLOB INC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(${PROJECT_NAME} ${INC_FILES} ${SRC_FILES})

