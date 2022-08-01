#include "sipClient.h"
#include "eXosip2/eXosip.h"
#include "osip2/osip_mt.h"
#include <winsock2.h>
#include <windows.h>
#include <thread>
#include <sstream>
#include "xml/tinyxml.h"
#include "xml/tinystr.h"

namespace vsnc
{
	namespace sip
	{
		/// <summary>
		/// 注册处理
		/// </summary>
		/// <param name="excontext">sip句柄</param>
		/// <param name="osipEvent">事件句柄</param>
		static void __on_register(eXosip_t* excontext, eXosip_event_t* osipEvent , const SIPAuthInfo& authInfo);
		
		/// <summary>
		/// 解析注册信息
		/// </summary>
		/// <param name="request">注册总信息</param>
		/// <param name="reqId">注册ID</param>
		/// <param name="regInfo">注册信息，填入一个空的，获取相关信息</param>
		static void __parser_register_info(osip_message_t* request, const int reqId, SIPRegisterInfo& regInfo);
		
		/// <summary>
		/// 打印信息
		/// </summary>
		/// <param name="info">注册信息</param>
		static void __print_register_info(const SIPRegisterInfo& info);

		/// <summary>
		/// 发送注册回复
		/// </summary>
		/// <param name="excontext">sip句柄</param>
		/// <param name="info">注册信息</param>
		static void __send_register_answer(eXosip_t* excontext, const SIPRegisterInfo& info);
	}
}

void vsnc::sip::__on_register(eXosip_t* excontext, eXosip_event_t* osipEvent ,const SIPAuthInfo& authInfo)
{
	SIPRegisterInfo regInfo;
	// 解析注册报文
	__parser_register_info(osipEvent->request,osipEvent->tid, regInfo);


	// 打印解析后的注册报文
	__print_register_info(regInfo);
	// 回复响应
	__send_register_answer(excontext, regInfo);
}

void vsnc::sip::__parser_register_info(osip_message_t* request, const int reqId, SIPRegisterInfo& regInfo)
{
	// 方法名
	regInfo.BaseInfo.Method = request->sip_method;
	// From
	regInfo.BaseInfo.From.SetSIPHeader(request->from->url->username,
		request->from->url->host,std::stoi(request->from->url->port), 0);
	// Proxy
	regInfo.BaseInfo.Proxy.SetSIPHeader(request->to->url->username,
		request->to->url->host, std::stoi(request->to->url->port), 0);

	// 获取expires
	osip_header_t* header = nullptr;
	{
		osip_message_header_get_byname(request, "expires",0,&header);
		if (header && header->hvalue)
		{
			regInfo.BaseInfo.Expires = std::stoi(header->hvalue);
		}
	}
	// 获取contact字段
	osip_contact_t* contact = nullptr;
	osip_message_get_contact(request, 0, &contact);
	if (contact)
	{
		regInfo.BaseInfo.Contact.SetSIPHeader(contact->url->username,
			contact->url->host, std::stoi(contact->url->port), regInfo.BaseInfo.Expires);
	}
	// 注册返回 ，由发送方维护的请求ID 接收方接收后原样返回即可
	regInfo.BaseInfo.SipRequestId = reqId;

	// CallId
	regInfo.BaseInfo.CallId = request->call_id->number;

	// 解析context消息
	osip_body_t* body = nullptr;
	osip_message_get_body(request, 0, &body);
	if (body)
	{
		regInfo.BaseInfo.Content = body->body;
	}

	// 鉴权信息
	osip_authorization_t* authentication = nullptr;
	{
		osip_message_get_authorization(request, 0, &authentication);
		if (!authentication)
		{
			regInfo.IsAuthNull = true;
		}
		else
		{

			regInfo.IsAuthNull = false;
			// 用户名
			regInfo.AuthInfo.Username = authentication->username;
			// 加密方法名
			regInfo.AuthInfo.Algorithm = authentication->algorithm;
			// 主机名
			if (authentication->realm)
			{
				regInfo.AuthInfo.DigestRealm = authentication->realm;
			}
			// 随机数
			regInfo.AuthInfo.Nonce = authentication->nonce;
			// 密码
			regInfo.AuthInfo.Response = authentication->response;
			// 平台地址
			regInfo.AuthInfo.Uri = authentication->uri;

			
			

		}
	}
	authentication = nullptr;
}

void vsnc::sip::__print_register_info(const SIPRegisterInfo& info)
{
}

