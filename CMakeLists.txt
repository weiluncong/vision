cmake_minimum_required(VERSION 3.5)
project(cavision)

set(CMAKE_BUILD_TYPE Debug)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

string(TIMESTAMP COMPILE_TIME %m-%d_%H-%M) # 月日时分: %m%d%H%M

add_compile_options(-fPIC -g -std=c++11 -Wall)
add_definitions(-w)

set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/third_party/Qt)
set(OpenCV_DIR ${CMAKE_SOURCE_DIR}/third_party/OpenCV)

set(CPACK_GENERATOR "DEB")
# DPKG执行顺序
# 1.正常安装: preinst->unpack->set up->postinst
# 2.正常卸载: remove->prerm->postrm
# 3.覆盖安装: prerm->preinst->unpack->postrm->set up->postinst
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_SOURCE_DIR}/postinst;${CMAKE_SOURCE_DIR}/prerm;")
set(CPACK_SET_DESTDIR TRUE)
set(CPACK_INSTALL_PREFIX /opt/)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
set(CPACK_PACKAGE_CONTACT "cwl")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PROJECT_NAME}_${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}-${COMPILE_TIME}")

set(CMAKE_AUTOMOC on)
set(CMAKE_AUTOUIC on)
set(CMAKE_AUTORCC on)
set(QT_USE_QTOPENGL TRUE)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

configure_file(
    ${CMAKE_SOURCE_DIR}/src/version.h.in
    ${CMAKE_SOURCE_DIR}/src/version.h
)

# 添加工程依赖库及获取依赖库地址
list(APPEND QT_LIBS Qt5::Core Qt5::Widgets Qt5::Gui Qt5::Charts Qt5::OpenGL OpenCV)
foreach(lib ${QT_LIBS})
    if(${lib} MATCHES ".*:.*")
        string(REGEX REPLACE ".*:(.*)" "\\1" com ${lib})
        find_package(Qt5 COMPONENTS ${com} REQUIRED)
        list(APPEND mlibs ${lib})
    else()
        find_package(${lib} REQUIRED)
        list(APPEND mlibs ${${lib}_LIBS})
    endif()  
endforeach()

qt5_add_resources(QRC_SOURCES  ${CMAKE_CURRENT_SOURCE_DIR}/icon.qrc)

list(APPEND includePath
	        ${CMAKE_SOURCE_DIR}/third_party
            ${CMAKE_SOURCE_DIR}/third_party/yaml
            ${CMAKE_SOURCE_DIR}/third_party/Qt/include
            ${CMAKE_SOURCE_DIR}/third_party/OpenCV/include
            ${CMAKE_SOURCE_DIR}/third_party/eigen3
	        ${CMAKE_SOURCE_DIR}/third_party/mdf4
            ${CMAKE_SOURCE_DIR}/third_party/pack
            ${CMAKE_SOURCE_DIR}/third_party/Ogre
            ${CMAKE_SOURCE_DIR}/third_party/Ogre/OGRE
            ${CMAKE_SOURCE_DIR}/third_party/Ogre/OGRE/Overlay
            ${CMAKE_SOURCE_DIR}/third_party/Boost
            ${CMAKE_SOURCE_DIR}/third_party/pcan
            ${CMAKE_SOURCE_DIR}/third_party/python3.7m/include
	        ${CMAKE_SOURCE_DIR}/third_party/capilot/include
            ${CMAKE_SOURCE_DIR}/third_party/ffmpeg
            ${CMAKE_SOURCE_DIR}/third_party/DDS/include
)

link_directories(
    /opt/${PROJECT_NAME}/third_party
    ${CMAKE_SOURCE_DIR}/third_party/yaml
    ${CMAKE_SOURCE_DIR}/third_party/Qt/lib
    ${CMAKE_SOURCE_DIR}/third_party/OpenCV/lib
    ${CMAKE_SOURCE_DIR}/third_party/mdf4
    ${CMAKE_SOURCE_DIR}/third_party/pack
    ${CMAKE_SOURCE_DIR}/third_party/Ogre
    ${CMAKE_SOURCE_DIR}/third_party/Boost
    ${CMAKE_SOURCE_DIR}/third_party/Ogre/OGRE-1.9.0
    ${CMAKE_SOURCE_DIR}/third_party/pcan
    ${CMAKE_SOURCE_DIR}/third_party/python3.7m/lib
    ${CMAKE_SOURCE_DIR}/third_party/capilot/lib
    ${CMAKE_SOURCE_DIR}/third_party/ffmpeg
    ${CMAKE_SOURCE_DIR}/third_party/DDS/lib
)

include_directories(${includePath})
list(APPEND mlibs caframework common protobuf gflags glog kpb zmq_cm zmq pthread yaml-cpp OgreOverlay OgreMain pcanbasic ddsc)
list(APPEND mlibs boost_filesystem boost_program_options boost_system boost_thread)# boost
list(APPEND mlibs avutil swresample postproc swscale avcodec avformat avfilter)# ffmpeg

add_subdirectory(src/common)
add_subdirectory(src/data_center)
add_subdirectory(src/data_record)
add_subdirectory(src/main_view)
add_subdirectory(src/ca_compoments)
add_subdirectory(src/ca_widgets)
add_subdirectory(src/parser)
add_subdirectory(src/data_producer)
add_subdirectory(src/data_scheduler)

list(APPEND mlibs ccommon main_view ca_compoments ca_widgets parser data_producer data_scheduler)

add_executable(${PROJECT_NAME}_ ${CMAKE_SOURCE_DIR}/src/main.cpp ${QRC_SOURCES})
target_link_libraries(${PROJECT_NAME}_ ${mlibs})


FILE(GLOB_RECURSE script "${CMAKE_SOURCE_DIR}/script/*.sh")
install(DIRECTORY third_party/ DESTINATION ${PROJECT_NAME}/third_party/)
install(DIRECTORY dbc/ DESTINATION ${PROJECT_NAME}/dbc)
install(FILES ${script} DESTINATION ${PROJECT_NAME})
# install(TARGETS ${PROJECT_NAME} DESTINATION ${PROJECT_NAME}/third_party)
install(TARGETS ${PROJECT_NAME}_ DESTINATION ${PROJECT_NAME})
include(InstallRequiredSystemLibraries)
include(CPack)
