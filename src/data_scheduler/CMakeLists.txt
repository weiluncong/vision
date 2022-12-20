set(model_name data_scheduler)
file(GLOB_RECURSE src "./*.cpp" "./*.h")
add_library(${model_name} SHARED ${src})
target_include_directories(${model_name} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${model_name} ${mlibs} ccommon data_center ca_widgets parser)
install(DIRECTORY include/ DESTINATION cavision/include)
install(TARGETS ${model_name} DESTINATION cavision/third_party)