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
		/// SIP�ͻ���UAC
		/// </summary>
		class SIPClient
		{
		public:
			/// <summary>
			/// ���캯��
			/// </summary>
			/// <param name="sipParam">����</param>
			explicit SIPClient(SIPUACParam sipParam);

			/// <summary>
			///��������
			/// </summary>
			~SIPClient();

			/// <summary>
			/// �ر�
			/// </summary>
			void Close() noexcept;

			/// <summary>
			/// ע��
			/// </summary>
			/// <returns>�ɹ�����true�����󷵻�false</returns>
			bool Reister() noexcept;

			/// <summary>
			/// ����ע��
			/// </summary>
			/// <returns>�ɹ�����true�����󷵻�false</returns>
			bool Refresh() noexcept;

			/// <summary>
			/// ע��ע��
			/// </summary>
			/// <param name="reason">ע������</param>
			/// <returns>�ɹ�����true�����󷵻�false</returns>
			bool Logout(const std::string reason) noexcept;
			

			/// <summary>
			/// �������
			/// </summary>
			/// <param name="sdp">sdp</param>
			/// <returns>�ɹ�����true�����󷵻�false</returns>
			bool Invite(const std::string sdp) noexcept;


            /// <summary>
            /// �������
            /// </summary>
            /// <param name="sdp">sdp</param>
            /// <returns>�ɹ�����true�����󷵻�false</returns>
            bool InviteV2(const std::string& sdp,const SIPHeaderParam& param) noexcept;

			/// <summary>
			/// ��������
			/// </summary>
			/// <returns>�ɹ�����true�����󷵻�false</returns>
			bool Bye() noexcept;

			/// <summary>
			/// ������Ϣ
			/// </summary>
			/// <param name="context">��Ϣ���ݣ�xml��</param>
			/// <returns>�ɹ�����true�����󷵻�false</returns>
			bool Message(const std::string context) noexcept;

			/// <summary>
			/// ֪ͨ
			/// </summary>
			/// <param name="context">��Ϣ���ݣ�xml��</param>
			/// <returns>�ɹ�����true�����󷵻�false</returns>
			bool Notify(const std::string context) noexcept;
			
			/// <summary>
			/// ����
			/// </summary>
			/// <param name="subParam">���Ĳ���</param>
			/// <returns>�ɹ�����true�����󷵻�false</returns>
			bool Subscription(const SubscriptionParam subParam) noexcept;
		
			/// <summary>
			/// ���������߳�
			/// </summary>
			void StartWork();

			/// <summary>
			/// ��ȡ���յ��¼�
			/// </summary>
			/// <param name="call">�¼�ö��</param>
			/// <returns>�ɹ�����true,���󷵻�false</returns>
			bool GetCall(Call& call) noexcept;

			/// <summary>
			/// �ظ��¼�
			/// </summary>
			/// <param name="callRely">�ظ��¼�ö��</param>
			/// <returns>�ɹ�����true,���󷵻�false</returns>
			bool GetCallReply(CallReply& callRely) noexcept;

			/// <summary>
			/// �ϴ���Դ�ṹ��
			/// </summary>
			/// <returns>�ϴ���Դ�ṹ��</returns>
			ResponsePushResourceParam getResponsePushResourceParam() { return m_pPushResource; }
			
			/// <summary>
			/// ������Դ�ṹ��
			/// </summary>
			/// <returns>������Դ�ṹ��</returns>
			ResponseResponseResourceParam getResponseResponseResourceParam() { return m_pResponseResource; }
			
			/// <summary>
			/// ��ʷ�澯�ṹ��
			/// </summary>
			/// <returns>��ʷ�澯�ṹ��</returns>
			ResponseHistoryAlarmParam getResponseHistoryAlarmParam() { return m_pHistoryAlarm; }
			
			/// <summary>
			/// ��ʷ¼��ṹ��
			/// </summary>
			/// <returns>��ʷ¼��ṹ��</returns>
			ResponseHistoryVideoParam getResponseHistoryVideoParam() { return m_pHistoryVideo; }
		protected:

			/// <summary>
			/// �����߳̾��
			/// </summary>
			void serverHander();

			/// <summary>
			/// �����߳�
			/// </summary>
			/// <param name="sip">���ָ��</param>
			static void threadFun(SIPClient* sip);
		private:

			/// <summary>Ȩ��</summary>
			eXosip_t*    m_pExcontext;
			/// <summary>�ͻ�����Ϣ</summary>
			SIPHeader    m_pFrom;
			/// <summary>�������Ϣ</summary>
			SIPHeader    m_pTo;
			/// <summary>��֤��Ϣ</summary>
			SIPAuthInfo  m_pSIPAuth;
			/// <summary>ע��ID</summary>
			int          m_iRegisterID;
			/// <summary>�ỰID��cid)</summary>
			int          m_iCallID;
			/// <summary>�ỰID��did)</summary>
			int          m_iDialogID;
			/// <summary>�����߳�������־</summary>
			bool         m_bHander;
			/// <summary>�ϱ���Դ������</summary>
			ResponsePushResourceParam m_pPushResource;
			/// <summary>��ȡ��Դ��Ϣ������</summary>
			ResponseResponseResourceParam m_pResponseResource;
			/// <summary>��ʷ�澯</summary>
			ResponseHistoryAlarmParam m_pHistoryAlarm;
			/// <summary>¼���ļ�������</summary>
			ResponseHistoryVideoParam m_pHistoryVideo;
			/// <summary>�ظ�����</summary>
			Response*                 m_pResponse;
			/// <summary>�����¼�</summary>
			std::list<Call>           m_lstCall;
			/// <summary>�ظ��¼�</summary>
			std::list<CallReply>      m_lstCallReply;
			/// <summary>�߳���</summary>
			std::mutex                m_pMutex;
		};
	}
}

#endif // !__VSCM_SIP_CLIENT_H_

