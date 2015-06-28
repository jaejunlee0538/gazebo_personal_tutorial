#include <gazebo/gazebo.hh>
const double D2R = 3.141592/180.0;

int main(){

    gazebo::math::Pose pose_0_1, pose_0_2;
    pose_0_1 = gazebo::math::Pose(10,-4,0,  0,0,-30*D2R);
    pose_0_2 = gazebo::math::Pose(-3,-6,0,  0,0,60*D2R);

    gazebo::math::Pose pose_2_1;
    pose_2_1.pos = pose_0_1.pos - pose_0_2.pos;
    pose_2_1.pos = pose_0_2.rot.RotateVectorReverse(pose_2_1.pos);


    //TODO : Which one is right?
    //Only with yaw, rotation is commutable.
    //(means only with single rotation axis)
    pose_2_1.rot = pose_0_1.rot * pose_0_2.rot.GetInverse();
    pose_2_1.rot = pose_0_2.rot.GetInverse() * pose_0_1.rot;


    printf("pos : %.4f %.4f %.4f\n",pose_2_1.pos.x, pose_2_1.pos.y, pose_2_1.pos.z);
    printf("rot : [w, x, y, z] = [%.4f, %.4f, %.4f, %.4f]\n",
    pose_2_1.rot.w, pose_2_1.rot.x, pose_2_1.rot.y, pose_2_1.rot.z);

    return 1;
}