import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    # params.yaml 在编译后被安装到 share/robot_status_node/config 下面
    default_params_file = os.path.join(
        get_package_share_directory("robot_status_node"),
        "config",
        "params.yaml",
    )

    namespace = LaunchConfiguration("namespace")
    params_file = LaunchConfiguration("params_file")

    # 题目里说的 node_name 就是发布者重命名后的节点名
    node_name = "atil_1"

    return LaunchDescription([
        # 统一命名空间，默认是 robot1
        DeclareLaunchArgument(
            "namespace",
            default_value="robot1",
            description="所有节点的命名空间",
        ),
        # 参数文件路径，一般不用改
        DeclareLaunchArgument(
            "params_file",
            default_value=default_params_file,
            description="yaml 参数文件路径",
        ),

        # 发布者：题目要求把节点重命名为 atil_1
        # ROS2 launch 里重命名节点用的是 name 参数（题目里把它叫 node_name）
        Node(
            package="robot_status_node",
            executable="robot_status_publisher",
            name=node_name,
            namespace=namespace,
            output="screen",
            parameters=[params_file],
        ),

        # 订阅者
        Node(
            package="robot_status_node",
            executable="robot_status_subscriber",
            name="robot_status_subscriber",
            namespace=namespace,
            output="screen",
            parameters=[params_file],
        ),

        # 服务端
        Node(
            package="service_node",
            executable="add_two_ints_server",
            name="add_two_ints_server",
            namespace=namespace,
            output="screen",
            parameters=[params_file],
        ),

        # 客户端
        Node(
            package="service_node",
            executable="add_two_ints_client",
            name="add_two_ints_client",
            namespace=namespace,
            output="screen",
            parameters=[params_file],
        ),
    ])
