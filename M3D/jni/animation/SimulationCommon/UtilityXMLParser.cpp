// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file		UtilityXMLParser.cpp
*
*	@brief	XML解析相关代码
*
*
*	@par	历史:
*
****************************************************************************/

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "UtilityXMLParser.h"
#include "UtilityTextParser.h"
#include "vhash.h"
#include "vlist.h"


#define STATUS_DELIMITER_NOT_FOUND 0
#define STATUS_TAG_FINISHED 1
#define STATUS_TAG_NOT_FINISHED 2
#define STATUS_SPECIAL_TAG_SKIPPED 3
#define STATUS_SPECIAL_TAG_NOT_SKIPPED 4
#define STATUS_NO_SUBTAGS 5
#define STATUS_SUBTAGS_FOUND 6
#define UINT2bool(__uint__val)  (__uint__val > 0) ?  true: false

//SA_NAMESPACE_BEGIN


static delimiter_t find_tagname_td[] = {{"\x21-\x7E|", true, "/|>| |", true, false}};
static delimiter_t skip_tag_td[] = {{"<", true, ">", true, true}};
static delimiter_t find_properites_td[] = {{">", true, 0, true, true},
											{"/", true, 0, true, true},
											{"\x21-\x7E|", false, " |=", true, false}};
static	delimiter_t find_properites_value_td[] = {{"\"", true, "\"", true, false},
													{"\x21-\x7E|", false," |>|/|", true, false}};
	
static	delimiter_t find_equal_td[] = {{"=", true, 0, true, true},
										{"\x21-\x7E|", true, 0, true, true}};
static	delimiter_t check_for_start_tag_td[] = {{"\x21-\x7E|", false, 0, true, false}};	
static	delimiter_t find_item_value_td[] = { {"<", false,0, true, false},
												{"\"", false, "\"", false, false},
												{"[", false,"]", false, true},
												{"\"", false,"\"", true, false},
												{"\x21-\x7E|", false," |<|\n|\x09|", true, false}};
static delimiter_t find_doctype_td[] = {{"\"", true, "\"", true, false},
										{"\x21-\x7E|", false, " |>", true, false},
										{"[", false, 0, true, false}};
static delimiter_t skip_bracket_td[] = {{"[", true, "]", true, true}};

 
static	int dlum;
static	char result[SA_BUFFER_SIZE_BIG];	
 
static void delete_property_struct(
	void * property_struct,
	const char * property_name,
	void * user_data)
{
	struct vhash_s * m_pPropertyHash = (struct vhash_s *) user_data;
	CUtilityXMLProperty * pstruct = (CUtilityXMLProperty *) property_struct;

	delete(pstruct);
	vhash_remove_string_key(m_pPropertyHash, property_name);
}



static void delete_tagcallback_struct(
	void * tagcallback_struct,
	const char * tag_name,
	void * user_data)
{
	struct vhash_s * tagcallback_hash = (struct vhash_s *) user_data;
	CUtilityXMLTagCallback * pstruct = (CUtilityXMLTagCallback *) tagcallback_struct;

	delete(pstruct);
	vhash_remove_string_key(tagcallback_hash, tag_name);
}


CUtilityXMLTag::CUtilityXMLTag(CUtilityXMLParser *xmlparser)
{
	m_pPropertyHash = 0;
	m_pItemList = 0;
	m_pTagName = 0;
	m_pPropertyList = 0;
	m_pCommentList = 0;
	m_bOpen = false;
	m_pXMLParser = xmlparser;
	Init();
}

CUtilityXMLTag::~CUtilityXMLTag()
{
	FreeMemory();
}

void CUtilityXMLTag::Init()
{
	FreeMemory();
	m_pPropertyHash = new_vhash(1, malloc, free);
	m_pItemList = new_vlist(malloc, free);
	m_pCommentList = new_vlist(malloc, free);
}

