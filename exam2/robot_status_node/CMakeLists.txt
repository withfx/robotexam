cmake_minimum_required(VERSION 3.8)
project(robot_status_node)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(robot_status_msg REQUIRED)

# 话题发布者
add_executable(robot_status_publisher src/publisher.cpp)
ament_target_dependencies(robot_status_publisher
  rclcpp
  robot_status_msg
)

# 话题订阅者
add_executable(robot_status_subscriber src/subscriber.cpp)
ament_target_dependencies(robot_status_subscriber
  rclcpp
  robot_status_msg
)

# 把可执行文件装到 lib/robot_status_node 下，ros2 run 才能找到
install(TARGETS
  robot_status_publisher
  robot_status_subscriber
  DESTINATION lib/${PROJECT_NAME}
)

# launch 文件和参数文件装到 share/robot_status_node 下
install(DIRECTORY launch config
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
