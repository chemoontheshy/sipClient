#ifndef __VSNC_STRUCTURER_H__
#define __VSNC_STRUCTURER_H__
#include <string>
#include <iostream>
#include <sstream>
#include <list>

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
			SIPHeader() = default;
			
			/// <summary>
			/// 析构函数
			/// </summary>
			~SIPHeader() = default;
			
			void SetSIPHeader(const std::string addrCode, const std::string addrIp,
				const int addrPort, const int expires) noexcept
			{
				m_sAddrCode = addrCode;
				m_sAddrIp = addrIp;
				m_sAddrPort = addrPort;
				m_iExpires = expires;

			}

			/// <summary>
			/// 获取SIPHeader eg:sip:100010111@127.0.0.1:5061
			/// </summary>
			/// <returns>SIPHeader</returns>
			std::string GetSipHeader() const noexcept
			{
				std::stringstream stream;
				stream << "<sip:" << m_sAddrCode << "@" << m_sAddrIp << ":" << m_sAddrPort<<">";
				return stream.str();
			}

			std::string GetContact() const noexcept
			{
				std::string sTmp = GetSipHeader();
				std::stringstream stream;
				stream << ";" << "expires=" << m_iExpires;
				sTmp += stream.str();
				return sTmp;
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
			int         m_iExpires = 0;
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
			SIPHeader      From;
			/// <summary>服务端</summary>
			SIPHeader      To;
			/// <summary>验证信息</summary>
			SIPAuthInfo    SIPAuth;
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
		
		/// <summary>
		/// SIP注册信息
		/// </summary>
		struct SIPRegisterInfo
		{
			/// <summary>
			/// 基本信息
			/// </summary>
			SIPContextInfo BaseInfo;

			/// <summary>
			/// 注册验证校验信息
			/// </summary>
			SIPAuthInfo    AuthInfo;

			/// <summary>
			/// 是否带有验证信息
			/// </summary>
			bool           IsAuthNull;
		};


		struct RequestResource
		{
			/// <summary>事件类型</summary>
			std::string  EventType;
			/// <summary>节点地址编码</summary>
			std::string  Code;
			/// <summary>期望返回的起始记录数</summary>
			std::string  FromIndex;
			/// <summary>期望返回的结束记录数</summary>
			std::string  ToIndex;
		};

		typedef struct RequestHistoryAlarm
		{
			/// <summary>事件类型</summary>
			std::string  EventType;
			/// <summary>查询告警地址编码</summary>
			std::string  Code;
			/// <summary>用户地址编码</summary>
			std::string  UserCode;
			/// <summary>告警类型</summary>
			std::string  Type;
			/// <summary>开始时间，格式如：1990-01-01T00:00:00Z</summary>
			std::string  BeginTime;
			/// <summary>结束时间，格式如：1990-01-01T00:00:00Z</summary>
			std::string  EndTime;
			/// <summary>告警级别</summary>
			std::string  Level;
			/// <summary>期望返回的起始记录数</summary>
			std::string  FromIndex;
			/// <summary>期望返回的结束记录数</summary>
			std::string  ToIndex;
		}RequestHistoryVideo;
	
		/// <summary>
		/// 资源上报Head
		/// </summary>
		struct ResourceHead
		{
			/// <summary>上报资源：Push_Resource，获取资源:Response_Resource</summary>
			std::string  EventType;

			/// <summary>节点地址编码</summary>
			std::string  Code;

			/// <summary>上报资源：当前节点包含的节点数，获取资源：实际返回资源</summary>
			int32_t      SubNum;
			/// <summary>上报资源：无，获取资源：实际包含资源</summary>
			int32_t      RealNum;
			/// <summary>上报资源：无，获取资源：起始节点数，起始值为1</summary>
			int32_t      FromIndex;
			/// <summary>上报资源：无，获取资源：结束节点数</summary>
			int32_t      ToIndex;
		};

		

		///XML Schema，资源上报。参数定义

		/// <summary>
		/// 资源上报Body
		/// </summary>
		struct ResourceBody
		{
			/// <summary>当前节点包含的节点数</summary>
			int32_t       SubNum;
			/// <summary>节点名称</summary>
			std::string   Name;

			/// <summary>节点状态值 0：不可用，1：可用</summary>
			int32_t       Status;

			/// <summary>解码插值标签，参照文档中的RTP PayLoad值</summary>
			int32_t       DecoderTag;

			/// <summary>经度值</summary>
			double        Longitude;

			/// <summary>维度值</summary>
			double        Latitude;
		};

		/// <summary>
		/// 录像检索Body
		/// </summary>
		struct HistoryVideoBody
		{
			/// <summary>录像文件名称</summary>
			std::string FileName;
			/// <summary>录像文件URL</summary>
			std::string FileUrl;
			/// <summary>开始时间，格式如：1990-01-01T00:00:00Z</summary>
			std::string BeginTime;
			/// <summary>结束时间，格式如：1990-01-01T00:00:00Z</summary>
			std::string EndTime;
			/// <summary>录像文件大小</summary>
			int32_t     Size;
			/// <summary>解码插值标签，参照文档中的RTP PayLoad值</summary>
			int32_t     DecoderTag;
			/// <summary>录像类型，按定义录像类型值，支持32位录像类型定义，1为有效，0为无效。</summary>
			int32_t     Type;
		};

		/// <summary>
		/// 资源包
		/// </summary>
		struct Resource
		{
			/// <summary>资源包Head</summary>
			ResourceHead                Head;
			/// <summary>资源包Body</summary>
			std::list<ResourceBody>     Body;
		};

		/// <summary>
		/// 录像文件包
		/// </summary>
		struct HistoryVideo
		{
			/// <summary>录像文件包Head</summary>
			ResourceHead                Head;
			/// <summary>录像文件包Body</summary>
			std::list<HistoryVideoBody> Body;
		};

	}
}

#endif // !__VSNC_STRUCTURER_H__

