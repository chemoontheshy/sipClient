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
    //命令
    char command;
    //注册id
    int regId = 0;
    //注册信息
    osip_message_t* reg       = nullptr;
    osip_message_t* invite    = nullptr;
    osip_message_t* ack       = nullptr;
    osip_message_t* message   = nullptr;
    osip_message_t* notify    = nullptr;
    osip_message_t* subscribe = nullptr;
    //会议ID
    int callId = -1;
    int dialogId = -1;
    while (flag)
    {
        std::cout << "Please input the commamd:" << std::endl;
        std::cin >> command;
        switch (command)
        {
         //注册
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
                //一般处理401/407采用库默认处理
                eXosip_lock(excontext);
                eXosip_automatic_action(excontext);
                eXosip_unlock(excontext);
                if (!je)
                {
                    std::cout << "No response or the time is over" << std::endl;
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
            //增加sdp
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
                switch (je->type) //可能会到来的事件
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
                    //回送ack应答消息
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
                case EXOSIP_CALL_ACK://收到ACK received
                {
                    std::cout << "ACK received" << std::endl;
                    break;
                }
                default://收到其他应答
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
           //传输MESSAGE方法，也就是即时消息，和INFO方法相比，我认为主要区别是：
           //MESSAGE不用建立连接，直接传输信息，而INFO消息必须在建立INVITE的基础上传输
            std::cout << "the method : MESSAGE" << std::endl;
            eXosip_message_build_request(excontext, &message, "MESSAGE", dstCall, srcCall, nullptr);
            std::string msg = " This is a sip message(Method:MESSAGE)";
            osip_message_set_body(message, msg.c_str(), msg.length());
            //假设格式是xml
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
            //假设格式是xml
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
            //假设格式是xml
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