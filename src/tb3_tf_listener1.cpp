#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
//#include "sensor_msgs/LaserScan.h"

int main(int argc, char** argv)
{
	// 初始化ROS节点initialize ros node
	ros::init(argc, argv, "my_tf_listener");

    // 创建节点句柄
	ros::NodeHandle node;

	// 请求产生turtle2
	//ros::service::waitForService("/spawn");
	//ros::ServiceClient add_turtle = node.serviceClient<turtlesim::Spawn>("/spawn");
	//turtlesim::Spawn srv;
	//add_turtle.call(srv);

	// 创建发布tb3_1速度控制指令的发布者 create publisher to control the speed of tb3_1
	ros::Publisher tb3_1_vel = node.advertise<geometry_msgs::Twist>("/tb3_1/cmd_vel", 10);

	// 创建tf的监听器 create tf listener
	tf::TransformListener listener;

	ros::Rate rate(10.0);
	while (node.ok())
	{
		// 获取turtle1与turtle2坐标系之间的tf数据 get coordinate transform tf data
		tf::StampedTransform transformfl;
		tf::StampedTransform transformlf;
		try
		{
		        listener.waitForTransform("/tb3_1", "/tb3_0", ros::Time(0), ros::Duration(3.0));
			listener.lookupTransform("/tb3_1", "/tb3_0", ros::Time(0), transformfl);
			
		}
		catch (tf::TransformException &ex) 
		{
			ROS_ERROR("%s",ex.what());
			ros::Duration(1.0).sleep();
			continue;
		}
		try
		{
		        listener.waitForTransform("/tb3_0", "/tb3_1", ros::Time(0), ros::Duration(3.0));
			listener.lookupTransform("/tb3_0", "/tb3_1", ros::Time(0), transformlf);
			
		}
		catch (tf::TransformException &ex) 
		{
			ROS_ERROR("%s",ex.what());
			ros::Duration(1.0).sleep();
			continue;
		}

		// 根据tb3_0与tb3_1坐标系之间的位置关系，发布turtle2的速度控制指令calculate the speed of follower1
		geometry_msgs::Twist vel_msg;
		vel_msg.angular.z = 4.0 * atan2(transformfl.getOrigin().y(),
				                        transformfl.getOrigin().x());
		vel_msg.linear.x = 0.5 * sqrt(pow(transformfl.getOrigin().x(), 2) +
				                      pow(transformfl.getOrigin().y(), 2));
		tb3_1_vel.publish(vel_msg);

		rate.sleep();
	}
	return 0;
};
