file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(${PROJECT_LIB} ${SRC_FILES})

target_link_libraries(${PROJECT_LIB} ${CONAN_LIBS})
target_include_directories(${PROJECT_LIB} PUBLIC 
                            ${CMAKE_SOURCE_DIR}
                            ${CONAN_INCLUDE_DIRS}
                            )
target_compile_features(${PROJECT_LIB} PRIVATE cxx_std_17)

add_executable(${PROJECT_EXE} main.cpp)

target_link_libraries(${PROJECT_EXE} ${PROJECT_LIB} ${CONAN_LIBS}  ${Pangolin_LIBRARIES})
target_include_directories(${PROJECT_EXE} PUBLIC 
                            ${CMAKE_SOURCE_DIR}
                            ${CONAN_INCLUDE_DIRS}
                            )
target_compile_features(${PROJECT_EXE} PRIVATE cxx_std_17)

                            