void CUtilityXMLTag::AddComment(char *comment)
{
    char * text = new char[strlen(comment)+1];
    strcpy(text, comment);
    vlist_add_last(m_pCommentList, text);
}
void CUtilityXMLTag::CleanupPropertyList()
{
	if (m_pPropertyList)
	{
		vhash_string_key_pair_t *temp;

		while ((temp = (vhash_string_key_pair_t *)vlist_remove_first(m_pPropertyList)) != 0) {
			//this one was allocated by malloc via function pointer passed to 
                        //vhash_string_keys_to_vlist, so it needs to be matched with free, not delete
                        free( temp ); 
		}
		delete_vlist(m_pPropertyList);
		m_pPropertyList = 0;
	}
}


void CUtilityXMLTag::FreeMemory()
{
	if (m_pPropertyHash)
	{
		vhash_string_key_map_function(m_pPropertyHash, delete_property_struct,(void*)m_pPropertyHash);
		delete_vhash(m_pPropertyHash);
		m_pPropertyHash = 0;
	}
	
	CleanupPropertyList();

	if (m_pItemList)
	{
		char *temp;

		while ((temp = (char *)vlist_remove_first(m_pItemList)) != 0) {
			delete [] temp;
		}
		delete_vlist(m_pItemList);
		m_pItemList = 0;
	}
	if (m_pTagName)
	{
		delete [] m_pTagName;
		m_pTagName = 0;
	}
	if (m_pCommentList)
	{
	    START_LIST_ITERATION(char, m_pCommentList);
		delete [] temp;
	    END_LIST_ITERATION(m_pCommentList);
	    delete_vlist(m_pCommentList);
	    m_pCommentList = 0;
	}

}
 
void CUtilityXMLTag::SetTagname(const char *tagname)
{

	if (m_pTagName)
		delete [] m_pTagName;
	m_pTagName = new char[strlen(tagname) + 1];
	strcpy(m_pTagName, tagname);
}


bool CUtilityXMLTag::AddProperty(const char *name, const char *text, bool hasQuotes)
{
	CUtilityXMLProperty *preturn = 0;
	CUtilityXMLProperty *ptype = new CUtilityXMLProperty;
	ptype->m_pText = new char[strlen(text) + 1];
	strcpy(ptype->m_pText, text);
	ptype->m_HasQuotes = hasQuotes;

	if (vhash_replace_string_key_item(m_pPropertyHash, name, (void *)ptype, (void **)&preturn) == VHASH_STATUS_SUCCESS)
	{
		if (preturn)
		{
			delete preturn;
			return false;
		}
	}
	return true;

}

 
bool CUtilityXMLTag::GetProperty(const char *name, CUtilityXMLProperty **prop)
{
	CUtilityXMLProperty *preturn = 0;
  	if (vhash_lookup_string_key_item(m_pPropertyHash, name , (void **)&preturn) == VHASH_STATUS_SUCCESS)
	{
		*prop = preturn;
		return true;
	}
	return false;

}

bool CUtilityXMLTag::GetProperty(const char *name, char *pcont)
{
	CUtilityXMLProperty *p;
	if (GetProperty(name, &p))
	{
		strcpy(pcont, p->m_pText);
		return true;
	}
	else
		return false; 
}

bool CUtilityXMLTag::GetProperty(const char *name, int &ii)
{
	CUtilityXMLProperty *p;
	if (GetProperty(name, &p))
	{
 		ii = atoi(p->m_pText);
		return true;
	}
	else
		return false;

 
}

bool CUtilityXMLTag::GetProperty(const char *name, bool &ii)
{
	CUtilityXMLProperty *p;
	if (GetProperty(name, &p))
	{
 		ii = UINT2bool(atoi(p->m_pText));
		return true;
	}
	else
		return false;

 
}




void CUtilityXMLTag::AddListItem(const char *text)
{
	char *itemtext = new char[strlen(text) + 1];
	strcpy(itemtext, text);
	vlist_add_last(m_pItemList, (void *)itemtext);
}

void CUtilityXMLTag::StartListItemsQuery()
{
	vlist_reset_cursor(m_pItemList);
}

void CUtilityXMLTag::CleanListItems()
{
    START_LIST_ITERATION(char, m_pItemList)
	delete [] temp;
    END_LIST_ITERATION(m_pItemList);
    vlist_flush(m_pItemList);

}
bool CUtilityXMLTag::GetNextListItem(char **text)
{

	void *temp;
  	if (!(temp = vlist_peek_cursor(m_pItemList)))
				return false;
	*text = (char *)temp;
 	vlist_advance_cursor(m_pItemList);	
	return true;
}

