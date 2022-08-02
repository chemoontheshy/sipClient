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
    std::cout << "r REGISTER        向服务器注册"     << std::endl;
    std::cout << "u UPDATE          更新注册"         << std::endl;
    std::cout << "c CANCEL REGISTER 取消注册"         << std::endl;
    std::cout << "i INVITE          邀请,发起呼叫请求" << std::endl;
    std::cout << "h BYE             结束会话"         << std::endl;
    std::cout << "q EXIT            退出程序"         << std::endl;
    std::cout << "f INFO            消息"             << std::endl;
    std::cout << "m MESSAGE         消息"             << std::endl;
    std::cout << "n NOTIFY          资源上报"          << std::endl;
    std::cout << "s SUBSCRIBE       订阅"             << std::endl;

    bool flag = true;
    SIPUACParam UACParam;
    UACParam.From.SetSIPHeader("100110001001000000", "172.168.7.234", 5062, 1800);
    // 摄像头编号
    UACParam.To.SetSIPHeader("100110000003010003", "172.168.7.234", 5060, 1800);
    UACParam.SIPAuth.Uri = UACParam.From.GetSipHeader();
    UACParam.SIPAuth.Nonce = NONCE;
    UACParam.SIPAuth.Username = "admin";
    UACParam.SIPAuth.Response = "hs123456";
    UACParam.SIPAuth.DigestRealm = "172.168.7.234";
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
         //注册
        case 'r':
        {
            std::cout << "Reister" << std::endl;
            if (!sipClient.Reister())
            {
                std::cout << "error" << std::endl;
            }
            break;
        }
        case 'u':
        {
            std::cout << "update" << std::endl;
            sipClient.Refresh();
            break;
        }
        case 'c':
        {
            std::cout << "cancel" << std::endl;
            sipClient.Logout("TER");
            break;
        }
        case 'i':
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
            //增加sdp
            sipClient.Invite(sdp);
            break;
        }
        case 'h':
        {
            std::cout << "Holded" << std::endl;
            sipClient.Bye();
            break;
        }
        case 'q':
        {
            sipClient.Close();
            std::cout << "Exit the setup" << std::endl;
            flag = false;
            break;
        }
        case 'g':
        {
            // 资源获取
            std::cout << "the method : MESSAGE" << std::endl;
            RequestResourceParam requestParam;
            requestParam.EventType = "Request_Resource";
            requestParam.Code = "100110001001000000";
            requestParam.FromIndex = "1";
            requestParam.ToIndex = "30";
            request.SetParams(&requestParam, RequestAction::B_RESOURCE);
            request.GetString(strXml);
            sipClient.Message(strXml);
            break;
        }
        case 'l':
        {
            // 历史告警查询
            RequestHistoryParam requestParam;
            requestParam.EventType = "Request_History_Alarm";
            requestParam.Code = "100110000003010002";
            requestParam.UserCode = "990010000000000000";
            requestParam.Type = "-1";
            requestParam.BeginTime = "2019-07-22T00:00:00Z";
            requestParam.EndTime = "2022-07-31T23:59:59Z";
            requestParam.Level = "1";
            requestParam.FromIndex = "1";
            requestParam.ToIndex = "30";
            request.SetParams(&requestParam, RequestAction::B_HISTORY_ALARM);
            request.GetString(strXml);
            sipClient.Message(strXml);
            break;
        }
        case 'o':
        {
            // 录像检索
            RequestHistoryParam requestParam;
            requestParam.EventType = "Request_History_Video";
            requestParam.Code = "100110000003010002";
            requestParam.UserCode = "990010000000000000";
            requestParam.Type = "-1";
            requestParam.BeginTime = "2019-07-22T00:00:00Z";
            requestParam.EndTime = "2022-07-31T23:59:59Z";
            requestParam.Level = "1";
            requestParam.FromIndex = "1";
            requestParam.ToIndex = "30";
            request.SetParams(&requestParam, RequestAction::B_HISTORY_VIDEO);
            request.GetString(strXml);
            sipClient.Message(strXml);
            break;
        }
        
        case '1':
        {
            RequestControl requestParam;
            requestParam.EventType = "Control_Camera";
            requestParam.Code = "100110000003010002";
            requestParam.Command = "0404";
            requestParam.CommandPara1 = "5";
            requestParam.CommandPara2 = "5";
            requestParam.CommandPara3 = "0";
            request.SetParams(&requestParam, RequestAction::B_CONTROL_CAMERA);
            request.GetString(strXml);
            std::cout << strXml << std::endl;
            sipClient.Message(strXml);
            break;
        }
        case '2':
        {
            RequestControl requestParam;
            requestParam.EventType = "Control_Camera";
            requestParam.Code = "100110000003010002";
            requestParam.Command = "403";
            requestParam.CommandPara1 = "5";
            requestParam.CommandPara2 = "5";
            requestParam.CommandPara3 = "0";
            request.SetParams(&requestParam, RequestAction::B_CONTROL_CAMERA);
            request.GetString(strXml);
            std::cout << strXml << std::endl;
            sipClient.Message(strXml);
            break;
        }
        case '3':
        {
            RequestControl requestParam;
            requestParam.EventType = "Control_Camera";
            requestParam.Code = "100110000003010002";
            requestParam.Command = "0402";
            requestParam.CommandPara1 = "5";
            requestParam.CommandPara2 = "5";
            requestParam.CommandPara3 = "0";
            request.SetParams(&requestParam, RequestAction::B_CONTROL_CAMERA);
            request.GetString(strXml);
            std::cout << strXml << std::endl;
            sipClient.Message(strXml);
            break;
        }
        case '4':
        {
            RequestControl requestParam;
            requestParam.EventType = "Control_Camera";
            requestParam.Code = "100110000003010002";
            requestParam.Command = "401";
            requestParam.CommandPara1 = "5";
            requestParam.CommandPara2 = "5";
            requestParam.CommandPara3 = "0";
            request.SetParams(&requestParam, RequestAction::B_CONTROL_CAMERA);
            request.GetString(strXml);
            std::cout << strXml << std::endl;
            sipClient.Message(strXml);
            break;
        }
        case 'n':
        {
           
            std::string notify = "<xml>notify</xml>";
            sipClient.Notify(notify);
            break;
        }
        case 's':
        {
            std::cout << "the method : SUBSCRIBE" << std::endl;
            SubscriptionParam subParam{ "<xml>notify</xml>" ,"presence",3600 };
            sipClient.Subscription(subParam);
            break;
        }
        default:
            break;
        }
    }
    sipClient.Close();
    return 0;
}