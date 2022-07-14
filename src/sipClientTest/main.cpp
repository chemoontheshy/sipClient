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
    auto ret = eXosip_init(excontext);
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
    //����
    char command;
    //ע��id
    int regId = 0;
    //ע����Ϣ
    osip_message_t* reg       = nullptr;
    osip_message_t* invite    = nullptr;
    osip_message_t* ack       = nullptr;
    osip_message_t* message   = nullptr;
    osip_message_t* notify    = nullptr;
    osip_message_t* subscribe = nullptr;
    //����ID
    int callId = -1;
    int dialogId = -1;
    while (flag)
    {
        std::cout << "Please input the commamd:" << std::endl;
        std::cin >> command;
        switch (command)
        {
         //ע��
        case 'r':
        {
            
            eXosip_lock(excontext);
            regId = eXosip_register_build_initial_register(excontext,
                srcCall, dstCall, nullptr, 1800, &reg);
            if ( regId < 0)
            {
                eXosip_unlock(excontext);
                std::cout << "eXosip_register_build_initial_register failed" << std::endl;
                exit(-1);
            }
            eXosip_add_authentication_info(excontext, "hskj", "hskj", "12456", "MD5", nullptr);
            eXosip_register_send_register(excontext, regId, reg);
            eXosip_unlock(excontext);

            bool waitFlag = true;
            while (waitFlag)
            {
                auto je = eXosip_event_wait(excontext, 0, 1200);
                //һ�㴦��401/407���ÿ�Ĭ�ϴ���
                eXosip_lock(excontext);
                eXosip_automatic_action(excontext);
                eXosip_unlock(excontext);
                if (!je)
                {
                    std::cout << "No response or the time is over" << std::endl;
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

            eXosip_lock(excontext);
            regId = eXosip_register_build_register(excontext,regId,1800,&reg);
            if (regId < 0)
            {
                eXosip_unlock(excontext);
                std::cout << "eXosip_register_build_initial_register failed" << std::endl;
                exit(-1);
            }
            eXosip_register_send_register(excontext, regId, reg);
            eXosip_unlock(excontext);
            break;
        }
        case 'c':
        {
            eXosip_lock(excontext);
            auto i = eXosip_register_build_register(excontext, regId, 0, &reg);
            if (i < 0)
            {
                eXosip_unlock(excontext);
                std::cout << "cancel register failed" << std::endl;
                exit(-1);
            }
            osip_message_set_header(reg, "Logout-Reason", "logout");
            eXosip_register_send_register(excontext, regId, reg);
            eXosip_unlock(excontext);
            break;
        }
        case 'i':
        {
            auto i = eXosip_call_build_initial_invite(excontext,
                &invite, dstCall, srcCall, nullptr, "This is a call for conversation");
            if (i != 0)
            {
                std::cout << "Initial INVITE failed!" << std::endl;
                break;
            }
            std::string temp = "v=0\r\n\
                o=anonymous 0 0 IN IP4 0.0.0.0\r\n\
                t=1 10\r\n\
                a=username:hskj\r\n\
                a=password:123456\r\n";
            //����sdp
            osip_message_set_body(invite, temp.c_str(), temp.size());
            osip_message_set_content_type(invite, "application/sdp");

            eXosip_lock(excontext);
            eXosip_call_send_initial_invite(excontext, invite);
            eXosip_unlock(excontext);
            bool waitFlag = true;
            while (waitFlag)
            {
                auto je = eXosip_event_wait(excontext, 0, 200);
                if (!je)
                {
                    std::cout << "No response or the time is over" << std::endl;
                    break;
                }
                switch (je->type) //���ܻᵽ�����¼�
                {
                case EXOSIP_CALL_INVITE:
                {
                    std::cout << "a new invite received" << std::endl;
                    break;
                }
                case EXOSIP_CALL_PROCEEDING:
                {
                    std::cout << "proceeding" << std::endl;
                    break;
                }
                case EXOSIP_CALL_RINGING:
                {
                    std::cout << "ringing" << std::endl;
                    break;
                }
                case EXOSIP_CALL_ANSWERED:
                {
                    std::cout << "ok!connected!" << std::endl;
                    callId = je->cid;
                    dialogId = je->did;
                    std::cout << "callId: " << callId << " dialogId: " << dialogId << std::endl;
                    //����ackӦ����Ϣ
                    eXosip_call_build_ack(excontext, je->did, &ack);
                    eXosip_call_send_ack(excontext, je->did, ack);
                    waitFlag = 0;
                    break;
                }
                case EXOSIP_CALL_CLOSED:
                {
                    std::cout << "the other sid close" << std::endl;
                    break;
                }
                case EXOSIP_CALL_ACK://�յ�ACK received
                {
                    std::cout << "ACK received" << std::endl;
                    break;
                }
                default://�յ�����Ӧ��
                    std::cout << "other response" << std::endl;
                    break;
                }
                eXosip_event_free(je);
            }
            break;
        }
        case 'h':
        {
            std::cout << "Holded" << std::endl;
            eXosip_lock(excontext);
            if (callId > 0 && dialogId > 0)
            {
                eXosip_call_terminate(excontext, callId, dialogId);
                callId = -1;
                dialogId = -1;
            }
            else {
                std::cout << " no invite" << std::endl;
            }
            eXosip_unlock(excontext);
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
            osip_message_set_body(message, msg.c_str(), msg.length());
            //�����ʽ��xml
            osip_message_set_content_type(message, "text/xml");
            eXosip_message_send_request(excontext, message);
            break;
        }
        case 'n':
        {
            std::cout << "the method : NOTIFY" << std::endl;
            eXosip_message_build_request(excontext, &notify, "NOTIFY", dstCall, srcCall, nullptr);
            osip_message_set_header(notify, "Event", "event");
            std::string msg = " This is a sip notify(Method:MESSAGE)\r\n";
            osip_message_set_body(notify, msg.c_str(), msg.length());
            //�����ʽ��xml
            osip_message_set_content_type(notify, "text/xml");
            eXosip_message_send_request(excontext, notify);
            break;
        }
        case 's':
        {
            std::cout << "the method : SUBSCRIBE" << std::endl;
            eXosip_subscription_build_initial_subscribe(excontext, &subscribe, dstCall, srcCall, nullptr,/*"Catalog"*/"presence", 90);
            std::string msg = " This is a sip SUBSCRIBE";
            osip_message_set_body(subscribe, msg.c_str(), msg.length());
            //�����ʽ��xml
            osip_message_set_content_type(subscribe, "text/xml");
            eXosip_subscription_send_initial_request(excontext, subscribe);
            break;
        }
        default:
            break;
        }
    }

    return 0;
}