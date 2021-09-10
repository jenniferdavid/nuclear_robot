#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
//#include "sensor_msgs/LaserScan.h"

int main(int argc, char** argv)
{
	// 初始化ROS节点
	ros::init(argc, argv, "my_tf_listener");

    // 创建节点句柄
	ros::NodeHandle node;

	// 请求产生turtle3
	//ros::service::waitForService("/spawn");
	//ros::ServiceClient add_turtle = node.serviceClient<turtlesim::Spawn>("/spawn");
	//turtlesim::Spawn srv;
	//add_turtle.call(srv);

	// 创建发布tb3_2速度控制指令的发布者
	ros::Publisher tb3_2_vel = node.advertise<geometry_msgs::Twist>("/tb3_2/cmd_vel", 10);

	// 创建tf的监听器
	tf::TransformListener listener;

	ros::Rate rate(10.0);
	while (node.ok())
	{
		// 获取turtle2与turtle3坐标系之间的tf数据
		tf::StampedTransform transform12;
		tf::StampedTransform transform21;
		try
		{
		        listener.waitForTransform("/tb3_0", "/tb3_2", ros::Time(0), ros::Duration(3.0));
			listener.lookupTransform("/tb3_0", "/tb3_2", ros::Time(0), transform12);
			
		}
		catch (tf::TransformException &ex) 
		{
			ROS_ERROR("%s",ex.what());
			ros::Duration(1.0).sleep();
			continue;
		}

		// 根据tb3_1与tb3_2坐标系之间的位置关系，发布turtle3的速度控制指令calculate the speed of follower2
		geometry_msgs::Twist vel_msg;
		vel_msg.angular.z = 4.0 * atan2(transform12.getOrigin().y(),transform12.getOrigin().x());
		vel_msg.linear.x = 0.5 * sqrt(pow(transform12.getOrigin().x(), 2) + pow(transform12.getOrigin().y(), 2));
		tb3_2_vel.publish(vel_msg);

		rate.sleep();
	}
	return 0;
};
