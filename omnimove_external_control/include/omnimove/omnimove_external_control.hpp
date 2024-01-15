#ifndef OMNIMOVE_CONTROLLER_HPP
#define OMNIMOVE_CONTROLLER_HPP

#include <hardware_interface/system_interface.hpp>
#include <hardware_interface/handle.hpp>
#include <omnimove/external_control_message.hpp>
#include <boost/asio.hpp>
#include <boost/circular_buffer.hpp>
#include <rcl/logging.h>
#include <boost/chrono.hpp>
namespace omnimove{
    class OmnimoveExternalControl:public hardware_interface::SystemInterface{
    public:
        OmnimoveExternalControl();
        ~OmnimoveExternalControl();
        rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_init(const hardware_interface::HardwareInfo& info) override;
        hardware_interface::return_type read(const rclcpp::Time & time, const rclcpp::Duration & period) override;
        hardware_interface::return_type write(const rclcpp::Time & time, const rclcpp::Duration & period) override;
        std::vector<hardware_interface::StateInterface> export_state_interfaces() override;
        std::vector<hardware_interface::CommandInterface> export_command_interfaces() override;
        hardware_interface::CallbackReturn on_configure(const rclcpp_lifecycle::State & previous_state) override;
        hardware_interface::CallbackReturn on_cleanup(const rclcpp_lifecycle::State & previous_state) override;
        hardware_interface::CallbackReturn on_shutdown(const rclcpp_lifecycle::State&) override;
        hardware_interface::CallbackReturn on_error(const rclcpp_lifecycle::State&) override;
        hardware_interface::CallbackReturn on_activate (const rclcpp_lifecycle::State &) override;
     private:
        int vel_cmd_timeout_ms_;
        std::vector<double> velocity_state_;
        std::vector<double> position_state_;
        std::vector<double> velocity_commands_;
        boost::chrono::system_clock::time_point last_sent_velocity_time_;
        std::vector<double> last_sent_velocity_commands_;
        std::vector<double> position_commands_;
        std::string agv_type_;
        std::string protocol_version_;
        int external_control_port_;
        boost::asio::io_context io_context_;
        std::unique_ptr<boost::asio::ip::tcp::acceptor> acceptor_;
        std::unique_ptr<boost::asio::ip::tcp::socket> client_socket_;
        boost::circular_buffer<uint8_t> read_buffer_;
        ExternalControlData parseLastMessageFromBuffer();



    };
}
#endif // OMNIMOVE_CONTROLLER_HPP