file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(${PROJECT_TEST} ${SRC_FILES})

target_link_libraries(${PROJECT_TEST} ${PROJECT_LIB} ${CONAN_LIBS})
target_include_directories(${PROJECT_TEST} PUBLIC 
                            ${CMAKE_SOURCE_DIR}
                            ${CONAN_INCLUDE_DIRS}
                            )