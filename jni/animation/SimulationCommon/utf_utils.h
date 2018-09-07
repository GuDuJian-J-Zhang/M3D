
#ifndef _UTF_UTILS
#define _UTF_UTILS

#ifdef USE_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#ifdef _WIN32_WCE
#	define NEEDS_STR_CASE_CMP
#	define NEEDS_WCS_CASE_CMP
#endif // _WIN32_WCE
#ifdef SOLARIS_SYSTEM
#	define NEEDS_WCS_CASE_CMP
#endif // SOLARIS_SYSTEM
#ifdef OSX_SYSTEM
#	define NEEDS_WCS_CASE_CMP
#endif // OSX_SYSTEM
#ifdef HPUX_SYSTEM
#	define NEEDS_WCS_CASE_CMP
#endif // HPUX_SYSTEM


#ifdef V_PREFIX

#define VNAME_TMP_2(_x2_,_y2_) _x2_##_y2_
#define VNAME_TMP_1(_x1_,_y1_) VNAME_TMP_2(_x1_,_y1_)
#define VNAME(_x_) VNAME_TMP_1(V_PREFIX,_x_)

/* remap the names for special prefix */
#define SA_URI VNAME(SA_URI)
#define SA_UTF8 VNAME(SA_UTF8)
#define SA_UTF16 VNAME(SA_UTF16)
#define SA_UTF32 VNAME(SA_UTF32)
#define SA_WCS VNAME(SA_WCS)
#define SA_FORMAT_TEXT VNAME(SA_FORMAT_TEXT)
#define SAUtilityUnicodeStr VNAME(SAUtilityUnicodeStr)
#define SAUtilityAsciiStr VNAME(SAUtilityAsciiStr)

#ifndef _MSC_VER
#define wfopen VNAME(wfopen)
#define wremove VNAME(wremove)
#endif

#ifdef NEEDS_STR_CASE_CMP
#define strcasecmp VNAME(strcasecmp)
#endif
#ifdef NEEDS_WCS_CASE_CMP
#define wcscasecmp VNAME(wcscasecmp)
#endif

#endif

#include <stdio.h>
#include <memory.h>
#include <stdarg.h>
#include <string.h>

#ifndef _MSC_VER
#define __wchar_t wchar_t
#endif


#ifdef NEEDS_STR_CASE_CMP
int strcasecmp ( char const * string1, char const * string2 ); 
#endif
#ifdef NEEDS_WCS_CASE_CMP
int wcscasecmp ( __wchar_t const * string1, __wchar_t const * string2 ); 
#endif

typedef unsigned char utf8_char;

class SA_UTF32;
class SA_UTF16;
class SA_UTF8;
class SA_URI;

#ifdef _MSC_VER
typedef SA_UTF16 SA_WCS;
typedef unsigned int utf32_char;
typedef wchar_t utf16_char;
#define wfopen _wfopen
#define wremove _wremove
#else
typedef SA_UTF32 SA_WCS;
typedef wchar_t utf32_char;
typedef unsigned short utf16_char;
FILE * wfopen(wchar_t const *, wchar_t const *);
int wremove(wchar_t const *);
#endif

class SA_UTF32
{
public:
    SA_UTF32(SA_UTF32 const & rhs);
    SA_UTF32 const & operator = (SA_UTF32 const & rhs);
    SA_UTF32(__wchar_t const * text);
#ifdef _MSC_VER
    SA_UTF32(unsigned short const * text);
#endif
	SA_UTF32(char const * text);
	SA_UTF32();

    /* Conversion constructors. */
    SA_UTF32(SA_UTF16 const & unicode_text);
    SA_UTF32(SA_UTF8 const & unicode_text);
    SA_UTF32(SA_URI const & unicode_text);
    ~SA_UTF32();

    size_t length() const;

    template <typename T>
		T length(size_t const s){
		if(s > m_size){
			delete [] m_utf32;
			m_utf32 = new utf32_char[m_size = s +1];
			m_utf32[0] = 0;
		}
		return reinterpret_cast<T>(m_utf32);
    }

    template <typename T>
      SA_UTF32 const & encodedText(T * text_in){
      delete [] m_utf32;
      m_utf32 = 0;
      m_size = 0;
      utf32_char const * text = reinterpret_cast<utf32_char const*>(text_in);
      if(text){
	while(text[m_size++] != 0);
	m_utf32 = new utf32_char[m_size];
	memcpy(m_utf32, text, m_size * sizeof(utf32_char));
      }
      return *this;
    }

