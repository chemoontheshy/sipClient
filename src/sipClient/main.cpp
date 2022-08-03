#include "sipClient.h"
#include <thread>

#include "eXosip2/eXosip.h"
#include "osip2/osip_mt.h"
#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include "composeRequest.h"

using namespace vsnc::sip;

int main()
{
    std::cout << "r REGISTER        �������ע��"     << std::endl;
    std::cout << "u UPDATE          ����ע��"         << std::endl;
    std::cout << "c CANCEL REGISTER ȡ��ע��"         << std::endl;
    std::cout << "i INVITE          ����,�����������" << std::endl;
    std::cout << "h BYE             �����Ự"         << std::endl;
    std::cout << "q EXIT            �˳�����"         << std::endl;
    std::cout << "f INFO            ��Ϣ"             << std::endl;
    std::cout << "m MESSAGE         ��Ϣ"             << std::endl;
    std::cout << "n NOTIFY          ��Դ�ϱ�"          << std::endl;
    std::cout << "s SUBSCRIBE       ����"             << std::endl;

    bool flag = true;
    SIPUACParam UACParam;
    // �û��ˣ���ƽ̨��
    UACParam.From.SetSIPHeader("100000000000000001", "172.168.7.234", 5062, 1800);
    // ����ͷ���
    UACParam.To.SetSIPHeader("100110000003010002", "172.168.7.234", 5060, 1800);
    UACParam.SIPAuth.Uri = UACParam.From.GetSipHeader();
    UACParam.SIPAuth.Nonce = NONCE;
    UACParam.SIPAuth.Username = "admin";
    UACParam.SIPAuth.Response = "hs123456";
    UACParam.SIPAuth.DigestRealm = "172.168.7.234";
    UACParam.SIPAuth.Algorithm = ALGORITHIMH;
    SIPClient sipClient(UACParam);
    char command;
    sipClient.StartWork();
    // ����XML
    vsnc::sip::Request request;
    std::string strXml;
    while (flag)
    {
        std::cout << "Please input the commamd:" << std::endl;
        std::cin >> command;
        switch (command)
        {
      
        case '3':
        {
            // ��Դ��ȡ
            std::cout << "the method : MESSAGE" << std::endl;
            RequestResourceParam requestParam;
            requestParam.EventType = "Request_Resource";
            requestParam.Code = "100110001001000000";
            requestParam.FromIndex = "1";
            requestParam.ToIndex = "30";
            request.SetParams(&requestParam, BInterfaceAction::B_RESPONSE_RESOURCE);
            request.GetString(strXml);
            sipClient.Message(strXml);
            break;
        }
        case '4':
        {
            // ��ʷ�澯��ѯ
            RequestHistoryParam requestParam;
            requestParam.EventType = "Request_History_Alarm";
            requestParam.Code = "100110000003010002";
            requestParam.UserCode = "100000000000000001";
            requestParam.Type = "0";
            requestParam.BeginTime = "2019-07-22T00:00:00Z";
            requestParam.EndTime = "2022-07-31T23:59:59Z";
            requestParam.Level = "1";
            requestParam.FromIndex = "1";
            requestParam.ToIndex = "30";
            request.SetParams(&requestParam, BInterfaceAction::B_HISTORY_ALARM);
            request.GetString(strXml);
            std::cout << strXml << std::endl;
            sipClient.Message(strXml);
            break;
        }
        case '5':
        {
            // ¼�����
            RequestHistoryParam requestParam;
            requestParam.EventType = "Request_History_Video";
            requestParam.Code = "100110000003010002";
            requestParam.UserCode = "100000000000000001";
            requestParam.Type = "1";
            requestParam.BeginTime = "2019-07-22T00:00:00Z";
            requestParam.EndTime = "2022-07-31T23:59:59Z";
            requestParam.Level = "1";
            requestParam.FromIndex = "1";
            requestParam.ToIndex = "30";
            request.SetParams(&requestParam, BInterfaceAction::B_HISTORY_VIDEO);
            request.GetString(strXml);
            std::cout << strXml << std::endl;
            sipClient.Message(strXml);
            break;
        }
        case '6':
        {
            std::cout << "invite" << std::endl;
            std::string sdp = "v=0\r\n\
o=- 0 0 IN IP4 172.168.7.234\r\n\
s=Play\r\n\
c=IN IP4 172.168.7.234\r\n\
m=video 8000 RTP/AVP 100\r\n\
y=123456\r\n\
artmap:100 H264/9000\r\n\
afmtp:100 CIF=1;4CIF=1;F=1;K=1\r\n\
a=rate:sub\r\n\
a=recvonly\r\n";
            //����sdp
            sipClient.Invite(sdp);
            break;
        }
        case 'q':
        {
            RequestControl requestParam;
            requestParam.EventType = "Control_Camera";
            requestParam.Code = "100110000003010002";
            requestParam.Command = "0404";
            requestParam.CommandPara1 = "5";
            requestParam.CommandPara2 = "5";
            requestParam.CommandPara3 = "0";
            request.SetParams(&requestParam, BInterfaceAction::B_CONTROL_CAMERA);
            request.GetString(strXml);
            std::cout << strXml << std::endl;
            sipClient.Message(strXml);
            break;
        }
        case 'e':
        {
            RequestControl requestParam;
            requestParam.EventType = "Control_Camera";
            requestParam.Code = "100110000003010002";
            requestParam.Command = "403";
            requestParam.CommandPara1 = "5";
            requestParam.CommandPara2 = "5";
            requestParam.CommandPara3 = "0";
            request.SetParams(&requestParam, BInterfaceAction::B_CONTROL_CAMERA);
            request.GetString(strXml);
            std::cout << strXml << std::endl;
            sipClient.Message(strXml);
            break;
        }
        case 'a':
        {
            RequestControl requestParam;
            requestParam.EventType = "Control_Camera";
            requestParam.Code = "100110000003010002";
            requestParam.Command = "0402";
            requestParam.CommandPara1 = "5";
            requestParam.CommandPara2 = "5";
            requestParam.CommandPara3 = "0";
            request.SetParams(&requestParam, BInterfaceAction::B_CONTROL_CAMERA);
            request.GetString(strXml);
            std::cout << strXml << std::endl;
            sipClient.Message(strXml);
            break;
        }
        case 'd':
        {
            RequestControl requestParam;
            requestParam.EventType = "Control_Camera";
            requestParam.Code = "100110000003010002";
            requestParam.Command = "401";
            requestParam.CommandPara1 = "5";
            requestParam.CommandPara2 = "5";
            requestParam.CommandPara3 = "0";
            request.SetParams(&requestParam, BInterfaceAction::B_CONTROL_CAMERA);
            request.GetString(strXml);
            std::cout << strXml << std::endl;
            sipClient.Message(strXml);
            break;
        }
        case '9':
        {
            std::cout << "the method : NOTIFY" << std::endl;
            sipClient.Notify(strXml);
            break;
        }
        case 's':
        {
            std::cout << "the method : SUBSCRIBE" << std::endl;
            RequestSubAlarmParam requestParam;
            requestParam.EventType = "Subscribe_Alarm";
            //�˿̵�Code�����壬�ɲ�Ҫ
            requestParam.Code = "100110000003010002";
            RequestAlarmParam alarmParam1;
            alarmParam1.Code = "100110000003010001";
            alarmParam1.Type = "255";
            RequestAlarmParam alarmParam2;
            alarmParam2.Code = "100110000003010002";
            alarmParam2.Type = "255";
            requestParam.lstAlarmParam.push_back(alarmParam1);
            requestParam.lstAlarmParam.push_back(alarmParam2);
            request.SetParams(&requestParam, BInterfaceAction::B_SUBSRIBE_ALARM);
            request.GetString(strXml);
            std::cout << strXml << std::endl;
            SubscriptionParam subParam{ strXml ,"presence",3600 };
            sipClient.Subscription(subParam);
            break;
        }
        case 'v':
        {
            std::cout << "the method : PlayBack" << std::endl;
            std::string sdp = "v=0\r\n\
o=- 0 0 IN IP4 172.168.7.234\r\n\
s=Playback\r\n\
u=rtsp://172.168.7.234/Playback/20220622190024T20220623115422\r\n\
c=IN IP4 172.168.7.234\r\n\
m=video 8000 RTP/AVP 100\r\n\
y=123456\r\n\
artmap:100 H264/9000\r\n\
afmtp:100 CIF=1;4CIF=1;F=1;K=1\r\n\
a=recvonly\r\n";
            //����sdp
            sipClient.Invite(sdp);
            break;
        }
        case 'p':
        {
            std::string rtspPlay = "Play RTSP/1.0\r\n\
Session: 123456\r\n\
CSwq: 2\r\n\
Range: ntp=10-28\r\n";
            sipClient.Message(rtspPlay);
        }
        case 'c':
        {
            RequestCameraSnapParam requestParam;
            requestParam.Code = "100110000003010002";
            requestParam.EventType = "Camera_Snap";
            requestParam.PicServer = "http://127.0.0.1:8000/AlarmPic";
            requestParam.SnapType = "0";
            requestParam.Range = "0:92700";
            requestParam.Interval = "100";
            request.SetParams(&requestParam, BInterfaceAction::B_CAMERA_SNAP);
            request.GetString(strXml);
            std::cout << strXml << std::endl;
            sipClient.Message(strXml);
        }
        case 'u':
        {
            RequestSnapshotNotifyParam requestParam;
            requestParam.Code = "100110000003010002";
            requestParam.EventType = "Snapshot_Notify";
            RequestSnapshotParam param;
            param.Code = "100110000003010002";
            param.Type = "0";
            param.FileUrl = "http://172.168.7.100:8080/AlarmPic//100110000003010099_20220722T164347Z.jpg";
            param.FileSize = "353392";
            param.Verfiy = "@#22hd2d";
            param.Time = "20220802T130204Z";
            requestParam.lstSnapshotParam.push_back(param);
            request.SetParams(&requestParam, BInterfaceAction::B_SNAPSHOT_NOTIFY);
            request.GetString(strXml);
            std::cout << strXml << std::endl;
            sipClient.Notify(strXml);
        }
        default:
            break;
        }
    }
    sipClient.Close();
    return 0;
}