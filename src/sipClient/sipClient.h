#ifndef __VSNC_SIP_CLIENT_H_
#define __VSNC_SIP_CLIENT_H_
#include "structurer.h"

struct eXosip_t;

namespace vsnc
{
	namespace sip
	{
		/// <summary>
		/// SIP客户端UAC
		/// </summary>
		class SIPClient
		{
		public:
			/// <summary>
			/// 构造函数
			/// </summary>
			/// <param name="sipParam">参数</param>
			explicit SIPClient(SIPUACParam sipParam);

			/// <summary>
			///析构函数
			/// </summary>
			~SIPClient();

			/// <summary>
			/// 关闭
			/// </summary>
			void Close() noexcept;

			/// <summary>
			/// 注册
			/// </summary>
			/// <returns>成功返回true，错误返回false</returns>
			bool Reister() noexcept;

			/// <summary>
			/// 更新注册
			/// </summary>
			/// <returns>成功返回true，错误返回false</returns>
			bool Refresh() noexcept;

			/// <summary>
			/// 注销注册
			/// </summary>
			/// <param name="reason">注销理由</param>
			/// <returns>成功返回true，错误返回false</returns>
			bool Logout(const std::string reason) noexcept;
			

			/// <summary>
			/// 邀请会议
			/// </summary>
			/// <param name="sdp">sdp</param>
			/// <returns>成功返回true，错误返回false</returns>
			bool Invite(const std::string sdp) noexcept;

			/// <summary>
			/// 结束会议
			/// </summary>
			/// <returns>成功返回true，错误返回false</returns>
			bool Bye() noexcept;

			/// <summary>
			/// 发送消息
			/// </summary>
			/// <param name="context">消息内容（xml）</param>
			/// <returns>成功返回true，错误返回false</returns>
			bool Message(const std::string context) noexcept;

			/// <summary>
			/// 通知
			/// </summary>
			/// <param name="context">消息内容（xml）</param>
			/// <returns>成功返回true，错误返回false</returns>
			bool Notify(const std::string context) noexcept;
			
			/// <summary>
			/// 订阅
			/// </summary>
			/// <param name="subParam">订阅参数</param>
			/// <returns>成功返回true，错误返回false</returns>
			bool Subscription(const SubscriptionParam subParam) noexcept;
		
			/// <summary>
			/// 启动接收线程
			/// </summary>
			void startWork();
		protected:
			/// <summary>
			/// 接收线程句柄
			/// </summary>
			void serverHander();

			/// <summary>
			/// 接收线程
			/// </summary>
			/// <param name="sip">类的指针</param>
			static void threadFun(SIPClient* sip);
		private:

			/// <summary>权柄</summary>
			eXosip_t*    m_pExcontext;
			/// <summary>客户端信息</summary>
			SIPHeader    m_pFrom;
			/// <summary>服务端信息</summary>
			SIPHeader    m_pTo;
			/// <summary>验证信息</summary>
			SIPAuthInfo  m_pSIPAuth;
			/// <summary>注册ID</summary>
			int          m_iRegisterID;
			/// <summary>会话ID（cid)</summary>
			int          m_iCallID;
			/// <summary>会话ID（did)</summary>
			int          m_iDialogID;
			bool         m_bHander;
		};
	}
}

#endif // !__VSCM_SIP_CLIENT_H_

