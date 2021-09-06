#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include "tf/transform_datatypes.h"

 std::string waffle;

void poseCallback(const nav_msgs::Odometry::ConstPtr& msg)
{
 // create tf broadcaster
 static tf::TransformBroadcaster br;

 // initialiaze tf data
 tf::Transform transform;
 transform.setOrigin( tf::Vector3(msg->pose.pose.position.x, msg->pose.pose.position.y, 0.0) );
 double roll, pitch, yaw;
 tf::Quaternion q;
 tf::Quaternion quat;
 tf::quaternionMsgToTF(msg->pose.pose.orientation, quat);
   tf::Matrix3x3(quat).getRPY(roll, pitch, yaw);
 q.setRPY(0.0, 0.0, yaw);
 transform.setRotation(q);

 // 广播world与海龟坐标系之间的tf数据 publish tf data
 br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "world", waffle));
}

int main(int argc,char**argv)
{
    ros::init(argc,argv,"my_tf_broadcaster");
    if(argc!=2)
    {
        ROS_ERROR("need turtle name as argument!");
	return -1;
    }
    waffle=argv[1];
    ros::NodeHandle node;
    ros::Subscriber sub = node.subscribe(waffle+"/base_pose_ground_truth", 10, poseCallback);
    ros::spin();
    return 0;
}
