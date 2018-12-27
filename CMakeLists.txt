#
cmake_minimum_required (VERSION 2.8)
set (CMAKE_CXX_STANDARD 11)

project(partial_synth C CXX)

set (SOURCES
        "${PROJECT_SOURCE_DIR}/src/BasePartial.cpp"
	"${PROJECT_SOURCE_DIR}/src/ResPartial.cpp"
	"${PROJECT_SOURCE_DIR}/src/ResSynth.cpp"
	"${PROJECT_SOURCE_DIR}/src/SinePartial.cpp"
	"${PROJECT_SOURCE_DIR}/src/SineSynth.cpp"
	"${PROJECT_SOURCE_DIR}/src/SineTables.cpp"
    )



include_directories(${PROJECT_SOURCE_DIR}/src)


add_library (partial_synth_lib ${SOURCES} ${AUTO_SOURCES})
set_target_properties(partial_synth_lib PROPERTIES LINKER_LANGUAGE CXX)

#if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
#target_link_libraries(block_engine_lib "-undefined dynamic_lookup")
#set(CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS} -undefined dynamic_lookup")
#endif()
