#ifndef __VSNC_STRUCTURER_H__
#define __VSNC_STRUCTURER_H__
#include <string>
#include <iostream>
#include <sstream>
namespace vsnc
{
	namespace sip
	{
		/// <summary>测试地址编码</summary>
		const std::string ADDRCODE = "100110000103520001";
		
		/// <summary>UAS赋值的认证随机数</summary>
		const std::string NONCE = "9bd055";
		
		/// <summary> USA默认加密算法</summary>
		const std::string ALGORITHIMH = "MD5";

		/// <summary>
		/// UAC命令
		/// </summary>
		enum class UACCMD
		{
			/// <summary> 注册</summary>
			REGISTER,
			/// <summary> 刷新注册</summary>
			REFRESH,
			/// <summary> 注销注册</summary>
			LOGOUT,
			/// <summary> 邀请</summary>
			INVITE,
			/// <summary> 结束</summary>
			BYE,
			/// <summary> 消息</summary>
			MESSAGE,
			/// <summary> 资源上报</summary>
			NOTIFY,
			/// <summary> 订阅</summary>
			SUBSRIBE
		};

		/// <summary>
		/// 通信传输协议
		/// </summary>
		enum class NetworkProtocol
		{
			/// <summary> UDP</summary>
			UDP,
			/// <summary> TCP</summary>
			TCP
		};

		/// <summary>
		/// SIPHeader
		/// </summary>
		class SIPHeader
		{
		public:
			/// <summary>
			/// 构造函数
			/// </summary>
			/// <param name="addrCode">地址编码</param>
			/// <param name="addrIp">域名IP地址</param>
			/// <param name="addrPort">端口</param>
			SIPHeader(const std::string addrCode,const std::string addrIp,
				const int addrPort,const int expires) :
				m_sAddrCode(addrCode),
				m_sAddrIp(addrIp),
				m_sAddrPort(addrPort),
				m_iExpires(expires) {};
			
			/// <summary>
			/// 析构函数
			/// </summary>
			~SIPHeader() = default;
			
			
			/// <summary>
			/// 获取SIPHeader eg:sip:100010111@127.0.0.1:5061
			/// </summary>
			/// <returns>SIPHeader</returns>
			std::string GetSipHeader() const noexcept
			{
				std::stringstream stream;
				stream << "sip:" << m_sAddrCode << "@" << m_sAddrIp << ":" << m_sAddrPort;
				return stream.str();
			}

			/// <summary>
			/// 获取地址编码
			/// </summary>
			/// <returns>地址编码</returns>
			std::string GetAddrCode() const noexcept { return m_sAddrCode; }
			
			/// <summary>
			/// 获取域名或者IP地址
			/// </summary>
			/// <returns>域名或者IP地址</returns>
			std::string GetAddrIp()   const noexcept { return m_sAddrIp; }
			
		    /// <summary>
		    /// 获取端口
		    /// </summary>
		    /// <returns>端口</returns>
		    int         GetAddrPort() const noexcept { return m_sAddrPort; }

			/// <summary>
			/// 获取超时时间
			/// </summary>
			/// <returns>超时时间</returns>
			int         GetExpires()  const noexcept { return m_iExpires; }
		private:
			/// <summary>地址编码</summary>
			std::string m_sAddrCode;
			/// <summary>域名或IP地址</summary>
			std::string m_sAddrIp;
			/// <summary>端口</summary>
			int         m_sAddrPort;
			/// <summary>超时时间，Contract的200必须带此参数</summary>
			int         m_iExpires;
		};


		/// <summary>
		/// SIP消息
		/// </summary>
		struct SIPContextInfo
		{
			/// <summary>Sip层返回的请求的标志，响应时返回即可</summary>
			int             SipRequestId;
			/// <summary>维护一次注册</summary>
			std::string     CallId;
			/// <summary>消息所属的功能方法名字符串</summary>
			std::string     Method;
			/// <summary>地址编码@域名或IP地址:连接端口，例如sip:1111@127.0.0.1:5060</summary>
			SIPHeader       From;
			/// <summary>地址编码@域名或IP地址:连接端口，例如sip:1111@127.0.0.1:5060</summary>
			SIPHeader       Proxy;
			/// <summary>地址编码@域名或IP地址:连接端口，例如sip:1111@127.0.0.1:5060</summary>
			SIPHeader       Contact;
			/// <summary>消息内容，一般为DDCP消息体XML文档，或者具体协议帧要求的其他字符串文本</summary>
			std::string     Content;
			/// <summary>响应状态信息</summary>
			std::string     Status;
			/// <summary>超时，时间单位为秒</summary>
			int             Expires;

		};

		struct SIPAuthInfo
		{
			/// <summary>平台主机名</summary>
			std::string DigestRealm;
			/// <summary>平台提供的随机数</summary>
			std::string Nonce;
			/// <summary>用户名</summary>
			std::string Username;
			/// <summary>密码</summary>
			std::string Response;
			/// <summary>"sip:平台地址"，不需要uac赋值</summary>
			std::string Uri;
			/// <summary>加密算法MD5</summary>
			std::string Algorithm;
		};

		/// <summary>
		/// SIP客户端参数
		/// </summary>
		struct SIPUACParam
		{
			/// <summary>客户端</summary>
			SIPHeader*      From;
			/// <summary>服务端</summary>
			SIPHeader*      To;
			/// <summary>验证信息</summary>
			SIPAuthInfo*    SIPAuth;
			/// <summary>通信传输协议</summary>
			NetworkProtocol Protocol;
		};

		/// <summary>
		/// 订阅消息
		/// </summary>
		struct SubscriptionParam
		{
			/// <summary>内容</summary>
			std::string     Context;
			/// <summary>订阅事件类型：告警事件：Alarm 状态事件 presence</summary>
			std::string     Event;
			/// <summary>验证信息</summary>
			int             Expires;
		};

	}
}

#endif // !__VSNC_STRUCTURER_H__

