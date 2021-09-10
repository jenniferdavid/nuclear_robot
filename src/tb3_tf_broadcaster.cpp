#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include "tf/transform_datatypes.h"

int main(int argc, char** argv){
  ros::init(argc, argv, "my_tf_broadcaster");
  ros::NodeHandle node;

  tf::TransformBroadcaster br1;
  tf::Transform transform1;

  tf::TransformBroadcaster br2;
  tf::Transform transform2;

  ros::Rate rate(10.0);
  while (node.ok()){
    transform1.setOrigin( tf::Vector3(0.0, 0.0, 0.0) );
    transform1.setRotation( tf::Quaternion(0, 0, 0, 1) );
    br1.sendTransform(tf::StampedTransform(transform1, ros::Time::now(), "tb3_0/base_footprint", "tb3_1/odom"));

    transform2.setOrigin( tf::Vector3(0.0, -2.0, 0.0) );
    transform2.setRotation( tf::Quaternion(0, 0, 0, 1) );
    br2.sendTransform(tf::StampedTransform(transform2, ros::Time::now(), "tb3_0/base_footprint", "tb3_2/odom"));

    rate.sleep();
  }
  return 0;
};
