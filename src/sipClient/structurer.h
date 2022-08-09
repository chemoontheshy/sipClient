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
		/// <summary>���Ե�ַ����</summary>
		const std::string ADDRCODE = "100110000103520001";
		
		/// <summary>UAS��ֵ����֤�����</summary>
		const std::string NONCE = "9bd055";
		
		/// <summary> USAĬ�ϼ����㷨</summary>
		const std::string ALGORITHIMH = "MD5";

		/// <summary>
		/// B�ӿ�Э��ӿ�ö��
		/// </summary>
		enum class BInterfaceAction
		{
			/// <summary>ע��</summary>
			B_REGISTER,
			/// <summary>��Դ�ϱ�</summary>
			B_PUSH_RESOURCE,
			/// <summary>��Դ��Ϣ��ȡ</summary>
			B_RESPONSE_RESOURCE,
			/// <summary>��ʷ�澯��ѯ</summary>
			B_HISTORY_ALARM,
			/// <summary>¼�����</summary>
			B_HISTORY_VIDEO,
			/// <summary>����ʵʱ��Ƶ</summary>
			B_PLAY_VIDEO,
			/// <summary>�����Խ��͹㲥</summary>
			B_TALK_AUDIO,
			/// <summary>�ƾ�����</summary>
			B_CONTROL_CAMERA,
			/// <summary>¼��ط�</summary>
			B_PLAY_BACK_VIDEO,
			/// <summary>�¼����ļ�֪ͨ</summary>
			B_SUBSRIBE_ALARM,
			/// <summary>ͼƬץ��</summary>
			B_CAMERA_SNAP,
			/// <summary>ץ��ͼƬ�����ϴ�</summary>
			B_UPLOAD_SNAP,
			/// <summary>ͼ�������ϱ�</summary>
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
			//�ظ�
			REPLY_ACK,
			REPLY_200,
			REPLY_401
		};

		/// <summary>
		/// UAC����
		/// </summary>
		enum class UACCMD
		{
			/// <summary> ע��</summary>
			REGISTER,
			/// <summary> ˢ��ע��</summary>
			REFRESH,
			/// <summary> ע��ע��</summary>
			LOGOUT,
			/// <summary> ����</summary>
			INVITE,
			/// <summary> ����</summary>
			BYE,
			/// <summary> ��Ϣ</summary>
			MESSAGE,
			/// <summary> ��Դ�ϱ�</summary>
			NOTIFY,
			/// <summary> ����</summary>
			SUBSRIBE
		};

		/// <summary>
		/// ͨ�Ŵ���Э��
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
			/// ���캯��
			/// </summary>
			/// <param name="addrCode">��ַ����</param>
			/// <param name="addrIp">����IP��ַ</param>
			/// <param name="addrPort">�˿�</param>
			SIPHeader() = default;
			
			/// <summary>
			/// ��������
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
			/// ��ȡSIPHeader eg:sip:100010111@127.0.0.1:5061
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
			/// ��ȡ��ַ����
			/// </summary>
			/// <returns>��ַ����</returns>
			std::string GetAddrCode() const noexcept { return m_sAddrCode; }
			
			/// <summary>
			/// ��ȡ��������IP��ַ
			/// </summary>
			/// <returns>��������IP��ַ</returns>
			std::string GetAddrIp()   const noexcept { return m_sAddrIp; }
			
		    /// <summary>
		    /// ��ȡ�˿�
		    /// </summary>
		    /// <returns>�˿�</returns>
		    int         GetAddrPort() const noexcept { return m_sAddrPort; }

			/// <summary>
			/// ��ȡ��ʱʱ��
			/// </summary>
			/// <returns>��ʱʱ��</returns>
			int         GetExpires()  const noexcept { return m_iExpires; }
		private:
			/// <summary>��ַ����</summary>
			std::string m_sAddrCode;
			/// <summary>������IP��ַ</summary>
			std::string m_sAddrIp;
			/// <summary>�˿�</summary>
			int         m_sAddrPort = 0;
			/// <summary>��ʱʱ�䣬Contract��200������˲���</summary>
			int         m_iExpires = 0;
		};


		/// <summary>
		/// SIP��Ϣ
		/// </summary>
		struct SIPContextInfo
		{
			/// <summary>Sip�㷵�ص�����ı�־����Ӧʱ���ؼ���</summary>
			int             SipRequestId;
			/// <summary>ά��һ��ע��</summary>
			std::string     CallId;
			/// <summary>��Ϣ�����Ĺ��ܷ������ַ���</summary>
			std::string     Method;
			/// <summary>��ַ����@������IP��ַ:���Ӷ˿ڣ�����sip:1111@127.0.0.1:5060</summary>
			SIPHeader       From;
			/// <summary>��ַ����@������IP��ַ:���Ӷ˿ڣ�����sip:1111@127.0.0.1:5060</summary>
			SIPHeader       Proxy;
			/// <summary>��ַ����@������IP��ַ:���Ӷ˿ڣ�����sip:1111@127.0.0.1:5060</summary>
			SIPHeader       Contact;
			/// <summary>��Ϣ���ݣ�һ��ΪDDCP��Ϣ��XML�ĵ������߾���Э��֡Ҫ��������ַ����ı�</summary>
			std::string     Content;
			/// <summary>��Ӧ״̬��Ϣ</summary>
			std::string     Status;
			/// <summary>��ʱ��ʱ�䵥λΪ��</summary>
			int             Expires;

		};

		struct SIPAuthInfo
		{
			/// <summary>ƽ̨������</summary>
			std::string DigestRealm;
			/// <summary>ƽ̨�ṩ�������</summary>
			std::string Nonce;
			/// <summary>�û���</summary>
			std::string Username;
			/// <summary>����</summary>
			std::string Response;
			/// <summary>"sip:ƽ̨��ַ"������Ҫuac��ֵ</summary>
			std::string Uri;
			/// <summary>�����㷨MD5</summary>
			std::string Algorithm;
		};

		/// <summary>
		/// SIP�ͻ��˲���
		/// </summary>
		struct SIPUACParam
		{
			/// <summary>�ͻ���</summary>
			SIPHeader      From;
			/// <summary>�����</summary>
			SIPHeader      To;
			/// <summary>��֤��Ϣ</summary>
			SIPAuthInfo    SIPAuth;
			/// <summary>ͨ�Ŵ���Э��</summary>
			NetworkProtocol Protocol;
		};

		/// <summary>
		/// ������Ϣ
		/// </summary>
		struct SubscriptionParam
		{
			/// <summary>����</summary>
			std::string     Context;
			/// <summary>�����¼����ͣ��澯�¼���Alarm ״̬�¼� presence</summary>
			std::string     Event;
			/// <summary>��֤��Ϣ</summary>
			int             Expires;
		};
		
		/// <summary>
		/// SIPע����Ϣ
		/// </summary>
		struct SIPRegisterInfo
		{
			/// <summary>
			/// ������Ϣ
			/// </summary>
			SIPContextInfo BaseInfo;

			/// <summary>
			/// ע����֤У����Ϣ
			/// </summary>
			SIPAuthInfo    AuthInfo;

			/// <summary>
			/// �Ƿ������֤��Ϣ
			/// </summary>
			bool           IsAuthNull;
		};


	}
}

#endif // !__VSNC_STRUCTURER_H__

