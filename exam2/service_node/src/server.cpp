#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "add_two_ints_srv/srv/add_two_ints.hpp"

// 给类型起一个短一点的名字，后面写起来方便
using AddTwoInts = add_two_ints_srv::srv::AddTwoInts;

// 加法服务端
class AddTwoIntsServer : public rclcpp::Node
{
public:
    AddTwoIntsServer() : Node("add_two_ints_server")
    {
        // 服务名从参数里读，默认是 add_two_ints
        this->declare_parameter<std::string>("service_name", "add_two_ints");
        service_name_ = this->get_parameter("service_name").as_string();

        // 收到请求后调用 handle_add
        // 两个参数分别是 Request 和 Response
        service_ = this->create_service<AddTwoInts>(
            service_name_,
            std::bind(&AddTwoIntsServer::handle_add, this,
                      std::placeholders::_1, std::placeholders::_2));

        RCLCPP_INFO(this->get_logger(), "服务端已启动：服务=%s",
                    service_name_.c_str());
    }

private:
    void handle_add(
        const std::shared_ptr<AddTwoInts::Request> request,
        std::shared_ptr<AddTwoInts::Response> response)
    {
        // 把请求里的 a 和 b 加起来，放进响应的 sum 里
        response->sum = request->a + request->b;

        RCLCPP_INFO(this->get_logger(), "收到请求：%ld + %ld = %ld",
                    static_cast<long>(request->a),
                    static_cast<long>(request->b),
                    static_cast<long>(response->sum));
    }

    rclcpp::Service<AddTwoInts>::SharedPtr service_;
    std::string service_name_;
};

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<AddTwoIntsServer>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
