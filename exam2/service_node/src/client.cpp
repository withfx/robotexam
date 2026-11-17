#include <chrono>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "add_two_ints_srv/srv/add_two_ints.hpp"

using AddTwoInts = add_two_ints_srv::srv::AddTwoInts;

// 加法服务客户端
class AddTwoIntsClient : public rclcpp::Node
{
public:
    AddTwoIntsClient() : Node("add_two_ints_client")
    {
        // 服务名、a、b 都从参数里读
        this->declare_parameter<std::string>("service_name", "add_two_ints");
        this->declare_parameter<std::int64_t>("a", 3);
        this->declare_parameter<std::int64_t>("b", 5);

        service_name_ = this->get_parameter("service_name").as_string();
        a_ = this->get_parameter("a").as_int();
        b_ = this->get_parameter("b").as_int();

        client_ = this->create_client<AddTwoInts>(service_name_);

        // 每 3 秒发一次请求
        // 这样 launch 启动以后客户端节点不会马上退出，node list 里能一直看到它
        timer_ = this->create_wall_timer(
            std::chrono::seconds(3),
            std::bind(&AddTwoIntsClient::send_request, this));

        RCLCPP_INFO(this->get_logger(),
                    "客户端已启动：服务=%s，每 3 秒发送一次 %ld + %ld",
                    service_name_.c_str(),
                    static_cast<long>(a_), static_cast<long>(b_));
    }

private:
    void send_request()
    {
        // 服务端可能还没启动，先检查一下，避免一直等
        if (!client_->service_is_ready()) {
            RCLCPP_WARN(this->get_logger(), "服务 %s 还没准备好，3 秒后重试",
                        service_name_.c_str());
            return;
        }

        auto request = std::make_shared<AddTwoInts::Request>();
        request->a = a_;
        request->b = b_;

        RCLCPP_INFO(this->get_logger(), "发送请求：%ld + %ld",
                    static_cast<long>(request->a),
                    static_cast<long>(request->b));

        // 异步发送，服务端返回后执行后面的 lambda
        client_->async_send_request(
            request,
            [this](rclcpp::Client<AddTwoInts>::SharedFuture future) {
                auto response = future.get();
                RCLCPP_INFO(this->get_logger(), "收到响应：%ld + %ld = %ld",
                            static_cast<long>(a_), static_cast<long>(b_),
                            static_cast<long>(response->sum));
            });
    }

    rclcpp::Client<AddTwoInts>::SharedPtr client_;
    rclcpp::TimerBase::SharedPtr timer_;
    std::string service_name_;
    std::int64_t a_ = 3;
    std::int64_t b_ = 5;
};

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<AddTwoIntsClient>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
