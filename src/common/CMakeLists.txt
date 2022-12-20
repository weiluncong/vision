set(model_name ccommon)
file(GLOB_RECURSE src "./*.cpp" "./*.h")
add_library(${model_name} SHARED ${src})
target_include_directories(${model_name} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/proto_pool
    ${CMAKE_CURRENT_SOURCE_DIR}/include/thread_pool
    ${CMAKE_CURRENT_SOURCE_DIR}/include/yaml
    ${CMAKE_CURRENT_SOURCE_DIR}/include/stm
    ${CMAKE_CURRENT_SOURCE_DIR}/include/data_struct)
target_link_libraries(${model_name} ${mlibs})
install(DIRECTORY include/ DESTINATION cavision/include)
install(TARGETS ${model_name} DESTINATION cavision/third_party)