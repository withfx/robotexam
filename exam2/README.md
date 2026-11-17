# 考核二：ROS2 自定义话题、自定义服务、launch + yaml

本目录是一个 ROS2 工作空间，四个包全部用 **C++（rclcpp）** 编写，开发和验证环境是：

- Ubuntu 22.04
- ROS2 Humble
- colcon
- C++17

## 一、四个包分别做什么

```text
exam2/
├── robot_status_msg/     自定义消息接口 RobotStatus.msg
├── add_two_ints_srv/     自定义服务接口 AddTwoInts.srv
├── robot_status_node/    话题发布者、订阅者，以及 launch 和 params.yaml
└── service_node/         服务端、客户端
```

接口定义：

```text
# robot_status_msg/msg/RobotStatus.msg
string robot_name
float32 speed
float32 position_x
float32 position_y
```

```text
# add_two_ints_srv/srv/AddTwoInts.srv
int64 a
int64 b
---
int64 sum
```

## 二、编译

先打开终端，加载 ROS2 环境，再进入 `exam2` 目录：

```bash
source /opt/ros/humble/setup.bash
cd robotexam/exam2
colcon build --symlink-install
source install/setup.bash
```

编译成功的话，最后会看到类似：

```text
Summary: 4 packages finished
```

> 如果终端里提示找不到 `ros2`，说明没有 source ROS2 的环境；如果提示找不到 `robot_status_msg`，说明没有 source 当前工作空间的 `install/setup.bash`。这两个 `source` 每次新开终端都要做一遍。

## 三、第一题：自定义话题通信

### 1. 启动发布者和订阅者

开两个终端，都先执行：

```bash
source /opt/ros/humble/setup.bash
cd robotexam/exam2
source install/setup.bash
```

终端 A 启动发布者：

```bash
ros2 run robot_status_node robot_status_publisher
```

终端 B 启动订阅者：

```bash
ros2 run robot_status_node robot_status_subscriber
```

发布者每秒发布一次，输出类似：

```text
[INFO] [robot_status_publisher]: 发布者已启动：话题=/robot_status，频率=1.0 Hz
[INFO] [robot_status_publisher]: 发布状态：名称=agri_robot_01，速度=0.50 m/s，坐标=(0.50, 0.25)
[INFO] [robot_status_publisher]: 发布状态：名称=agri_robot_01，速度=0.50 m/s，坐标=(1.00, 0.50)
```

订阅者会收到并输出：

```text
[INFO] [robot_status_subscriber]: 订阅者已启动：话题=/robot_status
[INFO] [robot_status_subscriber]: 收到状态：名称=agri_robot_01，速度=0.50 m/s，坐标=(0.50, 0.25)
[INFO] [robot_status_subscriber]: 收到状态：名称=agri_robot_01，速度=0.50 m/s，坐标=(1.00, 0.50)
```

时间和坐标的数值会变，只要能看到订阅者持续收到消息就说明通信正常。

### 2. 用命令检查

再开一个终端，source 环境后执行：

```bash
ros2 topic list
ros2 topic info /robot_status
ros2 topic hz /robot_status
ros2 interface show robot_status_msg/msg/RobotStatus
```

其中：

- `ros2 topic list` 应该能看到 `/robot_status`；
- `ros2 topic info /robot_status` 应该显示 `Publisher count: 1` 和 `Subscription count: 1`；
- `ros2 topic hz /robot_status` 的频率应该在 `1.000` 左右；
- `ros2 interface show ...` 应该显示四个字段。

## 四、第二题：自定义服务通信

### 1. 先启动服务端

终端 A：

```bash
source /opt/ros/humble/setup.bash
cd robotexam/exam2
source install/setup.bash
ros2 run service_node add_two_ints_server
```

看到下面的日志说明服务已经起来了：

```text
[INFO] [add_two_ints_server]: 服务端已启动：服务=/add_two_ints
```

### 2. 再启动客户端

终端 B：

```bash
source /opt/ros/humble/setup.bash
cd robotexam/exam2
source install/setup.bash
ros2 run service_node add_two_ints_client
```

客户端每 3 秒发送一次 `3 + 5`，输出类似：

```text
[INFO] [add_two_ints_client]: 客户端已启动：服务=/add_two_ints，每 3 秒发送一次 3 + 5
[INFO] [add_two_ints_client]: 发送请求：3 + 5
[INFO] [add_two_ints_client]: 收到响应：3 + 5 = 8
```

服务端同时会打印：

```text
[INFO] [add_two_ints_server]: 收到请求：3 + 5 = 8
```

> 客户端没有发一次就退出，而是用定时器每 3 秒发一次。这样执行 launch 以后，`ros2 node list` 里一直能看到四个节点。想停止就按 `Ctrl+C`。

### 3. 用命令行检查服务

终端 C：

```bash
ros2 service list | grep add_two_ints
ros2 service type /add_two_ints
ros2 service call /add_two_ints add_two_ints_srv/srv/AddTwoInts "{a: 10, b: 20}"
```