    utf32_char const * encodedText() const {return m_utf32;};

    SA_UTF32 & format(__wchar_t const * fmt, ...);
#ifdef _MSC_VER
	SA_UTF32 & format(unsigned short const * fmt, ...);
#endif
    SA_UTF32 & format(char const * fmt, ...);

    SA_UTF32 & append(__wchar_t const * txt);
#ifdef _MSC_VER
    SA_UTF32 & append(unsigned short const * txt);
#endif
	SA_UTF32 & append(char const * txt);

    class iterator {
    private:
        mutable utf32_char const * m_text;
        int get_and_advance(utf32_char const * & text) const;
    public:
        iterator(utf32_char const * text);
        iterator operator ++ () const;
        iterator operator ++ (int) const;
        int operator * () const;
        operator bool () const;
    };

    iterator begin() const;
    iterator end() const;

	bool operator == (SA_UTF32 const & rhs) const;
	bool operator != (SA_UTF32 const & rhs) const {return !operator==(rhs);};

private:
    utf32_char * m_utf32;
    size_t m_size;
};

class SA_UTF16
{
public:

    SA_UTF16(SA_UTF16 const & rhs);
    SA_UTF16 const & operator = (SA_UTF16 const & rhs);
    SA_UTF16(__wchar_t const * text);
#ifdef _MSC_VER
	SA_UTF16(unsigned short const * text);
#endif
	SA_UTF16(char const * text);
	SA_UTF16();

    /* Conversion constructors. */
    SA_UTF16(SA_UTF32 const & unicode_text);
    SA_UTF16(SA_UTF8 const & unicode_text);
    SA_UTF16(SA_URI const & unicode_text);
    ~SA_UTF16();

    size_t length() const;

    template <typename T>
		T length(size_t const s){
		if(s > m_size){
			delete [] m_utf16;
			m_utf16 = new utf16_char[m_size = s +1];
			m_utf16[0] = 0;
		}
		return reinterpret_cast<T>(m_utf16);
    }

    template <typename T>
      SA_UTF16 const & encodedText(T const * text_in){
      delete [] m_utf16;
      m_utf16 = 0;
      m_size = 0;
      utf16_char const * text = reinterpret_cast<utf16_char const*>(text_in);
      if(text){
	while(text[m_size++] != 0);
	m_utf16 = new utf16_char[m_size];
	memcpy(m_utf16, text, m_size * sizeof(utf16_char));
      }
      return *this;
    }

    utf16_char const * encodedText() const {return m_utf16;};

    SA_UTF16 & format(__wchar_t const * fmt, ...);
#ifdef _MSC_VER
    SA_UTF16 & format(unsigned short const * fmt, ...);
#endif
	SA_UTF16 & format(char const * fmt, ...);

	SA_UTF16 & append(__wchar_t const * txt);
#ifdef _MSC_VER
	SA_UTF16 & append(unsigned short const * txt);
#endif
	SA_UTF16 & append(char const * txt);

    class iterator {
    private:
        mutable utf16_char const * m_text;
        int get_and_advance(utf16_char const * & text) const;
    public:
        iterator(utf16_char const * text);
        iterator operator ++ () const;
        iterator operator ++ (int) const;
        int operator * () const;
        operator bool () const;
    };

    iterator begin() const;
    iterator end() const;

	bool operator == (SA_UTF16 const & rhs) const;
	bool operator != (SA_UTF16 const & rhs) const {return !operator==(rhs);};

private:
    utf16_char * m_utf16;
    size_t m_size;
};

class SA_UTF8
{
public:
    SA_UTF8(SA_UTF8 const & rhs);
    SA_UTF8 const & operator = (SA_UTF8 const & rhs);
    SA_UTF8(__wchar_t const * text);
#ifdef _MSC_VER
    SA_UTF8(unsigned short const * text);
#endif
	SA_UTF8(char const * text);
	SA_UTF8();

    /* Constructors for conversion */
    SA_UTF8(SA_UTF32 const & unicode_text);
    SA_UTF8(SA_UTF16 const & unicode_text);
    SA_UTF8(SA_URI const & unicode_text);
    ~SA_UTF8();

    size_t length() const;

    template <typename T>
		T length(size_t const s){
		if(s > m_size){
			delete [] m_utf8;
			m_utf8 = new utf8_char[m_size = s +1];
			m_utf8[0] = 0;
		}
		return reinterpret_cast<T>(m_utf8);
    }

