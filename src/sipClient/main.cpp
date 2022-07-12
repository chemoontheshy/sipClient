#include "sipClient.h"
#include <thread>
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
            std::cout << "Exit the setup" << std::endl;
            flag = false;
            break;
        }
        case 'm':
        {
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