# This is the root ITK CMakeLists file.
cmake_minimum_required(VERSION 2.8.9)
if(COMMAND CMAKE_POLICY)
  cmake_policy(SET CMP0003 NEW)
endif()


# This project is designed to be built outside the Insight source tree.
project(TesteITK)

# Find ITK.
find_package(ITK REQUIRED)

include(${ITK_USE_FILE})
if (ITKVtkGlue_LOADED)
  find_package(VTK REQUIRED)  
  include(${VTK_USE_FILE})
else()
  find_package(ItkVtkGlue REQUIRED)
  include(${ItkVtkGlue_USE_FILE})
  set(Glue ItkVtkGlue)
endif()

find_package(OpenCV REQUIRED)

add_executable(TesteITK main.cpp)

target_link_libraries(TesteITK ${Glue} ${ITK_LIBRARIES} ${VTK_LIBRARIES} ${OpenCV_LIBS})