    template <typename T>
      SA_UTF8 const & encodedText(T * text_in){
      delete [] m_utf8;
      m_utf8 = 0;
      m_size = 0;
      utf8_char const * text = reinterpret_cast<utf8_char const *>(text_in);
      if(text){
	while(text[m_size++] != 0);
	m_utf8 = new utf8_char[m_size];
	memcpy(m_utf8, text, m_size * sizeof(utf8_char));
      }
      return *this;
    }

    utf8_char const * encodedText() const {return m_utf8;};

    SA_UTF8 & format(__wchar_t const * fmt, ...);
#ifdef _MSC_VER
    SA_UTF8 & format(unsigned short const * fmt, ...);
#endif
	SA_UTF8 & format(char const * fmt, ...);

    SA_UTF8 & append(__wchar_t const * txt);
#ifdef _MSC_VER
    SA_UTF8 & append(unsigned short const * txt);
#endif
	SA_UTF8 & append(char const * txt);

    class iterator {
    private:
        mutable utf8_char const * m_text;
        int get_and_advance(utf8_char const * & text) const;
    public:
        iterator(utf8_char const * text);
        iterator operator ++ () const;
        iterator operator ++ (int) const;
        int operator * () const;
        operator bool () const;
    };

    iterator begin() const;
    iterator end() const;

	bool operator == (SA_UTF8 const & rhs) const;
	bool operator != (SA_UTF8 const & rhs) const {return !operator==(rhs);};

private:
    utf8_char * m_utf8;
    size_t m_size;
};

/* SA_URI encodes a utf8 encoded string.  It replaces all non-ascii chars with percent encodings.
 * so the string "AA" could be URI encoded as "%65%65" (but as the chars are ascii, it will be
 * encoded as "AA". */
class SA_URI
{
public:
    SA_URI(SA_URI const & rhs);
    SA_URI const & operator = (SA_URI const & rhs);
    SA_URI(__wchar_t const * text);
#ifdef _MSC_VER
    SA_URI(unsigned short const * text);
#endif
    SA_URI(char const * text);
    SA_URI();

    /* Constructors for conversion */
    SA_URI(SA_UTF32 const & unicode_text);
    SA_URI(SA_UTF16 const & unicode_text);
    SA_URI(SA_UTF8 const & unicode_text);
    ~SA_URI();

    size_t length() const;

    template <typename T>
		T length(size_t const s){
		if(s > m_size){
			delete [] m_uri;
			m_uri = new utf8_char[m_size = s +1];
			m_uri[0] = 0;
		}
		return reinterpret_cast<T>(m_uri);
    }

    template <typename T>
      SA_URI const & encodedText(T * text_in){
      delete [] m_uri;
      m_uri = 0;
      m_size = 0;
      utf8_char const * text = reinterpret_cast<utf8_char const *>(text_in);
      if(text){
	while(text[m_size++] != 0);
	m_uri = new utf8_char[m_size];
	memcpy(m_uri, text, m_size * sizeof(utf8_char));
      }
      return *this;
    }

    utf8_char const * encodedText() const {return m_uri;};

    SA_URI & format(__wchar_t const * fmt, ...);
#ifdef _MSC_VER
    SA_URI & format(unsigned short const * fmt, ...);
#endif
    SA_URI & format(char const * fmt, ...);

    SA_URI & append(__wchar_t const * txt);
#ifdef _MSC_VER
    SA_URI & append(unsigned short const * txt);
#endif
    SA_URI & append(char const * txt);

    class iterator {
    private:
        mutable utf8_char const * m_text;
        int get_and_advance(utf8_char const * & text) const;
    public:
        iterator(utf8_char const * text);
        iterator operator ++ () const;
        iterator operator ++ (int) const;
        int operator * () const;
        operator bool () const;
    };

    iterator begin() const;
    iterator end() const;

    bool operator == (SA_URI const & rhs) const;
    bool operator != (SA_URI const & rhs) const {return !operator==(rhs);};

private:
    utf8_char * m_uri;
    size_t m_size;
};

/* SA_FORMAT_TEXT is by far, hands down, the awesomest C++ class ever written. */ 
class SA_FORMAT_TEXT
{
private:
	char * m_str;	//! formatted string

	/*! This is a portable version of vsnprintf */
	static int const sa_vsnprintf(char * const buf, size_t const n, char const * const format, va_list valist);
public:

