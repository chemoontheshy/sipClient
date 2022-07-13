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
		std::cout << "SIP��ʼ��ʧ��" << std::endl;
		eXosip_quit(excontext);
		return -1;
	}

	if (eXosip_listen_addr(excontext, IPPROTO_UDP, nullptr, 5061, AF_INET, 0) != OSIP_SUCCESS)
	{
		std::cout << "SIP����ʧ��" << std::endl;
		eXosip_quit(excontext);
		return -1;
	}
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
         //ע��
        case 'r':
        {
            std::cout << "Reister" << std::endl;
            sipClient.Reister();
                    break;
                }
                switch (je->type) //���ܻᵽ�����¼�
                {

                case EXOSIP_REGISTRATION_FAILURE:
                {

                    std::cout << "fail: status_code" << je->response->status_code << std::endl;;
                    waitFlag = false;
                    break;//ע��ʧ��
                }
                case EXOSIP_REGISTRATION_SUCCESS:
                {
                    std::cout << "success: status_code" << je->request->status_code << std::endl;;
                    break;//ע��ɹ�
                }
                default://�յ�����Ӧ��
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
            //����sdp
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
           //����MESSAGE������Ҳ���Ǽ�ʱ��Ϣ����INFO������ȣ�����Ϊ��Ҫ�����ǣ�
           //MESSAGE���ý������ӣ�ֱ�Ӵ�����Ϣ����INFO��Ϣ�����ڽ���INVITE�Ļ����ϴ���
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