void vsnc::sip::__send_register_answer(eXosip_t* excontext, const SIPRegisterInfo& info)
{
	osip_message_t* answer = nullptr;
	int iStauts;
	(info.IsAuthNull) ? iStauts = 401 : iStauts = 200;
	eXosip_lock(excontext);
	{
		int ret = eXosip_message_build_answer(excontext, info.BaseInfo.SipRequestId, iStauts,&answer);
	
		if (iStauts == 401)
		{
			std::string strAnswer;
			strAnswer = "Digest realm=\"" + info.BaseInfo.From.GetAddrIp() +
				"\",nonce=\"" + NONCE +"\",algorithm=" + ALGORITHIMH;
			osip_message_set_header(answer, "WWW-Authenticate", strAnswer.c_str());
			std::cout << "status code 401" << std::endl;
		}
		else if (iStauts == 200)
		{
			osip_message_set_header(answer, "Contact", info.BaseInfo.Contact.GetContact().c_str());

		}
		if (OSIP_SUCCESS != ret)
		{
			std::cout << "error" << std::endl;

			auto sendRet = eXosip_message_send_answer(excontext, info.BaseInfo.SipRequestId, 400, nullptr);
			std::cout << sendRet << std::endl;
		}
		else
		{
			eXosip_message_send_answer(excontext, info.BaseInfo.SipRequestId, 200, answer);
		}
		if (0 == info.BaseInfo.Expires)
		{
			eXosip_register_remove(excontext, info.BaseInfo.Expires);
		}
	}
	eXosip_unlock(excontext);
}
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
		ret = eXosip_listen_addr(m_pExcontext, IPPROTO_TCP, nullptr, m_pFrom.GetAddrPort(), AF_INET, 0);
	}
	else
	{
		ret = eXosip_listen_addr(m_pExcontext, IPPROTO_UDP, nullptr, m_pFrom.GetAddrPort(), AF_INET, 0);
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
		m_pFrom.GetSipHeader().c_str(), m_pTo.GetSipHeader().c_str(),
		nullptr, m_pFrom.GetExpires(), &reg);

	if (m_iRegisterID < 0)
	{
		std::cout << "eXosip_register_build_initial_register failed" << std::endl;
		eXosip_unlock(m_pExcontext);
		return false;
	}
	//增加账号验证信息
	auto ret = eXosip_add_authentication_info(m_pExcontext,
		m_pSIPAuth.Username.c_str(),
		m_pSIPAuth.Username.c_str(),
		m_pSIPAuth.Response.c_str(),
		m_pSIPAuth.Algorithm.c_str(),
		m_pSIPAuth.DigestRealm.c_str());
	//eXosip_add_authentication_info(m_pExcontext, "hskj", "hskj", "12456", "MD5", nullptr);
	eXosip_register_send_register(m_pExcontext, m_iRegisterID, reg);

	eXosip_unlock(m_pExcontext);
	return true;
}

bool vsnc::sip::SIPClient::Refresh() noexcept
{
	osip_message_t* reg;
	eXosip_lock(m_pExcontext);
	m_iRegisterID = eXosip_register_build_register(m_pExcontext, m_iRegisterID, m_pFrom.GetExpires(), &reg);
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
		&invite, m_pTo.GetSipHeader().c_str(),
		m_pFrom.GetSipHeader().c_str(),
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
		m_pTo.GetSipHeader().c_str(),
		m_pFrom.GetSipHeader().c_str(),
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
		m_pTo.GetSipHeader().c_str(),
		m_pFrom.GetSipHeader().c_str(),
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
		m_pTo.GetSipHeader().c_str(),
		m_pFrom.GetSipHeader().c_str(),
		m_pTo.GetSipHeader().c_str(),
		subParam.Event.c_str(),
		subParam.Expires);
	osip_message_set_body(subscribe, subParam.Context.c_str(), subParam.Context.length());
	osip_message_set_content_type(subscribe, "application/xml");
	eXosip_subscription_send_initial_request(m_pExcontext, subscribe);
	return true;
}


