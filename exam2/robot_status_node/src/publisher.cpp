#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "robot_status_msg/msg/robot_status.hpp"

// 机器人状态发布者
class RobotStatusPublisher : public rclcpp::Node
{
public:
    RobotStatusPublisher() : Node("robot_status_publisher")
    {
        // 先声明参数，再从参数服务器读取
        // 第二个参数是默认值：如果 yaml 里没有配置，就用这个值
        this->declare_parameter<std::string>("topic_name", "robot_status");
        this->declare_parameter<double>("publish_frequency", 1.0);

        topic_name_ = this->get_parameter("topic_name").as_string();
        publish_frequency_ = this->get_parameter("publish_frequency").as_double();

        // 防止把频率配成 0 或负数，导致后面除以 0
        if (publish_frequency_ <= 0.0) {
            RCLCPP_WARN(this->get_logger(), "发布频率必须大于 0，已改回 1.0 Hz");
            publish_frequency_ = 1.0;
        }

        publisher_ = this->create_publisher<robot_status_msg::msg::RobotStatus>(
            topic_name_, 10);

        // 1 Hz 就是每 1000 毫秒发一次
        int period_ms = static_cast<int>(1000.0 / publish_frequency_);
        if (period_ms < 1) {
            period_ms = 1;
        }

        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(period_ms),
            std::bind(&RobotStatusPublisher::timer_callback, this));

        RCLCPP_INFO(this->get_logger(), "发布者已启动：话题=%s，频率=%.1f Hz",
                    topic_name_.c_str(), publish_frequency_);
    }

private:
    void timer_callback()
    {
        auto message = robot_status_msg::msg::RobotStatus();

        message.robot_name = "agri_robot_01";
        message.speed = 0.5f;

        // 每发一次坐标增加一点，看起来像机器人在慢慢移动
        position_x_ += 0.5f;
        position_y_ += 0.25f;
        message.position_x = position_x_;
        message.position_y = position_y_;

        publisher_->publish(message);

        RCLCPP_INFO(this->get_logger(),
                    "发布状态：名称=%s，速度=%.2f m/s，坐标=(%.2f, %.2f)",
                    message.robot_name.c_str(), message.speed,
                    message.position_x, message.position_y);
    }

    rclcpp::Publisher<robot_status_msg::msg::RobotStatus>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    std::string topic_name_;
    double publish_frequency_ = 1.0;
    float position_x_ = 0.0f;
    float position_y_ = 0.0f;
};

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<RobotStatusPublisher>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
