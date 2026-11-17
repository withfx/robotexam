# robotexam｜农业机器人实验室考核项目

> 作者：冯翔（26 级地理信息科学 2 班）  
> GitHub：[@withfx](https://github.com/withfx)  
> 仓库：<https://github.com/withfx/robotexam>  
> 内容：C++ 基础、ROS2 话题/服务/launch、Git 与 GitHub 学习报告  
> 语言：C++（ROS2 部分使用 rclcpp）

这是我在准备学校农业机器人实验室面试时完成的考核作业。项目里的代码全部使用 C++，没有使用 Python 写业务节点；`exam2` 里只有一个 `.py` 文件，那是 ROS2 要求的 launch 启动脚本。

## 一、目录结构

```text
robotexam/
├── exam1/                          # 考核一：C++ 代码能力
│   ├── question1/                  # 第一题：成绩降序排序
│   │   ├── main.cpp
│   │   └── README.md
│   ├── question2/                  # 第二题：Book 类
│   │   ├── main.cpp
│   │   └── README.md
│   └── question3/                  # 第三题：字符串统计
│       ├── main.cpp
│       └── README.md
├── exam2/                          # 考核二：ROS2 基础知识
│   ├── robot_status_msg/           # 自定义话题消息接口
│   ├── add_two_ints_srv/           # 自定义服务接口
│   ├── robot_status_node/          # 话题发布者、订阅者、launch、params.yaml
│   ├── service_node/               # 服务端、客户端
│   └── README.md
└── README.md                       # 本文件：项目说明 + 学习研究报告
```

## 二、运行环境

| 软件 | 版本 / 说明 |
| --- | --- |
| 系统 | Ubuntu 22.04（ROS2 部分） |
| ROS2 | Humble |
| 编译器 | 支持 C++17 的 g++ |
| 构建工具 | CMake、colcon |
| Python | 3.10，仅用于运行 ROS2 launch 文件 |

考核一只需要 g++，在 Windows 或 Ubuntu 上都能编译。考核二需要先安装 ROS2 Humble，建议在 Ubuntu 虚拟机或双系统里运行。

## 三、考核一：C++ 代码能力

| 题目 | 内容 | 主要知识点 |
| --- | --- | --- |
| 第一题 | 10 个成绩从高到低排序 | 数组、冒泡排序、循环、交换 |
| 第二题 | Book 类 | 类、private/public、构造/析构、new/delete |
| 第三题 | 统计字母、数字、其他字符 | string、getline、字符判断、循环计数 |

### 第一题：成绩降序排序

题目已经给出了 10 个成绩，所以代码直接把成绩放进数组，用冒泡排序从大到小排列。

```bash
cd exam1/question1
g++ -std=c++17 main.cpp -o main
./main
```

输出：

```text
从高到低排序后的成绩：100 95 92 88 85 82 78 70 65 60
```

### 第二题：Book 类

`Book` 类把书名、价格、库存放在 `private` 里，通过 `input()` 和 `display()` 操作。`main()` 里用 `new` 创建对象，用指针调用成员函数，最后用 `delete` 释放。

```bash
cd exam1/question2
g++ -std=c++17 main.cpp -o main
./main
```

输入：

```text
C++入门
45.5
20
```

输出：

```text
书名：C++入门
价格：45.5
库存：20
```

### 第三题：字符串统计

用 `getline` 读一整行，再用 `isalpha` 和 `isdigit` 分别统计英文字母和数字，剩下的都算其他字符。

```bash
cd exam1/question3
g++ -std=c++17 main.cpp -o main
./main
```

输入：

```text
Hello, ROS2 2024!
```

输出：

```text
英文字母个数：8
数字个数：5
其他字符个数：4
```

每道题的详细思路、易错点和运行结果都写在对应目录的 `README.md` 里。三份代码我已经用 C++17 编译器实际编译运行过，上面展示的就是实际输出。

## 四、考核二：ROS2 自定义话题、服务与 launch

`exam2` 本身就是一个 ROS2 工作空间，里面有四个包：

- `robot_status_msg`：消息接口 `RobotStatus.msg`；
- `add_two_ints_srv`：服务接口 `AddTwoInts.srv`；
- `robot_status_node`：话题发布者、订阅者，以及 launch 和参数文件；
- `service_node`：加法服务端、客户端。

### 1. 编译

```bash
source /opt/ros/humble/setup.bash
cd exam2
colcon build --symlink-install
source install/setup.bash
```

### 2. 单独运行话题节点

```bash
# 终端 A
ros2 run robot_status_node robot_status_publisher

# 终端 B
ros2 run robot_status_node robot_status_subscriber
```

发布者以 1 Hz 的频率在 `/robot_status` 上发布机器人名称、速度、坐标，订阅者收到后打印出来。

检查：

```bash
ros2 topic list
ros2 topic info /robot_status
ros2 topic hz /robot_status
```

### 3. 单独运行服务节点

```bash
# 终端 A：先启动服务端
ros2 run service_node add_two_ints_server

# 终端 B：再启动客户端
ros2 run service_node add_two_ints_client
```

客户端发送 `3 + 5`，服务端计算后返回 `8`。

检查：

```bash
ros2 service list | grep add_two_ints
ros2 service call /add_two_ints add_two_ints_srv/srv/AddTwoInts "{a: 10, b: 20}"
```

### 4. 一键启动四个节点

```bash
ros2 launch robot_status_node exam2.launch.py
```

节点都在 `robot1` 命名空间下：

```text
/robot1/atil_1
/robot1/robot_status_subscriber
/robot1/add_two_ints_server
/robot1/add_two_ints_client
```

发布者被重命名为 `atil_1`，其余三个节点保留题目给出的名字。四个节点不能都叫 `atil_1`，否则节点名会冲突，这一点在 `exam2/README.md` 里有详细说明。

### 5. 参数配置

`robot_status_node/config/params.yaml` 里配置了：

| 节点 | 参数 | 值 |
| --- | --- | --- |
| `/robot1/atil_1` | `topic_name` | `/robot_status` |
| `/robot1/atil_1` | `publish_frequency` | `1.0` |
| `/robot1/robot_status_subscriber` | `topic_name` | `/robot_status` |
| `/robot1/add_two_ints_server` | `service_name` | `/add_two_ints` |
| `/robot1/add_two_ints_client` | `service_name` | `/add_two_ints` |
| `/robot1/add_two_ints_client` | `a` / `b` | `3` / `5` |

代码里都是先 `declare_parameter` 声明，再用 `get_parameter` 读取，没有把话题名、服务名、频率写死。

更完整的编译、运行、验证步骤和常见问题见 [`exam2/README.md`](exam2/README.md)。

## 五、考核三：Git 与 GitHub 学习报告

### 1. 学习过程

之前我保存代码的方式是“复制一份文件夹，改名叫最终版、最终版2”，时间一长就不知道哪份是最后的，也不知道每次改了什么。这次我先把 Git 的基础用法学了一遍，接下来准备把项目传到 GitHub。

我看的主要资料：

- B 站尚硅谷 Git 教程：[Git 全套教程（GitHub、Gitee、GitLab）](https://www.bilibili.com/video/BV1vy4y1s7k6/)
- GitHub 官方中文文档：[开始使用 Git](https://docs.github.com/zh/get-started/learning-to-code/getting-started-with-git)
- GitHub 官方中文文档：[使用 GitHub Desktop 创建第一个仓库](https://docs.github.com/zh/desktop/overview/creating-your-first-repository-using-github-desktop)

视频里主要看了 Git 的安装和配置、工作区/暂存区/本地仓库的区别、提交、远程仓库、分支，以及 GitHub Desktop 的图形化操作。遇到不认识的命令，再去官方文档里查具体参数。

### 2. Git 到底在做什么

我自己的理解是：Git 会给项目拍“快照”。每次 `commit` 就是给当前代码拍一张照片，并写一句说明。以后改坏了，可以回到之前的某张照片；也可以把照片传到 GitHub，换一台电脑继续写。

```text
工作区（正在改的代码）
   │  git add
   ▼
暂存区（这次准备提交的改动）
   │  git commit
   ▼
本地仓库（本机的历史记录）
   │  git push
   ▼
GitHub 远程仓库（网上的备份和展示）
```

`git add` 不是“保存文件”，而是“把这次想提交的改动挑出来”。`git commit` 才是真正记录一次版本。

### 3. 常用命令

| 命令 | 作用 | 我自己的理解 |
| --- | --- | --- |
| `git init` | 初始化仓库 | 在项目文件夹里生成一个隐藏的 `.git` 目录 |
| `git status` | 查看状态 | 最常用，能看到哪些文件改了、哪些还没提交 |
| `git add .` | 添加到暂存区 | 把当前目录的改动挑进“待提交”列表 |
| `git commit -m "说明"` | 提交 | 给这次改动拍快照，说明要写清楚改了什么 |
| `git log --oneline` | 查看提交记录 | 一行一个版本，方便回退时找编号 |
| `git branch` | 查看分支 | 当前分支前面会有 `*` |
| `git checkout -b dev` | 新建并切换分支 | 新功能可以先在 `dev` 上写，不影响 `main` |
| `git remote add origin 地址` | 关联远程仓库 | `origin` 是给远程地址起的默认名字 |
| `git push -u origin main` | 推送到 GitHub | 第一次加 `-u`，以后直接 `git push` |
| `git pull` | 拉取远程更新 | 多人协作或换电脑时，先拉再改 |
| `git clone 地址` | 克隆仓库 | 把 GitHub 上的整个项目下载到本地 |
| `git diff` | 查看具体改了什么 | 提交前检查一遍，避免把调试代码传上去 |

### 4. GitHub Desktop 的使用

命令行有时候容易敲错，GitHub Desktop 可以把同样的操作变成点按钮：

1. 安装 GitHub Desktop，登录自己的 GitHub 账号；
2. 点 `File` → `Add local repository`，选择本地项目文件夹；
3. 左边 `Changes` 会列出改动过的文件，勾选这次要提交的文件；
4. 左下角填写 `Summary`（相当于 commit 说明），点 `Commit to main`；
5. 如果是新仓库，点右上角 `Publish repository`；
6. 发布时注意 **不要** 勾选 `Keep this code private`，这样仓库就是 Public；
7. 以后改完代码，重复“查看 Changes → 写 Summary → Commit → Push origin”。

如果仓库已经发布但类别是 Private，可以到 GitHub 网页的 `Settings` → `General` → 最下面的 `Danger Zone` → `Change visibility`，改成 Public。

### 5. 把这个项目上传到 GitHub 的步骤

仓库已经建好：<https://github.com/withfx/robotexam>。如果以后换一台电脑，或者需要在本地重新关联远程仓库，可以按下面的步骤操作：

```bash
cd robotexam

# 如果之前编译过，先把编译产物删掉，只提交源码
rm -rf exam2/build exam2/install exam2/log

git init
git add exam1 exam2 README.md
git status
git commit -m "完成 C++ 与 ROS2 实验室考核作业"
git branch -M main

git remote add origin https://github.com/withfx/robotexam.git
git push -u origin main
```

`git status` 这一步不要省，它能看出有没有把 `build/`、`install/`、`log/` 或 `.exe` 文件加进去。

> 老师要求提交的压缩包里不要放编译后的文件，所以这个项目没有加 `.gitignore`。没有 `.gitignore` 的话，Git 不会自动忽略编译产物，提交前必须自己确认一遍。如果已经误加了，可以执行 `git rm -r --cached exam2/build exam2/install exam2/log` 把它们从 Git 的记录里移除，再重新提交。

### 6. 为什么说这个项目“改路径就能用”

- 考核一的三道题都是标准的 `main.cpp`，在哪个目录下都能用 `g++ main.cpp -o main` 编译；
- `exam2` 是完整的 colcon 工作空间，不依赖某个固定的绝对路径；
- launch 文件用 `get_package_share_directory("robot_status_node")` 找参数文件，没有写死 `/home/xxx/...`；
- 话题名、服务名、发布频率、a 和 b 都放在 `params.yaml` 里，换机器人或改频率时只改 yaml，不用重新改代码；
- 如果把整个文件夹换到另一台 Ubuntu 电脑，只要重新 `colcon build` 并 `source install/setup.bash` 就能运行。

### 7. 踩过的坑

| 问题 | 原因 | 解决 |
| --- | --- | --- |
| `ros2 run` 提示找不到包 | 新终端没有 source 当前工作空间 | `source install/setup.bash` |
| `colcon build` 找不到接口 | 在子包目录里编译了 | 回到包含四个包的 `exam2` 目录再编译 |
| yaml 里的参数不生效 | 键名没写完整节点名 | 写 `/robot1/atil_1`，并用 `ros2 param get` 检查 |
| 话题变成了 `/robot1/robot_status` | 话题名写的是相对名字 | 要求全局话题就在 yaml 里写 `/robot_status` |
| 四个节点都想叫 `atil_1` | ROS2 节点名冲突 | 只重命名发布者，其余节点保留功能名 |
| `git push` 让输密码，输登录密码失败 | GitHub 不再支持密码推送 | 用 Personal Access Token 代替密码 |
| `src refspec main does not match any` | 还没有任何 commit | 先 `git add`、`git commit`，再推送 |
| 误把 `build/` 提交了 | 没有忽略编译产物 | `git rm -r --cached build` 后再提交 |

### 8. 上传前检查清单

- [ ] GitHub 仓库类型是 Public（发布时不要勾选 `Keep this code private`）；
- [ ] README 写清楚了项目结构、编译命令和运行方法；
- [ ] 代码里没有写死自己电脑的绝对路径；
- [ ] 没有提交 `build/`、`install/`、`log/`、`.exe` 等编译产物；
- [ ] 没有把 GitHub 密码、Token 写进代码；
- [ ] 每个源文件都能单独说明它解决的是哪道题；
- [ ] commit 说明写的是具体内容，不是一堆 `update`。

## 六、学习总结

### C++ 部分

- 第一题让我把数组下标、循环边界和交换变量弄清楚了。冒泡排序虽然简单，但边界写错一位就会越界。
- 第二题第一次真正区分了“对象”和“指向对象的指针”。`new` 出来的对象要自己 `delete`，`delete` 时会自动调用析构函数。
- 第三题知道了 `cin >>` 和 `getline` 的区别：前者遇到空格会停，后者能读一整行；字符判断函数要包含 `<cctype>`。

### ROS2 部分

- 话题是“发布者一直发、订阅者一直收”，适合传感器数据；服务是“客户端请求一次、服务端响应一次”，适合加法这种明确的任务。
- 自定义接口要先编译接口包，节点包才能 `#include` 生成的头文件。
- ROS2 里名字非常重要：节点名、命名空间、话题名、服务名、参数文件的键，只要有一个对不上，可能不报错但就是不工作。
- launch 文件可以把多个节点、命名空间、参数文件放在一起，一条命令启动整个系统。

### Git 部分

Git 最大的好处是“敢改”了。以前怕改坏，只能不断复制文件；现在每次提交都是一个可以回去的版本，也能清楚看到自己改了什么。GitHub Desktop 适合日常提交，命令行适合更细的操作，两个一起用效率更高。

## 七、还可以继续改进的地方

- C++ 里可以用 `std::vector` 和 `std::sort` 代替固定长度数组和手写冒泡排序；
- Book 类可以用智能指针 `std::unique_ptr` 管理动态对象，进一步避免忘记 `delete`；
- ROS2 可以继续学习 QoS、生命周期节点、tf2 坐标变换和 Nav2；
- 可以给节点加一些单元测试，或者用 `ros2 bag` 录下话题数据再回放；
- Git 可以继续学习分支合并、Pull Request 和 GitHub Actions。

## 八、参考资料

- [B 站：尚硅谷 Git 全套教程](https://www.bilibili.com/video/BV1vy4y1s7k6/)
- [GitHub 文档：开始使用 Git](https://docs.github.com/zh/get-started/learning-to-code/getting-started-with-git)
- [GitHub 文档：使用 GitHub Desktop 创建第一个仓库](https://docs.github.com/zh/desktop/overview/creating-your-first-repository-using-github-desktop)
- [ROS2 Humble 官方文档](https://docs.ros.org/en/humble/)
- [ROS2 官方教程：编写简单的发布者和订阅者（C++）](https://docs.ros.org/en/humble/Tutorials/Beginner-Client-Libraries/Writing-A-Simple-Cpp-Publisher-And-Subscriber.html)
- [ROS2 官方教程：编写简单的服务和客户端（C++）](https://docs.ros.org/en/humble/Tutorials/Beginner-Client-Libraries/Writing-A-Simple-Cpp-Service-And-Client.html)

## 九、提交说明

- 压缩包命名：`26-地信2-冯翔.zip`；
- 仓库地址：<https://github.com/withfx/robotexam>；
- 压缩包里只放源码和 README，不放 `build/`、`install/`、`log/`；
- 邮件正文放 GitHub 仓库链接；
- 仓库类别设为 Public。
