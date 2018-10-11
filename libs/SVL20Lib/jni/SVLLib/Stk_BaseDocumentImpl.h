// All Rights Reserved. Copyright (C) 2017 Hoteamsoft. 
/**************************************************************************
*	@class	Stk_BaseDocumentImpl
*	@brief	Œƒµµπ‹¿Ìª˘¿‡
*	@par ¿˙ ∑º«¬º
*				2017/06/09 ¥¥Ω®°£likun(Hoteamsoft)
**************************************************************************/
#ifndef _SVLLIB_BASEDOCUMENTIMPL_H_
#define _SVLLIB_BASEDOCUMENTIMPL_H_

#include "document.h"
#include "stringbuffer.h"
#include "writer.h"
#include "prettywriter.h"
#include "allocators.h"
#include "RemoteClient.h"
#include "Stk_API.h"
#include "Stk_ObjectImpl.h"
#include "Mutex.h"
#define RAPIDJSON_HAS_STDSTRING 1

using rapidjson::Value;
using rapidjson::Document;
using rapidjson::StringBuffer;
using rapidjson::PrettyWriter;
using rapidjson::Writer;
using rapidjson::StringRef;
using rapidjson::MemoryPoolAllocator;

using namespace std;

namespace HoteamSoft {
	namespace SVLLib {
		class Stk_BaseDocument;
		class Stk_BaseDocumentImpl: public Stk_ObjectImpl
        {
            STK_CLASS_IMPL_DECLARE(Stk_BaseDocument)
		public:
			Stk_BaseDocumentImpl(void);
			virtual ~Stk_BaseDocumentImpl(void);
		public:
			std::wstring m_strVersion;				//!< Œƒº˛∞Ê±æ∫≈
			Stk_Document* m_pDocument;				//!< ◊‹ÃÂ ˝æ›π‹¿Ì¿‡∂‘œÛ
			Stk_BaseDocument* m_pPubl;
			std::wstring m_strFilePath;				//!< Œƒº˛µƒ¬∑æ∂
			RemoteClient*    m_pRemoteClient;
			Mutex			m_mutexDocRead;
			Document m_jsonDoc;						//!< JsonŒƒµµ∂‘œÛ
		public:
			/****************************************************************************
			*	@brief		ªÒ»°Œƒº˛µƒ¿©’π√˚
			*	@param[in]	strFilePath Œƒº˛√˚≥∆ªÚ¬∑æ∂
			*	@param[out]	Œﬁ
			*	@retval		Œƒº˛µƒ¿©’π√˚
			*	@note		Œﬁ
			*	@attention
			****************************************************************************/
			const std::wstring GetFileExt(const std::wstring& strFilePath);

			/****************************************************************************
			*	@brief		∑µªÿJson∏Ò Ω◊÷∑˚¥Æ
			*	@param[in]	Œﬁ
			*	@param[out]	strJson ◊÷∑˚¥Æƒ⁄»›
			*	@retval		STK_SUCCESS			≥…π¶
			*	@retval		∆‰À˚				 ß∞‹
			*	@note		Œﬁ
			*	@attention
			****************************************************************************/
			STK_STATUS ToJsonString(string& strJson);

			/****************************************************************************
			*	@brief		∏˘æ›Json∏Ò Ω◊÷∑˚¥Æ≥ı ºªØµ±«∞∂‘œÛ
			*	@param[in]	strJson ◊÷∑˚¥Æƒ⁄»›
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			≥…π¶
			*	@retval		∆‰À˚				 ß∞‹
			*	@note		Œﬁ
			*	@attention
			****************************************************************************/
			STK_STATUS FromJsonString(string& strJson);

			/****************************************************************************
			*	@brief		∏˘æ›Json∂‘œÛ≥ı ºªØ ˝æ›
			*	@param[in]	jsonDoc		JsonŒƒµµ∂‘œÛ
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			∂¡»°≥…π¶
			*	@retval		∆‰À˚				∂¡»° ß∞‹
			*	@note		Œﬁ
			*	@attention
			****************************************************************************/
			virtual STK_STATUS FromJson(Document& jsonDoc) { return STK_ERROR; }

			/****************************************************************************
			*	@brief		Ω´ ˝æ›◊™ªªŒ™Json∂‘œÛ
			*	@param[in]	Œﬁ
			*	@param[out]	jsonDoc		JsonŒƒµµ∂‘œÛ
			*	@retval		STK_SUCCESS			∂¡»°≥…π¶
			*	@retval		∆‰À˚				∂¡»° ß∞‹
			*	@note		Œﬁ
			*	@attention
			****************************************************************************/
			virtual STK_STATUS ToJson(Document& jsonDoc) { return STK_ERROR; }

			/****************************************************************************
			*	@brief		∂¡»°JsonŒƒº˛
			*	@param[in]	strFilePath	Œƒº˛¬∑æ∂
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			∂¡»°≥…π¶
			*	@retval		∆‰À˚				∂¡»° ß∞‹
			*	@note		Œﬁ
			*	@attention
			****************************************************************************/
			STK_STATUS LoadJsonFile(const std::wstring& strFilePath);

			/****************************************************************************
			*	@brief		±£¥ÊJsonŒƒº˛
			*	@param[in]	strFilePath	Œƒº˛¬∑æ∂
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			–¥»Î≥…π¶
			*	@retval		∆‰À˚				–¥»Î ß∞‹
			*	@note		Œﬁ
			*	@attention
			****************************************************************************/
			STK_STATUS WriteJsonFile(const std::wstring& strFilePath);

			/****************************************************************************
			*	@brief		∂¡»°∂˛Ω¯÷∆Œƒº˛
			*	@param[in]	strFilePath	Œƒº˛¬∑æ∂
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			∂¡»°≥…π¶
			*	@retval		∆‰À˚				∂¡»° ß∞‹
			*	@note		Œﬁ
			*	@attention
			****************************************************************************/
			//Buffer* LoadBinFile(const std::wstring& strFilePath);

			/****************************************************************************
			*	@brief		∂¡»° ˝æ›¡˜
			*	@param[in]	‘› ±Œ¥∂®
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			∂¡»°≥…π¶
			*	@retval		∆‰À˚				∂¡»° ß∞‹
			*	@note		Œﬁ
			*	@attention
			****************************************************************************/
			//STK_STATUS	OpenRemote(const std::string& strIP, int port,
			//	const std::string& userName, const std::string& projectCode, const std::string& fileName);
			void unLoad();
		};
	}//svllib
}//hoteamsoft

#endif
