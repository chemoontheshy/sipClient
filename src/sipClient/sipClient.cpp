#include "sipClient.h"
#include "eXosip2/eXosip.h"
#include "osip2/osip_mt.h"
#include <winsock2.h>
#include <windows.h>
#include <thread>

vsnc::sip::SIPClient::SIPClient(SIPUACParam sipParam):
	m_pFrom(sipParam.From),
	m_pTo(sipParam.To),
	m_pSIPAuth(sipParam.SIPAuth),
	m_iRegisterID(-1),
	m_iCallID(-1),
	m_iDialogID(-1),
	m_bHander(true)
{
	//初始化
	m_pExcontext = eXosip_malloc();
	if (eXosip_init(m_pExcontext) < 0)
	{
		std::cout << "sip init fail." << std::endl;
		Close();
		exit(-1);
	}
	//监听端口
	int ret;
	if (sipParam.Protocol == NetworkProtocol::TCP)
	{
		ret = eXosip_listen_addr(m_pExcontext, IPPROTO_TCP, nullptr, m_pFrom->GetAddrPort(), AF_INET, 0);
	}
	else
	{
		ret = eXosip_listen_addr(m_pExcontext, IPPROTO_UDP, nullptr, m_pFrom->GetAddrPort(), AF_INET, 0);
	}
	if (OSIP_SUCCESS != ret)
	{
		std::cout << "sip listen init fail." << std::endl;
		Close();
		exit(-1);
	}
}

vsnc::sip::SIPClient::~SIPClient()
{
	Close();
}

void vsnc::sip::SIPClient::Close() noexcept
{
	m_bHander = false;
	eXosip_quit(m_pExcontext);
}

