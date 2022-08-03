#ifndef __VSNC_PARSER_RESPONESE_H__
#define __VSNC_PARSER_RESPONESE_H__

#include "structurer.h"

namespace vsnc
{
	namespace sip
	{
		/// <summary>
		/// Responseͨ�ò���
		/// </summary>
		struct ResponseParam
		{
			/// <summary>�¼�����</summary>
			std::string EventType;
			/// <summary>����</summary>
			std::string Code;
			/// <summary>����</summary>
			std::string SubNum;
		};

		/// <summary>
		/// ��ԴItem
		/// </summary>
		struct ResponseResourceParam
		{
			/// <summary>����ͷ���</summary>
			std::string Code;
			/// <summary>����ͷ����</summary>
			std::string Name;
			/// <summary>�ڵ�״ֵ̬��0�����ã�1����</summary>
			std::string Satuts;
			/// <summary>�����ֵ��ǩ�������ĵ��е�RTP PayLoadֵ</summary>
			std::string DecoderTag;
			/// <summary>����ͷ����</summary>
			std::string Longitude;
			/// <summary>����ͷ����</summary>
			std::string Latitude;
			/// <summary>����ͷ����</summary>
			std::string SubNum;
		};

		/// <summary>
		/// �ϱ���Դ��
		/// </summary>
		struct ResponsePushResourceParam : public ResponseParam
		{
			/// <summary>����ͷ����</summary>
			std::list<ResponseResourceParam> lstResource;
		};

		/// <summary>
		/// ��Դ��Ϣ��ȡ��
		/// </summary>
		struct ResponseResponseResourceParam : public ResponseParam
		{
			/// <summary>ʵ�ʰ����ڵ���</summary>
			std::string RealNum;
			/// <summary>��ʼ�ڵ���</summary>
			std::string FromIndex;
			/// <summary>�����ڵ���</summary>
			std::string ToIndex;
			/// <summary>�������</summary>
			std::list<ResponseResourceParam> lstResource;
		};

		/// <summary>
		/// �澯Item
		/// </summary>
		struct ResponseAlarmParam
		{
			/// <summary>����ͷ�����ַ</summary>
			std::string Code;
			/// <summary>ʱ�俪ʼʱ��</summary>
			std::string BeginTime;
			/// <summary>�澯״̬</summary>
			std::string Status;
			/// <summary>����</summary>
			std::string Type;
		};

		/// <summary>
		/// ��ʷ�澯
		/// </summary>
		struct ResponseHistoryAlarmParam : public ResponseParam
		{
			/// <summary>ʵ�ʰ����ڵ���</summary>
			std::string RealNum;
			/// <summary>��ʼ�ڵ���</summary>
			std::string FromIndex;
			/// <summary>�����ڵ���</summary>
			std::string ToIndex;
			/// <summary>�������</summary>
			std::list<ResponseAlarmParam> lstAlarm;
		};

		/// <summary>
		/// ¼��Item
		/// </summary>
		struct ResponseVideoParam
		{
			/// <summary>�ļ���</summary>
			std::string FileName;
			/// <summary>�ļ���ַ</summary>
			std::string FileUrl;
			/// <summary>ʱ�俪ʼʱ��</summary>
			std::string BeginTime;
			/// <summary>ʵ�ʽ���ʱ��</summary>
			std::string EndTime;
			/// <summary>�ļ���С</summary>
			std::string Size;
			/// <summary>�����ǩ</summary>
			std::string DecoderTag;
			/// <summary>����ֵ</summary>
			std::string Type;
		};
		/// <summary>
		/// ¼�����
		/// </summary>
		struct ResponseHistoryVideoParam : public ResponseParam
		{
			/// <summary>ʵ�ʵļ�¼��</summary>
			std::string RealNum;
			/// <summary>�������ص���ʼ��¼��</summary>
			std::string FromIndex;
			/// <summary>�������صĽ������</summary>
			std::string ToIndex;
			/// <summary>�������</summary>
			std::list<ResponseVideoParam> lstVideo;
		};


		/// <summary>
		/// ����Item
		/// </summary>
		struct ResponseStatusParam
		{
			/// <summary>����ͷ����</summary>
			std::string Code;
			/// <summary>����ͷ����</summary>
			std::string Name;
			/// <summary>�״̬</summary>
			std::string Status;
		};

		/// <summary>
		/// ���ĺ��֪ͨ
		/// </summary>
		struct ResponseNotifyStatusParam
		{
			/// <summary>֪ͨ״̬</summary>
			std::list<ResponseStatusParam> lstStatus;
		};

		class Response
		{
		public:
			~Response() = default;
			bool Parser(const std::string& str, const BInterfaceAction action, ResponseParam& response) noexcept;
		};
	}

	
}


#endif // !__VSNC_PARSER_RESPONESE_H__

