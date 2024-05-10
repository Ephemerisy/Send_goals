#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include<iostream>
using namespace std;

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
int main(int argc, char** argv){
    ros::init(argc, argv, "send_goals_node");
    ros::NodeHandle n;
    ros::Publisher goal_pub = n.advertise<geometry_msgs::PoseStamped>("move_base_simple/goal",10);
    MoveBaseClient ac("move_base", true);
    
    while(!ac.waitForServer( ros::Duration( 5.0 ) )){
        ROS_INFO("Waiting for the move_base action server to come up");
    }

    ros::Rate loop_rate(10);
    uint8_t goal_number = 0;
    uint8_t total_goal = 4;
    move_base_msgs::MoveBaseGoal goal[total_goal];
    geometry_msgs::PoseStamped goal_simple[total_goal];
    double x[total_goal],y[total_goal],z[total_goal],w[total_goal];

    x[0] = 1.00; y[0] = 0.00; z[0] = 0.00; w[0] = 1.00;
    x[1] = 3.00; y[1] = 0.00; z[1] = 0.00; w[1] = 1.00;
    x[2] = 5.00; y[2] = 0.00; z[2] = 0.00; w[2] = 1.00;
    x[3] = 7.00; y[3] = 0.00; z[3] = 0.00; w[3] = 1.00;
 

    for(int i=0;i<total_goal;i++){
        goal[i].target_pose.pose.position.x = x[i];
        goal[i].target_pose.pose.position.y = y[i];
        goal[i].target_pose.pose.orientation.z = z[i];
        goal[i].target_pose.pose.orientation.w = w[i];

        goal_simple[i].pose.position.x = x[i];
        goal_simple[i].pose.position.y = y[i];
        goal_simple[i].pose.orientation.z = z[i];
        goal_simple[i].pose.orientation.w = w[i];
    } 
    
    ROS_INFO(" Init success!!! ");
    while(goal_number < total_goal && ros::ok())    // total is 4 goals
    {
        goal[goal_number].target_pose.header.frame_id = "map";
        goal[goal_number].target_pose.header.stamp = ros::Time::now();
        ac.sendGoal(goal[goal_number]);

        goal_simple[goal_number].header.frame_id = "map";
        goal_simple[goal_number].header.stamp = ros::Time::now();
        goal_pub.publish(goal_simple[goal_number]);
        
        ROS_INFO("Send NO. %d Goal !!!", goal_number );

        ac.waitForResult();
        if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED){
            ROS_INFO("The NO. %d Goal achieved success !!!", goal_number );
            goal_number++;
        }else{
            ROS_WARN("The NO. %d Goal Planning...",goal_number);
        }
        loop_rate.sleep();
    }
  return 0;
}