void vsnc::sip::SIPClient::serverHander()
{
	size_t num = 0;
	while (m_bHander)
	{
		auto je = eXosip_event_wait(m_pExcontext, 0, 1000);
		//一般处理401/407采用库默认处理
		eXosip_lock(m_pExcontext);
		eXosip_automatic_action(m_pExcontext);
		eXosip_unlock(m_pExcontext);

		if (!je)
		{
			//std::cout << "No response or the time is over num=" << num++ << std::endl;
			continue;
		}
		
		switch (je->type)
		{
		case EXOSIP_REGISTRATION_SUCCESS:
		{
			std::cout << "success: status_code" << je->request->status_code << std::endl;
			break;//注册成功
		}
		case EXOSIP_REGISTRATION_FAILURE:
		{
			std::cout << "fail: status_code" << je->response->status_code << std::endl;
			break;//注册失败
		}
		case EXOSIP_MESSAGE_NEW:
		{
			//消息
			if (MSG_IS_MESSAGE(je->request))
			{
				// 打印接收到信息
				{
					osip_body_t* body;
					osip_message_get_body(je->request, 0, &body);
					std::cout << "I get the msg is : " << body->body << std::endl;
				}
				//按照规则，需要回复OK信息
				osip_message_t* answer;
				eXosip_message_build_answer(m_pExcontext, je->tid, 200, &answer);
				eXosip_message_send_answer(m_pExcontext, je->tid, 200, answer);
			}
			else if (MSG_IS_NOTIFY(je->request))
			{
				std::cout << "NOTIFY" << std::endl;
				// 打印接收到信息
				{
					osip_body_t* body;
					osip_message_get_body(je->request, 0, &body);
					std::cout << "----------" << std::endl;
					std::cout  << body->body << std::endl;
					// XML操作
					TiXmlDocument* pDocument = new TiXmlDocument();
					//pDocument->LoadFile(body->body, TIXML_ENCODING_UTF8);
					pDocument->Parse(body->body);
					auto pDeclar = pDocument->FirstChild()->ToDeclaration();
					
					// 版本
					if (pDeclar)
					{
						std::stringstream stream;
						stream << "version: " << pDeclar->Version() << ";" << "encoding: " << pDeclar->Encoding();
						std::cout << stream.str() << std::endl;
					}
					
					// 根节点
					auto root = pDocument->FirstChildElement("SIP_XML");
					if (root)
					{
						m_pPassiveResoure.Head.EventType = root->Attribute("EventType");
						m_pPassiveResoure.Head.Code = root->FirstChildElement("Code")->GetText();
						auto subList = root->FirstChildElement("SubList");
						if (subList)
						{
							m_pPassiveResoure.Head.SubNum = std::stoi(subList->Attribute("SubNum"));
							auto item = subList->FirstChildElement("Item");
							for (; item != nullptr; item = item->NextSiblingElement("Item"))
							{
								SetConsoleOutputCP(CP_UTF8);
								ResourceBody body;
								body.SubNum = std::stoi(item->Attribute("SubNum"));
								body.Name = item->Attribute("Name");
								body.Status = std::stoi(item->Attribute("Status"));
								body.DecoderTag = std::stoi(item->Attribute("DecoderTag"));
								std::string longitude = item->Attribute("Longitude");
								std::string latitude = item->Attribute("Latitude");
								(longitude.empty()) ? body.Longitude = 0 : body.Longitude = std::stoi(longitude);
								(latitude.empty()) ? body.Latitude = 0 : body.Latitude = std::stoi(latitude);
								m_pPassiveResoure.Body.push_back(body);
								std::cout << body.Name << std::endl;
							}
						}
					}
					
				}

				osip_message_t* notify = nullptr;
				eXosip_message_build_answer(m_pExcontext, je->tid, 200, &notify);
				eXosip_message_send_answer(m_pExcontext, je->tid, 200, notify);
			}
			else if (MSG_IS_REGISTER(je->request))
			{
				std::cout << "register" << std::endl;
				//返回注册信息
				//OnRegister(excontext, pSipEvent);
				__on_register(m_pExcontext, je,m_pSIPAuth);
				std::cout << "register ok" << std::endl;
			}
			// 消息
			break;
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
			osip_body_t* body;
			osip_message_get_body(je->response, 0, &body);
			std::cout << "I get the msg is : " << body->body << std::endl;
			// XML操作
			TiXmlDocument* pDocument = new TiXmlDocument();
			pDocument->Parse(body->body);
			auto pDeclar = pDocument->FirstChild()->ToDeclaration();

			// 根节点
			auto root = pDocument->FirstChildElement("SIP_XML");
			if (root)
			{
				m_pActiveResoure.Head.EventType = root->Attribute("EventType");
				auto subList = root->FirstChildElement("SubList");
				if (subList)
				{
					m_pActiveResoure.Head.Code = subList->Attribute("Code");
					m_pActiveResoure.Head.SubNum = std::stoi(subList->Attribute("SubNum"));
					m_pActiveResoure.Head.RealNum = std::stoi(subList->Attribute("RealNum"));
					m_pActiveResoure.Head.FromIndex = std::stoi(subList->Attribute("FromIndex"));
					m_pActiveResoure.Head.ToIndex = std::stoi(subList->Attribute("ToIndex"));
					auto item = subList->FirstChildElement("Item");
					for (; item != nullptr; item = item->NextSiblingElement("Item"))
					{
						SetConsoleOutputCP(CP_UTF8);
						ResourceBody body;
						body.SubNum = std::stoi(item->Attribute("SubNum"));
						body.Name = item->Attribute("Name");
						body.Status = std::stoi(item->Attribute("Status"));
						body.DecoderTag = std::stoi(item->Attribute("DecoderTag"));
						std::string longitude = item->Attribute("Longitude");
						std::string latitude = item->Attribute("Latitude");
						(longitude.empty()) ? body.Longitude = 0 : body.Longitude = std::stoi(longitude);
						(latitude.empty()) ? body.Latitude = 0 : body.Latitude = std::stoi(latitude);
						m_pActiveResoure.Body.push_back(body);
						std::cout << body.Name << std::endl;
					}
				}
			}

			break;//消息应答
		}
		case EXOSIP_SUBSCRIPTION_ANSWERED:
		{
			std::cout << "SUBSCRIPTION received" << std::endl;
			break;//消息应答
		}
		case EXOSIP_CALL_RELEASED:
		{
			std::cout << "EXOSIP_CALL_RELEASED" << std::endl;
			break;
		}
		case EXOSIP_MESSAGE_REQUESTFAILURE:
		{
			std::cout << "MESSAGE Failed" << std::endl;
			break;
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

void vsnc::sip::SIPClient::StartWork()
{
	std::thread workThread(threadFun, this);
	workThread.detach();
}