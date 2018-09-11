﻿// All Rights Reserved. Copyright (C) 2014 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file		UtilityTextParser.cpp
*
*	@brief	文本解析相关代码
*
*
*	@par	历史:
*
****************************************************************************/

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "UtilityTextParser.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//SA_NAMESPACE_BEGIN

CUtilityTextParser::CUtilityTextParser()
{
	m_BufferPos		= 0;
	m_pTextBuffer	= 0;
	m_BufferLength	= 0;
	m_OldBufferPos	= 0;

}

CUtilityTextParser::~CUtilityTextParser()
{

}

void CUtilityTextParser::InsertBuffer(const char *buffer, unsigned int bufferlength)
{

	m_pTextBuffer = buffer;
	m_BufferLength = bufferlength;
	m_BufferPos = 0;



}

bool CUtilityTextParser::ParseBuffer(const char *searchbuffer,  char *tempbuf, int &dnum, const delimiter_t *dlist, unsigned int length, bool checkforquote)
{
	InsertBuffer(searchbuffer, (unsigned int)strlen(searchbuffer));
	return ParseBuffer(tempbuf, dnum, dlist, length, checkforquote);
}

bool CUtilityTextParser::ParseBuffer(char *tempbuf, int &dnum, const delimiter_t *dlist, unsigned int length, bool checkforquote)
{
 	int tbpos = 0;
	int level = 0;
	bool inquote = false;
	m_OldBufferPos = m_BufferPos;
	SetDelimiterList(dlist, length);
	bool ret = false;
	while (1)
	{
		if (m_BufferPos>=m_BufferLength)
		{
			ret =  false;
			goto cleanup;
		}
		tempbuf[tbpos] = m_pTextBuffer[m_BufferPos];
 		if (checkforquote && tempbuf[tbpos] == 34)
			inquote = !inquote;

		if (!inquote && CheckAllDelimiters(true, tempbuf, tbpos, dnum))
		{
			tempbuf[0] = tempbuf[tbpos];
			tempbuf[1] = 0;
 			tbpos = 1;
			if (m_pDelimiterList[dnum].sd_exclude)
				tbpos = 0;
			m_StartChar = tempbuf[0];
 			m_BufferPos++;
			if (!m_pDelimiterList[dnum].ed)
			{
				ret =  true;
				goto cleanup;
			}

		
			while (1)
			{	
				if (m_BufferPos >= m_BufferLength)
				{
					m_BufferPos = m_OldBufferPos;
					{
						ret =  false;
						goto cleanup;
					}
				}
				tempbuf[tbpos] = m_pTextBuffer[m_BufferPos];
				
				if (checkforquote && tempbuf[tbpos] == 34)
					inquote = !inquote;
				if (!inquote && m_pDelimiterList[dnum].checkopenclose && CheckDelimiter(true, tempbuf, tbpos, dnum))
					level++;
				if (!inquote && CheckDelimiter(false, tempbuf, tbpos, dnum))
				{
					if (level == 0)
					{
						tempbuf[tbpos+1] = 0;
						m_EndChar = tempbuf[tbpos];
 						if (m_pDelimiterList[dnum].ed_exclude)
 							tempbuf[tbpos] = 0;
 
					m_BufferPos++;
					ret =  true;				
					goto cleanup;
					}
					else
						level --;

				}
				tbpos++;
				m_BufferPos++;
 
			}
		}
		tbpos++;
		m_BufferPos++;
		if (m_BufferPos >= m_BufferLength)
		{
			m_BufferPos = m_OldBufferPos;
			{
				ret =  false;
				goto cleanup;
			}

		}
	}
cleanup:
	delete [] m_pDelimiterFullList1;
	delete [] m_pDelimiterFullList2;
	return ret;
}

unsigned long CUtilityTextParser::FindNextChar(char const * const text,
											   char const c,
											   int start)
{
	size_t const slen = strlen(text);
	while (1)
	{
		if (text[start] == c)
			return start;
		++start;
		if (start >= slen)
			return start;
	}
}

unsigned long CUtilityTextParser::FindNextChar(char const * const text,
											   int const slen,
											   char const c,
											   int start)
{
	while (1)
	{
		if (text[start] == c)
			return start;
		++start;
		if (start >= slen)
			return start;
	}
}
 

 bool CUtilityTextParser::MultiCompare(char const * const text, char const * const ctext, delimiter_full_t *ft)
{
 	char *mc1 = ft->mc1;
	char *mc2 = ft->mc2;
 	int i1 = ft->i1;
   	for (int i = 0; i < i1; ++i)
	{
		if (mc2[i] == 0)
		{
			if (text[0] == mc1[i])
				return true;
		}
		else
		{
			if (text[0] >= mc1[i] && text[0] <= mc2[i])
				return true;
		}
		
	}
	return false;
}
		
		

bool CUtilityTextParser::CheckDelimiter(bool startdelimiter, char *buf, unsigned int len, int dnum)
{
	buf[len+1] = 0;
	char *tbuf = 0;
 	char tbuf2[2];
	delimiter_full_t *tbuf_d = 0;

	if (startdelimiter)
	{
		if (m_pDelimiterList[dnum].sd){
  			tbuf= (char *)m_pDelimiterList[dnum].sd;
 			tbuf_d = &m_pDelimiterFullList1[dnum]; 		
		}

 
	}
	else
	{
 		if (m_pDelimiterList[dnum].ed){
 			tbuf = (char *)m_pDelimiterList[dnum].ed;
 			tbuf_d = &m_pDelimiterFullList2[dnum]; 			
		}
	}
	if (len+1 > 0)
	{
		tbuf2[0] = buf[len];
		tbuf2[1] = 0;	
 		if (MultiCompare(tbuf2, tbuf, tbuf_d))
 			return true;
	}
	return false;
}


bool CUtilityTextParser::CheckAllDelimiters(bool startdelimiter, char *buf, unsigned int len, int &dnum)
{
 	for (unsigned long i=0;i<m_DelimterListLength;i++)
	{
		
		if (CheckDelimiter(startdelimiter, buf, len, i))
		{
			dnum = i;
			return true;
		}
	}
 	return false;
}




void CUtilityTextParser::SetDelimiterList(const delimiter_t *dlist, unsigned int length)
{
 	m_pDelimiterList = dlist;
	m_DelimterListLength = length;
	m_pDelimiterFullList1 = new delimiter_full_t[length];
	m_pDelimiterFullList2 = new delimiter_full_t[length];

	for (unsigned int i=0;i<length;i++)
	{	
		if (dlist[i].sd)
			SetFullDelimiter(dlist[i].sd, &m_pDelimiterFullList1[i]);

		if (dlist[i].ed)
			SetFullDelimiter(dlist[i].ed, &m_pDelimiterFullList2[i]);
	}
}
	


void CUtilityTextParser::SetFullDelimiter(const char *ctext, delimiter_full_t *ft)
{
 	
		ft->i1 = 0;
		ft->i2 = 0;
		size_t slen = strlen(ctext);

		ft->len = slen;
		while (1)
		{
			if ((FindNextChar(ctext, '|', ft->i2) > (unsigned long)(ft->i2 + 1)) && slen > 1)
			{ 
				ft->mc1[ft->i1]   = ctext[ft->i2];
				ft->mc2[ft->i1++] = ctext[ft->i2+2];
				ft->i2+=4;
			}
			else
			{
				ft->mc1[ft->i1] = ctext[ft->i2++];
				ft->mc2[ft->i1++] = 0;
				++ft->i2;
			}
			if (ft->i2 >= slen)
				break;
		}
}

//SA_NAMESPACE_END

 