应该能看到：

```text
/add_two_ints
add_two_ints_srv/srv/AddTwoInts
```

命令行调用服务会返回：

```text
response:
add_two_ints_srv.srv.AddTwoInts_Response(sum=30)
```

服务端也会多一行 `收到请求：10 + 20 = 30`。

## 五、第三题：launch 一键启动 + yaml 参数

### 1. 一键启动

先结束前面单独启动的节点，避免端口和节点名冲突，然后执行：

```bash
source /opt/ros/humble/setup.bash
cd robotexam/exam2
source install/setup.bash
ros2 launch robot_status_node exam2.launch.py
```

四个节点会同时启动，并且全部设置 `output="screen"`，日志直接打印在当前终端。

### 2. 检查四个节点

另开一个终端，source 环境后执行：

```bash
ros2 node list
```

应该看到：

```text
/robot1/atil_1
/robot1/robot_status_subscriber
/robot1/add_two_ints_server
/robot1/add_two_ints_client
```

它们都在 `robot1` 命名空间下。发布者按题目要求被重命名成了 `atil_1`。

### 3. 检查话题、服务和参数

```bash
ros2 topic list
ros2 service list | grep add_two_ints
ros2 param get /robot1/atil_1 topic_name
ros2 param get /robot1/atil_1 publish_frequency
ros2 param get /robot1/robot_status_subscriber topic_name
ros2 param get /robot1/add_two_ints_client a
ros2 param get /robot1/add_two_ints_client b
```

这时 `ros2 topic list` 里应该有 **`/robot_status`**，但不会出现 `/robot1/robot_status`。原因是 `params.yaml` 里的话题名写成了 `/robot_status`，开头有 `/` 表示“全局名字”，不会再被命名空间拼接。服务名 `/add_two_ints` 也是一样。

这样做是为了同时满足题目的两个要求：

- 第一题明确要求话题是 `/robot_status`，第二题明确要求服务是 `/add_two_ints`；
- 第三题要求节点统一放在 `robot1` 命名空间下。

命名空间影响的是节点名，全局话题名/服务名仍然保持题目要求的样子。

参数查询应该得到：

```text
/robot1/atil_1 的 topic_name        -> /robot_status
/robot1/atil_1 的 publish_frequency -> 1.0
/robot1/robot_status_subscriber 的 topic_name -> /robot_status
/robot1/add_two_ints_client 的 a    -> 3
/robot1/add_two_ints_client 的 b    -> 5
```

### 4. 参数文件是怎么对应到节点上的

`params.yaml` 里的键必须是节点的**完整名字**：

```yaml
/robot1/atil_1:
  ros__parameters:
    topic_name: "/robot_status"
    publish_frequency: 1.0
```

因为 launch 里设置了 `namespace="robot1"`，发布者又被 `name=node_name`（值是 `atil_1`）重命名，所以它的完整节点名就是 `/robot1/atil_1`。只写 `atil_1` 或只写 `robot_status_publisher` 都匹配不上，参数不会报错，但也不会生效，节点会继续使用代码里的默认值。这是 ROS2 参数文件很容易踩的坑。

## 六、为什么只把发布者改名成 atil_1

题目一方面列出了四个节点名：

```text
robot_status_publisher
robot_status_subscriber
add_two_ints_server
add_two_ints_client
```

另一方面又要求“通过 node_name 重命名节点（节点重命名为 atil_1）”。

四个节点在同一个命名空间里不能都叫 `atil_1`，否则 `ros2 node list` 里会出现四个同名节点，参数也会互相覆盖。所以我的处理是：

- **发布者**：用 `name=node_name`（`node_name = "atil_1"`）重命名，满足“重命名为 atil_1”的要求；
- **订阅者、服务端、客户端**：保留题目给出的名字，方便区分它们的功能。

`ros2 node list` 里四个节点都能看到，通信应该正常。

## 七、常见问题

### 1. `Package 'robot_status_node' not found`

没有 source 工作空间：

```bash
cd robotexam/exam2
source install/setup.bash
```

### 2. `colcon build` 找不到接口包

接口包和节点包要放在同一个 `exam2` 目录下一起编译，不要在 `robot_status_node` 里面单独执行 `colcon build`。正确的目录是包含四个包的 `exam2`。

### 3. 参数没有生效

先确认节点的完整名字：

```bash
ros2 node list
ros2 param list /robot1/atil_1
```

再检查 `params.yaml` 的键是不是 `/robot1/atil_1`，以及 `parameters=[params_file]` 有没有写。

### 4. 话题名变成了 `/robot1/robot_status`

说明 yaml 里的 `topic_name` 写成了相对名字 `robot_status`。如果确实希望话题跟着命名空间走，可以去掉开头的 `/`；本作业为了严格对应题目里的 `/robot_status`，保留了全局名字。

### 5. 客户端一直提示服务还没准备好

先启动服务端，再启动客户端。如果使用 launch，四个节点是一起启动的，客户端前几秒可能会重试一两次，等服务端起来后就会正常收到响应。