bool vsnc::sip::SIPClient::Reister() noexcept
{
	osip_message_t* reg;
	eXosip_lock(m_pExcontext);
	m_iRegisterID = eXosip_register_build_initial_register(m_pExcontext,
		m_pFrom->GetSipHeader().c_str(), m_pTo->GetSipHeader().c_str(),
		nullptr, m_pFrom->GetExpires(), &reg);

	if (m_iRegisterID < 0)
	{
		std::cout << "eXosip_register_build_initial_register failed" << std::endl;
		eXosip_unlock(m_pExcontext);
		return false;
	}
	//增加账号验证信息
	//auto ret = eXosip_add_authentication_info(m_pExcontext,
	//	m_pSIPAuth->Username.c_str(),
	//	m_pSIPAuth->Username.c_str(),
	//	m_pSIPAuth->Response.c_str(),
	//	m_pSIPAuth->Algorithm.c_str(),
	//	m_pSIPAuth->DigestRealm.c_str());
	eXosip_add_authentication_info(m_pExcontext, "hskj", "hskj", "12456", "MD5", nullptr);
	eXosip_register_send_register(m_pExcontext, m_iRegisterID, reg);

	eXosip_unlock(m_pExcontext);
	bool waitFlag = true;
	while (waitFlag)
	{
		auto je = eXosip_event_wait(m_pExcontext, 0, 1200);
		//一般处理401/407采用库默认处理
		eXosip_lock(m_pExcontext);
		eXosip_automatic_action(m_pExcontext);
		eXosip_unlock(m_pExcontext);
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
	return true;
}

bool vsnc::sip::SIPClient::Refresh() noexcept
{
	osip_message_t* reg;
	eXosip_lock(m_pExcontext);
	m_iRegisterID = eXosip_register_build_register(m_pExcontext, m_iRegisterID, m_pFrom->GetExpires(), &reg);
	if (m_iRegisterID < 0)
	{
		std::cout << "eXosip_register_build_initial_register failed" << std::endl;
		eXosip_unlock(m_pExcontext);
		return false;
	}
	eXosip_register_send_register(m_pExcontext, m_iRegisterID, reg);
	eXosip_unlock(m_pExcontext);
	return true;
}

bool vsnc::sip::SIPClient::Logout(const std::string reason) noexcept
{
	osip_message_t* reg;
	eXosip_lock(m_pExcontext);
	m_iRegisterID = eXosip_register_build_register(m_pExcontext, m_iRegisterID, 0, &reg);
	if (m_iRegisterID < 0)
	{
		std::cout << "eXosip_register_build_initial_register failed" << std::endl;
		eXosip_unlock(m_pExcontext);
		return false;
	}
	osip_message_set_header(reg, "Logout-Reason", reason.c_str());
	eXosip_register_send_register(m_pExcontext, m_iRegisterID, reg);
	eXosip_unlock(m_pExcontext);
	return true;
}


bool vsnc::sip::SIPClient::Invite(const std::string sdp) noexcept
{
	osip_message_t* invite;
	eXosip_call_build_initial_invite(m_pExcontext,
		&invite, m_pTo->GetSipHeader().c_str(),
		m_pFrom->GetSipHeader().c_str(),
		nullptr, "This is a call for conversation");
	osip_message_set_body(invite, sdp.c_str(), sdp.size());
	osip_message_set_content_type(invite, "application/sdp");
	eXosip_lock(m_pExcontext);
	eXosip_call_send_initial_invite(m_pExcontext, invite);
	eXosip_unlock(m_pExcontext);
	return true;
}

bool vsnc::sip::SIPClient::Bye() noexcept
{
	eXosip_lock(m_pExcontext);
	if (m_iCallID > 0 && m_iDialogID > 0)
	{
		eXosip_call_terminate(m_pExcontext, m_iCallID, m_iDialogID);
		m_iCallID = -1;
		m_iDialogID = -1;
	}
	else
	{
		std::cout << "No invite" << std::endl;
	}
	eXosip_unlock(m_pExcontext);
	return true;
}

bool vsnc::sip::SIPClient::Message(const std::string context) noexcept
{
	osip_message_t* message;
	eXosip_message_build_request(m_pExcontext,
		&message,
		"MESSAGE",
		m_pTo->GetSipHeader().c_str(),
		m_pFrom->GetSipHeader().c_str(),
		nullptr);
	osip_message_set_body(message, context.c_str(), context.length());
	osip_message_set_content_type(message, "application/xml");
	eXosip_message_send_request(m_pExcontext, message);
	return true;
}

bool vsnc::sip::SIPClient::Notify(const std::string context) noexcept
{
	osip_message_t* notify;
	eXosip_message_build_request(m_pExcontext,
		&notify,
		"NOTIFY",
		m_pTo->GetSipHeader().c_str(),
		m_pFrom->GetSipHeader().c_str(),
		nullptr);
	osip_message_set_body(notify, context.c_str(), context.length());
	osip_message_set_content_type(notify, "application/xml");
	eXosip_message_send_request(m_pExcontext, notify);
	return true;
}


bool vsnc::sip::SIPClient::Subscription(const SubscriptionParam subParam) noexcept
{
	osip_message_t* subscribe;
	eXosip_subscription_build_initial_subscribe(m_pExcontext,
		&subscribe,
		m_pTo->GetSipHeader().c_str(),
		m_pFrom->GetSipHeader().c_str(),
		m_pTo->GetSipHeader().c_str(),
		subParam.Event.c_str(),
		subParam.Expires);
	osip_message_set_body(subscribe, subParam.Context.c_str(), subParam.Context.length());
	osip_message_set_content_type(subscribe, "application/xml");
	eXosip_subscription_send_initial_request(m_pExcontext, subscribe);
	return false;
}


void vsnc::sip::SIPClient::serverHander()
{
	size_t num = 0;
	while (m_bHander)
	{
		auto je = eXosip_event_wait(m_pExcontext, 0, 1000);
		//一般处理401/407采用库默认处理
		/*eXosip_lock(m_pExcontext);
		eXosip_automatic_action(m_pExcontext);
		eXosip_unlock(m_pExcontext);*/

		if (!je)
		{
			//std::cout << "No response or the time is over num=" << num++ << std::endl;
			continue;
		}
		
		switch (je->type)
		{

		case EXOSIP_REGISTRATION_FAILURE:
		{
			std::cout << "fail: status_code" << je->response->status_code << std::endl;
			break;//注册失败
		}
		case EXOSIP_REGISTRATION_SUCCESS:
		{
			std::cout << "success: status_code" << je->request->status_code << std::endl;
			break;//注册成功
		}
		case EXOSIP_CALL_RINGING:
		{
			std::cout << "ringing" << std::endl;
			break;
		}
		case EXOSIP_CALL_PROCEEDING:
		{

			std::cout << "PROCEEDING" << std::endl;
			break;
		}
		case EXOSIP_CALL_MESSAGE_ANSWERED:
		{

			std::cout << "CALL_MESSAGE_ANSWERED" << std::endl;
			break;
		}
		case EXOSIP_CALL_ANSWERED:
		{
			eXosip_lock(m_pExcontext);
			osip_message_t* ack;
			std::cout << "ok!connected!" << std::endl;
			m_iCallID = je->cid;
			m_iDialogID = je->did;
			std::cout << "callId: " << m_iCallID << " dialogId: " << m_iDialogID << std::endl;
			//回送ack应答消息
			eXosip_call_build_ack(m_pExcontext, je->did, &ack);
			eXosip_call_send_ack(m_pExcontext, je->did, ack);
			eXosip_unlock(m_pExcontext);
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
		case EXOSIP_MESSAGE_ANSWERED:
		{
			std::cout << "MESSAGE received" << std::endl;
			break;//消息应答
		}
		case EXOSIP_SUBSCRIPTION_ANSWERED:
		{
			std::cout << "SUBSCRIPTION received" << std::endl;
			break;//消息应答
		}
		default://收到其他应答

			std::cout << "other response" << std::endl;
			break;
		}
		eXosip_event_free(je);
	}
}

void vsnc::sip::SIPClient::threadFun(SIPClient* sip)
{
	sip->serverHander();
}

void vsnc::sip::SIPClient::startWork()
{
	std::thread workThread(threadFun, this);
	workThread.detach();
}