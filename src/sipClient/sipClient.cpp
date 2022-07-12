#include "sipClient.h"
#include "eXosip2/eXosip.h"
#include "osip2/osip_mt.h"
#include <winsock2.h>
#include <windows.h>



vsnc::sip::SIPClient::SIPClient(SIPParam sipParam):
	m_pFrom(sipParam.From),
	m_pTo(sipParam.To),
	m_iRegisterID(-1)
{
	//初始化
	m_pExcontent = eXosip_malloc();
	if (eXosip_init(m_pExcontent) < 0)
	{
		std::cout << "sip init fail." << std::endl;
		Close();
		exit(-1);
	}
	//监听端口
	int ret;
	if (sipParam.Protocol == NetworkProtocol::TCP)
	{
		ret = eXosip_listen_addr(m_pExcontent, IPPROTO_TCP, nullptr, m_pFrom->GetAddrPort(), AF_INET, 0);
	}
	else
	{
		ret = eXosip_listen_addr(m_pExcontent, IPPROTO_UDP, nullptr, m_pFrom->GetAddrPort(), AF_INET, 0);
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
	eXosip_quit(m_pExcontent);
}

bool vsnc::sip::SIPClient::Reister() noexcept
{
	osip_message_t* reg;
	eXosip_lock(m_pExcontent);
	m_iRegisterID = eXosip_register_build_initial_register(m_pExcontent,
		m_pFrom->GetSipHeader().c_str(), m_pFrom->GetSipHeader().c_str(),
		nullptr, m_pFrom->GetExpires(), &reg);
	if (m_iRegisterID < 0)
	{
		std::cout << "eXosip_register_build_initial_register failed" << std::endl;
		eXosip_unlock(m_pExcontent);
		return false;
	}
	//增加账号验证信息
	eXosip_add_authentication_info(m_pExcontent,
		m_pSIPAuth->Username.c_str(),
		m_pSIPAuth->Username.c_str(),
		m_pSIPAuth->Response.c_str(),
		m_pSIPAuth->Algorithm.c_str(),
		m_pSIPAuth->DigestRealm.c_str());
	eXosip_register_send_register(m_pExcontent, m_iRegisterID, reg);
	eXosip_unlock(m_pExcontent);
	return true;
}

bool vsnc::sip::SIPClient::Refresh() noexcept
{
	osip_message_t* reg;
	eXosip_lock(m_pExcontent);
	m_iRegisterID = eXosip_register_build_register(m_pExcontent, m_iRegisterID, m_pFrom->GetExpires(), &reg);
	if (m_iRegisterID < 0)
	{
		std::cout << "eXosip_register_build_initial_register failed" << std::endl;
		eXosip_unlock(m_pExcontent);
		return false;
	}
	eXosip_register_send_register(m_pExcontent, m_iRegisterID, reg);
	eXosip_unlock(m_pExcontent);
	return true;
}

bool vsnc::sip::SIPClient::Logout(const std::string reason) noexcept
{
	osip_message_t* reg;
	eXosip_lock(m_pExcontent);
	m_iRegisterID = eXosip_register_build_register(m_pExcontent, m_iRegisterID, 0, &reg);
	if (m_iRegisterID < 0)
	{
		std::cout << "eXosip_register_build_initial_register failed" << std::endl;
		eXosip_unlock(m_pExcontent);
		return false;
	}
	osip_message_set_header(reg, "Logout-Reason", reason.c_str());
	eXosip_register_send_register(m_pExcontent, m_iRegisterID, reg);
	eXosip_unlock(m_pExcontent);
	return true;
}


bool vsnc::sip::SIPClient::Invite(const std::string sdp) noexcept
{
	osip_message_t* invite;
	eXosip_call_build_initial_invite(m_pExcontent,
		&invite, m_pTo->GetSipHeader().c_str(),
		m_pFrom->GetSipHeader().c_str(),
		nullptr, "This is a call for conversation");
	osip_message_set_body(invite, sdp.c_str(), sdp.size());
	osip_message_set_content_type(invite, "application/sdp");
	eXosip_lock(m_pExcontent);
	eXosip_call_send_initial_invite(m_pExcontent, invite);
	eXosip_unlock(m_pExcontent);
	return false;
}

bool vsnc::sip::SIPClient::Bye() noexcept
{
	eXosip_lock(m_pExcontent);
	if (m_iCallID > 0 && m_iDialogID > 0)
	{
		eXosip_call_terminate(m_pExcontent, m_iCallID, m_iDialogID);
		m_iCallID = -1;
		m_iDialogID = -1;
	}
	else
	{
		std::cout << "No invite" << std::endl;
	}
	eXosip_unlock(m_pExcontent);
	return false;
}

bool vsnc::sip::SIPClient::Message(const std::string context) noexcept
{
	osip_message_t* message;
	eXosip_message_build_request(m_pExcontent,
		&message,
		"MESSAGE",
		m_pTo->GetSipHeader().c_str(),
		m_pFrom->GetSipHeader().c_str(),
		nullptr);
	osip_message_set_body(message, context.c_str(), context.length());
	osip_message_set_content_type(message, "application/xml");
	eXosip_message_send_request(m_pExcontent, message);
	return true;
}

bool vsnc::sip::SIPClient::Notify(const std::string context) noexcept
{
	osip_message_t* notify;
	eXosip_message_build_request(m_pExcontent,
		&notify,
		"NOTIFY",
		m_pTo->GetSipHeader().c_str(),
		m_pFrom->GetSipHeader().c_str(),
		nullptr);
	osip_message_set_body(notify, context.c_str(), context.length());
	osip_message_set_content_type(notify, "application/xml");
	eXosip_message_send_request(m_pExcontent, notify);
	return true;
}


bool vsnc::sip::SIPClient::Subscription(const SubscriptionParam subParam) noexcept
{
	osip_message_t* subscribe;
	eXosip_subscription_build_initial_subscribe(m_pExcontent,
		&subscribe,
		m_pTo->GetSipHeader().c_str(),
		m_pFrom->GetSipHeader().c_str(),
		m_pTo->GetSipHeader().c_str(),
		subParam.Event.c_str(),
		subParam.Expires);
	osip_message_set_body(subscribe, subParam.Context.c_str(), subParam.Context.length());
	osip_message_set_content_type(subscribe, "application/xml");
	eXosip_subscription_send_initial_request(m_pExcontent, subscribe);
	return false;
}
