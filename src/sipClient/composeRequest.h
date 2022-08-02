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
		/// 通用参数
		/// </summary>
		struct RequestParam
		{
			/// <summary>事件类型</summary>
			std::string  EventType;
			/// <summary>编码</summary>
			std::string  Code;
		};

		/// <summary>
		/// 资源获取
		/// </summary>
		struct RequestResourceParam : RequestParam
		{
			/// <summary>期望返回的起始记录数</summary>
			std::string  FromIndex;
			/// <summary>期望返回的结束记录数</summary>
			std::string  ToIndex;
		};

		/// <summary>
		/// 历史告警查询和录像检索
		/// </summary>
		struct RequestHistoryParam : public RequestParam
		{
			/// <summary>用户地址编码</summary>
			std::string  UserCode;
			/// <summary>告警类型</summary>
			std::string  Type;
			/// <summary>开始时间，格式如：1990-01-01T00:00:00Z</summary>
			std::string  BeginTime;
			/// <summary>结束时间，格式如：1990-01-01T00:00:00Z</summary>
			std::string  EndTime;
			/// <summary>告警级别</summary>
			std::string  Level;
			/// <summary>期望返回的起始记录数</summary>
			std::string  FromIndex;
			/// <summary>期望返回的结束记录数</summary>
			std::string  ToIndex;
		};

		struct RequestControl : public RequestParam
		{
			/// <summary>云镜控制动作</summary>
			std::string  Command;
			/// <summary>横向速度:取值范围[1,9]</summary>
			std::string  CommandPara1;
			/// <summary>纵向速度:取值范围[1,9]</summary>
			std::string  CommandPara2;
			/// <summary>保留值</summary>
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
		/// 标准构建接口
		/// </summary>
		class Request
		{
		public:
			/// <summary>
			/// 析构函数
			/// </summary>
			~Request() = default;

			void SetParams(RequestParam* request, const RequestAction ation) noexcept;

			/// <summary>
			/// 获取打包好的XML字符串
			/// </summary>
			/// <param name="str">输入:空的字符串，输出：XML字符串</param>
			/// <returns>成功返回true,错误返回false</returns>
			bool GetString(std::string& str) noexcept;

		private:
			/// <summary>
			/// 返回
			/// </summary>
			std::string m_sXML;
		};

	}
}

#endif // !__VSNC_COMPOSE_REQUEST_H__



