#ifndef __VSNC_SIP_CLIENT_H_
#define __VSNC_SIP_CLIENT_H_
#include "structurer.h"

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
			void startWork();
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
			bool         m_bHander;
		};
	}
}

#endif // !__VSCM_SIP_CLIENT_H_

