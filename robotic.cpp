#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dhdc.h"
#include "drdc.h"
#include "DianaAPI.h"
#include "DianaAPIDef.h"

int main()
{   
    //// ���ֵĳ�ʼ��������������������
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
    // initSrv�����������ǳ�ʼ����е�ۣ�������ʼ����е�۵��������ӡ���ʼ����е�۵�״̬��Ϣ����ʼ����е�۵Ĵ�����Ϣ�ȡ�
    if (ret < 0)
    {
        printf("192.168.10.75 initSrv failed! Return value = %d\n", ret);
    }
    if (pinfo)
    {
        delete pinfo;
        pinfo = nullptr;
    }
    //MasterHand������p����posesǰ�������ֱ���xyz������������ı䣬�м�Ӧ���Ƚ�Dianaĩ�˵�λ���ƶ���ָ��λ�ã��ٽ��п�ʼ����

    // get encoder values
    // apply zero force for convenience
    dhdSetForceAndTorqueAndGripperForce(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);


    double poses[6] = { 0.087, 0.0, 1.0827, 0.0, 0.0, 0.0 }; //�мǣ�һ��Ҫ�ĳɻ�е�۵ĳ�ʼλ�ˣ���ֹ�����쳣
    double vel = 0.2;// ��е���ٶ�
    double acc = 0.4;// ��е�ۼ��ٶ�

    //��е�ۺ����˶�����
    while (!done)
    {
        int ret = moveJToPose(poses, vel, acc, nullptr, strIpAddress);
        if (ret < 0)
        {
            printf("moveJToPose failed! Return value = %d\n", ret);
        }
        // wait_move(strIpAddress);
        // wait_move�����������ǵȴ���е���˶���Ŀ��λ�á��Ƿ���Ҫ�������Ƿ���Ҫ�ȴ���ã�ȡ���ڻ�е�۵��˶��ٶȺͼ��ٶȡ�
        dhdGetPosition(&p[0], &p[1], &p[2]);
        // check for exit condition
        if (dhdKbHit() && dhdKbGet() == 'q')
            done = 1;
    }

    //������е�ۣ�дһ��һ����������ֺͻ�е�ۣ�
    destroySrv(strIpAddress);
}

void logRobotState(StrRobotStateInfo* pinfo, const char* strIpAddress)
// logRobotState�����������Ǵ�ӡ��е�۵�״̬��Ϣ�������ؽ�λ�á��ؽڵ������ؽ����ء�TCPλ�õȡ�
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
// errorControl�����������Ǵ�ӡ��е�۵Ĵ�����Ϣ��
{
    strIpAddress = "192.168.10.75";
    const char* strError = formatError(e); // �ú�����������
    printf("error code (%d):%s\n", e, strError);
}
