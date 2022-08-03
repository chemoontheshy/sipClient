#ifndef __VSNC_COMPOSE_REQUEST_H__
#define __VSNC_COMPOSE_REQUEST_H__

#include "structurer.h"

namespace vsnc
{
	namespace sip
	{
		/// <summary>
		/// Requestͨ�ò���
		/// </summary>
		struct RequestParam
		{
			/// <summary>�¼�����</summary>
			std::string  EventType;
			/// <summary>����</summary>
			std::string  Code;
		};

		/// <summary>
		/// ��Դ��ȡ
		/// </summary>
		struct RequestResourceParam : RequestParam
		{
			/// <summary>�������ص���ʼ��¼��</summary>
			std::string  FromIndex;
			/// <summary>�������صĽ�����¼��</summary>
			std::string  ToIndex;
		};

		/// <summary>
		/// ��ʷ�澯��ѯ��¼�����
		/// </summary>
		struct RequestHistoryParam : public RequestParam
		{
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
		};

		struct RequestControl : public RequestParam
		{
			/// <summary>�ƾ����ƶ���</summary>
			std::string  Command;
			/// <summary>�����ٶ�:ȡֵ��Χ[1,9]</summary>
			std::string  CommandPara1;
			/// <summary>�����ٶ�:ȡֵ��Χ[1,9]</summary>
			std::string  CommandPara2;
			/// <summary>����ֵ</summary>
			std::string  CommandPara3;
		};

		/// <summary>
		/// ����Item����
		/// </summary>
		struct RequestAlarmParam
		{
			/// <summary>����ͷ����</summary>
			std::string Code;
			/// <summary>����</summary>
			std::string Type;
		};

		/// <summary>
		/// ������Ϣ
		/// </summary>
		struct RequestSubAlarmParam : public RequestParam
		{
			/// <summary>Item�б�</summary>
			std::list<RequestAlarmParam> lstAlarmParam;
		};

		/// <summary>
		/// ͼƬץ��
		/// </summary>
		struct RequestCameraSnapParam : public RequestParam
		{
			/// <summary>ͼƬ�ϴ���ַ</summary>
			std::string PicServer;
			/// <summary>ץ������</summary>
			std::string SnapType;
			/// <summary>ʱ�䷶Χ</summary>
			std::string Range;
			/// <summary>ץ�ļ��</summary>
			std::string Interval;
		};

		struct RequestSnapshotParam
		{
			/// <summary>����ͷ����</summary>
			std::string Code;
			/// <summary>����</summary>
			std::string Type;
			/// <summary>ץ��ʱ�䣬eg��20220802T130204Z</summary>
			std::string Time;
			/// <summary>ץ��ͼƬ�����ص�ַ</summary>
			std::string FileUrl;
			/// <summary>�ļ���С</summary>
			std::string FileSize;
			/// <summary>SH265sum</summary>
			std::string Verfiy;
		};

		struct RequestSnapshotNotifyParam : public RequestParam
		{
			/// <summary>Item�б�</summary>
			std::list<RequestSnapshotParam> lstSnapshotParam;
		};
		/// <summary>
		/// ��׼�����ӿ�
		/// </summary>
		class Request
		{
		public:
			/// <summary>
			/// ��������
			/// </summary>
			~Request() = default;

			void SetParams(RequestParam* request, const BInterfaceAction ation) noexcept;

			/// <summary>
			/// ��ȡ����õ�XML�ַ���
			/// </summary>
			/// <param name="str">����:�յ��ַ����������XML�ַ���</param>
			/// <returns>�ɹ�����true,���󷵻�false</returns>
			bool GetString(std::string& str) noexcept;

		private:
			/// <summary>
			/// ����
			/// </summary>
			std::string m_sXML;
		};

	}
}

#endif // !__VSNC_COMPOSE_REQUEST_H__



