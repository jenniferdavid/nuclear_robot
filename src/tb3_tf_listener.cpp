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

	// 创建发布tb3_1速度控制指令的发布者 create publisher to control the speed of tb3_1
	ros::Publisher tb3_1_vel = node.advertise<geometry_msgs::Twist>("tb3_1/cmd_vel", 10);
	ros::Publisher tb3_2_vel = node.advertise<geometry_msgs::Twist>("tb3_2/cmd_vel", 10);


	// 创建tf的监听器 create tf listener
	tf::TransformListener listener1;
	tf::TransformListener listener2;

	ros::Rate rate(10.0);
	while (node.ok())
	{
		// 获取turtle1与turtle2坐标系之间的tf数据 get coordinate transform tf data
		tf::StampedTransform transformfl1;
		tf::StampedTransform transformfl2;
		
		try
		{
		    listener1.waitForTransform("/tb3_0/base_footprint", "/tb3_1/odom", ros::Time(0), ros::Duration(3.0));
			listener1.lookupTransform("/tb3_0/base_footprint", "/tb3_1/odom", ros::Time(0), transformfl1);
		}
		catch (tf::TransformException &ex) 
		{
			ROS_ERROR("%s",ex.what());
			ros::Duration(1.0).sleep();
			continue;
		}
	
		try
		{
		    listener2.waitForTransform("/tb3_0/base_footprint", "/tb3_2/odom", ros::Time(0), ros::Duration(3.0));
			listener2.lookupTransform("/tb3_0/base_footprint", "/tb3_2/odom", ros::Time(0), transformfl2);
		}
		catch (tf::TransformException &ex) 
		{
			ROS_ERROR("%s",ex.what());
			ros::Duration(1.0).sleep();
			continue;
		}

		// 根据tb3_0与tb3_1坐标系之间的位置关系，发布turtle2的速度控制指令calculate the speed of follower1
		geometry_msgs::Twist vel_msg1;
		geometry_msgs::Twist vel_msg2;

		vel_msg1.angular.z = 4.0 * atan2(transformfl1.getOrigin().y(),transformfl1.getOrigin().x());
		vel_msg1.linear.x = 0.5 * sqrt(pow(transformfl1.getOrigin().x(), 2) + pow(transformfl1.getOrigin().y(), 2));
		
	    vel_msg2.angular.z = 4.0 * atan2(transformfl2.getOrigin().y(),transformfl2.getOrigin().x());
	    vel_msg2.linear.x = 0.5 * sqrt(pow(transformfl2.getOrigin().x(), 2) + pow(transformfl2.getOrigin().y(), 2));
		
		tb3_1_vel.publish(vel_msg1);
		tb3_2_vel.publish(vel_msg2);

		rate.sleep();
	}
	return 0;
};
