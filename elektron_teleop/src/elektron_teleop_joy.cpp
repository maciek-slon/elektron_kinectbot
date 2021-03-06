#include <ros/ros.h>
#include <joy/Joy.h>
#include <geometry_msgs/Twist.h>

class ElektronTeleopJoy {
public:
	ElektronTeleopJoy();

private:
	void joyCallback(const joy::Joy::ConstPtr& joy);

	ros::NodeHandle nh_;

	int linear_, angular_;
	double l_scale_, a_scale_;
	ros::Publisher vel_pub_;
	ros::Subscriber joy_sub_;

};

ElektronTeleopJoy::ElektronTeleopJoy() {
	nh_.param("axis_linear", linear_, 1);
	nh_.param("axis_angular", angular_, 0);
	nh_.param("scale_angular", a_scale_, 1.0);
	nh_.param("scale_linear", l_scale_, 0.23);

	vel_pub_ = nh_.advertise<geometry_msgs::Twist> ("cmd_vel", 1);

	joy_sub_ = nh_.subscribe<joy::Joy> ("joy", 10,
			&ElektronTeleopJoy::joyCallback, this);

}

void ElektronTeleopJoy::joyCallback(const joy::Joy::ConstPtr& joy) {
	geometry_msgs::Twist vel;
	vel.angular.z = a_scale_ * joy->axes[angular_];
	vel.linear.x = l_scale_ * joy->axes[linear_];
	vel_pub_.publish(vel);
}

int main(int argc, char** argv) {
	ros::init(argc, argv, "teleop_turtle");
	ElektronTeleopJoy elektron_teleop;

	ros::spin();
}
