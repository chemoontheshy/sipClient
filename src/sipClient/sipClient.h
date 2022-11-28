#ifndef __VSNC_SIP_CLIENT_H_
#define __VSNC_SIP_CLIENT_H_
#include "parserResponse.h"
#include <thread>
#include <mutex>

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
            /// 邀请会议
            /// </summary>
            /// <param name="sdp">sdp</param>
            /// <returns>成功返回true，错误返回false</returns>
            bool InviteV2(const std::string& sdp,const SIPHeaderParam& param) noexcept;

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
			void StartWork();

			/// <summary>
			/// 获取接收到事件
			/// </summary>
			/// <param name="call">事件枚举</param>
			/// <returns>成功返回true,错误返回false</returns>
			bool GetCall(Call& call) noexcept;

			/// <summary>
			/// 回复事件
			/// </summary>
			/// <param name="callRely">回复事件枚举</param>
			/// <returns>成功返回true,错误返回false</returns>
			bool GetCallReply(CallReply& callRely) noexcept;

			/// <summary>
			/// 上传资源结构体
			/// </summary>
			/// <returns>上传资源结构体</returns>
			ResponsePushResourceParam getResponsePushResourceParam() { return m_pPushResource; }
			
			/// <summary>
			/// 请求资源结构体
			/// </summary>
			/// <returns>请求资源结构体</returns>
			ResponseResponseResourceParam getResponseResponseResourceParam() { return m_pResponseResource; }
			
			/// <summary>
			/// 历史告警结构体
			/// </summary>
			/// <returns>历史告警结构体</returns>
			ResponseHistoryAlarmParam getResponseHistoryAlarmParam() { return m_pHistoryAlarm; }
			
			/// <summary>
			/// 历史录像结构体
			/// </summary>
			/// <returns>历史录像结构体</returns>
			ResponseHistoryVideoParam getResponseHistoryVideoParam() { return m_pHistoryVideo; }
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
			/// <summary>接收线程启动标志</summary>
			bool         m_bHander;
			/// <summary>上报资源的数据</summary>
			ResponsePushResourceParam m_pPushResource;
			/// <summary>获取资源信息的数据</summary>
			ResponseResponseResourceParam m_pResponseResource;
			/// <summary>历史告警</summary>
			ResponseHistoryAlarmParam m_pHistoryAlarm;
			/// <summary>录像文件的数据</summary>
			ResponseHistoryVideoParam m_pHistoryVideo;
			/// <summary>回复解析</summary>
			Response*                 m_pResponse;
			/// <summary>接收事件</summary>
			std::list<Call>           m_lstCall;
			/// <summary>回复事件</summary>
			std::list<CallReply>      m_lstCallReply;
			/// <summary>线程锁</summary>
			std::mutex                m_pMutex;
		};
	}
}

#endif // !__VSCM_SIP_CLIENT_H_

