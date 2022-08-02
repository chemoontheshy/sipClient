#ifndef __VSNC_PARSER_RESPONESE_H__
#define __VSNC_PARSER_RESPONESE_H__

#include "structurer.h"

namespace vsnc
{
	namespace sip
	{
		/// <summary>
		/// Response通用参数
		/// </summary>
		struct ResponseParam
		{
			/// <summary>事件类型</summary>
			std::string EventType;
			/// <summary>编码</summary>
			std::string Code;
			/// <summary>总数</summary>
			std::string SubNum;
		};

		/// <summary>
		/// 资源Item
		/// </summary>
		struct ResponseResourceParam
		{
			/// <summary>摄像头编号</summary>
			std::string Code;
			/// <summary>摄像头名称</summary>
			std::string Name;
			/// <summary>节点状态值：0不可用，1可用</summary>
			std::string Satuts;
			/// <summary>解码插值标签，参照文档中的RTP PayLoad值</summary>
			std::string DecoderTag;
			/// <summary>摄像头名称</summary>
			std::string Longitude;
			/// <summary>摄像头名称</summary>
			std::string Latitude;
			/// <summary>摄像头名称</summary>
			std::string SubNum;
		};

		/// <summary>
		/// 上报资源包
		/// </summary>
		struct ResponsePushResourceParam : public ResponseParam
		{
			/// <summary>摄像头名称</summary>
			std::list<ResponseResourceParam> lstResource;
		};

		/// <summary>
		/// 资源信息获取包
		/// </summary>
		struct ResponseResponseResourceParam : public ResponseParam
		{
			/// <summary>实际包含节点数</summary>
			std::string RealNum;
			/// <summary>起始节点数</summary>
			std::string FromIndex;
			/// <summary>结束节点数</summary>
			std::string ToIndex;
			/// <summary>检索结果</summary>
			std::list<ResponseResourceParam> lstResource;
		};

		/// <summary>
		/// 告警Item
		/// </summary>
		struct ResponseAlarmParam
		{
			/// <summary>摄像头编码地址</summary>
			std::string Code;
			/// <summary>时间开始时间</summary>
			std::string BeginTime;
			/// <summary>告警状态</summary>
			std::string Status;
			/// <summary>类型</summary>
			std::string Type;
		};

		/// <summary>
		/// 历史告警
		/// </summary>
		struct ResponseHistoryAlarmParam : public ResponseParam
		{
			/// <summary>实际包含节点数</summary>
			std::string RealNum;
			/// <summary>起始节点数</summary>
			std::string FromIndex;
			/// <summary>结束节点数</summary>
			std::string ToIndex;
			/// <summary>检索结果</summary>
			std::list<ResponseAlarmParam> lstAlarm;
		};

		/// <summary>
		/// 录像Item
		/// </summary>
		struct ResponseVideoParam
		{
			/// <summary>文件名</summary>
			std::string FileName;
			/// <summary>文件地址</summary>
			std::string FileUrl;
			/// <summary>时间开始时间</summary>
			std::string BeginTime;
			/// <summary>实际结束时间</summary>
			std::string EndTime;
			/// <summary>文件大小</summary>
			std::string Size;
			/// <summary>解码标签</summary>
			std::string DecoderTag;
			/// <summary>类型值</summary>
			std::string Type;
		};
		/// <summary>
		/// 录像检索
		/// </summary>
		struct ResponseHistoryVideoParam : public ResponseParam
		{
			/// <summary>实际的记录数</summary>
			std::string RealNum;
			/// <summary>期望返回的起始记录数</summary>
			std::string FromIndex;
			/// <summary>期望返回的结束后果</summary>
			std::string ToIndex;
			/// <summary>检索结果</summary>
			std::list<ResponseVideoParam> lstVideo;
		};


		/// <summary>
		/// 订阅Item
		/// </summary>
		struct ResponseStatusParam
		{
			/// <summary>摄像头编码</summary>
			std::string Code;
			/// <summary>摄像头名称</summary>
			std::string Name;
			/// <summary>活动状态</summary>
			std::string Status;
		};

		/// <summary>
		/// 订阅后的通知
		/// </summary>
		struct ResponseNotifyStatusParam
		{
			/// <summary>通知状态</summary>
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

