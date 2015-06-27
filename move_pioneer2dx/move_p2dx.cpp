#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/gazebo.hh>
#define DEG2RAD(x)  x*0.017453293
#define RAD2DEG(x)  x*57.29577951

int main(int argc, char** argv){

    gazebo::setupClient(argc, argv);

    gazebo::transport::NodePtr node(new gazebo::transport::Node());
    node->Init();

    /*
        DiffDrivePlugin is subscribing ~/${model_name}/vel_cmd
        and calculates left/right wheel speed based on vel_cmd message.
    */
    gazebo::transport::PublisherPtr pub =
            node->Advertise<gazebo::msgs::Pose>("~/my_p2dx/vel_cmd");

    pub->WaitForConnection();

    gazebo::common::Timer timer;
    timer.Start();

    while(timer.GetElapsed() < 5.0)
    {
        //velocity command is expressed in gazebo::math::Pose form.
        gazebo::math::Pose pose(1.0, 0.0, 0.0, 0.0, 0.0, DEG2RAD(20));
        gazebo::msgs::Pose pose_msg;
        gazebo::msgs::Set(&pose_msg, pose);//generate message

        pub->Publish(pose_msg); //publish message
        gazebo::common::Time::MSleep(20);//sleep
    }

    gazebo::shutdown();

}