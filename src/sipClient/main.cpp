#include "sipClient.h"
#include <thread>

#include "eXosip2/eXosip.h"
#include "osip2/osip_mt.h"
#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include "composeRequest.h"

using namespace vsnc::sip;
bool TestApi(SIPClient* sip,const BInterfaceAction action, RequestParam* requestParam, ResponseParam* responseParam)
{
    switch (action)
    {
    case vsnc::sip::BInterfaceAction::B_REGISTER:
    {
        break;
    }
    case vsnc::sip::BInterfaceAction::B_PUSH_RESOURCE:
    {
        
        auto temp = reinterpret_cast<ResponsePushResourceParam*>(responseParam);
        auto a = sip->getResponsePushResourceParam();
        temp = &a;
        return true;
        break;
    }
    case vsnc::sip::BInterfaceAction::B_RESPONSE_RESOURCE:
    {
        break;
    }
    case vsnc::sip::BInterfaceAction::B_HISTORY_ALARM:
    {
        break;
    }
    case vsnc::sip::BInterfaceAction::B_HISTORY_VIDEO:
    {
        break;
    }
    case vsnc::sip::BInterfaceAction::B_PLAY_VIDEO:
    {
        break;
    }
    case vsnc::sip::BInterfaceAction::B_TALK_AUDIO:
    {
        break;
    }
    case vsnc::sip::BInterfaceAction::B_CONTROL_CAMERA:
    {
        break;
    }
    case vsnc::sip::BInterfaceAction::B_PLAY_BACK_VIDEO:
    {
        break;
    }
    case vsnc::sip::BInterfaceAction::B_SUBSRIBE_ALARM:
    {
        break;
    }
    case vsnc::sip::BInterfaceAction::B_CAMERA_SNAP:
    {
        break;
    }
    case vsnc::sip::BInterfaceAction::B_UPLOAD_SNAP:
    {
        break;
    }
    case vsnc::sip::BInterfaceAction::B_SNAPSHOT_NOTIFY:
    {
        break;
    }
    default:
        break;
    }
    return false;
}


