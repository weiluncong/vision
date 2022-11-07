set(model_name main_view)
file(GLOB_RECURSE src "./*.cpp" "./*.h")
add_library(${model_name} SHARED ${src})
target_include_directories(${model_name} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${model_name} ${mlibs} ccommon data_scheduler data_producer ca_widgets data_center data_record)
install(DIRECTORY include/ DESTINATION cavision/include)
install(TARGETS ${model_name} DESTINATION cavision/third_party)