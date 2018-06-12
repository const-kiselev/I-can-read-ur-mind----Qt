cmake_minimum_required(VERSION 3.10)
project(I_can_read_ur_mind)

set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES src/main.cpp src/view.cpp src/view.h src/model.cpp src/model.h src/controller.cpp src/controller.h src/additional_header.h src/additional_lib.cpp src/view/admin-view.cpp src/view/admin-view.h src/view/calibration.cpp src/view/calibration.h src/view/gaze-point-test.cpp src/view/gaze-point-test.h src/view/main-window.cpp src/view/main-window.h src/view/menu.cpp src/view/menu.h src/view/test-view.cpp src/view/test-view.h src/model/admin.cpp src/model/admin.h src/model/eye-tracker.cpp src/model/eye-tracker.h src/model/tests-controller.cpp src/model/tests-controller.h)
#add_library(mylibrary STATIC 3rdparty/tobii_research/include/tobii_research.h)

find_library(3rdparty/tobii_research REQUIRED)
find_package(Qt5Widgets REQUIRED)

add_executable(I_can_read_ur_mind ${SOURCE_FILES})
target_link_libraries(I_can_read_ur_mind Qt5::Widgets)
