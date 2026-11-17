#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "robot_status_msg/msg/robot_status.hpp"

// 机器人状态订阅者
class RobotStatusSubscriber : public rclcpp::Node
{
public:
    RobotStatusSubscriber() : Node("robot_status_subscriber")
    {
        // 订阅的话题名也从参数里读，默认是 robot_status
        this->declare_parameter<std::string>("topic_name", "robot_status");
        topic_name_ = this->get_parameter("topic_name").as_string();

        // 收到消息就调用 topic_callback
        // std::placeholders::_1 表示回调函数的第一个参数
        subscription_ = this->create_subscription<robot_status_msg::msg::RobotStatus>(
            topic_name_, 10,
            std::bind(&RobotStatusSubscriber::topic_callback,
                      this, std::placeholders::_1));

        RCLCPP_INFO(this->get_logger(), "订阅者已启动：话题=%s",
                    topic_name_.c_str());
    }

private:
    void topic_callback(const robot_status_msg::msg::RobotStatus::SharedPtr message)
    {
        RCLCPP_INFO(this->get_logger(),
                    "收到状态：名称=%s，速度=%.2f m/s，坐标=(%.2f, %.2f)",
                    message->robot_name.c_str(), message->speed,
                    message->position_x, message->position_y);
    }

    rclcpp::Subscription<robot_status_msg::msg::RobotStatus>::SharedPtr subscription_;
    std::string topic_name_;
};

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<RobotStatusSubscriber>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