bool CUtilityXMLTag::HasMoreItems()
{
   	if (!vlist_peek_cursor(m_pItemList))
				return false;
	else
		return true;
}


bool CUtilityXMLTag::HasListItems()
{
	vlist_reset_cursor(m_pItemList);
	if (!vlist_peek_cursor(m_pItemList))
		return false;
	else 
		return true;
}



void CUtilityXMLTag::StartPropertyQuery()
{
	CleanupPropertyList();
 
	m_pPropertyList = new_vlist(malloc, free);
 
 	vhash_string_keys_to_vlist(m_pPropertyHash, m_pPropertyList, malloc);
	vlist_reset_cursor(m_pPropertyList);
}


bool CUtilityXMLTag::GetNextPropertyItem(char **propname, CUtilityXMLProperty **prop)
{
	void *temp;
 	
  	if (!(temp = vlist_peek_cursor(m_pPropertyList)))
			return false;
	vhash_string_key_pair_t *pair = (vhash_string_key_pair_t *)temp;

	*propname = (char *)pair->key;
	*prop = (CUtilityXMLProperty *)pair->item;
 	vlist_advance_cursor(m_pPropertyList);	
	return true;

}

CUtilityXMLTag* CUtilityXMLTag::Clone()
{
    CUtilityXMLTag *tag = new CUtilityXMLTag(m_pXMLParser);
    tag->SetTagname(GetTagname());
    StartPropertyQuery();
    while (1)
    {
	char *name;
	CUtilityXMLProperty *prop;
	if (!GetNextPropertyItem(&name, &prop))
	    break;
	tag->AddProperty(name, prop->m_pText, prop->m_HasQuotes);
    }

    StartListItemsQuery();

    while (1)
    {
	char *text;
 	if (!GetNextListItem(&text))
	    break;
	tag->AddListItem(text);
    }

    START_LIST_ITERATION(char, m_pCommentList)
	tag->AddComment(temp);
    END_LIST_ITERATION(m_pCommentList);
    
    return tag;
}
void CUtilityXMLParser::GetFloatPoint(const char *text, AniPoint &point)
{
	CUtilityTextParser tp;
 	tp.InsertBuffer(text, (unsigned int)strlen(text));
	if (text[0] == '[')
 		tp.SkipAhead(1); 
	GetFloatPoint(&tp, point);
}

void CUtilityXMLParser::GetFloatPoint(CUtilityTextParser *tp, AniPoint &point)
{
	static delimiter_t number_td[] = {{"\x30-\x39|-|", false, " |]", true, false}};
 
	char result[SA_BUFFER_SIZE_BIG];
	int dummy;
	tp->ParseBuffer(result, dummy, number_td, 1);
	SACLOCALE(point.x = (float)atof(result));
	tp->ParseBuffer(result, dummy, number_td, 1);
	SACLOCALE(point.y = (float)atof(result));
	tp->ParseBuffer(result, dummy, number_td, 1);
	SACLOCALE(point.z = (float)atof(result));
}

void CUtilityXMLParser::GetRBGA(const char *text, float &r, float &g, float &b, float &a)
{
	CUtilityTextParser tp;
 	tp.InsertBuffer(text, (unsigned int)strlen(text));
	if (text[0] == '[')
 		tp.SkipAhead(1); 
	
	static delimiter_t number_td[] = {{"\x30-\x39|-|", false, " |]", true, false}};
 
	char result[SA_BUFFER_SIZE_BIG];
	int dummy;
	tp.ParseBuffer(result, dummy, number_td, 1);
	SACLOCALE(r = (float)atof(result));
	tp.ParseBuffer(result, dummy, number_td, 1);
	SACLOCALE(g = (float)atof(result));
	tp.ParseBuffer(result, dummy, number_td, 1);
	SACLOCALE(b = (float)atof(result));
	tp.ParseBuffer(result, dummy, number_td, 1);
	SACLOCALE(a = (float)atof(result));
}


