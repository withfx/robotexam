cmake_minimum_required(VERSION 3.8)
project(robot_status_msg)

find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)

# 根据 msg/RobotStatus.msg 生成 C++ 头文件和 Python 模块
rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/RobotStatus.msg"
)

ament_export_dependencies(rosidl_default_runtime)
ament_package()
