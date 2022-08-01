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
			int         m_sAddrPort;
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


		struct RequestResource
		{
			/// <summary>�¼�����</summary>
			std::string  EventType;
			/// <summary>�ڵ��ַ����</summary>
			std::string  Code;
			/// <summary>�������ص���ʼ��¼��</summary>
			std::string  FromIndex;
			/// <summary>�������صĽ�����¼��</summary>
			std::string  ToIndex;
		};

		typedef struct RequestHistoryAlarm
		{
			/// <summary>�¼�����</summary>
			std::string  EventType;
			/// <summary>��ѯ�澯��ַ����</summary>
			std::string  Code;
			/// <summary>�û���ַ����</summary>
			std::string  UserCode;
			/// <summary>�澯����</summary>
			std::string  Type;
			/// <summary>��ʼʱ�䣬��ʽ�磺1990-01-01T00:00:00Z</summary>
			std::string  BeginTime;
			/// <summary>����ʱ�䣬��ʽ�磺1990-01-01T00:00:00Z</summary>
			std::string  EndTime;
			/// <summary>�澯����</summary>
			std::string  Level;
			/// <summary>�������ص���ʼ��¼��</summary>
			std::string  FromIndex;
			/// <summary>�������صĽ�����¼��</summary>
			std::string  ToIndex;
		}RequestHistoryVideo;
	
		/// <summary>
		/// ��Դ�ϱ�Head
		/// </summary>
		struct ResourceHead
		{
			/// <summary>�ϱ���Դ��Push_Resource����ȡ��Դ:Response_Resource</summary>
			std::string  EventType;

			/// <summary>�ڵ��ַ����</summary>
			std::string  Code;

			/// <summary>�ϱ���Դ����ǰ�ڵ�����Ľڵ�������ȡ��Դ��ʵ�ʷ�����Դ</summary>
			int32_t      SubNum;
			/// <summary>�ϱ���Դ���ޣ���ȡ��Դ��ʵ�ʰ�����Դ</summary>
			int32_t      RealNum;
			/// <summary>�ϱ���Դ���ޣ���ȡ��Դ����ʼ�ڵ�������ʼֵΪ1</summary>
			int32_t      FromIndex;
			/// <summary>�ϱ���Դ���ޣ���ȡ��Դ�������ڵ���</summary>
			int32_t      ToIndex;
		};

		

		///XML Schema����Դ�ϱ�����������

		/// <summary>
		/// ��Դ�ϱ�Body
		/// </summary>
		struct ResourceBody
		{
			/// <summary>��ǰ�ڵ�����Ľڵ���</summary>
			int32_t       SubNum;
			/// <summary>�ڵ�����</summary>
			std::string   Name;

			/// <summary>�ڵ�״ֵ̬ 0�������ã�1������</summary>
			int32_t       Status;

			/// <summary>�����ֵ��ǩ�������ĵ��е�RTP PayLoadֵ</summary>
			int32_t       DecoderTag;

			/// <summary>����ֵ</summary>
			double        Longitude;

			/// <summary>ά��ֵ</summary>
			double        Latitude;
		};

		/// <summary>
		/// ¼�����Body
		/// </summary>
		struct HistoryVideoBody
		{
			/// <summary>¼���ļ�����</summary>
			std::string FileName;
			/// <summary>¼���ļ�URL</summary>
			std::string FileUrl;
			/// <summary>��ʼʱ�䣬��ʽ�磺1990-01-01T00:00:00Z</summary>
			std::string BeginTime;
			/// <summary>����ʱ�䣬��ʽ�磺1990-01-01T00:00:00Z</summary>
			std::string EndTime;
			/// <summary>¼���ļ���С</summary>
			int32_t     Size;
			/// <summary>�����ֵ��ǩ�������ĵ��е�RTP PayLoadֵ</summary>
			int32_t     DecoderTag;
			/// <summary>¼�����ͣ�������¼������ֵ��֧��32λ¼�����Ͷ��壬1Ϊ��Ч��0Ϊ��Ч��</summary>
			int32_t     Type;
		};

		/// <summary>
		/// ��Դ��
		/// </summary>
		struct Resource
		{
			/// <summary>��Դ��Head</summary>
			ResourceHead                Head;
			/// <summary>��Դ��Body</summary>
			std::list<ResourceBody>     Body;
		};

		/// <summary>
		/// ¼���ļ���
		/// </summary>
		struct HistoryVideo
		{
			/// <summary>¼���ļ���Head</summary>
			ResourceHead                Head;
			/// <summary>¼���ļ���Body</summary>
			std::list<HistoryVideoBody> Body;
		};

	}
}

#endif // !__VSNC_STRUCTURER_H__

