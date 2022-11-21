set(model_name ca_compoments)
file(GLOB_RECURSE src "./*.cpp" "./*.h")
add_library(${model_name} SHARED ${src})
target_include_directories(${model_name} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/camera_compoments
    ${CMAKE_CURRENT_SOURCE_DIR}/include/topview_compoments
    ${CMAKE_CURRENT_SOURCE_DIR}/include/topview_compoments/setter_compoments
    ${CMAKE_CURRENT_SOURCE_DIR}/include/topview_compoments/item_compoments)
target_link_libraries(${model_name} ${mlibs} ccommon)
install(DIRECTORY include/ DESTINATION cavision/include)
install(TARGETS ${model_name} DESTINATION cavision/third_party)