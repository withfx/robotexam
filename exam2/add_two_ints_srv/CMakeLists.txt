cmake_minimum_required(VERSION 3.8)
project(add_two_ints_srv)

find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)

# 根据 srv/AddTwoInts.srv 生成服务接口
rosidl_generate_interfaces(${PROJECT_NAME}
  "srv/AddTwoInts.srv"
)

ament_export_dependencies(rosidl_default_runtime)
ament_package()