int main()
{
    std::cout << "3 RESPONSE_RESOURCE      资源获取"       << std::endl;
    std::cout << "4 HISTORY_ALARM          历史告警"       << std::endl;
    std::cout << "5 B_HISTORY_VIDEO        录像检索"       << std::endl;
    std::cout << "6 INVITE          邀请,发起呼叫请求,实时流海康" << std::endl;
    std::cout << "7 INVITE          邀请,发起呼叫请求,实时流大华" << std::endl;
    std::cout << "8 INVITE          邀请,发起呼叫请求,录像流海康" << std::endl;
    std::cout << "h BYE             结束会话"         << std::endl;
    std::cout << "q EXIT            退出程序"         << std::endl;
    std::cout << "f INFO            消息"             << std::endl;
    std::cout << "m MESSAGE         消息"             << std::endl;
    std::cout << "n NOTIFY          资源上报"          << std::endl;
    std::cout << "s SUBSCRIBE       订阅"             << std::endl;

    bool flag = true;
    SIPUACParam UACParam;
    // 用户端（即平台端
    UACParam.From.SetSIPHeader("100110001001000000", "172.168.8.41", 8062, 1800);
    //
    UACParam.To.SetSIPHeader("100110001001000001", "172.168.8.41", 5060, 1800);
    UACParam.SIPAuth.Uri = UACParam.From.GetSipHeader();
    UACParam.SIPAuth.Nonce = NONCE;
    UACParam.SIPAuth.Username = "admin";
    //UACParam.SIPAuth.Response = "admin888";
    UACParam.SIPAuth.Response = "hs123456";
    UACParam.SIPAuth.DigestRealm = "172.168.8.41";
    UACParam.SIPAuth.Algorithm = ALGORITHIMH;
    SIPClient sipClient(UACParam);
    char command;
    sipClient.StartWork();
    // 生成XML
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
            // 资源获取
            std::cout << "the method : MESSAGE" << std::endl;
            RequestResourceParam requestParam;
            requestParam.EventType = "Request_Resource";
            requestParam.Code = "100110001001000001";
            requestParam.FromIndex = "1";
            requestParam.ToIndex = "30";
            request.SetParams(&requestParam, BInterfaceAction::B_RESPONSE_RESOURCE);
            request.GetString(strXml);
            sipClient.Message(strXml);
            break;
        }
        case '4':
        {
            // 历史告警查询
            RequestHistoryParam requestParam;
            requestParam.EventType = "Request_History_Alarm";
            requestParam.Code = "100110000003010001";
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
            // 录像检索
            RequestHistoryParam requestParam;
            requestParam.EventType = "Request_History_Video";
            requestParam.Code = "100110000003010001";
            requestParam.UserCode = "100000000000000001";
            requestParam.Type = "1";
            requestParam.BeginTime = "2019-10-22T00:00:00Z";
            requestParam.EndTime = "2022-11-21T23:59:59Z";
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
            SIPHeaderParam param;
            param.AddrCode = "100110000003010001";
            param.AddrIp = "172.168.8.41";
            param.AddrPort = "8060";
            std::cout << "invite" << std::endl;
            std::string sdp = "v=0\r\n\
o=- 0 0 IN IP4 172.168.8.41\r\n\
s=Play\r\n\
c=IN IP4 172.168.8.41\r\n\
m=video 4000 RTP/AVP 100\r\n\
y=123456\r\n\
artmap:100 H264/9000\r\n\
afmtp:100 CIF=1;4CIF=1;F=1;K=1\r\n\
a=rate:sub\r\n\
a=recvonly\r\n";
            //增加sdp
            sipClient.InviteV2(sdp, param);
            break;
        }
        case '7':
        {
            std::cout << "invite" << std::endl;
            std::string sdp = "v=0\r\n\
o=- 0 0 IN IP4 172.168.8.41\r\n\
s=Play\r\n\
y=654321\r\n\
c=IN IP4 172.168.8.41\r\n\
m=video 4000 RTP/AVP 100\r\n\
artmap:100 H264/9000\r\n\
afmtp:100 CIF=1;4CIF=1;F=1;K=1\r\n\
a=rate:main\r\n\
a=recvonly\r\n"; 
            //增加sdp
            sipClient.Invite(sdp);
            break;
        }
        case '8':
        {
            std::cout << "invite" << std::endl;
            std::string sdp = "v=0\r\n\
o=- 0 0 IN IP4 172.168.8.41\r\n\
s=Playback\r\n\
u=rtsp://172.168.8.41/Playback/20221120204119T20221121042724\r\n\
y=654321\r\n\
c=IN IP4 172.168.8.41\r\n\
m=video 4000 RTP/AVP 110\r\n\
artmap:110 H264/9000\r\n\
afmtp:110 CIF=1;4CIF=1;F=1;K=1\r\n\
a=rate:main\r\n\
a=setup:passive\r\n\
a=connection:new\r\n\
a=recvonly\r\n";
            //增加sdp
            SIPHeaderParam param;
            param.AddrCode = "100110000003010001";
            param.AddrIp = "172.168.8.241";
            param.AddrPort = "8000";
            sipClient.InviteV2(sdp, param);
            break;
        }
        case 'b':
        {
            sipClient.Bye();
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
            //此刻的Code无意义，可不要
            requestParam.Code = "100110000003010002";
            RequestAlarmParam alarmParam1;
            alarmParam1.Code = "100110000003010003";
            alarmParam1.Type = "255";
            RequestAlarmParam alarmParam2;
            alarmParam2.Code = "100110000003010002";
            alarmParam2.Type = "255";
            requestParam.lstAlarmParam.push_back(alarmParam1);
            requestParam.lstAlarmParam.push_back(alarmParam2);
            request.SetParams(&requestParam, BInterfaceAction::B_SUBSRIBE_ALARM);
            request.GetString(strXml);
            std::cout << strXml << std::endl;
            SubscriptionParam subParam{ strXml ,"alarm",3600 };
            sipClient.Subscription(subParam);
            break;
        }
        case 'k':
        {
            std::cout << "the method : SUBSCRIBE" << std::endl;
            RequestSubAlarmParam requestParam;
            requestParam.EventType = "Subscribe_Status";
            //此刻的Code无意义，可不要
            requestParam.Code = "100110000003010002";
            RequestAlarmParam alarmParam1;
            alarmParam1.Code = "100110000003010003";
            alarmParam1.Type = "-1";
            RequestAlarmParam alarmParam2;
            alarmParam2.Code = "100110000003010000";
            alarmParam2.Type = "-1";
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
            //增加sdp
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
        case 't':
        {
            
            //ResponsePushResourceParam pParam;
            //if (TestApi(&sipClient,BInterfaceAction::B_PUSH_RESOURCE, nullptr, &pParam))
            //{
            //    std::cout << pParam->lstResource.size();
            //    if (!pParam->lstResource.empty())
            //    {
            //        std::cout << "test95";
            //        for (const auto& resource : pParam->lstResource)
            //        {
            //            std::cout << __LINE__;
            //            //qDebug() << QString::fromStdString(resource.Name);
            //            //ui->boxPushResource->addItem(QString::fromStdString(resource.Name));
            //        }
            //    }

            //}
            //sd
        }
        default:
            break;
        }
    }
    sipClient.Close();
    return 0;
}