CUtilityXMLParser::CUtilityXMLParser()
{
	m_pTextParser = new CUtilityTextParser();
	tagcallback_hash = new_vhash(1, malloc, free);
	stackdata_list = new_vlist(malloc, free);
	m_pInvalidTag = 0;
 
}

CUtilityXMLParser::~CUtilityXMLParser()
{
	if (tagcallback_hash)
	{
		vhash_string_key_map_function(tagcallback_hash, delete_tagcallback_struct,(void*)tagcallback_hash);
		delete_vhash(tagcallback_hash);
		tagcallback_hash = 0;
	}
	if (stackdata_list)
		delete_vlist(stackdata_list);

	delete m_pTextParser;
	
}

bool CUtilityXMLParser::SetTagCallback(const char *tagname, void *(*callbackfunction)(CUtilityXMLTag *, bool open, void *m_pExtraData), void *m_pExtraData)
{
	CUtilityXMLTagCallback *preturn = 0;
	CUtilityXMLTagCallback *ptype = new CUtilityXMLTagCallback;
	ptype->m_pTagCallback = callbackfunction;
	ptype->m_pExtraData = m_pExtraData;

	int result;
	if (!tagname)
		result = vhash_replace_string_key_item(tagcallback_hash, "@defaulttag", (void *)ptype, (void **)&preturn);
	else
		result = vhash_replace_string_key_item(tagcallback_hash, tagname, (void *)ptype, (void **)&preturn);

	if (result == VHASH_STATUS_SUCCESS)
	{
		if (preturn)
		{
			delete preturn;
			return false;
		}
	}
	return true;

}



void CUtilityXMLParser::SetupParseBuffer(const char *buffer)
{
	m_pTextParser->InsertBuffer((char *)buffer, (unsigned int)strlen(buffer));
}


int CUtilityXMLParser::AddProperties(CUtilityXMLTag *xmltag)
{
  	while (1)
	{
		m_pTextParser->ParseBuffer(result, dlum, find_properites_td, 3);
		if (dlum == 0)
			return STATUS_TAG_NOT_FINISHED;
		else if (dlum == 1)
		{
			m_pTextParser->SkipAhead(1);
			return STATUS_TAG_FINISHED;
		}
		else
		{
			char propname[SA_BUFFER_SIZE_BIG];
			strcpy(propname, result);
			
			m_pTextParser->SkipAhead(-1);
			m_pTextParser->ParseBuffer(result, dlum, find_equal_td, 2);
			assert(dlum == 0);
			
			m_pTextParser->ParseBuffer(result, dlum, find_properites_value_td, 2, false);

			//过滤xml属性不支持的字符
			SATools::StrReplace(result,"&quot;","\"");
			SATools::StrReplace(result,"&apos;","'");
			SATools::StrReplace(result,"&lt;","<");
			SATools::StrReplace(result,"&gt;",">");
			SATools::StrReplace(result,"&#xD;","\r");
			SATools::StrReplace(result,"&#xA;","\n");
			SATools::StrReplace(result,"&amp;","&");

			if (m_pTextParser->GetEndDelimiter() == '\"')
				xmltag->AddProperty(propname, result, true);
			else
				xmltag->AddProperty(propname, result, false);
			if (m_pTextParser->GetEndDelimiter() == '>')
				return STATUS_TAG_NOT_FINISHED;
			if (m_pTextParser->GetEndDelimiter() == '/')
				return STATUS_TAG_FINISHED;
			
		} 						
	}
																										
}

 
void CUtilityXMLParser::AddListItems(CUtilityXMLTag *xmltag)
{
	m_pTextParser->Rewind();
	while (1)
	{
 		m_pTextParser->ParseBuffer(result, dlum, find_item_value_td, 5, false);
		//过滤与xml冲突的字符
		SATools::StrReplace(result,"&lt;","<");
		SATools::StrReplace(result,"&gt;",">");
		SATools::StrReplace(result,"&amp;","&");
		if (dlum == 0)
		{			
			m_pTextParser->SkipAhead(-1);							
			break;
		}
		else if (m_pTextParser->GetEndDelimiter() == '<')
		{
			xmltag->AddListItem(result);
			m_pTextParser->SkipAhead(-1);
			break;
		}
		else
			xmltag->AddListItem(result);		
	}
	DoCallback(xmltag, true);
}



