

#include "stdafx.h"
#include "ICustomVideoSrc.h"

namespace mray
{
namespace video
{

	void ICustomVideoSrc::SetEncoderType(const std::string &type)
	{
		m_encoder = type;
		m_encoderParams.clear();

		if (type == "H264")
		{
			m_encoderParams["speed-preset"] = "superfast";
			m_encoderParams["tune"] = "zerolatency";
			m_encoderParams["pass"] = "cbr";
			m_encoderParams["sliced-threads"] = "true";
			m_encoderParams["sync-lookahead"] = "0";
			m_encoderParams["rc-lookahead"] = "0";
			m_encoderParams["quantizer"] = "15";
		}
		else if (type == "VP8")
		{
			m_encoderParams["end-usage"] = "cbr";
			m_encoderParams["threads"] = "4";
		}
	}

	std::string ICustomVideoSrc::BuildStringH264(int i)
{
	std::string videoStr = "";


	// 			videoStr += " ! videoconvert ! autovideosink sync=false ";
	// 			return videoStr;

	videoStr += " ! videoconvert ! x264enc bitrate=" + core::StringConverter::toString(m_bitRate / GetStreamsCount()) + " ";

	/*
	" speed-preset=superfast  tune=zerolatency pass=cbr sliced-threads=true"//" key-int-max=5"
	" sync-lookahead=0 rc-lookahead=0"
	" psy-tune=none "//interlaced=true sliced-threads=false  "//
	" quantizer=15 ";*/

	//fill in parameters
	std::map<std::string, std::string>::iterator it = m_encoderParams.begin();
	for (; it != m_encoderParams.end(); ++it)
		videoStr += it->first + "=" + it->second + " ";

	if(false)
		videoStr += " ! mylistener name=encoderlistener" + core::StringConverter::toString(i);
	
	videoStr += " ! rtph264pay mtu=" + core::StringConverter::toString(m_mtuSize) + " ";

	//videoStr += "! autovideosink";

	return videoStr;
}

	std::string ICustomVideoSrc::BuildStringVP8(int i)
{
	std::string videoStr="";

	videoStr += "! vp8enc  target-bitrate=" + core::StringConverter::toString((1000 * m_bitRate) / GetStreamsCount()) + " ";

	//fill in parameters
	std::map<std::string, std::string>::iterator it = m_encoderParams.begin();
	for (; it != m_encoderParams.end(); ++it)
		videoStr += it->first + "=" + it->second + " ";

	//	" keyframe-mode=1 keyframe-max-dist=1 threads=4 "// ip-factor=1.8 interlaced=true sliced-threads=false  "// 

	videoStr += " ! rtpvp8pay mtu=" + core::StringConverter::toString(m_mtuSize) + " ";
	/*
	//videoStr += " ! vp8enc ! rtpvp8pay ";
	videoStr += " ! theoraenc ! rtptheorapay ";*/

	//videoStr += " ! autovideosink sync=false ";

	return videoStr;

}
void ICustomVideoSrc::LoadParameters(xml::XMLElement* elem)
{
	if (!elem)
		return;
	xml::XMLAttribute* attr;
	xml::XMLElement* e;
	attr = elem->getAttribute("Type");
	if (attr)
	{
		SetEncoderType(attr->value);
	}
	attr = elem->getAttribute("SeparateStreams");
	if (attr)
		SetSeparateStreams(core::StringConverter::toBool(attr->value));
	attr = elem->getAttribute("mtu");
	if (attr)
		m_mtuSize=core::StringConverter::toInt(attr->value);

	e = elem->getSubElement("Param");
	while (e)
	{
		m_encoderParams[e->getAttribute("Name")->value] = e->getAttribute("Value")->value;
		e = e->nextSiblingElement("Param");
	}
}
}
}