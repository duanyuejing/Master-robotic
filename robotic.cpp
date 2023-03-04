#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dhdc.h"
#include "drdc.h"
#include "DianaAPI.h"
#include "DianaAPIDef.h"

int main()
{   
    //// 主手的初始参数；；；；；；；；
    int i, j, n;
    int done = 0;
    double r[3][3];
    int enc[DHD_MAX_DOF];
    double p[3];
    int encCount;

    const char* strIpAddress = "192.168.10.75";
    srv_net_st* pinfo = new srv_net_st();
    memset(pinfo->SrvIp, 0x00, sizeof(pinfo->SrvIp));
    memcpy(pinfo->SrvIp, "192.168.10.75", strlen("192.168.10.75"));
    pinfo->LocHeartbeatPort = 0;
    pinfo->LocRobotStatePort = 0;
    pinfo->LocSrvPort = 0;
    int ret = initSrv(errorControl, logRobotState, pinfo);
    // initSrv函数的作用是初始化机械臂，包括初始化机械臂的网络连接、初始化机械臂的状态信息、初始化机械臂的错误信息等。
    if (ret < 0)
    {
        printf("192.168.10.75 initSrv failed! Return value = %d\n", ret);
    }
    if (pinfo)
    {
        delete pinfo;
        pinfo = nullptr;
    }
    //MasterHand的数组p传给poses前三个，分别是xyz，可以是情况改变，切记应该先将Diana末端的位置移动到指定位置，再进行开始传参

    // get encoder values
    // apply zero force for convenience
    dhdSetForceAndTorqueAndGripperForce(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);


    double poses[6] = { 0.087, 0.0, 1.0827, 0.0, 0.0, 0.0 }; //切记，一定要改成机械臂的初始位姿，防止出现异常
    double vel = 0.2;// 机械臂速度
    double acc = 0.4;// 机械臂加速度

    //机械臂核心运动程序
    while (!done)
    {
        int ret = moveJToPose(poses, vel, acc, nullptr, strIpAddress);
        if (ret < 0)
        {
            printf("moveJToPose failed! Return value = %d\n", ret);
        }
        // wait_move(strIpAddress);
        // wait_move函数的作用是等待机械臂运动到目标位置。是否需要，或者是否需要等待多久，取决于机械臂的运动速度和加速度。
        dhdGetPosition(&p[0], &p[1], &p[2]);
        // check for exit condition
        if (dhdKbHit() && dhdKbGet() == 'q')
            done = 1;
    }

    //结束机械臂（写一个一块结束，主手和机械臂）
    destroySrv(strIpAddress);
}

void logRobotState(StrRobotStateInfo* pinfo, const char* strIpAddress)
// logRobotState函数的作用是打印机械臂的状态信息，包括关节位置、关节电流、关节力矩、TCP位置等。
{
    strIpAddress = "192.168.10.75";
    static int staCnt = 1;
    if ((staCnt++ % 1000 == 0) && pinfo)
    {
        for (int i = 0; i < 7; ++i)
        {
            printf("jointPos[%d] = %f \n", i, pinfo->jointPos[i]);
            printf("jointCurrent [%d] = %f \n", i, pinfo->jointCurrent[i]);
            printf("jointTorque [%d] = %f \n", i, pinfo->jointTorque[i]);
            if (i < 6)
            {
                printf("tcpPos [%d] = %f \n", i, pinfo->tcpPos[i]);
            }
        }
    }
}
void errorControl(int e, const char* strIpAddress)
// errorControl函数的作用是打印机械臂的错误信息。
{
    strIpAddress = "192.168.10.75";
    const char* strError = formatError(e); // 该函数后面会介绍
    printf("error code (%d):%s\n", e, strError);
}
