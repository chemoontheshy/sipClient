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
		/// B接口协议接口枚举
		/// </summary>
		enum class BInterfaceAction
		{
			/// <summary>注册</summary>
			B_REGISTER,
			/// <summary>资源上报</summary>
			B_PUSH_RESOURCE,
			/// <summary>资源信息获取</summary>
			B_RESPONSE_RESOURCE,
			/// <summary>历史告警查询</summary>
			B_HISTORY_ALARM,
			/// <summary>录像检索</summary>
			B_HISTORY_VIDEO,
			/// <summary>调阅实时视频</summary>
			B_PLAY_VIDEO,
			/// <summary>语音对讲和广播</summary>
			B_TALK_AUDIO,
			/// <summary>云镜控制</summary>
			B_CONTROL_CAMERA,
			/// <summary>录像回访</summary>
			B_PLAY_BACK_VIDEO,
			/// <summary>事件订阅及通知</summary>
			B_SUBSRIBE_ALARM,
			/// <summary>图片抓拍</summary>
			B_CAMERA_SNAP,
			/// <summary>抓拍图片数据上传</summary>
			B_UPLOAD_SNAP,
			/// <summary>图像数据上报</summary>
			B_SNAPSHOT_NOTIFY
		};

		enum class Call
		{
			NO_CALL_NOW,
			REGISTRATION_SUCCESS,
			REGISTRATION_FAILURE,
			MSG_IS_MESSAGE,
			MSG_IS_NOTIFY,
			MSG_IS_REGISTER,
			CALL_RINGING,
			CALL_PROCEEDING,
			CALL_MESSAGE_ANSWERED,
			CALL_ANSWERED,
			CALL_CLOSED,
			CALL_ACK,
			CALL_RELEASED,
			MESSAGE_ANSWERED,
			SUBSCRIPTION_ANSWERED,
			MESSAGE_REQUESTFAILURE,
			SUBSCRIPTION_NOTIFY,
			OTHER_RESPONSE
		};
		enum class CallReply
		{
			REPLY_NONE,
			//回复
			REPLY_ACK,
			REPLY_200,
			REPLY_401
		};

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
			int         m_sAddrPort = 0;
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


	}
}

#endif // !__VSNC_STRUCTURER_H__