int CUtilityXMLParser::SkipSpecialTags(CUtilityXMLTag *xmltag)
{

	if (result[0] == '?' || result[0] == '!')
	{
		m_pTextParser->Rewind();
		m_pTextParser->ParseBuffer(result, dlum, skip_tag_td, 1);
		xmltag->AddComment(result);
		return STATUS_SPECIAL_TAG_SKIPPED;
	}
	return STATUS_SPECIAL_TAG_NOT_SKIPPED;
}

int CUtilityXMLParser::ProcessSubtags(CUtilityXMLTag *xmltag)
{	
	
	m_pTextParser->ParseBuffer(result, dlum, check_for_start_tag_td, 1, false);
	if (m_pTextParser->GetStartDelimiter() == '<' && m_pTextParser->PeekAt(0) != '/')
	{
		DoCallback(xmltag, true);
		m_pTextParser->Rewind();
 		ProcessXMLData();						
		return STATUS_SUBTAGS_FOUND;
	}
	else
		return STATUS_NO_SUBTAGS;
}


void CUtilityXMLParser::DoCallback(CUtilityXMLTag *xmltag, bool openclose)
{	
	
	
	
	CUtilityXMLTagCallback *preturn = 0;
	xmltag->SetOpen(openclose);
  	
	if (m_pInvalidTag == 0 || m_pInvalidTag == xmltag)
	{
	if (vhash_lookup_string_key_item(tagcallback_hash, xmltag->GetTagname() , (void **)&preturn) != VHASH_STATUS_SUCCESS)
	{
		if (vhash_lookup_string_key_item(tagcallback_hash, "@defaulttag" , (void **)&preturn) != VHASH_STATUS_SUCCESS)
		{
			if (openclose)
				m_pInvalidTag = xmltag;
			else
			{
				m_pInvalidTag = 0;
 				xmltag->Init();
			}
			return;
		}
	}
	void *stack_data = 0;

	if (preturn->m_pTagCallback)
	     stack_data = (preturn->m_pTagCallback)(xmltag, openclose, preturn->m_pExtraData);

	if (!openclose)
	{
		xmltag->Init();
		vlist_remove_first(stackdata_list);
	}
	else
		vlist_add_first(stackdata_list, (void *) stack_data);
	}
	else
		return;

}


bool CUtilityXMLParser::FindDocType(char *doctype, const char *buffer)
{
		
	if (buffer)
		SetupParseBuffer(buffer);
 	while (1)
	{		
 	if (m_pTextParser->ParseBuffer(result, dlum, find_tagname_td, 1))
	{

		if (m_pTextParser->GetStartDelimiter() != '<')
			return false;

		if (result[0] == '?')
		{
			m_pTextParser->Rewind();
			m_pTextParser->ParseBuffer(result, dlum, skip_tag_td, 1);
		}
		else if (result[0] == '!')
		{
			if (strcmp(result, "!DOCTYPE") == 0)
			{
				m_pTextParser->ParseBuffer(result, dlum, find_doctype_td, 3, false);
				m_pTextParser->ParseBuffer(result, dlum, find_doctype_td, 3, false);
				if (m_pTextParser->GetStartDelimiter() == '[')
				{
					m_pTextParser->Rewind();
					m_pTextParser->ParseBuffer(result, dlum, skip_bracket_td, 1, false);
					strcpy(doctype, result);
				}
				else
				{
					m_pTextParser->ParseBuffer(result, dlum, find_doctype_td, 3, false);
					strcpy(doctype, result);
				}
				return true;			
			}
			else{
				m_pTextParser->Rewind();
				m_pTextParser->ParseBuffer(result, dlum, skip_tag_td, 1);
			}
		}
		else
			return false;
	}
	}
	return false;
}


void *CUtilityXMLParser::GetStackData()
{
	return (vlist_peek_first(stackdata_list));
}
  