	/*! Use this constructor as a cast for format a string.  Do *NOT* set this equal to a
		variable.  char * s = SA_FORMAT_TEXT("") is a bug and s will point to freed memory.
		The only acceptable uses of this class is casting in a function parameter or initializing
		a static or automatic SA_FORMAT_TEXT variable.

	*/
	SA_FORMAT_TEXT(char const * const format, ...);
	SA_FORMAT_TEXT & Append(char const * format, ...);
	~SA_FORMAT_TEXT();

	/*! This returns the memory when the class is in scope. */
	operator char * () const;
	SA_FORMAT_TEXT const & operator=(SA_FORMAT_TEXT const & rhs);
	SA_FORMAT_TEXT(SA_FORMAT_TEXT const & rhs);
	SA_FORMAT_TEXT();

};

// SAUtilityUnicodeStr ////////////////////////////////////////////////////////
// Converts both 8-bit (foreign) and 16-bit (native) strings to 16-bit wideness

/*!
	SAUtilityUnicodeStr takes a regular (8bit - char*) or UNICODE (16 bit - wchar*) string and converts 
	to a UNICODE string. This UNICODE string can be used for later operations. The converted UNICODE 
	string can only be used as long as the SAUtilityUnicodeStr object exists. 
*/
class  SAUtilityUnicodeStr
{
public:
	
	/*! Constructor
		\param p8	char* pointing to an 8-bit string*/
    SAUtilityUnicodeStr(const char *p8);

	/*! Constructor
		\param p16	wchar_t* pointing to an 16-bit string*/
    SAUtilityUnicodeStr(const __wchar_t *p16);
#ifdef _MSC_VER
	SAUtilityUnicodeStr(const unsigned short *p16);
#endif

	/*! destructor - A Non-virtual destructor (this class is concrete) */
	~SAUtilityUnicodeStr();

	/*! conversion operator to UNICODE */
	operator const __wchar_t * (void) const;
#ifdef _MSC_VER
	operator const unsigned short * (void) const;
#endif

private:
	/*! protect against assignment! 
		\param str2 Reference to another SAUtilityUnicodeStr object*/
	SAUtilityUnicodeStr(const SAUtilityUnicodeStr& str2);

	/*! protect against assignment! 
		\param str2 Reference to another SAUtilityUnicodeStr object*/
    SAUtilityUnicodeStr& operator=(const SAUtilityUnicodeStr& str2);

	SA_WCS m_wcs;
};

// SAUtilityAsciiStr /////////////////////////////////////////////////////////

/*!
	SAUtilityAsciiStr takes a regular (8bit - char*) or UNICODE (16 bit - wchar*) string and converts 
	to a regular 8-bit string. This string can be used for later operations. The converted 
	string can only be used as long as the SAUtilityAsciiStr object exists. 
*/
class  SAUtilityAsciiStr 
{

public:

	/*! Constructor
		\param p8	char* pointing to an 8-bit string*/
	SAUtilityAsciiStr(const char *p8);

	/*! Constructor
		\param p16	wchar_t* pointing to an 16-bit string*/
	SAUtilityAsciiStr(const __wchar_t *p16);
#ifdef _MSC_VER
	SAUtilityAsciiStr(const unsigned short *p16);
#endif

	/*! destructor - A Non-virtual destructor (this class is concrete) */
	~SAUtilityAsciiStr(void);
	
	/*! conversion operator to ASCII */
	operator const char * (void) const;
	
	
private:

	char *m_sz;			/*!< The 8-bit string */
	
	// protect against assignment!
	SAUtilityAsciiStr(const SAUtilityAsciiStr&);
	SAUtilityAsciiStr& operator=(const SAUtilityAsciiStr&);
	
};


/*! \def SA_TEXT - use this macro where-ever the api itself takes different type of argument 
    during compilation depending upon preprocessor 
*/
#ifdef _UNICODE
typedef SAUtilityUnicodeStr	SA_TEXT;
#else
typedef SAUtilityAsciiStr SA_TEXT;
#endif

/*! \def SA_UNICODE_TEXT  Converts any string (UNICODE or ASCII) to UNICODE string */
//typedef SAUtilityUnicodeStr	SA_UNICODE_TEXT;

#define SA_UNICODE_TEXT(_x_) (const wchar_t *)SAUtilityUnicodeStr(_x_)

/*! \def SA_ASCII_TEXT Converts any string (UNICODE or ASCII) to ASCII string */
//typedef SAUtilityAsciiStr	SA_ASCII_TEXT;

#define SA_ASCII_TEXT(_x_) (const char *)SAUtilityAsciiStr(_x_)

#ifdef USE_PACK_8
#pragma pack(pop)
#endif

#endif
