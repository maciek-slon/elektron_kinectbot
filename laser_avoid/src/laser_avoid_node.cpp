#include <ros/ros.h>

#include <sensor_msgs/LaserScan.h>
#include <geometry_msgs/Twist.h>


geometry_msgs::Twist vel;

void laserCallback(const sensor_msgs::LaserScanConstPtr& msg)
{
  double l_range, r_range;

  l_range = r_range = msg->range_max;

  for(unsigned int i = 0; i<msg->ranges.size(); i++)
  {
    if(i<(msg->ranges.size()/2))
    {
      if((l_range > msg->ranges[i]) && (msg->ranges[i] > 0.0))
        l_range = msg->ranges[i];
    } else
    { 
      if((r_range > msg->ranges[i]) && (msg->ranges[i] > 0.0))
        r_range = msg->ranges[i];
    } 
  }


//  double l = (1e5*r_range)/500-100;
//  double r = (1e5*l_range)/500-100;
  double l = 75 * r_range - 50;
  double r = 75 * l_range - 50;

  if (l > 50)
    l = 50;
  if (r > 50)
    r = 50;

  vel.linear.x = (r+l)/1e3;


  double turnrate = (r-l);
  if(turnrate > 10.0)
    turnrate = 10.0;
  if(turnrate < -10.0)
    turnrate = -10.0;

  vel.angular.z = -turnrate * M_PI/180.0;

  ROS_INFO("l: %lf r: %lf x: %lf a: %lf", l_range, r_range, vel.linear.x, vel.angular.z);
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "laser_avoid");

  ros::NodeHandle nh;

  ros::Publisher vel_pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 1);
  ros::Subscriber laser_sub = nh.subscribe("scan", 1, &laserCallback);

  ros::Rate loop_rate(25);

  vel.linear.x = 0.0;
  vel.angular.z = 0.0;

  while(ros::ok())
  {
    if (ros::isShuttingDown()) {
      vel.linear.x = 0.0;
      vel.angular.z = 0.0;
    }
    

    vel_pub.publish(vel);

    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}

