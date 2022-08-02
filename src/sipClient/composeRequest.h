#ifndef __VSNC_COMPOSE_REQUEST_H__
#define __VSNC_COMPOSE_REQUEST_H__

#include <iostream>
#include <list>
namespace vsnc
{
	namespace sip
	{
		enum class RequestAction
		{
			B_RESOURCE,
			B_HISTORY_ALARM,
			B_HISTORY_VIDEO,
			B_CONTROL_CAMERA,
			B_SUBSRIBE_ALARM,
			B_CAMERA_SNAP,
			B_SNAPSHOT_NOTIFY
		};


		/// <summary>
		/// ͨ�ò���
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

		struct RequestAlarmParam
		{
			std::string Code;
			std::string Type;
		};

		struct RequestSubAlarmParam : public RequestParam
		{
			std::list<RequestAlarmParam> lstAlarmParam;
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

			void SetParams(RequestParam* request, const RequestAction ation) noexcept;

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



