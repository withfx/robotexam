cmake_minimum_required(VERSION 3.8)
project(service_node)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(add_two_ints_srv REQUIRED)

# 服务端
add_executable(add_two_ints_server src/server.cpp)
ament_target_dependencies(add_two_ints_server
  rclcpp
  add_two_ints_srv
)

# 客户端
add_executable(add_two_ints_client src/client.cpp)
ament_target_dependencies(add_two_ints_client
  rclcpp
  add_two_ints_srv
)

install(TARGETS
  add_two_ints_server
  add_two_ints_client
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
