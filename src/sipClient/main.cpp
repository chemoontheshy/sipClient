#include "sipClient.h"
#include <thread>
using namespace vsnc::sip;


#include "eXosip2/eXosip.h"
#include "osip2/osip_mt.h"
#include <iostream>
#include <winsock2.h>
#include <windows.h>

#include <iostream>
#include <string>

const char* srcCall = "sip:client@127.0.0.1:5061";
const char* dstCall = "sip:server@127.0.0.1";

int main()
{
    auto excontext = eXosip_malloc();
	if (eXosip_init(excontext) < 0)
	{
		std::cout << "SIP初始化失败" << std::endl;
		eXosip_quit(excontext);
		return -1;
	}

	if (eXosip_listen_addr(excontext, IPPROTO_UDP, nullptr, 5061, AF_INET, 0) != OSIP_SUCCESS)
	{
		std::cout << "SIP监听失败" << std::endl;
		eXosip_quit(excontext);
		return -1;
	}
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
    SIPHeader from(ADDRCODE, "127.0.0.1", 5061, 1800);
    SIPHeader to(ADDRCODE, "127.0.0.1", 5060, 1800);
    SIPAuthInfo authInfo{ "127.0.0.1",NONCE,"hskj","123456",from.GetSipHeader(),ALGORITHIMH };
    SIPUACParam UACParam{&from,&to,&authInfo,NetworkProtocol::UDP};
    SIPClient sipClient(UACParam);
    char command;
    sipClient.startWork();
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
            sipClient.Reister();
                    break;
                }
                switch (je->type) //可能会到来的事件
                {

                case EXOSIP_REGISTRATION_FAILURE:
                {

                    std::cout << "fail: status_code" << je->response->status_code << std::endl;;
                    waitFlag = false;
                    break;//注册失败
                }
                case EXOSIP_REGISTRATION_SUCCESS:
                {
                    std::cout << "success: status_code" << je->request->status_code << std::endl;;
                    break;//注册成功
                }
                default://收到其他应答
                    std::cout << "other response" << std::endl;
                    break;
                }
                eXosip_event_free(je);
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
                o=anonymous 0 0 IN IP4 0.0.0.0\r\n\
                t=1 10\r\n\
                a=username:hskj\r\n\
                a=password:123456\r\n";
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
            eXosip_quit(excontext);
            std::cout << "Exit the setup" << std::endl;
            flag = false;
            break;
        }
        case 'm':
        {
           //传输MESSAGE方法，也就是即时消息，和INFO方法相比，我认为主要区别是：
           //MESSAGE不用建立连接，直接传输信息，而INFO消息必须在建立INVITE的基础上传输
            std::cout << "the method : MESSAGE" << std::endl;
            eXosip_message_build_request(excontext, &message, "MESSAGE", dstCall, srcCall, nullptr);
            std::string msg = " This is a sip message(Method:MESSAGE)";
            sipClient.Message(msg);
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