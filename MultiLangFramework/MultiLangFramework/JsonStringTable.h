#pragma once
#include "rapidjson/document.h"

using namespace rapidjson;

typedef GenericDocument<UTF16<> > WDocument;

class CJsonStringTable
{
public:
	CJsonStringTable();
	virtual ~CJsonStringTable();

public:
	BOOL Load();
	CString GetString(const CString &strKey);

private:
	BOOL m_IsbLoad;
	WDocument m_JsonDocument;
};

