#include "composeRequest.h"
#include "xml/tinyxml.h"
#include "xml/tinystr.h"

#include <string>

using vsnc::sip::Request;

namespace vsnc
{
	namespace sip
	{
		
	}
}

void Request::SetParams(RequestParam* request, const RequestAction action) noexcept
{
	// ����һ��XML
	TiXmlDocument* xmlDoc = new TiXmlDocument();
	// ͷ����Ϣ xml������(�������ԣ��汾�������ʽ�������մ�����) 
	TiXmlDeclaration* xmlDeclare = new TiXmlDeclaration("1.0", "utf-8", "");	// ����ͷ����ʽ
	xmlDoc->LinkEndChild(xmlDeclare);
	// ���ڵ�
	// ����ʱ��Ҫָ�����ڵ������
	TiXmlElement* root = new TiXmlElement("SIP_XML");
	// �ӽڵ�
	TiXmlElement* item = new TiXmlElement("Item");
	switch (action)
	{
	case vsnc::sip::RequestAction::B_RESOURCE:
	{
		auto req = reinterpret_cast<RequestResourceParam*>(request);
		// �¼�����
		root->SetAttribute("EventType", req->EventType.c_str());
		// ����
		item->SetAttribute("Code", req->Code.c_str());
		item->SetAttribute("FromIndex", req->FromIndex.c_str());
		item->SetAttribute("ToIndex", req->ToIndex.c_str());
		break;
	}
	case vsnc::sip::RequestAction::B_HISTORY_ALARM:
	{
		auto req = reinterpret_cast<RequestHistoryParam*>(request);
		// �¼�����
		root->SetAttribute("EventType", req->EventType.c_str());
		// ����
		item->SetAttribute("Code", req->Code.c_str());
		item->SetAttribute("UserCode", req->UserCode.c_str());
		item->SetAttribute("Type", req->Type.c_str());
		item->SetAttribute("BeginTime", req->BeginTime.c_str());
		item->SetAttribute("EndTime", req->EndTime.c_str());
		item->SetAttribute("Level", req->Level.c_str());
		item->SetAttribute("FromIndex", req->FromIndex.c_str());
		item->SetAttribute("ToIndex", req->ToIndex.c_str());
		break;
	}
	case vsnc::sip::RequestAction::B_HISTORY_VIDEO:
	{
		auto req = reinterpret_cast<RequestHistoryParam*>(request);
		// �¼�����
		root->SetAttribute("EventType", req->EventType.c_str());
		// ����
		item->SetAttribute("Code", req->Code.c_str());
		item->SetAttribute("UserCode", req->UserCode.c_str());
		item->SetAttribute("Type", req->Type.c_str());
		item->SetAttribute("BeginTime", req->BeginTime.c_str());
		item->SetAttribute("EndTime", req->EndTime.c_str());
		item->SetAttribute("Level", req->Level.c_str());
		item->SetAttribute("FromIndex", req->FromIndex.c_str());
		item->SetAttribute("ToIndex", req->ToIndex.c_str());
		break;
	}
	case vsnc::sip::RequestAction::B_CONTROL_CAMERA:
	{
		auto req = reinterpret_cast<RequestControl*>(request);
		// �¼�����
		root->SetAttribute("EventType", req->EventType.c_str());
		// ����
		item->SetAttribute("Code", req->Code.c_str());
		auto iCmd = std::stoi(req->Command, nullptr, 16);
		item->SetAttribute("Command",std::to_string(iCmd).c_str());
		item->SetAttribute("CommandPara1", req->CommandPara1.c_str());
		item->SetAttribute("CommandPara2", req->CommandPara2.c_str());
		item->SetAttribute("CommandPara3", req->CommandPara3.c_str());
		break;
	}
		
	case vsnc::sip::RequestAction::B_SUBSRIBE_ALARM:
	{
		auto req = reinterpret_cast<RequestSubAlarmParam*>(request);
		// �¼�����
		root->SetAttribute("EventType", req->EventType.c_str());
		for (const auto& alarmParam : req->lstAlarmParam)
		{
			TiXmlElement* temp = new TiXmlElement("Item");
			temp->SetAttribute("Code", alarmParam.Code.c_str());
			temp->SetAttribute("Type", alarmParam.Type.c_str());
			root->LinkEndChild(temp);
		}
		break;
	}
	case vsnc::sip::RequestAction::B_CAMERA_SNAP:
		break;
	case vsnc::sip::RequestAction::B_SNAPSHOT_NOTIFY:
		break;
	default:
		break;
	}
	if (action != vsnc::sip::RequestAction::B_SUBSRIBE_ALARM)
	{
		root->LinkEndChild(item);
	}
	xmlDoc->LinkEndChild(root);		// �Ѹ��ڵ���뵽�ĵ�����
	TiXmlPrinter printer;
	xmlDoc->Accept(&printer);
	m_sXML = printer.CStr();
}
bool Request::GetString(std::string& str) noexcept
{
	if (m_sXML.empty()) return false;
	str = m_sXML;
	m_sXML.clear();
	return true;
}
