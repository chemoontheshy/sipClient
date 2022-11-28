#include "parserResponse.h"
#include "xml/tinyxml.h"
#include "xml/tinystr.h"

bool vsnc::sip::Response::Parser(const std::string& str, const BInterfaceAction action, ResponseParam& response) noexcept
{
	// XML²Ù×÷
	TiXmlDocument* pDocument = new TiXmlDocument();
	//pDocument->LoadFile(body->body, TIXML_ENCODING_UTF8);
	pDocument->Parse(str.c_str());
	switch (action)
	{
	case vsnc::sip::BInterfaceAction::B_PUSH_RESOURCE:
	{
		auto res = reinterpret_cast<ResponsePushResourceParam*>(&response);
        res->lstResource.clear();
		auto root = pDocument->FirstChildElement("SIP_XML");
		res->EventType = root->Attribute("EventType");
		res->Code = root->FirstChildElement("Code")->GetText();
		auto subList = root->FirstChildElement("SubList");
		if (!subList) return false;
		res->SubNum = subList->Attribute("SubNum");
		auto item = subList->FirstChildElement("Item");
		for (; item != nullptr; item = item->NextSiblingElement("Item"))
		{
			ResponseResourceParam param;
			if (item)
			{
                param.Code       = item->Attribute("Code");
                param.SubNum     = item->Attribute("SubNum");
                param.Name       = item->Attribute("Name");
                param.Satuts     = item->Attribute("Status");
				param.DecoderTag = item->Attribute("DecoderTag");
                param.Longitude  = item->Attribute("Longitude");
                param.Latitude   = item->Attribute("Latitude");
				res->lstResource.push_back(param);
			}
		}
		break;
	}
	case vsnc::sip::BInterfaceAction::B_RESPONSE_RESOURCE:
	{
		auto res = reinterpret_cast<ResponseResponseResourceParam*>(&response);
        res->lstResource.clear();
		auto root = pDocument->FirstChildElement("SIP_XML");
		auto subList = root->FirstChildElement("SubList");
		if (!subList) return false;
		res->Code = subList->Attribute("Code");
		res->SubNum = subList->Attribute("SubNum");
		res->RealNum = subList->Attribute("RealNum");
		res->FromIndex = subList->Attribute("FromIndex");
		res->ToIndex = subList->Attribute("ToIndex");
		auto item = subList->FirstChildElement("Item");
		for (; item != nullptr; item = item->NextSiblingElement("Item"))
		{
			ResponseResourceParam param;
            param.Code       = item->Attribute("Code");
            param.SubNum     = item->Attribute("SubNum");
            param.Name       = item->Attribute("Name");
            param.Satuts     = item->Attribute("Status");
			param.DecoderTag = item->Attribute("DecoderTag");
            param.Longitude  = item->Attribute("Longitude");
            param.Latitude   = item->Attribute("Latitude");
			res->lstResource.push_back(param);
        }
		break;
	}
	case vsnc::sip::BInterfaceAction::B_HISTORY_ALARM:
	{
		auto res = reinterpret_cast<ResponseHistoryAlarmParam*>(&response);
        res->lstAlarm.clear();
		auto root      = pDocument->FirstChildElement("SIP_XML");
		auto subList   = root->FirstChildElement("SubList");
		if (!subList) return false;
		res->SubNum    = subList->Attribute("SubNum");
		res->RealNum   = subList->Attribute("RealNum");
		res->FromIndex = subList->Attribute("FromIndex");
		res->ToIndex   = subList->Attribute("ToIndex");
		auto item      = subList->FirstChildElement("Item");
		for (; item != nullptr; item = item->NextSiblingElement("Item"))
		{
			ResponseAlarmParam param;
			param.Code      = item->Attribute("Code");
			param.BeginTime = item->Attribute("BeginTime");
			param.Status    = item->Attribute("Status");
			param.Type      = item->Attribute("Type");
			res->lstAlarm.push_back(param);
		}
		break;
	}
	case vsnc::sip::BInterfaceAction::B_HISTORY_VIDEO:
	{
		auto res       = reinterpret_cast<ResponseHistoryVideoParam*>(&response);
        res->lstVideo.clear();
		auto root      = pDocument->FirstChildElement("SIP_XML");
		auto subList   = root->FirstChildElement("SubList");
		if (!subList) return false;
		res->SubNum    = subList->Attribute("SubNum");
		res->RealNum   = subList->Attribute("RealNum");
		res->FromIndex = subList->Attribute("FromIndex");
		res->ToIndex   = subList->Attribute("ToIndex");
		auto item = subList->FirstChildElement("Item");
		for (; item != nullptr; item = item->NextSiblingElement("Item"))
		{
			ResponseVideoParam param;
			param.FileName   = item->Attribute("FileName");
			param.FileUrl    = item->Attribute("FileUrl");
			param.BeginTime  = item->Attribute("BeginTime");
			param.EndTime    = item->Attribute("EndTime");
			param.DecoderTag = item->Attribute("DecoderTag");
			param.Size       = item->Attribute("Size");
			param.Type       = item->Attribute("Type");
			res->lstVideo.push_back(param);
		}
		break;
	}
	case vsnc::sip::BInterfaceAction::B_SUBSRIBE_ALARM:
	{
		break;
	}

	default:
		break;
	}
	return false;
}