bool CUtilityXMLParser::ProcessXMLData()
{
	CUtilityXMLTag xmltag(this);
		
 	while (1)
	{		
Start_Parsing:
	if (m_pTextParser->ParseBuffer(result, dlum, find_tagname_td, 1))
	{

		if (m_pTextParser->GetStartDelimiter() != '<')
			return false;

		if (SkipSpecialTags(&xmltag) == STATUS_SPECIAL_TAG_SKIPPED)
 			goto Start_Parsing;
		else
		{
			if (m_pTextParser->GetEndDelimiter() == '/' && strlen(result) == 0)
			{
				m_pTextParser->Rewind();
				if (xmltag.GetOpen() == true)
				{
 					DoCallback(&xmltag, false);	
					if (!m_pTextParser->ParseBuffer(result, dlum, skip_tag_td, 1))
						return false;
				}
				else
					return true;
			}
			else {
				if (m_pTextParser->GetEndDelimiter() == '/')
					m_pTextParser->SkipAhead(-1);
				
				if (m_pTextParser->GetEndDelimiter() == '>')
				{
					xmltag.SetTagname(result);
					xmltag.SetOpen(true);
				}
				else
				{
					xmltag.SetTagname(result);

					xmltag.SetOpen(true);
					if (AddProperties(&xmltag) == STATUS_TAG_FINISHED)
					{
							DoCallback(&xmltag, true);
 							DoCallback(&xmltag, false);
 							goto Start_Parsing;
					}				 
																														
				}

				if (ProcessSubtags(&xmltag) == STATUS_NO_SUBTAGS)
 						AddListItems(&xmltag);				 				
				
			}
			
			}
		}
		else
		{
			if (xmltag.GetOpen())
				return false;
			else
				return true;
		}
	}
	return true;
	
}

CUtilityXMLGenerator::CUtilityXMLGenerator()
{
	m_pTextBuffer = 0;
	m_BufferSize = 0;
	m_BufferPos = 0;
	m_Intend = 0;
	m_bFirstTag = true;
	
}


void CUtilityXMLGenerator::Reset()
{
	if (m_pTextBuffer)
		delete [] m_pTextBuffer;
	m_pTextBuffer = 0;
}

CUtilityXMLGenerator::~CUtilityXMLGenerator()
{
	Reset();
}


void CUtilityXMLGenerator::WriteToBufferInternal(const char *text, int len)
{
	if (m_BufferSize <= m_BufferPos + len + 1)
	{
		char *tbuf = new char[m_BufferSize];
		memcpy(tbuf, m_pTextBuffer, m_BufferSize);
		delete [] m_pTextBuffer;
		int newsize = (int)(m_BufferSize + strlen(text) + 200000);
		m_pTextBuffer = new char[newsize];
		memcpy(m_pTextBuffer, tbuf, m_BufferSize);
		m_BufferSize = newsize;
		delete [] tbuf;
	}
	memcpy(m_pTextBuffer + m_BufferPos, text, len);
	m_BufferPos +=len;
}

void CUtilityXMLGenerator::WriteToBuffer(const char *text)
{
	WriteToBufferInternal(text, (int)strlen(text));
}

void CUtilityXMLGenerator::Finalize()
{
	char temp = 0;
	WriteToBufferInternal(&temp, 1);
}


void CUtilityXMLGenerator::WriteTagname(const char *name)
{
	char temp[SA_BUFFER_SIZE_BIG];
	sprintf(temp, "<%s", name);
	WriteToBuffer(temp);
}
void CUtilityXMLGenerator::WriteComment(const char *name)
{
	char temp[SA_BUFFER_SIZE_BIG];
	sprintf(temp, "<%s>", name);
	WriteToBuffer(temp);
}


void CUtilityXMLGenerator::WriteEndtag(const char *name)
{
	char temp[SA_BUFFER_SIZE_BIG];
	sprintf(temp, "</%s>", name);
	WriteToBuffer(temp);
}


