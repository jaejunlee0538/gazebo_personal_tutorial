## About DiffDrivePlugin

* [plugin source code][diff_wheel_plugin]

Left/Right wheel velocity are calculated as follows.
Idea is longitudinal velocity(`vr`) added or subtracted by angular velocity(`va`).
`wheelSeparation` stands for the length of track(distance of left and right wheel).

```c++
void DiffDrivePlugin::OnVelMsg(ConstPosePtr &_msg)
{
  double vr, va;

  vr = _msg->position().x();
  va =  msgs::Convert(_msg->orientation()).GetAsEuler().z;

  this->wheelSpeed[LEFT] = vr + va * this->wheelSeparation / 2.0;
  this->wheelSpeed[RIGHT] = vr - va * this->wheelSeparation / 2.0;
}

void DiffDrivePlugin::OnUpdate()
{
  double leftVelDesired = (this->wheelSpeed[LEFT] / this->wheelRadius);
  double rightVelDesired = (this->wheelSpeed[RIGHT] / this->wheelRadius);

  this->leftJoint->SetVelocity(0, leftVelDesired);
  this->rightJoint->SetVelocity(0, rightVelDesired);

  this->leftJoint->SetMaxForce(0, this->torque);
  this->rightJoint->SetMaxForce(0, this->torque);
}
```

## Example Code

```c++
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
```

## How to Run

Run the following commands.

```shell
$ cd move_pioneer2dx
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./move_pioneer2dx
```

## Code Explanation
...

## Check Logged Data

You can check the history of every models's movement with `gzlog`.
You can locate logged data at **~/.gazebo/log/** directory.

Because there are a bunch of information, it would be better to extract the information only which you are interested in using `--filter` option.
Let's extract velocity information of chassis link.

```shell
$ gzlog step 2015-06-28T03\:04\:18.103597/gzserver/state.log --filter my_p2dx/*_wheel.velocity
...
<model name='my_p2dx'>
<link name='chassis'>
<velocity>0.992900 0.036500 0.005000 0.020200 0.009800 -0.290500</velocity>
</link>
</model>
....
<model name='my_p2dx'>
<link name='chassis'>
<velocity>0.993500 0.035700 -0.002600 0.023000 0.021100 -0.300100</velocity>
</link>
</model>
....
<model name='my_p2dx'>
<link name='chassis'>
<velocity>0.988500 0.038800 0.003400 -0.022500 -0.013600 -0.295700</velocity>
</link>
</model>
....
<model name='my_p2dx'>
<link name='chassis'>
<velocity>0.992900 0.036500 0.005000 0.020200 0.009800 -0.290500</velocity>
</link>
</model>
....
<model name='my_p2dx'>
<link name='chassis'>
<velocity>0.992000 0.037200 -0.000500 0.000000 -0.005800 -0.286200</velocity>
</link>
</model>
```

## Issues

* what `leftVelDesired, rightVelDesired` do mean?
* velocity of chassis did not match with hand calculated values(**lon:rot = 1.0:0.349**)


[diff_wheel_plugin]:https://bitbucket.org/osrf/gazebo/src/8091da8b3c529a362f39b042095e12c94656a5d1/plugins/DiffDrivePlugin.cc?at=gazebo2_2.2.5