void CUtilityXMLGenerator::WriteProperty(const char *name, CUtilityXMLProperty *xmlprop)
{
	//过滤xml属性不支持的字符
	char pText[SA_BUFFER_SIZE_BIG];
	strcpy(pText, xmlprop->m_pText);
	SATools::StrReplace(pText,"&","&amp;");
	SATools::StrReplace(pText,"\"","&quot;");
	SATools::StrReplace(pText,"'","&apos;");
	SATools::StrReplace(pText,"<","&lt;");
	SATools::StrReplace(pText,">","&gt;");
	SATools::StrReplace(pText,"\r","&#xD;");
	SATools::StrReplace(pText,"\n","&#xA;");

	char temp[SA_BUFFER_SIZE_BIG];
	if (!xmlprop->m_HasQuotes)
		sprintf(temp, "%s = %s ", name, pText);
	else
		sprintf(temp, "%s = \"%s\" ", name, pText);

	//char temp[SA_BUFFER_SIZE_BIG];
	//if (!xmlprop->m_HasQuotes)
	//	sprintf(temp, "%s = %s ", name, xmlprop->m_pText);
	//else
	//	sprintf(temp, "%s = \"%s\" ", name, xmlprop->m_pText);

	WriteToBuffer(temp);
}

void CUtilityXMLGenerator::WriteListItems(CUtilityXMLTag *xmltag)
{
	char temp[SA_BUFFER_SIZE_BIG];
	int linelen = 0;
	WriteNewline(1);
 	xmltag->StartListItemsQuery();
 	while (1)
	{			
		char *itemtext;
		if (!xmltag->GetNextListItem(&itemtext))
			break;
		linelen+=(int)strlen(itemtext);
		sprintf(temp, "%s ", itemtext);

		//过滤与xml冲突的字符
		SATools::StrReplace(temp,"&","&amp;");
		SATools::StrReplace(temp,"<","&lt;");
		SATools::StrReplace(temp,">","&gt;");

		WriteToBuffer(temp);
		if (linelen>60 && xmltag->HasMoreItems())
		{
			WriteNewline(1);
			linelen = 0;
		}
	}
}


void CUtilityXMLGenerator::WriteNewline(int extra)
{
	WriteToBuffer("\n");
	for (int i=0;i<m_Intend + extra;i++)
		WriteToBuffer("\x09");
}

	
void CUtilityXMLGenerator::AddTag(CUtilityXMLTag *xmltag)
{
	if (m_bFirstTag)
		m_bFirstTag = false;
	else
	{
		m_Intend++;
		WriteNewline();
	}	
	
	
	vlist_t * commentlist = xmltag->GetCommentList();	
	START_LIST_ITERATION(char, commentlist);
	    WriteComment(temp);
	    WriteNewline();
	END_LIST_ITERATION(commentlist);


	WriteTagname(xmltag->GetTagname());


	

	xmltag->StartPropertyQuery();
	CUtilityXMLProperty *xmlprop;
	char *propname;
 	bool noproperty = true;
	while (1)
	{
		if (!xmltag->GetNextPropertyItem(&propname, &xmlprop))
			break;
		if (noproperty)
		{
			WriteToBuffer(" ");
			noproperty = false;
		}
		WriteProperty(propname, xmlprop);

	}
	if (xmltag->GetOpen() || xmltag->HasListItems())
	{
		WriteToBuffer(">");
		if (xmltag->HasListItems())
		{
			WriteListItems(xmltag);
			WriteNewline(0);
			WriteEndtag(xmltag->GetTagname());
			m_Intend--;
		}
	}
	else
	{
		WriteToBuffer("/>");
		m_Intend--;
	}
	xmltag->CleanupPropertyList();

 	
}

void CUtilityXMLGenerator::CloseTag(CUtilityXMLTag *xmltag)
{
		WriteNewline();
		WriteEndtag(xmltag->GetTagname());
		m_Intend--;
}


bool CUtilityXMLTag::AddPropertyFloat(const char *name, float f)
{
	char prop[SA_BUFFER_SIZE_BIG];
	SACLOCALE(sprintf(prop,"%f", f));

	return (AddProperty(name, prop, true));


}

bool CUtilityXMLTag::AddPropertyInt(const char *name, int i)
{
	char prop[SA_BUFFER_SIZE_BIG];
	sprintf(prop,"%d", i);

	return (AddProperty(name, prop, true));


}

//SA_NAMESPACE_END
