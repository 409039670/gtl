#include "globalfuns.h"
#include <gl\gl.h>
#include <gl\glu.h>
#include <math.h>  
#include "sphere3d.h" 
#include "matrix4x4.h"


begin_gtl_namespace
begin_gdb_namespace
/*
* The CPLString class is derived from std::string, so the vast majority
* of the implementation comes from that.  This module is just the extensions
* we add.
*/

/************************************************************************/
/*                               printf()                               */
/************************************************************************/

String & String::printf(const char *pszFormat, ...)

{
	va_list args;

	va_start(args, pszFormat);
	vprintf(pszFormat, args);
	va_end(args);

	return *this;
}
/************************************************************************/
/*                              vprintf()                               */
/************************************************************************/

String &String::vprintf(const char *pszFormat, va_list args)

{
	/* -------------------------------------------------------------------- */
	/*      This implementation for platforms without vsnprintf() will      */
	/*      just plain fail if the formatted contents are too large.        */
	/* -------------------------------------------------------------------- */

#if !defined(HAVE_VSNPRINTF)
	char *pszBuffer = (char *)malloc(30000);
	if (vsprintf(pszBuffer, pszFormat, args) > 29998)
	{
		std::cerr<<"String::vprintf() ... buffer overrun.";
	}
	*this = pszBuffer;
	free(pszBuffer);

	/* -------------------------------------------------------------------- */
	/*      This should grow a big enough buffer to hold any formatted      */
	/*      result.                                                         */
	/* -------------------------------------------------------------------- */
#else
	char szModestBuffer[500];
	int nPR;
	va_list wrk_args;

#ifdef va_copy
	va_copy(wrk_args, args);
#else
	wrk_args = args;
#endif

	nPR = vsnprintf(szModestBuffer, sizeof(szModestBuffer), pszFormat,
		wrk_args);
	if (nPR == -1 || nPR >= (int) sizeof(szModestBuffer) - 1)
	{
		int nWorkBufferSize = 2000;
		char *pszWorkBuffer = (char *)malloc(nWorkBufferSize);

#ifdef va_copy
		va_end(wrk_args);
		va_copy(wrk_args, args);
#else
		wrk_args = args;
#endif
		while ((nPR = vsnprintf(pszWorkBuffer, nWorkBufferSize, pszFormat, wrk_args))
			>= nWorkBufferSize - 1
			|| nPR == -1)
		{
			nWorkBufferSize *= 4;
			pszWorkBuffer = (char *)realloc(pszWorkBuffer,
				nWorkBufferSize);
#ifdef va_copy
			va_end(wrk_args);
			va_copy(wrk_args, args);
#else
			wrk_args = args;
#endif
		}
		*this = pszWorkBuffer;
		free(pszWorkBuffer);
	}
	else
	{
		*this = szModestBuffer;
	}
	va_end(wrk_args);
#endif

	return *this;
}
/************************************************************************/
/*                              format()                               */
/************************************************************************/

/**
* Format double in C locale.
*
* The passed value is formatted using the C locale (period as decimal
* seperator) and appended to the target String.
*
* @param dfValue the value to format.
* @param pszFormat the sprintf() style format to use or omit for default.
* Note that this format string should only include one substitution argument
* and it must be for a double (%f or %g).
*
* @return a reference to the CPLString.
*/

String &String::format(double dfValue, const char *pszFormat)

{
	if (pszFormat == NULL)
		pszFormat = "%g";

	char szWork[512]; // presumably long enough for any number?

	sprintf(szWork, pszFormat, dfValue);
	assert(strlen(szWork) < sizeof(szWork));

	if (strchr(szWork, ',') != NULL)
	{
		char *pszDelim = strchr(szWork, ',');
		*pszDelim = '.';
	}

	*this += szWork;

	return *this;
}

/************************************************************************/
/*                                trim()                                */
/************************************************************************/

/**
* Trim white space.
*
* Trims white space off the let and right of the string.  White space
* is any of a space, a tab, a newline ('\n') or a carriage control ('\r').
*
* @return a reference to the String.
*/

String &String::trim()
{
	size_t iLeft, iRight;
	static const char szWhitespace[] = " \t\r\n";

	iLeft = find_first_not_of(szWhitespace);
	iRight = find_last_not_of(szWhitespace);

	if (iLeft == std::string::npos)
	{
		erase();
		return *this;
	}

	assign(substr(iLeft, iRight - iLeft + 1));

	return *this;
}

/************************************************************************/
/*                               recode()                               */
/************************************************************************/

String &String::recode(const char *pszSrcEncoding,
	const char *pszDstEncoding)

{
	/*if (pszSrcEncoding == NULL)
		pszSrcEncoding = CPL_ENC_UTF8;
	if (pszDstEncoding == NULL)
		pszDstEncoding = CPL_ENC_UTF8;

	if (strcmp(pszSrcEncoding, pszDstEncoding) == 0)
		return *this;

	char *pszRecoded = CPLRecode(c_str(),
		pszSrcEncoding,
		pszDstEncoding);

	if (pszRecoded == NULL)
		return *this;

	assign(pszRecoded);
	free(pszRecoded);*/

	return *this;
}

/************************************************************************/
/*                               ifind()                                */
/************************************************************************/

/**
* Case insensitive find() alternative.
*
* @param str substring to find.
* @param pos offset in the string at which the search starts.
* @return the position of substring in the string or std::string::npos if not found.
* @since GDAL 1.9.0
*/

size_t String::ifind(const std::string & str, size_t pos) const

{
	return ifind(str.c_str(), pos);
}

/**
* Case insensitive find() alternative.
*
* @param s substring to find.
* @param nPos offset in the string at which the search starts.
* @return the position of the substring in the string or std::string::npos if not found.
* @since GDAL 1.9.0
*/

size_t String::ifind(const char *s, size_t nPos) const

{
	const char *pszHaystack = c_str();
	char chFirst = (char) ::tolower(s[0]);
	int nTargetLen = strlen(s);

	if (nPos > size())
		nPos = size();

	pszHaystack += nPos;

	while (*pszHaystack != '\0')
	{
		if (chFirst == ::tolower(*pszHaystack))
		{
			if (cslNIEqualString(pszHaystack, s, nTargetLen))
				return nPos;
		}

		nPos++;
		pszHaystack++;
	}

	return std::string::npos;
}

/************************************************************************/
/*                              toupper()                               */
/************************************************************************/

/**
* Convert to upper case in place.
*/

String &String::toupper()

{
	size_t i;

	for (i = 0; i < size(); i++)
		(*this)[i] = (char) ::toupper((*this)[i]);

	return *this;
}

/************************************************************************/
/*                              tolower()                               */
/************************************************************************/

/**
* Convert to lower case in place.
*/

String &String::tolower()

{
	size_t i;

	for (i = 0; i < size(); i++)
		(*this)[i] = (char) ::tolower((*this)[i]);

	return *this;
}

//////////////////////////////////////////////////////////////////////
////////////////////C StringList functions
/**********************************************************************
*                       cslAddString()
*
* Append a string to a StringList and return a pointer to the modified
* StringList.
* If the input StringList is NULL, then a new StringList is created.
* Note that CSLAddString performance when building a list is in O(n^2)
* which can cause noticable slow down when n > 10000.
**********************************************************************/
CSTRLIST cslAddString(CSTRLIST papszStrList, const char *pszNewString)
{
	int nItems = 0;

	if (pszNewString == NULL)
		return papszStrList;    /* Nothing to do!*/

								/* Allocate room for the new string */
	if (papszStrList == NULL)
		papszStrList = (char**)calloc(2, sizeof(char*));
	else
	{
		nItems = cslCount(papszStrList);
		papszStrList = (char**)realloc(papszStrList,
			(nItems + 2)*sizeof(char*));
	}

	/* Copy the string in the list */
	papszStrList[nItems] = cslDuplicateString(pszNewString);
	papszStrList[nItems + 1] = NULL;

	return papszStrList;
}

/************************************************************************/
/*                              cslCount()                              */
/************************************************************************/

/**
* Return number of items in a string list.
*
* Returns the number of items in a string list, not counting the
* terminating NULL.  Passing in NULL is safe, and will result in a count
* of zero.
*
* Lists are counted by iterating through them so long lists will
* take more time than short lists.  Care should be taken to avoid using
* cslCount() as an end condition for loops as it will result in O(n^2)
* behavior.
*
* @param papszStrList the string list to count.
*
* @return the number of entries.
*/
int cslCount(CSTRLIST papszStrList)
{
	int nItems = 0;

	if (papszStrList)
	{
		while (*papszStrList != NULL)
		{
			nItems++;
			papszStrList++;
		}
	}

	return nItems;
}

/************************************************************************/
/*                            cslGetString()                             */
/*                                                                      */
/*      Fetches the indicated field, being careful not to crash if      */
/*      the field doesn't exist within this string list.  The           */
/*      returned pointer should not be freed, and doesn't               */
/*      necessarily last long.                                          */
/************************************************************************/

const char * cslGetString(CSTRLIST papszStrList, int iField)
{
	int         i;

	if (papszStrList == NULL || iField < 0)
		return("");

	for (i = 0; i < iField + 1; i++)
	{
		if (papszStrList[i] == NULL)
			return "";
	}

	return(papszStrList[iField]);
}
/**
* Find a string within a string list.
*
* Returns the index of the entry in the string list that contains the
* target string.  The string in the string list must be a full match for
* the target, but the search is case insensitive.
*
* @param papszList the string list to be searched.
* @param pszTarget the string to be searched for.
*
* @return the index of the string within the list or -1 on failure.
*/

int cslIFindString(CSTRLIST papszList, const char * pszTarget)
{
	int         i;

	if (papszList == NULL)
		return -1;

	for (i = 0; papszList[i] != NULL; i++)
	{
		if (cslIEqualString(papszList[i], pszTarget))
			return i;
	}

	return -1;
}
/************************************************************************/
/*                             CSLDestroy()                             */
/************************************************************************/

/**
* Free string list.
*
* Frees the passed string list (null terminated array of strings).
* It is safe to pass NULL.
*
* @param papszStrList the list to free.
*/
void cslDestroy(CSTRLIST papszStrList)
{
	char **papszPtr;

	if (papszStrList)
	{
		papszPtr = papszStrList;
		while (*papszPtr != NULL)
		{
			free(*papszPtr);
			papszPtr++;
		}

		free(papszStrList);
		papszStrList = 0;
	}
}

/************************************************************************/
/*                            cslDuplicate()                            */
/************************************************************************/

/**
* Clone a string list.
*
* Efficiently allocates a copy of a string list.  The returned list is
* owned by the caller and should be freed with cslDestroy().
*
* @param papszStrList the input string list.
*
* @return newly allocated copy.
*/

CSTRLIST cslDuplicate(CSTRLIST papszStrList)
{
	char **papszNewList, **papszSrc, **papszDst;
	int  nLines;

	nLines = cslCount(papszStrList);

	if (nLines == 0)
		return NULL;

	papszNewList = (char **)malloc((nLines + 1)*sizeof(char*));
	papszSrc = papszStrList;
	papszDst = papszNewList;

	while (*papszSrc != NULL)
	{
		*papszDst = cslDuplicateString(*papszSrc);

		papszSrc++;
		papszDst++;
	}
	*papszDst = NULL;

	return papszNewList;
}

char * cslDuplicateString(const char *pszString) {
	char   *pszReturn;
	if (pszString == NULL)
		pszString = "";
	pszReturn = (char *)malloc(strlen(pszString) + 1);
	if (pszReturn == NULL)
		return NULL;
	strcpy(pszReturn, pszString);
	return(pszReturn);
}
/************************************************************************/
/*                         cslTokenizeString()                         */
/************************************************************************/

/**
* Tokenize a string.
*
* This function will split a string into tokens based on specified'
* delimeter(s) with a variety of options.  The returned result is a
* string list that should be freed with CSLDestroy() when no longer
* needed.
*
* The available parsing options are:
*
* - CSLT_ALLOWEMPTYTOKENS: allow the return of empty tokens when two
* delimiters in a row occur with no other text between them.  If not set,
* empty tokens will be discarded;
* - CSLT_STRIPLEADSPACES: strip leading space characters from the token (as
* reported by isspace());
* - CSLT_STRIPENDSPACES: strip ending space characters from the token (as
* reported by isspace());
* - CSLT_HONOURSTRINGS: double quotes can be used to hold values that should
* not be broken into multiple tokens;
* - CSLT_PRESERVEQUOTES: string quotes are carried into the tokens when this
* is set, otherwise they are removed;
* - CSLT_PRESERVEESCAPES: if set backslash escapes (for backslash itself,
* and for literal double quotes) will be preserved in the tokens, otherwise
* the backslashes will be removed in processing.
*
* \b Example:
*
* Parse a string into tokens based on various white space (space, newline,
* tab) and then print out results and cleanup.  Quotes may be used to hold
* white space in tokens.

\code
char **papszTokens;
int i;

papszTokens =
CSLTokenizeString( pszCommand, " \t\n",
CSLT_HONOURSTRINGS | CSLT_ALLOWEMPTYTOKENS );

for( i = 0; papszTokens != NULL && papszTokens[i] != NULL; i++ )
printf( "arg %d: '%s'", papszTokens[i] );
CSLDestroy( papszTokens );
\endcode

* @param pszString the string to be split into tokens.
* @param pszDelimiters one or more characters to be used as token delimeters.
* @param nCSLTFlags an ORing of one or more of the CSLT_ flag values.
*
* @return a string list of tokens owned by the caller.
*/

CSTRLIST cslTokenizeString(const char * pszString,	const char * pszDelimiters,	int nCSLTFlags)
{
	if (pszString == NULL)
		return (char **)calloc(sizeof(char *), 1);
	char ** oRetList;
	char        *pszToken;
	int         nTokenMax, nTokenLen;
	int         bHonourStrings = (nCSLTFlags & CSLT_HONOURSTRINGS);
	int         bAllowEmptyTokens = (nCSLTFlags & CSLT_ALLOWEMPTYTOKENS);
	int         bStripLeadSpaces = (nCSLTFlags & CSLT_STRIPLEADSPACES);
	int         bStripEndSpaces = (nCSLTFlags & CSLT_STRIPENDSPACES);

	pszToken = (char *)calloc(10, 1);
	nTokenMax = 10;

	while (pszString != NULL && *pszString != '\0')
	{
		int     bInString = FALSE;
		int     bStartString = TRUE;

		nTokenLen = 0;

		/* Try to find the next delimeter, marking end of token */
		for (; *pszString != '\0'; pszString++)
		{

			/* End if this is a delimeter skip it and break. */
			if (!bInString && strchr(pszDelimiters, *pszString) != NULL)
			{
				pszString++;
				break;
			}

			/* If this is a quote, and we are honouring constant
			strings, then process the constant strings, with out delim
			but don't copy over the quotes */
			if (bHonourStrings && *pszString == '"')
			{
				if (nCSLTFlags & CSLT_PRESERVEQUOTES)
				{
					pszToken[nTokenLen] = *pszString;
					nTokenLen++;
				}

				if (bInString)
				{
					bInString = FALSE;
					continue;
				}
				else
				{
					bInString = TRUE;
					continue;
				}
			}

			/*
			* Within string constants we allow for escaped quotes, but in
			* processing them we will unescape the quotes and \\ sequence
			* reduces to \
			*/
			if (bInString && pszString[0] == '\\')
			{
				if (pszString[1] == '"' || pszString[1] == '\\')
				{
					if (nCSLTFlags & CSLT_PRESERVEESCAPES)
					{
						pszToken[nTokenLen] = *pszString;
						nTokenLen++;
					}

					pszString++;
				}
			}

			/*
			* Strip spaces at the token start if requested.
			*/
			if (!bInString && bStripLeadSpaces
				&& bStartString && isspace((unsigned char)*pszString))
				continue;

			bStartString = FALSE;

			/*
			* Extend token buffer if we are running close to its end.
			*/
			if (nTokenLen >= nTokenMax - 3)
			{
				nTokenMax = nTokenMax * 2 + 10;
				pszToken = (char *)realloc(pszToken, nTokenMax);
			}

			pszToken[nTokenLen] = *pszString;
			nTokenLen++;
		}

		/*
		* Strip spaces at the token end if requested.
		*/
		if (!bInString && bStripEndSpaces)
		{
			while (nTokenLen && isspace((unsigned char)pszToken[nTokenLen - 1]))
				nTokenLen--;
		}

		pszToken[nTokenLen] = '\0';

		/*
		* Add the token.
		*/
		if (pszToken[0] != '\0' || bAllowEmptyTokens)
			cslAddString(oRetList,pszToken);
	}

	/*
	* If the last token was empty, then we need to capture
	* it now, as the loop would skip it.
	*/
	if (*pszString == '\0' && bAllowEmptyTokens && cslCount(oRetList) > 0
		&& strchr(pszDelimiters, *(pszString - 1)) != NULL)
	{
		cslAddString(oRetList,"");
	}

	free(pszToken);

	if (oRetList == NULL)
	{
		// we prefer to return empty lists as a pointer to 
		// a null pointer since some client code might depend on this.
		oRetList=(char**)calloc(sizeof(char**), 1);
	}

	return oRetList;
} 

CSTRLIST cslTokenizeString(const char * pszString, const char * pszDelimiters, int bHonourStrings, int bAllowEmptyTokens) {
	int         nFlags = 0;

	if (bHonourStrings)
		nFlags |= CSLT_HONOURSTRINGS;
	if (bAllowEmptyTokens)
		nFlags |= CSLT_ALLOWEMPTYTOKENS;

	return cslTokenizeString(pszString, pszDelimiters, nFlags);
}
/**********************************************************************
*                       cslTokenizeString()
*
* Tokenizes a string and returns a StringList with one string for
* each token.
**********************************************************************/
CSTRLIST cslTokenizeString(const char *pszString)
{
	return cslTokenizeString(pszString, " ", CSLT_HONOURSTRINGS);
}

bool cslEqualString(const char * s1, const char * s2) {
	return strcmp(s1, s2) == 0;
}
bool cslIEqualString(const char * s1, const char * s2) {
	return stricmp(s1, s2) == 0;
}
bool cslNIEqualString(const char * s1, const char * s2, int n) {
	return strnicmp(s1, s2, n);
}
/************************************************************************/
/*                              cslCopyString()                            */
/************************************************************************/

/**
 * Copy source string to a destination buffer.
 *
 * This function ensures that the destination buffer is always NUL terminated
 * (provided that its length is at least 1).
 *
 * This function is designed to be a safer, more consistent, and less error
 * prone replacement for strncpy. Its contract is identical to libbsd's strlcpy.
 *
 * Truncation can be detected by testing if the return value of CPLStrlcpy
 * is greater or equal to nDestSize.

\verbatim
char szDest[5];
if (cslCopyString(szDest, "abcde", sizeof(szDest)) >= sizeof(szDest))
    fprintf(stderr, "truncation occured !\n");
\endverbatim

 * @param pszDest   destination buffer
 * @param pszSrc    source string. Must be NUL terminated
 * @param nDestSize size of destination buffer (including space for the NUL terminator character)
 *
 * @return the length of the source string (=strlen(pszSrc))
 *
 */
size_t cslCopyString(char* pszDest, const char* pszSrc, size_t nDestSize)
{
    char* pszDestIter = pszDest;
    const char* pszSrcIter = pszSrc;

    if (nDestSize == 0)
        return strlen(pszSrc);

    nDestSize --;
    while(nDestSize != 0 && *pszSrcIter != '\0')
    {
        *pszDestIter = *pszSrcIter;
        pszDestIter ++;
        pszSrcIter ++;
        nDestSize --;
    }
    *pszDestIter = '\0';
    return pszSrcIter - pszSrc + strlen(pszSrcIter);
}

/************************************************************************/
/*                              cslConcatenateString()                            */
/************************************************************************/

/**
 * Appends a source string to a destination buffer.
 *
 * This function ensures that the destination buffer is always NUL terminated
 * (provided that its length is at least 1 and that there is at least one byte
 * free in pszDest, that is to say strlen(pszDest_before) < nDestSize)
 *
 * This function is designed to be a safer, more consistent, and less error
 * prone replacement for strncat. Its contract is identical to libbsd's strlcat.
 *
 * Truncation can be detected by testing if the return value of CPLStrlcat
 * is greater or equal to nDestSize.

\verbatim
char szDest[5];
cslConcatenateString(szDest, "ab", sizeof(szDest));
if (cslConcatenateString(szDest, "cde", sizeof(szDest)) >= sizeof(szDest))
    fprintf(stderr, "truncation occured !\n");
\endverbatim

 * @param pszDest   destination buffer. Must be NUL terminated before running CPLStrlcat
 * @param pszSrc    source string. Must be NUL terminated
 * @param nDestSize size of destination buffer (including space for the NUL terminator character)
 *
 * @return the thoretical length of the destination string after concatenation
 *         (=strlen(pszDest_before) + strlen(pszSrc)).
 *         If strlen(pszDest_before) >= nDestSize, then it returns nDestSize + strlen(pszSrc)
 *
 */
size_t cslConcatenateString(char* pszDest, const char* pszSrc, size_t nDestSize)
{
    char* pszDestIter = pszDest;

    while(nDestSize != 0 && *pszDestIter != '\0')
    {
        pszDestIter ++;
        nDestSize --;
    }

    return pszDestIter - pszDest + cslCopyString(pszDestIter, pszSrc, nDestSize);
}

/**********************************************************************
*                       CSLAddNameValue()
*
* Add a new entry to a StringList of "Name=Value" pairs,
* ("Name:Value" pairs are also supported for backward compatibility
* with older stuff.)
*
* This function does not check if a "Name=Value" pair already exists
* for that name and can generate multiple entryes for the same name.
* Use CSLSetNameValue() if you want each name to have only one value.
*
* Returns the modified stringlist.
**********************************************************************/
CSTRLIST cslAddNameValue(CSTRLIST papszStrList,	const char *pszName, const char *pszValue)
{
	char *pszLine;

	if (pszName == NULL || pszValue == NULL)
		return papszStrList;

	pszLine = (char *)malloc(strlen(pszName) + strlen(pszValue) + 2);
	sprintf(pszLine, "%s=%s", pszName, pszValue);
	papszStrList = cslAddString(papszStrList, pszLine);
	free(pszLine);

	return papszStrList;
}
/**
* Assign value to name in StringList.
*
* Set the value for a given name in a StringList of "Name=Value" pairs
* ("Name:Value" pairs are also supported for backward compatibility
* with older stuff.)
*
* If there is already a value for that name in the list then the value
* is changed, otherwise a new "Name=Value" pair is added.
*
* @param papszList the original list, the modified version is returned.
* @param pszName the name to be assigned a value.  This should be a well
* formed token (no spaces or very special characters).
* @param pszValue the value to assign to the name.  This should not contain
* any newlines (CR or LF) but is otherwise pretty much unconstrained.  If
* NULL any corresponding value will be removed.
*
* @return modified stringlist.
*/

CSTRLIST cslSetNameValue(CSTRLIST papszList,const char *pszName, const char *pszValue)
{
	char **papszPtr;
	size_t nLen;

	if (pszName == NULL)
		return papszList;

	nLen = strlen(pszName);
	papszPtr = papszList;
	while (papszPtr && *papszPtr != NULL)
	{
		if (cslNIEqualString(*papszPtr, pszName, nLen)
			&& ((*papszPtr)[nLen] == '=' ||
				(*papszPtr)[nLen] == ':'))
		{
			/* Found it!
			* Change the value... make sure to keep the ':' or '='
			*/
			char cSep;
			cSep = (*papszPtr)[nLen];

			free(*papszPtr);

			/*
			* If the value is NULL, remove this entry completely/
			*/
			if (pszValue == NULL)
			{
				while (papszPtr[1] != NULL)
				{
					*papszPtr = papszPtr[1];
					papszPtr++;
				}
				*papszPtr = NULL;
			}

			/*
			* Otherwise replace with new value.
			*/
			else
			{
				*papszPtr = (char *)malloc(strlen(pszName) + strlen(pszValue) + 2);
				sprintf(*papszPtr, "%s%c%s", pszName, cSep, pszValue);
			}
			return papszList;
		}
		papszPtr++;
	}

	if (pszValue == NULL)
		return papszList;

	/* The name does not exist yet... create a new entry
	*/
	return cslAddNameValue(papszList, pszName, pszValue);
}

/**********************************************************************
*                       cslFetchNameValue()
*
* In a StringList of "Name=Value" pairs, look for the
* first value associated with the specified name.  The search is not
* case sensitive.
* ("Name:Value" pairs are also supported for backward compatibility
* with older stuff.)
*
* Returns a reference to the value in the StringList that the caller
* should not attempt to free.
*
* Returns NULL if the name is not found.
**********************************************************************/
const char *cslFetchNameValue(CSTRLIST papszStrList, const char *pszName)
{
	size_t nLen;

	if (papszStrList == NULL || pszName == NULL)
		return NULL;

	nLen = strlen(pszName);
	while (*papszStrList != NULL)
	{
		if (cslNIEqualString(*papszStrList, pszName, nLen)
			&& ((*papszStrList)[nLen] == '=' ||
				(*papszStrList)[nLen] == ':'))
		{
			return (*papszStrList) + nLen + 1;
		}
		papszStrList++;
	}
	return NULL;
}
/**********************************************************************
*                       cslParseNameValue()
**********************************************************************/

/**
* Parse NAME=VALUE string into name and value components.
*
* Note that if ppszKey is non-NULL, the key (or name) portion will be
* allocated using VSIMalloc(), and returned in that pointer.  It is the
* applications responsibility to free this string, but the application should
* not modify or free the returned value portion.
*
* This function also support "NAME:VALUE" strings and will strip white
* space from around the delimeter when forming name and value strings.
*
* Eventually CSLFetchNameValue() and friends may be modified to use
* CPLParseNameValue().
*
* @param pszNameValue string in "NAME=VALUE" format.
* @param ppszKey optional pointer though which to return the name
* portion.
* @return the value portion (pointing into original string).
*/

const char *cslParseNameValue(const char *pszNameValue, char **ppszKey)

{
	int  i;
	const char *pszValue;

	for (i = 0; pszNameValue[i] != '\0'; i++)
	{
		if (pszNameValue[i] == '=' || pszNameValue[i] == ':')
		{
			pszValue = pszNameValue + i + 1;
			while (*pszValue == ' ' || *pszValue == '\t')
				pszValue++;

			if (ppszKey != NULL)
			{
				*ppszKey = (char *)malloc(i + 1);
				strncpy(*ppszKey, pszNameValue, i);
				(*ppszKey)[i] = '\0';
				while (i > 0 &&
					((*ppszKey)[i] == ' ' || (*ppszKey)[i] == '\t'))
				{
					(*ppszKey)[i] = '\0';
					i--;
				}
			}

			return pszValue;
		}
	}

	return NULL;
}
/************************************************************************/
/*                            cslIFindName()                             */
/************************************************************************/

/**
* Find StringList entry with given key name.
*
* @param papszStrList the string list to search.
* @param pszName the key value to look for (case insensitive).
*
* @return -1 on failure or the list index of the first occurance
* matching the given key.
*/

int cslIFindName(CSTRLIST papszStrList, const char *pszName)
{
	size_t nLen;
	int    iIndex = 0;

	if (papszStrList == NULL || pszName == NULL)
		return -1;

	nLen = strlen(pszName);
	while (*papszStrList != NULL)
	{
		if (cslNIEqualString(*papszStrList, pszName, nLen)
			&& ((*papszStrList)[nLen] == '=' ||
				(*papszStrList)[nLen] == ':'))
		{
			return iIndex;
		}
		iIndex++;
		papszStrList++;
	}
	return -1;
}

/************************************************************************/
/*                          cslStringToNumber()                            */
/************************************************************************/

/**
* Converts ASCII string to floating point number using specified delimiter.
*
* This function converts the initial portion of the string pointed to
* by nptr to double floating point representation. This function does the
* same as standard strtod(3), but does not take locale in account. Instead of
* locale defined decimal delimiter you can specify your own one. Also see
* notes for CPLAtof() function.
*
* @param nptr Pointer to string to convert.
* @param endptr If is not NULL, a pointer to the character after the last
* character used in the conversion is stored in the location referenced
* by endptr.
* @param point Decimal delimiter.
*
* @return Converted value, if any.
*/
double cslStringToNumber(const char *nptr, char **endptr, char point)
{
	while (*nptr == ' ')
		nptr++;

	if (nptr[0] == '-')
	{
		if (strcmp(nptr, "-1.#QNAN") == 0 ||
			strcmp(nptr, "-1.#IND") == 0)
			return NAN;

		if (strcmp(nptr, "-inf") == 0 ||
			strcmp(nptr, "-1.#INF") == 0)
			return -INFINITY;
	}
	else if (nptr[0] == '1')
	{
		if (strcmp(nptr, "1.#QNAN") == 0)
			return NAN;
		if (strcmp(nptr, "1.#INF") == 0)
			return INFINITY;
	}
	else if (nptr[0] == 'i' && strcmp(nptr, "inf") == 0)
		return INFINITY;
	else if (nptr[0] == 'n' && strcmp(nptr, "nan") == 0)
		return NAN;

	/* -------------------------------------------------------------------- */
	/*  We are implementing a simple method here: copy the input string     */
	/*  into the temporary buffer, replace the specified decimal delimiter  */
	/*  with the one, taken from locale settings and use standard strtod()  */
	/*  on that buffer.                                                     */
	/* -------------------------------------------------------------------- */
	double      dfValue;
	int         nError;

	char*       pszNumber = cslReplacePointByLocalePoint(nptr, point);

	dfValue = strtod(pszNumber, endptr);
	nError = errno;

	if (endptr)
		*endptr = (char *)nptr + (*endptr - pszNumber);

	if (pszNumber != (char*)nptr)
		free(pszNumber);

	errno = nError;
	return dfValue;
}
double cslStringToNumber(const char *nptr) {
	return cslStringToNumber(nptr, 0, '.');
}
/**
* Converts ASCII string to floating point number using any numeric locale.
*
* This function converts the initial portion of the string pointed to
* by nptr to double floating point representation. This function does the
* same as standard atof(), but it allows a variety of locale representations.
* That is it supports numeric values with either a comma or a period for
* the decimal delimiter.
*
* PS. The M stands for Multi-lingual.
*
* @param nptr The string to convert.
*
* @return Converted value, if any.  Zero on failure.
*/
double cslMultilingualStringToNumber(const char *nptr) {
	int i;
	const static int nMaxSearch = 50;

	for (i = 0; i < nMaxSearch; i++)
	{
		if (nptr[i] == ',')
			return cslStringToNumber(nptr, 0, ',');
		else if (nptr[i] == '.' || nptr[i] == '\0')
			return cslStringToNumber(nptr, 0, '.');
	}

	return cslStringToNumber(nptr, 0, '.');
}
/**
* Test what boolean value contained in the string.
*
* If pszValue is "NO", "FALSE","F", "OFF" or "0" will be returned FALSE.
* Otherwise, TRUE will be returned.
*
* @param pszValue the string should be tested.
*
* @return TRUE or FALSE.
*/

bool cslTestBoolean(const char *pszValue)
{
	if (cslIEqualString(pszValue, "NO")
		|| cslIEqualString(pszValue, "FALSE")
		|| cslIEqualString(pszValue, "F")
		|| cslIEqualString(pszValue, "OFF")
		|| cslIEqualString(pszValue, "0"))
		return FALSE;
	else
		return TRUE;
}
/************************************************************************/
/*                      cslReplacePointByLocalePoint()                  */
/************************************************************************/

char* cslReplacePointByLocalePoint(const char* pszNumber, char point)
{
#if defined(WIN32CE) || defined(__ANDROID__)
	static char byPoint = 0;
	if (byPoint == 0)
	{
		char szBuf[16];
		sprintf(szBuf, "%.1f", 1.0);
		byPoint = szBuf[1];
	}
	if (point != byPoint)
	{
		const char* pszPoint = strchr(pszNumber, point);
		if (pszPoint)
		{
			char* pszNew = CPLStrdup(pszNumber);
			pszNew[pszPoint - pszNumber] = byPoint;
			return pszNew;
		}
	}
#else
	struct lconv *poLconv = localeconv();
	if (poLconv
		&& poLconv->decimal_point
		&& strlen(poLconv->decimal_point) > 0)
	{
		char    byPoint = poLconv->decimal_point[0];

		if (point != byPoint)
		{
			const char* pszPoint = strchr(pszNumber, point);
			if (pszPoint)
			{
				char* pszNew = cslDuplicateString(pszNumber);
				pszNew[pszPoint - pszNumber] = byPoint;
				return pszNew;
			}
		}
	}
#endif
	return (char*)pszNumber;
}


//��ȡһ�У�Ҳ������13��10ֹͣ
const char * readLine(FILE * fp) {	 
	std::string result; 
	if (readLine(fp, result) > 0)
		return result.c_str();
	else
		return 0;
}
size_t readLine(FILE * fp, std::string & result) {
	int c = fgetc(fp);
	char buf[512];
	int i = 0;
	std::string sz;
	while (c != 10 && c != 13 && !feof(fp)) {
		buf[i++] = (char)c;
		if (i == 512) {
			sz.assign(buf, 512);
			result = result + sz;
			i = 0;
		}
		c = fgetc(fp);
	}
	if (i > 0 && i < 512) {
		sz.assign(buf, i);
		result = result + sz;
	}
	return result.size();
}
/////////////////////////////////////////////////////////////////////

bool doubleEqual(double d1, double d2) {
	if (fabs(d1 - d2) < TOLERANCE)
		return true;
	else
		return false;
}

bool floatEqual(float d1, float d2) {
	if (fabs(d1 - d2) < TOLERANCE)
		return true;
	else
		return false;
}

void stringToWString(std::string & s, std::wstring & sd) {
	sd.clear();
	sd.resize(s.length());
	std::wstring::iterator it1 = sd.begin();
	for (std::string::iterator it = s.begin(); it != s.end(); it++, it1++) {
		*it1 = (wchar_t)(*it);
	}
}
//ֻ�д���ͬһ���뷽ʽ������²�����ȷ�ģ����������ϵ��ͬ���򱾺���������ȷ��ִ��
void wstringToString(std::wstring & s, std::string & sd) {
	sd.clear();
	for (std::wstring::iterator it = s.begin(); it != s.end(); it++) {
		char h = (char)((*it) >> 8);
		char l = (char)(*it);

		if (h != 0)
			sd.append(1, (char)h);
		sd.append(1, (char)l);
	}
}

void getCurTime(std::string & sz){
	time_t now;
	time(&now);
	tm   t;
	localtime_s(&t, &now);
	sz = numberToString<char, long>(t.tm_year + 1900) + "-" + numberToString<char, long>(t.tm_mon + 1) + "-" + numberToString<char, long>(t.tm_mday) + " " + numberToString<char, long>(t.tm_hour) + ":" + numberToString<char, long>(t.tm_min) + ":" + numberToString<char, long>(t.tm_sec);
}

std::string getCurTime(){
	std::string   sz;
	getCurTime(sz);
	return sz;
}
///////////////////////////////////////////////////////////////////////////////////
bool IsCCW(Vector3d * pdData, unsigned int * ps);
void GetConcavePos(Vector3d * pdData, std::vector<int> * rPS,
	std::vector<int> * cPos);
bool IsPartOfPolygon(unsigned int a, unsigned int b, unsigned int c, Vector3d * pdData,
	std::vector<int> * cPos);
int GetIntersectOfTwoLine(Vector3d A, Vector3d B,
	Vector3d C, Vector3d D,
	Vector3d P);
double GetDistance(const Vector3d A, const Vector3d B);
double Get_Angle_C_Of_ABC(Vector3d A, Vector3d B, Vector3d C);
void SetAtoB(Vector3d B, const Vector3d A);
void GetTriNormal(double dCoef[4], const Vector3d A, const Vector3d B,
	const Vector3d C, const bool bNormal);
double GetAverage(std::vector<double>& daZ, std::vector<double>* paAlpha);
//-------------------------------------------------------------------------
//*************************************************************************
// ʸ������
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	AaddB
// ��    ��:	ʸ���ӷ� Vr = A + B
// ��    ��:	const Vector3d A, const Vector3d B, Vector3d C
//   (���):	A: ʸ��A
//				B: ʸ��B
//				C: ʸ��C
//   (����):	C: ʸ��C
// ��    ��:	��
// ���÷���:	Vector3d A = {0.0, 1.0, 2.0},
//						B = {3.0, 4.0, 5.0},C;
//				AaddB(A, B, C);
//				�� C = {3.0, 5.0, 7.0}
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
void AaddB(const Vector3d A, const Vector3d B, Vector3d C)
////GeoView_Tech_Mark_End
{
	C[0] = A[0] + B[0];
	C[1] = A[1] + B[1];
	C[2] = A[2] + B[2];
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	AsubB
// ��    ��:	ʸ������ Vr = A - B
// ��    ��:	const Vector3d A, const Vector3d B, Vector3d C
//   (���):	A: ʸ��A
//				B: ʸ��B
//				C: ʸ��C
//   (����):	C: ʸ��C
// ��    ��:	Vector3d A = {0.0, 1.0, 2.0},
//						B = {3.0, 4.0, 5.0}, C;
//				AsubB(A, B, C);
//				�� C = {-3.0, -3.0, -3.0}
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
void AsubB(const Vector3d A, const Vector3d B, Vector3d C)
////GeoView_Tech_Mark_End
{
	C[0] = A[0] - B[0];
	C[1] = A[1] - B[1];
	C[2] = A[2] - B[2];
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	AdotB
// ��    ��:	ʸ����� Vr = A��B
// ��    ��:	const Vector3d A, const Vector3d B
//   (���):	A: ʸ��A
//				B: ʸ��B
//   (����):	��
// ��    ��:	double: ʸ�����
// ���÷���:	Vector3d A = {0.0, 1.0, 2.0},
//						B = {3.0, 4.0, 5.0};
//				double dRes = AdotB(A, B);
//				�� dRes = 14.0
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
double AdotB(const Vector3d A, const Vector3d B)
////GeoView_Tech_Mark_End
{
	double dRtn = A[0] * B[0] +
		A[1] * B[1] +
		A[2] * B[2];
	return dRtn;
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	AxB
// ��    ��:	ʸ����� Vr = A��B
// ��    ��:	const Vector3d A, const Vector3d B, Vector3d V
//   (���):	A: ʸ��A
//				B: ʸ��B
//				V: ʸ��V
//   (����):	V: ʸ��V
// ��    ��:	��
// ���÷���:	Vector3d A = {0.0, 1.0, 2.0},
//						B = {3.0, 4.0, 5.0}, V;
//				AxB(A, B, V);
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
void AxB(const Vector3d A, const Vector3d B, Vector3d V)
////GeoView_Tech_Mark_End
{
	V[0] = A[1] * B[2] - A[2] * B[1];
	V[1] = A[2] * B[0] - A[0] * B[2];
	V[2] = A[0] * B[1] - A[1] * B[0];
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	Vec
// ��    ��:	����������
// ��    ��:	const double r, const double s, const double t, Vector3d A
//   (���):	r: X�������
//				s: Y�������
//				t: Z�������
//				A: ����
//   (����):	A: ����,�����������ֱ�Ϊr, s, t
// ��    ��:	��
// ���÷���:	double r = 1.0, s = 2.0, t = 3.0;
//				Vector3d A;
//				Vec(r, s, t, A);
//				�� A = {1.0, 2.0, 3.0}
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
void Vec(const double r, const double s, const double t, Vector3d A)
////GeoView_Tech_Mark_End
{
	A[0] = r;
	A[1] = s;
	A[2] = t;
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	UnVec
// ��    ��:	�������껯
// ��    ��:	const Vector3d A, double *r, double *s, double *t
//   (���):	A: ����
//				r: X�����ָ��
//				s: Y�����ָ��
//				t: Z�����ָ��
//   (����):	r: X�����ָ��
//				s: Y�����ָ��
//				t: Z�����ָ��
// ��    ��:	��
// ���÷���:	double r = 0.0, s = 0.0, t = 0.0;
//				Vector3d A = {1.0, 2.0, 3.0};
//				UnVec(A, &r, &s, &t);
//				�� r = 1.0, s = 2.0, t = 3.0
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
void UnVec(const Vector3d A, double *r, double *s, double *t)
////GeoView_Tech_Mark_End
{
	*r = A[0];
	*s = A[1];
	*t = A[2];
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	VecLen
// ��    ��:	ʸ������
// ��    ��:	const Vector3d A
//   (���):	A: ʸ��
//   (����):	��
// ��    ��:	double: ʸ���ĳ���(ŷ�Ͽռ�)
// ���÷���:	Vector3d A = {1.0, 2.0, 2.0};
//				double dLength = VecLen(A);
//				�� dLength = 3.0
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
double VecLen(const Vector3d A)
////GeoView_Tech_Mark_End
{
	return sqrt(A[0] * A[0] + A[1] * A[1] + A[2] * A[2]);
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	VecNormalize
// ��    ��:	ʸ����һ��
// ��    ��:	Vector3d A
//   (���):	A: ʸ��
//   (����):	A: ��һ�����ʸ��
// ��    ��:	double: ԭʸ���ĳ���
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
double VecNormalize(Vector3d A)
////GeoView_Tech_Mark_End
{
	double dist = 0.0, invdist = 0.0;

	dist = VecLen(A);
	if (dist>0.0)
	{
		invdist = 1.0 / dist;
		A[0] *= invdist;
		A[1] *= invdist;
		A[2] *= invdist;
	}
	return dist;
}
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	VecNormalize
// ��    ��:	ʸ����һ��
// ��    ��:	Vertex3d &A
//   (���):	A: ʸ��
//   (����):	A: ��һ�����ʸ��
// ��    ��:	double: ԭʸ���ĳ���
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
double VecNormalize(Vertex3d &A)
////GeoView_Tech_Mark_End
{
	double invdist, dist = sqrt(A.x*A.x + A.y*A.y + A.z*A.z);
	if (dist>0.0)
	{
		invdist = 1 / dist;
		A.x *= invdist;
		A.y *= invdist;
		A.z *= invdist;
	}
	return dist;
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	VecCopy
// ��    ��:	ʸ������
// ��    ��:	const Vector3d A, Vector3d B
//   (���):	A:	Դʸ��
//				B:	Ŀ��ʸ��(����)
//   (����):	B:	Ŀ��ʸ��
// ��    ��:	��
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-29, Wengzp
// �޸�����:	
//==================================================================//
void VecCopy(const Vector3d A, Vector3d B)
////GeoView_Tech_Mark_End
{
	//	B[0] = A[0];
	//	B[1] = A[1];
	//	B[2] = A[2];
	memcpy(B, A, sizeof(Vector3d));
}

//��ȡ����ʸ���ļн�
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	VecAngle
// ��    ��:	��ȡ����ʸ���ļн�
// ��    ��:	const Vector3d A, Vector3d B
//   (���):	A:	Դʸ��
//				B:	Դʸ��
//   (����):	
// ��    ��:	��ʸ���ļн�
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-29, Wengzp
// �޸�����:	
//==================================================================//
double VecAngle(Vector3d A, Vector3d B)
////GeoView_Tech_Mark_End
{
	double lfRgn = AdotB(A, B);
	double lfLA = VecLen(A);
	double lfLB = VecLen(B);
	double cosa = lfRgn / (lfLA*lfLB);
	return acos(cosa);
}
//-------------------------------------------------------------------------
//*************************************************************************
// ���Ǻ���
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	Radiansf
// ��    ��:	�Ƕ�תΪ����
// ��    ��:	double dDegree
//   (���):	dDegree: �Ƕ�(��λ: ��)
//   (����):	��
// ��    ��:	double: ת����Ļ���ֵ
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
double Radiansf(double dDegree)
////GeoView_Tech_Mark_End
{
	return dDegree*PI / 180;
}

////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	Degreesf
// ��    ��:	����תΪ�Ƕ�
// ��    ��:	double dRadian
//   (���):	dRadian: �Ƕ�(��λ: ����)
//   (����):	��
// ��    ��:	double: ת����ĽǶ�ֵ
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
double Degreesf(double dRadian)
////GeoView_Tech_Mark_End
{
	return dRadian*PI / 180;
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	Cosf
// ��    ��:	�Ƕȵ�����
// ��    ��:	double dDegree
//   (���):	dDegree: �Ƕ�(��λ: ��)
//   (����):	��
// ��    ��:	double: ����ֵ
// ���÷���:	double dRes = Cosf(60.0);
//				�� dRes = 0.5;
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
double Cosf(double dDegree)
////GeoView_Tech_Mark_End
{
	return cos(Radiansf(dDegree));
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	Sinf
// ��    ��:	�Ƕȵ�����
// ��    ��:	double dDegree
//   (���):	dDegree: �Ƕ�(��λ: ��)
//   (����):	��
// ��    ��:	double: ����ֵ
// ���÷���:	double dRes = Sinf(30.0);
//				�� dRes = 0.5;
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
double Sinf(double dDegree)
////GeoView_Tech_Mark_End
{
	return sin(Radiansf(dDegree));
}

// ��������
int Roundf(double dX)
{
	if (dX<0)
	{
		return (int)(dX - 0.5);
	}
	else
	{
		return (int)(dX + 0.5);
	}
}

//-------------------------------------------------------------------------
//*************************************************************************
// ��ֵ����
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	Linear2D
// ��    ��:	ƽ�����Բ�ֵ
// ��    ��:	double x[], double y[], double tx[], double ty[], int n, int m
//   (���):	n: �����
//				m: ����ֵ����
//				x[]: һά����, ����X����ֵ( X0 < X1 < X2 < ... < Xn-1 )
//				y[]: һά����, ����Y����ֵ
//				tx[]: һά����, ����ֵ���X����ֵ
//				ty[]: һά����, ����ֵ���Y����ֵ(����)
//   (����):	ty[]: һά����, ����ֵ���Y����ֵ
// ��    ��:	��
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
void Linear2D(double x[], double y[], double tx[], double ty[], int n, int m)
////GeoView_Tech_Mark_End
{
	for (int i = 0, k = 0; i<m; i++)
	{
		k = 0;
		if (tx[i] >= x[n - 1]) k = n - 2;
		else
		{
			while (tx[i]>x[k + 1]) k++;
		}
		ty[i] = (y[k] * (x[k + 1] - tx[i]) + y[k + 1] * (tx[i] - x[k])) / (x[k + 1] - x[k]);
	}
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	Linear3D
// ��    ��:	��ά���Բ�ֵ
// ��    ��:	double x[], double y[], double z[], double tx[], double ty[], double tz[], int n, int m
//   (���):	n:	��������
//				m:	ÿ�εķֶ���
//				x[]: һά����, ����X����ֵ
//				y[]: һά����, ����Y����ֵ
//				z[]: һά����, ����Z����ֵ
//				tx[]: һά����, ����ֵ��(�������)��X����ֵ(����)
//				ty[]: һά����, ����ֵ��(�������)��Y����ֵ(����)
//				tz[]: һά����, ����ֵ��(�������)��Z����ֵ(����)
//   (����):	tx[]: һά����, ����ֵ��(�������)��X����ֵ
//				ty[]: һά����, ����ֵ��(�������)��Y����ֵ
//				tz[]: һά����, ����ֵ��(�������)��Z����ֵ
// ��    ��:	��
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	��ÿ������������ϵľ���DΪ�����, X, Y, Z �ֱ���ΪD�ĺ�������
//				��ά�����Բ�ֵ; ����, ����ֵ������������ȷ����: ���� n �����, 
//				����(n-1)��, ��ÿ��ƽ���ֳ� m ��, �����ֵ������Ϊ m(n-1)+1
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
void Linear3D(double x[], double y[], double z[], double tx[], double ty[], double tz[], int n, int m)
////GeoView_Tech_Mark_End
{
	int nTotal, i, j, k;
	double *dis, *disT, deltaDis = 0.0f;

	nTotal = m*(n - 1) + 1;
	dis = (double*)malloc(n*sizeof(double));
	disT = (double*)malloc(nTotal*sizeof(double));
	dis[0] = disT[0] = 0.0f;

	for (i = 1, k = 1; i<n; i++)
	{
		deltaDis = sqrt((x[i] - x[i - 1])*(x[i] - x[i - 1]) +
			(y[i] - y[i - 1])*(y[i] - y[i - 1]) +
			(z[i] - z[i - 1])*(z[i] - z[i - 1]));
		dis[i] = dis[i - 1] + deltaDis;
		deltaDis = deltaDis / m;

		for (j = 1; j <= m; j++, k++)
		{
			disT[k] = disT[k - 1] + deltaDis;
		}
	}

	Linear2D(dis, x, disT, tx, n, nTotal);
	Linear2D(dis, y, disT, ty, n, nTotal);
	Linear2D(dis, z, disT, tz, n, nTotal);

	free(dis);
	free(disT);
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	GetPartsPoint
// ��    ��:	��ȡ�ߵĵȷݵ�
// ��    ��:	std::vector<Vertex3d> &Line, std::vector<Vertex3d> &Parts, unsigned int dwParts = 1
//   (���):	Line:		������
//				Parts:		���صĵȷݵ�����
//				dwParts:	�ȷ���
//   (����):	Parts:		���صĵȷݵ�����
// ��    ��:	��
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	
// ��    ��:	
// ��������:	2005-10-11, Wengzp: add notes.
// �޸�����:	
//==================================================================//
void GetPartsPoint(std::vector<Vertex3d> &Line, std::vector<Vertex3d> &Parts, unsigned int dwParts)
////GeoView_Tech_Mark_End
{
	unsigned int dwLine = Line.size();
	double dLength = 0.0, // �����ĳ���
		dDist = 0.0;	// �����ĵȷݾ���
	unsigned int i = 0, j = 0;
	std::vector<double> daLength;	// ������ÿ�γ���

	if (dwLine<2 || dwParts<1)
	{
		return;
	}

	// ������շ��ص�����
	Parts.clear();
	Parts.resize(dwParts + 1);	// �ȷֵ�(�ȵȷ�����1)	
	daLength.resize(dwLine - 1);	// ÿ�εĳ���

	// �����ܳ���ÿ�γ���
	for (i = 0; i<dwLine - 1; i++)
	{
		daLength[i] = sqrt((Line[i].x - Line[i + 1].x)*(Line[i].x - Line[i + 1].x) +
			(Line[i].y - Line[i + 1].y)*(Line[i].y - Line[i + 1].y) +
			(Line[i].z - Line[i + 1].z)*(Line[i].z - Line[i + 1].z));
		dLength += daLength[i];
	}
	dDist = dLength / dwParts;	// �ȷ־���

	// �������
	double dL = daLength[0], dD = dDist;
	Parts[0] = Line[0];
	for (i = 1, j = 0; i<dwParts; i++)
	{
		while (dD>dL)
		{
			j++;
			dL += daLength[j];
		}
		Parts[i].x = (Line[j].x*(dL - dD) + Line[j + 1].x*(daLength[j] - dL + dD)) / daLength[j];
		Parts[i].y = (Line[j].y*(dL - dD) + Line[j + 1].y*(daLength[j] - dL + dD)) / daLength[j];
		Parts[i].z = (Line[j].z*(dL - dD) + Line[j + 1].z*(daLength[j] - dL + dD)) / daLength[j];
		dD += dDist;
	}
	Parts[dwParts] = Line[dwLine - 1];


	daLength.clear();
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	Spline2D
// ��    ��:	ƽ��������ֵ
// ��    ��:	double x[], double y[], double tx[], double ty[], int n, int m
//   (���):	n: �����
//				m: ����ֵ����
//				x[]: һά����, ����X����ֵ( X0 < X1 < X2 < ... < Xn-1 )
//				y[]: һά����, ����Y����ֵ
//				tx[]: һά����, ����ֵ���X����ֵ(����)
//				ty[]: һά����, ����ֵ���Y����ֵ(����)
//   (����):	tx[]: һά����, ����ֵ���X����ֵ
//				ty[]: һά����, ����ֵ���Y����ֵ
// ��    ��:	double: ���X��Χ�ɵ��������(����)
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:	��ʿ��. C�����㷨����(Edition 2). ����: �廪��ѧ������.1996
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
double Spline2D(double x[], double y[], double t[], double z[], int n, int m)
////GeoView_Tech_Mark_End
{
	int i, j;
	double h0, h1, alpha, beta, g, *s, *dy, *ddy, *dz, *ddz;

	if (n == 2)/// ������Ϊ����, ��ֱ�����Բ�ֵ
	{
		Linear2D(x, y, t, z, n, m);
		return 0.0;
	}

	// ����������:
	// g  :	������ [X0, Xn-1] �ϵĻ���(Χ�ɵ����)
	// dy :	�����ڽ�㴦��һ�׵���ֵ
	// ddy: �����ڽ�㴦�Ķ��׵���ֵ
	// dz : �����ڴ���ֵ�㴦��һ�׵���ֵ
	// ddz: �����ڴ���ֵ�㴦�Ķ��׵���ֵ
	// ����������Ϊ�����, ����ʵ��Ӧ��ʱ, û�б�Ҫ֪������ֵ, ����ں������
	s = (double*)malloc(n*sizeof(double));
	dy = (double*)malloc(n*sizeof(double));
	ddy = (double*)malloc(n*sizeof(double));
	// ԭ����, �ڶ��ֱ߽�������ddy[0]��ddy[n-1]���û�������, ������Ϊ�˷������, 
	//		��Ϊ0, ��ʾ�ǹ⻬��, ���������һ������ߵ������⻬���������
	ddy[0] = 0.0f;
	ddy[n - 1] = 0.0f;
	dz = (double*)malloc(m*sizeof(double));
	ddz = (double*)malloc(m*sizeof(double));

	dy[0] = -0.5;
	h0 = x[1] - x[0];
	s[0] = 3.0f*(y[1] - y[0]) / (2.0f*h0) - ddy[0] * h0 / 4.0f;
	for (j = 1; j <= n - 2; j++)
	{
		h1 = x[j + 1] - x[j];
		alpha = h0 / (h0 + h1);
		beta = (1.0f - alpha)*(y[j] - y[j - 1]) / h0;
		beta = 3.0f*(beta + alpha*(y[j + 1] - y[j]) / h1);
		dy[j] = -alpha / (2.0f + (1.0f - alpha)*dy[j - 1]);
		s[j] = (beta - (1.0f - alpha)*s[j - 1]);
		s[j] = s[j] / (2.0f + (1.0f - alpha)*dy[j - 1]);
		h0 = h1;
	}
	dy[n - 1] = (3.0f*(y[n - 1] - y[n - 2]) / h1 + ddy[n - 1] * h1 / 2.0f - s[n - 2]) / (2.0f + dy[n - 2]);
	for (j = n - 2; j >= 0; j--)
		dy[j] = dy[j] * dy[j + 1] + s[j];
	for (j = 0; j <= n - 2; j++)
		s[j] = x[j + 1] - x[j];
	for (j = 0; j <= n - 2; j++)
	{
		h1 = s[j] * s[j];
		ddy[j] = 6.0f*(y[j + 1] - y[j]) / h1 - 2.0f*(2.0f*dy[j] + dy[j + 1]) / s[j];
	}
	h1 = s[n - 2] * s[n - 2];
	ddy[n - 1] = 6.f*(y[n - 2] - y[n - 1]) / h1 + 2.f*(2.f*dy[n - 1] + dy[n - 2]) / s[n - 2];
	g = 0.0;
	for (i = 0; i <= n - 2; i++)
	{
		h1 = 0.5f*s[i] * (y[i] + y[i + 1]);
		h1 = h1 - s[i] * s[i] * s[i] * (ddy[i] + ddy[i + 1]) / 24.0f;
		g = g + h1;
	}
	for (j = 0; j <= m - 1; j++)
	{
		if (t[j] >= x[n - 1]) i = n - 2;
		else
		{
			i = 0;
			while (t[j]>x[i + 1]) i = i + 1;
		}
		h1 = (x[i + 1] - t[j]) / s[i];
		h0 = h1*h1;
		z[j] = (3.0f*h0 - 2.0f*h0*h1)*y[i];
		z[j] = z[j] + s[i] * (h0 - h0*h1)*dy[i];
		dz[j] = 6.0f*(h0 - h1)*y[i] / s[i];
		dz[j] = dz[j] + (3.0f*h0 - 2.0f*h1)*dy[i];
		ddz[j] = (6.0f - 12.0f*h1)*y[i] / (s[i] * s[i]);
		ddz[j] = ddz[j] + (2.0f - 6.0f*h1)*dy[i] / s[i];
		h1 = (t[j] - x[i]) / s[i];
		h0 = h1*h1;
		z[j] = z[j] + (3.0f*h0 - 2.0f*h0*h1)*y[i + 1];
		z[j] = z[j] - s[i] * (h0 - h0*h1)*dy[i + 1];
		dz[j] = dz[j] - 6.0f*(h0 - h1)*y[i + 1] / s[i];
		dz[j] = dz[j] + (3.0f*h0 - 2.0f*h1)*dy[i + 1];
		ddz[j] = ddz[j] + (6.0f - 12.0f*h1)*y[i + 1] / (s[i] * s[i]);
		ddz[j] = ddz[j] - (2.0f - 6.0f*h1)*dy[i + 1] / s[i];
	}
	free(s);
	free(dy);
	free(ddy);
	free(dz);
	free(ddz);
	return(g);
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	Spline3D
// ��    ��:	�ռ�������ֵ
// ��    ��:	double x[], double y[], double z[], double tx[], double ty[], double tz[], int n, int m
//   (���):	n:	��������
//				m:	ÿ�εķֶ���
//				x[]: һά����, ����X����ֵ
//				y[]: һά����, ����Y����ֵ
//				z[]: һά����, ����Z����ֵ
//				tx[]: һά����, ����ֵ��(�������)��X����ֵ(����)
//				ty[]: һά����, ����ֵ��(�������)��Y����ֵ(����)
//				tz[]: һά����, ����ֵ��(�������)��Z����ֵ(����)
//   (����):	tx[]: һά����, ����ֵ��(�������)��X����ֵ
//				ty[]: һά����, ����ֵ��(�������)��Y����ֵ
//				tz[]: һά����, ����ֵ��(�������)��Z����ֵ
// ��    ��:	��
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	��ÿ������������ϵľ���DΪ�����, X, Y, Z �ֱ���ΪD�ĺ������ж�ά��
//				������ֵ(�ڶ��ֱ߽�����)
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
void Spline3D(double x[], double y[], double z[], double tx[], double ty[], double tz[], int n, int m)
////GeoView_Tech_Mark_End
{
	int nTotal, i, j, k;
	double *dis, *disT, deltaDis = 0.0f;

	nTotal = m*(n - 1) + 1;
	dis = (double*)malloc(n*sizeof(double));
	disT = (double*)malloc(nTotal*sizeof(double));
	dis[0] = disT[0] = 0.0f;

	for (i = 1, k = 1; i<n; i++)
	{
		deltaDis = sqrt((x[i] - x[i - 1])*(x[i] - x[i - 1]) +
			(y[i] - y[i - 1])*(y[i] - y[i - 1]) +
			(z[i] - z[i - 1])*(z[i] - z[i - 1]));
		dis[i] = dis[i - 1] + deltaDis;
		deltaDis = deltaDis / m;

		for (j = 1; j <= m; j++, k++)
		{
			disT[k] = disT[k - 1] + deltaDis;
		}
	}

	Spline2D(dis, x, disT, tx, n, nTotal);
	Spline2D(dis, y, disT, ty, n, nTotal);
	Spline2D(dis, z, disT, tz, n, nTotal);

	for (i = 0, j = 0; i<n; i++, j += m)
	{
		tx[j] = x[i];
		ty[j] = y[i];
		tz[j] = z[i];
	}
	free(dis);
	free(disT);
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	Achol
// ��    ��:	ƽ���������Գ�����������
// ��    ��:	double* a, int n, int m, double* d
//   (���):	a:	n��n����, ���ϵ������(ӦΪ�Գ���������)
//				n:	������Ľ���
//				m:	�������Ҷ˳��������ĸ���
//				d:	n��m����, ��ŷ������Ҷ�m�鳣������
//   (����):	a:	n��n����, �����ǲ��ִ�ŷֽ���U����
//				d:	n��m����, ��ŷ������m���
// ��    ��:	int:	<0: ������ʧ��(���������), >0: ��������
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	AX = D
// ��    ��:	��ʿ��.C�����㷨����(�ڶ���).����:�廪��ѧ������, 2001, p17
// ��������:	2006-1-17, Wengzp: add notes.
// �޸�����:	
//==================================================================//
int Achol(double* a, int n, int m, double* d)
////GeoView_Tech_Mark_End
{
	int i, j, k, u, v;
	if ((a[0] + 1.0 == 1.0) || (a[0]<0.0))
	{
		return(-2);
	}
	a[0] = sqrt(a[0]);
	for (j = 1; j <= n - 1; j++) a[j] = a[j] / a[0];
	for (i = 1; i <= n - 1; i++)
	{
		u = i*n + i;
		for (j = 1; j <= i; j++)
		{
			v = (j - 1)*n + i;
			a[u] = a[u] - a[v] * a[v];
		}
		if ((a[u] + 1.0 == 1.0) || (a[u]<0.0))
		{
			return(-2);
		}
		a[u] = sqrt(a[u]);
		if (i != (n - 1))
		{
			for (j = i + 1; j <= n - 1; j++)
			{
				v = i*n + j;
				for (k = 1; k <= i; k++)
					a[v] = a[v] - a[(k - 1)*n + i] * a[(k - 1)*n + j];
				a[v] = a[v] / a[u];
			}
		}
	}
	for (j = 0; j <= m - 1; j++)
	{
		d[j] = d[j] / a[0];
		for (i = 1; i <= n - 1; i++)
		{
			u = i*n + i; v = i*m + j;
			for (k = 1; k <= i; k++)
				d[v] = d[v] - a[(k - 1)*n + i] * d[(k - 1)*m + j];
			d[v] = d[v] / a[u];
		}
	}
	for (j = 0; j <= m - 1; j++)
	{
		u = (n - 1)*m + j;
		d[u] = d[u] / a[n*n - 1];
		for (k = n - 1; k >= 1; k--)
		{
			u = (k - 1)*m + j;
			for (i = k; i <= n - 1; i++)
			{
				v = (k - 1)*n + i;
				d[u] = d[u] - a[v] * d[i*m + j];
			}
			v = (k - 1)*n + k - 1;
			d[u] = d[u] / a[v];
		}
	}
	return(2);
}

// ȫѡ��Ԫ��˹��ȥ�������Է�����
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	Agauss
// ��    ��:	ȫѡ��Ԫ��˹��ȥ�������Է�����
// ��    ��:	double* a, double* b, int n 
//   (���):	a:	n��n����, ��ŷ������ϵ������, ����ʱ���ƻ�
//				b:	һά����, ����Ϊn, ��ŷ������Ҷ˵ĳ�������
//				n:	������Ľ���
//   (����):	a:	n��n����, �����ǲ��ִ�ŷֽ��ľ���
//				b:	һά����, ����Ϊn, ��ŷ�����Ľ�����
// ��    ��:	int:	0: ʧ��; 1: �ɹ�
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	
// ��    ��:	��ʿ��.C�����㷨����(�ڶ���).����:�廪��ѧ������, 2001, p17
// ��������:	2006-1-22, zhangdan: add notes.
// �޸�����:	
//==================================================================//
int Agauss(double* a, double* b, int n)
////GeoView_Tech_Mark_End
{
	int *js = NULL, l, k, i, j, is, p, q;
	double d, t;
	js = (int*)malloc(n*sizeof(int));
	l = 1;
	for (k = 0; k <= n - 2; k++)
	{
		d = 0.0;
		for (i = k; i <= n - 1; i++)
		{
			for (j = k; j <= n - 1; j++)
			{
				t = fabs(a[i*n + j]);
				if (t>d)
				{
					d = t; js[k] = j; is = i;
				}
			}
		}
		if (d + 1.0 == 1.0)
			l = 0;
		else
		{
			if (js[k] != k)
				for (i = 0; i <= n - 1; i++)
				{
					p = i*n + k;
					q = i*n + js[k];
					t = a[p];
					a[p] = a[q];
					a[q] = t;
				}
			if (is != k)
			{
				for (j = k; j <= n - 1; j++)
				{
					p = k*n + j;
					q = is*n + j;
					t = a[p];
					a[p] = a[q];
					a[q] = t;
				}
				t = b[k]; b[k] = b[is]; b[is] = t;
			}
		}
		if (l == 0)
		{
			free(js);
			printf("fail\n");
			return(0);
		}
		d = a[k*n + k];
		for (j = k + 1; j <= n - 1; j++)
		{
			p = k*n + j;
			a[p] = a[p] / d;
		}
		b[k] = b[k] / d;
		for (i = k + 1; i <= n - 1; i++)
		{
			for (j = k + 1; j <= n - 1; j++)
			{
				p = i*n + j;
				a[p] = a[p] - a[i*n + k] * a[k*n + j];
			}
			b[i] = b[i] - a[i*n + k] * b[k];
		}
	}
	d = a[(n - 1)*n + n - 1];
	if (fabs(d) + 1.0 == 1.0)
	{
		free(js);
		printf("fail\n");
		return(0);
	}
	b[n - 1] = b[n - 1] / d;
	for (i = n - 2; i >= 0; i--)
	{
		t = 0.0;
		for (j = i + 1; j <= n - 1; j++)
		{
			t = t + a[i*n + j] * b[j];
		}
		b[i] = b[i] - t;
	}
	js[n - 1] = n - 1;
	for (k = n - 1; k >= 0; k--)
	{
		if (js[k] != k)
		{
			t = b[k]; b[k] = b[js[k]]; b[js[k]] = t;
		}
	}
	free(js);
	return(1);
}

// ȫѡ��Ԫ��˹-Լ����ȥ�������Է�����
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	AgaussJordan
// ��    ��:	ȫѡ��Ԫ��˹-Լ����ȥ�������Է�����
// ��    ��:	double* a, double* b, int n, int m
//   (���):	a:	n��n����, ��ŷ������ϵ������, ����ʱ���ƻ�
//				b:	n��m����, ��ŷ������Ҷ˵�m�鳣������(һ��Ϊһ��)
//				n:	������Ľ���
//				m:	�������Ҷ˳��������ĸ���
//   (����):	a:	n��n����, ��ŷ������ϵ������, ����ʱ���ƻ�
//				b:	n��m����, ���ش�ŵ�m�������
// ��    ��:	int:	0: ʧ��; 1: �ɹ�
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	
// ��    ��:	
// ��������:	2006-3-7, Wengzp: add notes.
// �޸�����:	
//==================================================================//
int AgaussJordan(double* a, double* b, int n, int m)
////GeoView_Tech_Mark_End
{
	int *js, l, k, i, j, is, p, q;
	double d, t;
	js = (int*)malloc(n*sizeof(int));
	l = 1;
	for (k = 0; k <= n - 1; k++)
	{
		d = 0.0;
		for (i = k; i <= n - 1; i++)
		{
			for (j = k; j <= n - 1; j++)
			{
				t = fabs(a[i*n + j]);
				if (t>d)
				{
					d = t;
					js[k] = j;
					is = i;
				}
			}
		}
		if (d + 1.0 == 1.0) l = 0;
		else
		{
			if (js[k] != k)
			{
				for (i = 0; i <= n - 1; i++)
				{
					p = i*n + k;
					q = i*n + js[k];
					t = a[p];
					a[p] = a[q];
					a[q] = t;
				}
			}
			if (is != k)
			{
				for (j = k; j <= n - 1; j++)
				{
					p = k*n + j;
					q = is*n + j;
					t = a[p];
					a[p] = a[q];
					a[q] = t;
				}
				for (j = 0; j <= m - 1; j++)
				{
					p = k*m + j;
					q = is*m + j;
					t = b[p];
					b[p] = b[q];
					b[q] = t;
				}
			}
		}
		if (l == 0)
		{
			free(js);
			printf("fail\n");
			return(0);
		}
		d = a[k*n + k];
		for (j = k + 1; j <= n - 1; j++)
		{
			p = k*n + j;
			a[p] = a[p] / d;
		}
		for (j = 0; j <= m - 1; j++)
		{
			p = k*m + j;
			b[p] = b[p] / d;
		}
		for (j = k + 1; j <= n - 1; j++)
		{
			for (i = 0; i <= n - 1; i++)
			{
				p = i*n + j;
				if (i != k)
					a[p] = a[p] - a[i*n + k] * a[k*n + j];
			}
		}
		for (j = 0; j <= m - 1; j++)
		{
			for (i = 0; i <= n - 1; i++)
			{
				p = i*m + j;
				if (i != k)
					b[p] = b[p] - a[i*n + k] * b[k*m + j];
			}
		}
	}
	for (k = n - 1; k >= 0; k--)
	{
		if (js[k] != k)
		{
			for (j = 0; j <= m - 1; j++)
			{
				p = k*m + j;
				q = js[k] * m + j;
				t = b[p];
				b[p] = b[q];
				b[q] = t;
			}
		}
	}
	free(js);
	return(1);
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	Isqt2
// ��    ��:	��Ԫ���Իع����
// ��    ��:	double* x, double* y, int m, int n, double* a, double* dt, double* v
//   (���):	x:	m��n����, ÿһ�д��m���Ա�����һ��۲�ֵ
//				y:	һά����, ����Ϊn, ����������y��n���۲�ֵ
//				m:	�Ա�������
//				n:	�۲����ݵ�����
//				a:	һά����, ����Ϊ(m+1), ���ػع�ϵ��a0, a1, ..., am-1, am
//				dt:	һά����, ����Ϊ4, ����, dt[0]����ƫ��ƽ����q, dt[1]����ƽ����׼ƫ��s,
//					dt[2]���ظ����ϵ��r, dt[3]���ػع�ƽ����u
//				v:	һά����, ����Ϊm, ����m���Ա�����ƫ���ϵ��
//   (����):	a:	һά����, ����Ϊ(m+1), ���ػع�ϵ��a0, a1, ..., am-1, am
//				dt:	һά����, ����Ϊ4, ����, dt[0]����ƫ��ƽ����q, dt[1]����ƽ����׼ƫ��s,
//					dt[2]���ظ����ϵ��r, dt[3]���ػع�ƽ����u
//				v:	һά����, ����Ϊm, ����m���Ա�����ƫ���ϵ��
// ��    ��:	��
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	Y = a0X0 + a1X1 + �� + am-1Xm-1 + am
// ��    ��:	��ʿ��.C�����㷨����(�ڶ���).����:�廪��ѧ������, 2001, p234
// ��������:	2006-1-17, Wengzp: add notes.
// �޸�����:	
//==================================================================//
void Isqt2(double* x, double* y, int m, int n, double* a, double* dt, double* v)
////GeoView_Tech_Mark_End
{
	int i = 0, j = 0, k = 0, mm = 0;
	double q, e, u, p, yy, s, r, pp, *b;
	b = (double*)malloc((m + 1)*(m + 1)*sizeof(double));
	mm = m + 1;
	b[mm*mm - 1] = n;
	for (j = 0; j <= m - 1; j++)
	{
		p = 0.0;
		for (i = 0; i <= n - 1; i++)
		{
			p = p + x[j*n + i];
		}
		b[m*mm + j] = p;
		b[j*mm + m] = p;
	}
	for (i = 0; i <= m - 1; i++)
	{
		for (j = i; j <= m - 1; j++)
		{
			p = 0.0;
			for (k = 0; k <= n - 1; k++)
			{
				p = p + x[i*n + k] * x[j*n + k];
			}
			b[j*mm + i] = p;
			b[i*mm + j] = p;
		}
	}
	a[m] = 0.0;
	for (i = 0; i <= n - 1; i++)
	{
		a[m] = a[m] + y[i];
	}
	for (i = 0; i <= m - 1; i++)
	{
		a[i] = 0.0;
		for (j = 0; j <= n - 1; j++)
		{
			a[i] = a[i] + x[i*n + j] * y[j];
		}
	}
	Achol(b, mm, 1, a);
	yy = 0.0;
	for (i = 0; i <= n - 1; i++)
	{
		yy = yy + y[i] / n;
	}
	q = 0.0; e = 0.0; u = 0.0;
	for (i = 0; i <= n - 1; i++)
	{
		p = a[m];
		for (j = 0; j <= m - 1; j++)
		{
			p = p + a[j] * x[j*n + i];
		}
		q = q + (y[i] - p)*(y[i] - p);
		e = e + (y[i] - yy)*(y[i] - yy);
		u = u + (yy - p)*(yy - p);
	}
	s = sqrt(q / n);
	r = sqrt(1.0 - q / e);
	for (j = 0; j <= m - 1; j++)
	{
		p = 0.0;
		for (i = 0; i <= n - 1; i++)
		{
			pp = a[m];
			for (k = 0; k <= m - 1; k++)
			{
				if (k != j) pp = pp + a[k] * x[k*n + i];
			}
			p = p + (y[i] - pp)*(y[i] - pp);
		}
		v[j] = sqrt(1.0 - q / p);
	}
	dt[0] = q; dt[1] = s; dt[2] = r; dt[3] = u;
	free(b); return;
}
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	BHLinker
// ��    ��:	��׵�����
// ��    ��:	unsigned int wNumBH, Vector3d* ptData, std::vector<double> &daX, std::vector<double> &daY, std::vector<double> &daZ
//   (���):	wNumBH:		��׵�����
//				ptData:		��׵�����(�����ǰ�����׵��Ⱥ�˳�����ӵ�)
//				daX:		�����ߵ�X��������(����)
//				daY:		�����ߵ�Y��������(����)
//				daZ:		�����ߵ�Z��������(����)
//				bLinear:	�Ƿ���������(ȱʡΪtrue)
//				m:			����������ֵ��ÿ�εķֶ���(ȱʡΪ8)
//   (����):	daX:		�����ߵ�X��������
//				daY:		�����ߵ�Y��������
//				daZ:		�����ߵ�Z��������
// ��    ��:	��
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	if ��׵���Ŀn��1, û��Ҫ����;
//				if n = 2, ���ѡ���������ӻ���������������, ����������������ȡ���߶ε�
//					���ֵ�ΪԼ����, ��Zֵ��IDW������(��Ϊ���Ը���,�� ������������Ե�);
//				if n��3, ���ѡ���������ӻ���������������(�ֶ���ȱʡΪ8)
// ��    ��:
// ��������:	2004-3-30, Wengzp
// �޸�����:	
//==================================================================//
void BHLinker(unsigned int wNumBH, Vector3d* ptData, std::vector<double> &daX, std::vector<double> &daY, std::vector<double> &daZ, bool bLinear, int m)
////GeoView_Tech_Mark_End
{
	if (wNumBH <= 0 || ptData == NULL || ((!bLinear) && m<1))
		return;
	if (wNumBH<2) // wNumBH = 1
	{
		daX.push_back(ptData[0][0]);
		daY.push_back(ptData[0][1]);
		daZ.push_back(ptData[0][2]);
		return;
	}
	unsigned int i = 0;
	if (bLinear)	// Linear Linker between boreholes.
	{
		for (i = 0; i<wNumBH; i++)
		{
			daX.push_back(ptData[i][0]);
			daY.push_back(ptData[i][1]);
			daZ.push_back(ptData[i][2]);
		}
	}
	else	// Unlinear linker between boreholes.
	{	//Spline3D(double x[], double y[], double z[], double tx[], double ty[], double tz[], int n, int m)
		double *x = NULL, *y = NULL, *z = NULL, *tx = NULL, *ty = NULL, *tz = NULL;
		if (wNumBH == 2)
		{
			wNumBH = 4;
			x = new double[4];	y = new double[4];	z = new double[4];
			x[0] = ptData[0][0];
			y[0] = ptData[0][1];
			z[0] = ptData[0][2];
			x[3] = ptData[1][0];
			y[3] = ptData[1][1];
			z[3] = ptData[1][2];
			x[1] = (x[0] * 2.0 + x[3]) / 3.0;
			y[1] = (y[0] * 2.0 + y[3]) / 3.0;
			z[1] = (z[0] * 4.0 + z[3]) / 5.0;
			x[2] = (x[0] + x[3] * 2.0) / 3.0;
			y[2] = (y[0] + y[3] * 2.0) / 3.0;
			z[2] = (z[0] + z[3] * 4.0) / 5.0;
		}
		else
		{
			x = new double[wNumBH];	y = new double[wNumBH];	z = new double[wNumBH];
			for (i = 0; i<wNumBH; i++)
			{
				x[i] = ptData[i][0];
				y[i] = ptData[i][1];
				z[i] = ptData[i][2];
			}
		}
		unsigned int wT = m*(wNumBH - 1) + 1;
		tx = new double[wT]; ty = new double[wT]; tz = new double[wT];
		// ����������ֵ
		Spline3D(x, y, z, tx, ty, tz, wNumBH, m);
		// ��ֵ
		for (i = 0; i<wT; i++)
		{
			daX.push_back(tx[i]);
			daY.push_back(ty[i]);
			daZ.push_back(tz[i]);
		}

		// �ͷſռ�
		if (x)
			delete[] x;
		if (y)
			delete[] y;
		if (z)
			delete[] z;
		if (tx)
			delete[] tx;
		if (ty)
			delete[] ty;
		if (tz)
			delete[] tz;
	}

	return;
}

//-------------------------------------------------------------------------
//*************************************************************************
// ��������
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	ZeroMatrix
// ��    ��:	��������
// ��    ��:	MATRIX4X4 A
//   (���):	A: ����
//   (����):	A: �����
// ��    ��:	��
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
void ZeroMatrix(MATRIX4X4 A)
////GeoView_Tech_Mark_End
{
	// Initialize the matrix to the following values:
	//		0.0		0.0		0.0		0.0
	//		0.0		0.0		0.0		0.0
	//		0.0		0.0		0.0		0.0
	//		0.0		0.0		0.0		0.0
	//
	int i, j;

	for (i = 0; i<4; i++) {
		for (j = 0; j<4; j++)
			A[i][j] = 0.0;
	}
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	DetOfMatrix
// ��    ��:	���������ʽ
// ��    ��:	MATRIX4X4 A
//   (���):	A: ����
//   (����):	��
// ��    ��:	double: ����A������ʽ
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
double DetOfMatrix(MATRIX4X4 Src)
////GeoView_Tech_Mark_End
{// �����(����)������ʽ
	double dRes = 0.0;

	double a[9];

	for (int j = 0, k = 0, nSign = -1; j<4; j++, k = 0)
	{
		nSign = -nSign;
		for (int m = 1; m<4; m++)
		{
			for (int n = 0; n<4; n++)
			{
				if (n != j)
					a[k++] = Src[m][n];
			}
		}

		dRes += nSign*Src[0][j] * (a[0] * a[4] * a[8] + a[3] * a[7] * a[2] + a[6] * a[5] * a[1]
			- a[2] * a[4] * a[6] - a[1] * a[3] * a[8] - a[0] * a[7] * a[5]);

	}
	return(dRes);
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	TraceOfMatrix
// ��    ��:	����ļ�
// ��    ��:	MATRIX4X4 A
//   (���):	A: ����
//   (����):	��
// ��    ��:	double: ����A�ļ�
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
double TraceOfMatrix(MATRIX4X4 A)
////GeoView_Tech_Mark_End
{
	return(A[0][0] + A[1][1] + A[2][2] + A[3][3]);
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	InverseOfMatrix
// ��    ��:	�������
// ��    ��:	MATRIX4X4 Src, MATRIX4X4 InvMatrix
//   (���):	Src: ����
//				InvMatrix: ����Src�������(����)
//   (����):	InvMatrix: ����Src�������
// ��    ��:	double: ����Src������ʽ(���Ϊ0.0, �򷵻ص������û������)
// ���÷���:	MATRIX4X4 Src = {{1.0, 0.0, 0.0, 0.0},
//							   {0.0, 1.0, 0.0, 0.0},
//							   {0.0, 0.0, 1.0, 0.0},
//							   {0.0, 0.0, 0.0, 1.0}}, InvMatrix;
//				double dRes = InverseOfMatrix(Src, InvMatrix);
//				if(dRes>0.0)	// �ж���������Ч��
//				{
//					double dTrace = TraceOfMatix(InvMatrix);
//				}
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	�����Src�������, ���ؾ���Des������ʽ(�� |Src| = 0.0, �� |Des| = 0.0, ��Ч)
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
double InverseOfMatrix(MATRIX4X4 Src, MATRIX4X4 Des)
////GeoView_Tech_Mark_End
{
	double dDet = DetOfMatrix(Src);
	if (fabs(dDet)<TOLERANCE)  // ����ʽΪ0�ľ���û�������
		return(0.0);

	double a[9];
	int k = 0;
	int mSign = 1;
	int nSign = 1;
	for (int i = 0; i<4; i++)
	{
		mSign = -mSign;
		for (int j = 0; j<4; j++)
		{
			nSign = -nSign;
			for (int m = 0; m<4; m++)
			{
				for (int n = 0; n<4; n++)
				{
					if (m != i && n != j)
						a[k++] = Src[m][n];
				}
			}
			k = 0;
			Des[j][i] = mSign*nSign*(a[0] * a[4] * a[8] + a[3] * a[7] * a[2] + a[6] * a[5] * a[1]
				- a[2] * a[4] * a[6] - a[1] * a[3] * a[8] - a[0] * a[7] * a[5]) / dDet;

		}
	}

	return(1.0 / dDet);	// ����������������ʽ��Ϊ����
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	IdentityMatrix
// ��    ��:	��λ����
// ��    ��:	MATRIX4X4 A
//   (���):	A: ����(����)
//   (����):	A: ��λ����
// ��    ��:	��
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
void IdentityMatrix(MATRIX4X4 A)
////GeoView_Tech_Mark_End
{
	// Initialize the matrix to the follow values:
	//		1.0		0.0		0.0		0.0
	//		0.0		1.0		0.0		0.0
	//		0.0		0.0		1.0		0.0
	//		0.0		0.0		0.0		1.0
	//
	ZeroMatrix(A);

	for (int i = 0; i<4; i++)
	{
		A[i][i] = 1.0;
	}
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	Translate3D
// ��    ��:	��ƽ�Ʊ任����(��˾���)
// ��    ��:	double tx, double ty, double tz, MATRIX4X4 A
//   (���):	tx: ƽ�Ʊ任��X������
//				ty: ƽ�Ʊ任��Y������
//				tz: ƽ�Ʊ任��Z������
//				A:  ƽ�Ʊ任����(����)
//   (����):	A:  ƽ�Ʊ任����
// ��    ��:	��
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
void Translate3D(double tx, double ty, double tz, MATRIX4X4 A)
////GeoView_Tech_Mark_End
{
	//	Translation matrix identified as:
	//		 ----------------
	//		| 1   0   0   Tx |
	//		| 0   1   0   Ty |
	//		| 0   0   1   Tz |
	//		| 0   0   0   1  |
	//		 ----------------
	int i;

	ZeroMatrix(A);
	for (i = 0; i<4; i++)
		A[i][i] = 1.0;

	A[0][3] = tx;
	A[1][3] = ty;
	A[2][3] = tz;
}

// ������任����(��˾���)
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	Scale3D
// ��    ��:	������任����(��˾���)
// ��    ��:	double sx, double sy, double sz, MATRIX4X4 A
//   (���):	sx: �����任��X������
//				sy: �����任��Y������
//				sz: �����任��Z������
//				A:  �����任����(����)
//   (����):	A:  �����任����
// ��    ��:	��
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
void Scale3D(double sx, double sy, double sz, MATRIX4X4 A)
////GeoView_Tech_Mark_End
{
	// Scaling matrix identified as:
	//		 ----------------
	//		| Sx  0   0   0 |
	//		| 0   Sy  0   0 |
	//		| 0   0   Sz  0 |
	//		| 0   0   0   1 |
	//		 ----------------

	ZeroMatrix(A);
	A[0][0] = sx;
	A[1][1] = sy;
	A[2][2] = sz;
	A[3][3] = 1.0;
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	Rotate3D
// ��    ��:	����ת�任����(��˾���)
// ��    ��:	int m, double Theta, MATRIX4X4 A
//   (���):	m: ��ת���������(0: X��, 1: Y��, 2: Z��)
//				Theta: ��ת�Ƕ�(��λ: ��)
//				A: ��ת�任����(����)
//   (����):	A: ��ת�任����
// ��    ��:	��
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
void Rotate3D(int m, double Theta, MATRIX4X4 A)
////GeoView_Tech_Mark_End
{
	double c, s;

	ZeroMatrix(A);
	c = Cosf(Theta);
	s = Sinf(Theta);

	// Compensate for rounding errors
	if (fabs(c)<TOLERANCE)
		c = 0.0f;
	if (fabs(s)<TOLERANCE)
		s = 0.0f;

	switch (m)
	{
	case 0:
		//	Rotation about the X-Axis matrix identified as:
		//		 -----------------------
		//		| 1     0      0      0 |
		//		| 0     cosX   -sinX  0 |
		//		| 0     sinX   cosX   0 |
		//		| 0     0      0      1 |
		//		 -----------------------

		A[0][0] = 1.0;
		A[1][1] = c;
		A[1][2] = -s;
		A[2][1] = s;
		A[2][2] = c;
		A[3][3] = 1.0;
		break;

	case 1:
		//	Rotation about the Y-Axis matrix identified as:
		//		 -----------------------
		//		| cosY  0      sinY   0 |
		//		| 0     1      0      0 |
		//		| -sinY 0      cosY   0 |
		//		| 0     0      0      1 |
		//		 -----------------------

		A[0][0] = c;
		A[0][2] = s;
		A[1][1] = 1.0;
		A[2][0] = -s;
		A[2][2] = c;
		A[3][3] = 1.0;
		break;

	case 2:
		//	Rotation about the Z-Axis matrix identified as:
		//		 -----------------------
		//		| cosZ  -sinZ  0      0 |
		//		| sinZ  cosZ   0      0 |
		//		| 0     0      1      0 |
		//		| 0     0      0      1 |
		//		 -----------------------

		A[0][0] = c;
		A[0][1] = -s;
		A[1][0] = s;
		A[1][1] = c;
		A[2][2] = 1.0;
		A[3][3] = 1.0;
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	MultiplyMatricies
// ��    ��:	�����ĳ˻�
// ��    ��:	MATRIX4X4 A, MATRIX4X4 B, MATRIX4X4 C
//   (���):	A: ����, ������
//				B: ����, ����
//				C: ����, �˻�(����)
//   (����):	C: �˻�����( C = A��B)
// ��    ��:	��
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
void MultiplyMatricies(MATRIX4X4 A, MATRIX4X4 B, MATRIX4X4 C)
////GeoView_Tech_Mark_End
{
	int   i, j, k;

	for (i = 0; i<4; i++) {
		for (j = 0; j<4; j++) {
			for (k = 0, C[i][j] = 0; k<4; k++)
				C[i][j] += A[i][k] * B[k][j];
		}
	}
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	MatrixCopy
// ��    ��:	������
// ��    ��:	MATRIX4X4 A, MATRIX4X4 B
//   (���):	A: �����ƾ���
//				B: ����(����)
//   (����):	B: ����
// ��    ��:	��
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
void MatrixCopy(MATRIX4X4 A, MATRIX4X4 B)
////GeoView_Tech_Mark_End
{
	int  i, j;

	for (i = 0; i<4; i++)
	{
		for (j = 0; j<4; j++)
			B[i][j] = A[i][j];
	}
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	TransposeMatrix
// ��    ��:	��ת�þ���
// ��    ��:	MATRIX4X4 A
//   (���):	A: ����
//   (����):	A: ת�ú�ľ���
// ��    ��:	��
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
void TransposeMatrix(MATRIX4X4 A)
////GeoView_Tech_Mark_End
{
	MATRIX4X4 M;
	int  i, j;

	for (i = 0; i<4; i++) {
		for (j = 0; j<4; j++)
			M[j][i] = A[i][j];
	}
	MatrixCopy(M, A);
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	VecTransform
// ��    ��:	���������ĳ˷�
// ��    ��:	Vector3d sx, Vector3d dx, MATRIX4X4 M
//   (���):	sx: ����
//				dx: �˻�����(����)
//				M:  ����
//   (����):	dx: �˻�����
// ��    ��:	��
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
void VecTransform(Vector3d S, Vector3d D, MATRIX4X4 M)
////GeoView_Tech_Mark_End
{
	double x, y, z;

	// Transform the Source vector 'S' by the matrix 'M'
	x = M[0][0] * S[0] + M[0][1] * S[1] + M[0][2] * S[2] + M[0][3];
	y = M[1][0] * S[0] + M[1][1] * S[1] + M[1][2] * S[2] + M[1][3];
	z = M[2][0] * S[0] + M[2][1] * S[1] + M[2][2] * S[2] + M[2][3];

	// Compensate for rounding errors
	if (fabs(x) < TOLERANCE)
		x = 0.0f;
	if (fabs(y) < TOLERANCE)
		y = 0.0f;
	if (fabs(z) < TOLERANCE)
		z = 0.0f;

	D[0] = x;
	D[1] = y;
	D[2] = z;
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	glMatrixTo4x4
// ��    ��:	OpenGL����Ϊ��������
// ��    ��:	double M[16], MATRIX4X4 A
//   (���):	M: OpenGL����(ʵ����16��Ԫ�ص�����, ��������)
//				A: ����(����)
//   (����):	A: ����
// ��    ��:	��
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
void glMatrixTo4x4(double M[16], MATRIX4X4 A)
////GeoView_Tech_Mark_End
{
	int i, j;

	for (i = 0; i<4; i++)
	{
		for (j = 0; j<4; j++)
		{
			A[i][j] = (float)M[(j * 4) + i];
		}
	}
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	Matx4x4ToglMatrix
// ��    ��:	��������ΪOpenGL����
// ��    ��:	MATRIX4X4 A, double M[16]
//   (���):	A: ����
//				M: OpenGL����(����, ʵ����16��Ԫ�ص�����, ��������)
//   (����):	M: OpenGL����
// ��    ��:	��
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
void Matx4x4ToglMatrix(MATRIX4X4 A, double M[16])
////GeoView_Tech_Mark_End
{
	int i, j;

	for (i = 0; i<4; i++)
	{
		for (j = 0; j<4; j++)
		{
			M[(i * 4) + j] = A[j][i];
		}
	}
}

//-------------------------------------------------------------------------
//*************************************************************************
// ��ѧ����
// (101) �Է����ཻ�Ķ���ν��������ʷ�
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	Triangulate
// ��    ��:	�Է����ཻ�Ķ���ν��������ʷ�
// ��    ��:	Vector3d * pdData, unsigned int * ps, std::vector<int> * pPsArray
//   (���):	pfData:	Vector3d ��ָ�����, �����������
//				ps: unsigned int ��ָ�����, ��ʾ��ǰ����ζ�������˽ṹ, �� 0 ��
//						  Ԫ��Ϊ������, �������Ϊ����������е����
//				pPsArray: �ʷֺ�����������˽ṹ����(ÿ����Ԫ�ر�ʾһ��������)
//   (����):	pPsArray: �ʷֺ�����������˽ṹ����(ÿ����Ԫ�ر�ʾһ��������)
// ��    ��:	��
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
void Triangulate(Vector3d * pdData, unsigned int * ps, std::vector<int> * pPsArray)
////GeoView_Tech_Mark_End
{
	// ��������
	if ((!pdData) || (!ps) || (!pPsArray))
	{
		return;
	}

	// ֻ������Ч��������ε�Ԫ
	if ((ps[0] != 1) || (ps[1]<3))
	{
		return;
	}

	unsigned int i, j, m, n, a, b, c;
	std::vector<int> rPS, cPos;
	bool bResult;

	// ��ɵ�ǰ����εĽڵ���
	n = ps[1];

	// �ж϶���εķ��򣬵õ���ʱ�뷽������
	rPS.clear();
	bResult = IsCCW(pdData, ps);
	if (bResult)
	{
		m = n + 2;
		for (i = 2; i<m; i++)
		{
			rPS.push_back(ps[i]);
		}
	}
	else
	{
		for (i = n + 1; i>1; i--)
		{
			rPS.push_back(ps[i]);
		}
	}

	// ��λ���㣬ȡ�ð����ڽڵ������е�λ��
	cPos.clear();
	GetConcavePos(pdData, &rPS, &cPos);
	m = cPos.size();
	if (m == n)
	{
		m = 0;
	}

	while (m>0)
	{
		m = cPos.size();
		n = rPS.size();
		// ÿ��ȡ (i-1, i, i+1) ����������д���
		for (i = 0; i<n; i++)
		{
			// �жϵ�ǰ�Ƿ�Ϊ����
			a = 0;
			b = rPS[i];
			for (j = 0; j<m; j++)
			{
				if (b == cPos[j])
				{
					a = 1;
					break;
				}
			}
			// �����ǰ��Ϊ����
			if (a == 1)
			{
				continue;
			}
			// ���������� ABC��A--rPS[i]��B--rPS[i-1]��C--rPS[i+1]
			// ��ȥ������ ABC ��ǰ�᣺ABC ��ȫ���ڶ����
			a = rPS[i];
			if (i == 0)
			{
				b = rPS[n - 1];
				c = rPS[1];
			}
			else if (i == n - 1)
			{
				b = rPS[n - 2];
				c = rPS[0];
			}
			else
			{
				b = rPS[i - 1];
				c = rPS[i + 1];
			}
			if (IsPartOfPolygon(a, b, c, pdData, &cPos) == true)
			{
				pPsArray->push_back(a);
				pPsArray->push_back(c);
				pPsArray->push_back(b);

				//.RemorPSveAt(i);
				rPS.erase(rPS.begin() + i);
				n--;
				i--;

				if (n<4)
				{
					break;
				}
			}
		}

		// ���¶�λ����
		m = 0;
		if (n>3)
		{
			a = cPos.size();
			cPos.clear();
			GetConcavePos(pdData, &rPS, &cPos);
			m = cPos.size();

			if (a == m)
			{
				m = 0;
			}
		}
	}

	// ���Ϊ͹����Σ����ǰ�����εĴ�����
	while (n>2)
	{
		for (i = 1; i<n; i++)
		{
			pPsArray->push_back(rPS[i - 1]);
			pPsArray->push_back(rPS[i]);
			pPsArray->push_back(rPS[i + 1]);

			//rPS.RemoveAt(i);
			rPS.erase(rPS.begin() + i);
			n--;
			i--;

			if (n<3)
			{
				return;
			}
		}
	}
}

// �Է����ཻ�Ķ���ν��������ʷ�
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	Triangulate
// ��    ��:	�Է����ཻ�Ķ���ν��������ʷ�
// ��    ��:	std::vector<Vertex3d>& data, std::vector<int>& waPs, std::vector<int>& waTris
//   (���):	data:	����εĶ���������
//				waPs:	����ε����˽ṹ, ȱʡΪ [0, 1, 2, 3, ...]
//				waTris:	�ʷֺ�����������˽ṹ����(ÿ����Ԫ�ر�ʾһ��������)
//   (����):	waTris:	�ʷֺ�����������˽ṹ����(ÿ����Ԫ�ر�ʾһ��������)
// ��    ��:	��
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	
// ��    ��:	
// ��������:	2006-3-4, Wengzp: add notes.
// �޸�����:	
//==================================================================//
void Triangulate(std::vector<Vertex3d>& data, std::vector<int>& waPs, std::vector<int>& waTris)
////GeoView_Tech_Mark_End
{
	int nSize = data.size(), nPs = waPs.size();
	if (nSize<3 || (nPs>0 && nPs<5))
	{
		return;
	}
	int i = 0;
	if (nPs == 0)
	{
		for (i = 0; i<nSize; i++)
		{
			waPs.push_back(i);
		}
	}

	Vector3d *pdData = new Vector3d[nSize];
	unsigned int *ps = new unsigned int[nPs + 2];
	for (i = 0; i<nSize; i++)
	{
		pdData[i][0] = data[i].x;	pdData[i][1] = data[i].y;	pdData[i][2] = data[i].z;
	}

	ps[0] = 1;
	ps[1] = nPs;
	for (i = 0; i<nPs; i++)
	{
		ps[i + 2] = waPs[i];
	}
	Triangulate(pdData, ps, &waTris);

	if (pdData)
		delete[] pdData;
	pdData = 0;
	if (ps)
		delete[] ps;
	ps = 0;
}

// �Դ�����2.5ά����ε������ʷ�
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	Triangulate2
// ��    ��:	�Դ�����2.5ά����ε������ʷ�
// ��    ��:	std::vector<Vertex3d>& data, std::vector<int>& waPs, std::vector<int>& waTris
//   (���):	data:	����εĶ�������
//				waPs:	����ε����˽ṹ, ��0Ϊ����εĸ���, ���˳��Ϊ��߽�ĵ����Լ����, �ٴ�Ϊ
//						�ڱ߽�ĵ����Լ����
//				waTris:	�ʷֺ�����������˽ṹ����(ÿ����Ԫ�ر�ʾһ��������)
//   (����):	waTris:	�ʷֺ�����������˽ṹ����(ÿ����Ԫ�ر�ʾһ��������)
// ��    ��:	��
// ���÷���:	
/*////////////////
CGeoMine3DDoc* pDoc = GetDocument();
ASSERT_VALID(pDoc);

std::vector<Vertex3d> dataXYZ, data;
std::vector<int> waPs, waTris;
CGV3dObject* pObj = NULL;
CGV3dLayer* pCurLayer = pDoc->m_Project.GetCurrentLayer();
CObList* pList = pDoc->m_pSelList;
POSITION pos = pList->GetHeadPosition();
unsigned int k=0, wLines = 0;
waPs.push_back(0);
while (pos)
{
pObj = (CGV3dObject*)pList->GetNext(pos);
if(pObj && pObj->GetClass() == CLASS_GV3D_POLYLINE)
{
((CGV3dPolyline*)pObj)->GetAll(data);
dataXYZ.Append(data);
wLines++;
unsigned int wSize = data.size();
waPs.push_back(wSize);
for(unsigned int i=0; i<wSize; i++, k++)
{
waPs.push_back(k);
}
}
}
if(dataXYZ.size()==0)
{
return;
}
waPs[0] = wLines;

Triangulate2(dataXYZ, waPs, waTris);
int nSize = waTris.size();
if(nSize>0)
{
for(int i=nSize-3; i>=0; i-=3)
{
waTris.InsertAt(i, 3);
}
waTris.InsertAt(0, nSize/3);

// ���������
CGV3dPolygon* pGon = new CGV3dPolygon;
pGon->SetAll(dataXYZ);
pGon->SetPolygons(waTris);
pGon->SetColor(255, 0, 0);
pCurLayer->AddObject(pGon);
}
Invalidate(false);
/*//////////
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	
// ��    ��:	
// ��������:	2006-3-4, Wengzp: add notes.
// �޸�����:	
//==================================================================//
void Triangulate2(std::vector<Vertex3d>& dataXYZ, std::vector<int>& waPs, std::vector<int>& waTris)
////GeoView_Tech_Mark_End
{
	/*///////////////////////////////////////////////////
	*************   Ҫ������߽�ķ����෴ **************
	��߽���չ��: ͨ��������߽����ڱ߽�Ŀɼ���(ֻҪ�ҵ�һ���Ϳ�����)
	ʹ��߽粻����չ,�������ֻ����߽���, �ٵ����������ε������ʷּ���.
	���������� ????????
	waPs:	[0]: �������߽�
	[1]: ��߽�ĵ���
	[2]... : ��߽�����
	//*//////////////////////////////////////////////////
	std::vector<Vertex3d> data(dataXYZ.size());
	std::copy(dataXYZ.begin(), dataXYZ.end(), data.begin());

	unsigned int nSize = data.size(), nPs = waPs.size();
	if (nSize<3 || nPs<5)
	{
		return;
	}

	unsigned int i = 0, j = 0, k = 0, m = 0, n = 0, nPosOut = 0, nPosIn = 0, nSizeIn = 0, nSizeOut = 0;
	std::vector<int> waOut, *pwaIn = NULL;			// ��߽�
	std::vector< std::vector<int>* > arrIns;	// �ڱ߽�����
	for (i = 0; i<nSize; i++)
	{
		data[i].z = 0.0;// ���XOYƽ����
	}

	for (i = 0, k = 2; i<waPs[1]; i++)// ��ʼ��߽�
	{
		waOut.push_back(waPs[k++]);
	}
	if (waPs[0]>1)
	{// �����ڱ߽�, ���ȡȫ�����ڱ߽�
		arrIns.resize(waPs[0] - 1);
		for (i = 1; i<waPs[0]; i++)
		{
			nSizeIn = waPs[k++];
			std::vector<int>* pwaIn = new std::vector<int>;
			pwaIn->resize(nSizeIn);
			for (j = 0; j<nSizeIn; j++)
			{
				(*pwaIn)[j] = waPs[k++];
			}
			arrIns[i - 1] = pwaIn;
		}
	}

	// ͨ�����ҿɼ��ߵķ�����չ��߽�
	Vector3d A = { 0.0, 0.0, 0.0 }, B = { 0.0, 0.0, 0.0 }, C = { 0.0, 0.0, 0.0 }, D = { 0.0, 0.0, 0.0 },
		P = { 0.0, 0.0, 0.0 }, v1 = { 0.0, 0.0, 0.0 }, v2 = { 0.0, 0.0, 0.0 },
		v0 = { 0.0, 0.0, 0.0 }, v21 = { 0.0, 0.0, 0.0 }, v20 = { 0.0, 0.0, 0.0 };
	int nInterType = 0;
	nSize = arrIns.size();
	while (nSize>0)
	{
		pwaIn = arrIns[0];
		nSizeIn = pwaIn->size();
		nSizeOut = waOut.size();
		// �����ɼ��߷���չ��߽�
		bool bVisiable = true;
		for (i = 0; i<nSizeOut; i++)
		{
			A[0] = data[waOut[i]].x, A[1] = data[waOut[i]].y;
			for (j = 0; j<nSizeIn; j++)
			{
				bVisiable = true;
				B[0] = data[(*pwaIn)[j]].x, B[1] = data[(*pwaIn)[j]].y;
				AsubB(B, A, v0);

				// �ȱȽ�����߽�Ŀɼ���
				waOut.push_back(waOut[0]);// ���ɷ�յ�
				for (m = 0; m<nSizeOut; m++)
				{
					C[0] = data[waOut[m]].x, C[1] = data[waOut[m]].y;
					D[0] = data[waOut[m + 1]].x, D[1] = data[waOut[m + 1]].y;

					nInterType = GetIntersectOfTwoLine(A, B, C, D, P);
					if (nInterType == 1)// �ཻ�������߶���, �򲻿ɼ�
					{
						if (GetDistance(P, A)>TOLERANCE)
						{
							bVisiable = false;
							break;
						}
						else
						{// ����ཻ�ڶ�������Ҫ���������ж��Ƿ�ɼ�
							if (m == 0)
							{
								v1[0] = data[waOut[nSizeOut - 1]].x - data[waOut[0]].x;
								v1[1] = data[waOut[nSizeOut - 1]].y - data[waOut[0]].y;
								///v1[2]=data[waOut[nSizeOut-1]].z-data[waOut[0]].z;
							}
							else
							{
								v1[0] = data[waOut[m - 1]].x - C[0];
								v1[1] = data[waOut[m - 1]].y - C[1];
								///v1[2]=data[waOut[m-1]].z-C[2];
							}
							v2[0] = D[0] - C[0];
							v2[1] = D[1] - C[1];
							///	v2[2]=D[2]-C[2];
							AxB(v2, v1, v21);
							AxB(v2, v0, v20);
							if (AdotB(v21, v20)<TOLERANCE)
							{
								bVisiable = false;
								break;
							}
						}
					}
				}

				waOut.erase(waOut.begin() + nSizeOut);
				if (!bVisiable) continue;	// �粻�ɼ�, ��һ��������

				// ����߽�ɼ������ٱȽ������е��ڱ߽�Ŀɼ���
				for (k = 0; k<nSize; k++)
				{
					std::vector<int>* pBIn = arrIns[k];
					unsigned int nSizeBin = pBIn->size();
					pBIn->push_back(pBIn->at(0));
					for (m = 0; m<nSizeBin; m++)
					{
						C[0] = data[(*pBIn)[m]].x;
						C[1] = data[(*pBIn)[m]].y;
						///	C[2]=data[(*pBIn)[m]].z;
						D[0] = data[(*pBIn)[m + 1]].x;
						D[1] = data[(*pBIn)[m + 1]].y;
						///	D[2]=data[(*pBIn)[m+1]].z;

						nInterType = GetIntersectOfTwoLine(A, B, C, D, P);
						if (nInterType == 1)// �ཻ�������߶���, �򲻿ɼ�
						{
							if (k>0 || GetDistance(P, B)>TOLERANCE)
							{
								bVisiable = false;
								break;
							}
						}
					}
					//pBIn->RemoveAt(nSizeBin);
					pBIn->erase(pBIn->begin() + nSizeBin);
					if (!bVisiable) break;	// �粻�ɼ�, ������, Ҫ��һ��������
				}
				if (bVisiable) break;
			}
			if (bVisiable) break;
		}

		// ���ҵ��ɼ�������չ��߽���ȥ�����ڱ߽磬����ŵ����
		if (bVisiable)
		{
			nPosOut = i;
			nPosIn = j;
			// ��չ
			for (m = nPosIn, k = nPosOut + 1; m<nSizeIn; m++)
			{
				//waOut.InsertAt(k++, (*pwaIn)[m]);
				waOut.insert(waOut.begin() + k, (*pwaIn)[m]);
				k++;
			}
			for (m = 0; m <= nPosIn; m++)
			{
				//waOut.InsertAt(k++, (*pwaIn)[m]);

				waOut.insert(waOut.begin() + k, (*pwaIn)[m]);
				k++;
			}
			//waOut.InsertAt(k, waOut[nPosOut]);
			waOut.insert(waOut.begin() + k, waOut[nPosOut]);


			pwaIn->clear();
			delete pwaIn;
			pwaIn = NULL;
			//arrIns.RemoveAt(0);
			arrIns.erase(arrIns.begin());
		}
		else
		{
			arrIns[0] = arrIns[nSize - 1];
			arrIns[nSize - 1] = pwaIn;
		}
		nSize = arrIns.size();
	}

	// �������ε������ʷ�
	Triangulate(data, waOut, waTris);
	waOut.clear();
}

// (102) �ж��Ե�ǰ���Ϊ������������Ƿ�Ϊ����ε�һ����
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	IsPartOfPolygon
// ��    ��:	�ж��Ե�ǰ���Ϊ������������Ƿ�Ϊ����ε�һ����
// ��    ��:	unsigned int a, unsigned int b, unsigned int c, Vector3d * pdData, std::vector<int> * cPos
//   (���):	a, b, c: ��ǰ�����ε������������
//				pdData: ��ǰ����εĽ������
//				cPos: ���������˽ṹ�����е�λ��
//   (����):	��
// ��    ��:	bool: true, �а������ڵ�ǰ��������; ����false
// ���÷���:	unsigned int a = ��, b = ��, c = ����
//				Vector3d pdData = new Vector3d [��];
//				ʵ���� pdData ...
//				std::vector<int> cPos;
//				ʵ���� cPos ...
//				bool bRlt = IsPartOfPolygon(a, b, c, pdData, &cPos);
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
bool IsPartOfPolygon(unsigned int a, unsigned int b, unsigned int c, Vector3d * pdData,
	std::vector<int> * cPos)
	////GeoView_Tech_Mark_End
{
	if ((!pdData) || (!cPos))
	{
		return false;
	}

	// �ж������� ABC �Ƿ�Ϊ��ȫ���ڶ���Σ�ֻ���ж��Ƿ��а��������� A Ϊ����������� ABC ��
	// Ҫ�ж��Ƿ��а������������� ABC �ڣ�ֻ���жϰ������߶� BC ���Ž��Ƿ���ڰ����� AB ����
	// �ǡ�AC ���Ž�֮��
	// �����ȣ���ʾ�������� ABC �⣬�ж������ĵ㣻
	// ������ȣ������������� ABC �ڣ���ʾ������ ABC ����ȫ���ڶ���Σ����� false

	unsigned int i, k, n;
	Vector3d A, B, C, M;
	double BMC, AMB, AMC;

	A[0] = pdData[a][0];
	A[1] = pdData[a][1];
	A[2] = pdData[a][2];

	B[0] = pdData[b][0];
	B[1] = pdData[b][1];
	B[2] = pdData[b][2];

	C[0] = pdData[c][0];
	C[1] = pdData[c][1];
	C[2] = pdData[c][2];

	n = cPos->size();
	for (i = 0; i<n; i++)
	{
		//�����ǰ����Ϊ C �㣬����Ҫ�ж�
		k = (*cPos)[i];
		if ((b == k) || (c == k))
		{
			continue;
		}

		M[0] = pdData[k][0];
		M[1] = pdData[k][1];
		M[2] = pdData[k][2];

		// ���Žǲ��ж�
		BMC = Get_Angle_C_Of_ABC(B, C, M);
		AMB = Get_Angle_C_Of_ABC(A, B, M);
		AMC = Get_Angle_C_Of_ABC(A, C, M);
		if (fabs(BMC + AMB + AMC - 360)<TOLERANCE)
			// if ( fabs(BMC+AMB+AMC-360)<0.001 )
		{
			return false;
		}
	}

	return true;
}

// (103) ��λ���������˽ṹ�����е�λ��
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	GetConcavePos
// ��    ��:	��λ����εİ���
// ��    ��:	Vector3d * pdData, std::vector<int> * rPS, std::vector<int> * cPos
//   (���):	pdData: ��ʾ��ǰ����εĽ������
//				rPS: ��������˽ṹ����
//				cPos: ��������(����)
//   (����):	cPos: ��������
// ��    ��:	��
// ���÷���:	Vector3d pdData = new Vector3d [��];
//				std::vector<int> rPS;
//				ʵ���� rPS����
//				std::vector<int> cPos;
//				GetConcavePos(pdData, rPS, cPos);
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
void GetConcavePos(Vector3d * pdData, std::vector<int> * rPS,
	std::vector<int> * cPos)
	////GeoView_Tech_Mark_End
{
	if ((!pdData) || (!rPS) || (!cPos))
	{
		return;
	}

	unsigned int i, n, a, b, c;
	double A[2], B[2], C[2];

	// rPS��[0, 1, 2..., n-2, n-1]
	n = rPS->size();
	for (i = 0; i<n; i++)
	{
		b = i;
		if (i == 0)
		{
			a = n - 1;
			c = 1;
		}
		else if (i == n - 1)
		{
			a = n - 2;
			c = 0;
		}
		else
		{
			a = i - 1;
			c = i + 1;
		}

		a = (*rPS)[a];
		A[0] = pdData[a][0];
		A[1] = pdData[a][1];

		c = (*rPS)[c];
		C[0] = pdData[c][0];
		C[1] = pdData[c][1];

		b = (*rPS)[b];
		B[0] = pdData[b][0];
		B[1] = pdData[b][1];

		if ((B[0] - A[0])*(C[1] - B[1]) - (B[1] - A[1])*(C[0] - B[0])<0)
		{
			cPos->push_back(b);
		}
	}
}

// (104) �жϵ�ǰ����α߽��㷽���Ƿ�Ϊ��ʱ��
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	IsCCW
// ��    ��:	�жϵ�ǰ����α߽��㷽���Ƿ�Ϊ��ʱ��
// ��    ��:	Vector3d * pdData, unsigned int * ps
//   (���):	pfData: Vector3d �͵�����, ��ʾ��ǰ����εĽ������
//				polygons: unsigned int �͵�����, ��ʾ��ǰ����ε����˽ṹ, ����ʽ����:
//					[��ɶ���εı߽�����, ��һ�����, �ڶ������..., ���һ�����]
//   (����):	��
// ��    ��:	true ��ʾΪ��ʱ��ģ�false ��ʾΪ˳ʱ���
// ���÷���:	unsigned int size = ��;
//				Vector3d * pfData = new Vector3d [size];
//				//������������
//				����
//				unsigned int n=��;//�߽�����
//				unsigned int * polygons = new unsigned int [n];
//				//�������˽ṹ
//				����
//				bool bResult = IsCCW(pfData, polygons);
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	�Ѷ���ηֽ�� n �����Σ��ֱ�������������
//					   n-1
//				area = �� [X(i+1)-X(i)] * [Y(i+1)+Y(i)] / 2
//					   i=0
//				���������� 0��Ϊ˳ʱ�룻С�� 0��Ϊ��ʱ�롣���� 0�����ཻ����Ϊ��ʱ�봦��
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
bool IsCCW(Vector3d * pdData, unsigned int * ps)
////GeoView_Tech_Mark_End
{
	if ((!pdData) || (!ps))
	{
		return true;
	}

	unsigned int i, n;
	double A[2], B[2], area;

	n = ps[0];

	// ������� ps Ϊһ������ζ����ĳһ��Ԫ
	if (n != 1)
	{
		// �������ն���Χ�����
		A[0] = pdData[ps[n]][0];
		A[1] = pdData[ps[n]][1];
		B[0] = pdData[ps[1]][0];
		B[1] = pdData[ps[1]][1];
		area = (B[0] - A[0]) * (B[1] + A[1]) / 2.0;

		n++;
		for (i = 2; i<n; i++)
		{
			A[0] = B[0];
			A[1] = B[1];

			B[0] = pdData[ps[i]][0];
			B[1] = pdData[ps[i]][1];

			area += (B[0] - A[0]) * (B[1] + A[1]) / 2.0;
		}

		if (area>0)
		{
			return false;
		}

		return true;
	}

	// ������� ps Ϊһ��������������ε�Ԫ���˽ṹ
	n = ps[1] + 1;

	// �������ն���Χ�����
	A[0] = pdData[ps[n]][0];
	A[1] = pdData[ps[n]][1];
	B[0] = pdData[ps[2]][0];
	B[1] = pdData[ps[2]][1];
	area = (B[0] - A[0]) * (B[1] + A[1]) / 2.0;

	n++;
	for (i = 3; i<n; i++)
	{
		A[0] = B[0];
		A[1] = B[1];

		B[0] = pdData[ps[i]][0];
		B[1] = pdData[ps[i]][1];

		area += (B[0] - A[0]) * (B[1] + A[1]) / 2.0;
	}

	if (area>0)
	{
		return false;
	}

	return true;
}

// (105) �������� ABC �Ľ� C �ĽǶ�
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	Get_Angle_C_Of_ABC
// ��    ��:	���������� ABC �Ķ��� C �ĽǶ� (0 ~ 180 ��ֵ)
// ��    ��:	Vector3d A, Vector3d B, Vector3d C
//   (���):	A: �����εĶ���A
//				B: �����εĶ���B
//				C: �����εĶ���C
//   (����):	��
// ��    ��:	double: ��ǰ������ ABC �Ķ��� C �ĽǶ�(��λ: ��)
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
double Get_Angle_C_Of_ABC(Vector3d A, Vector3d B, Vector3d C)
////GeoView_Tech_Mark_End
{
	Vector3d CA, CB;

	CA[0] = A[0] - C[0];
	CA[1] = A[1] - C[1];
	CA[2] = A[2] - C[2];

	CB[0] = B[0] - C[0];
	CB[1] = B[1] - C[1];
	CB[2] = B[2] - C[2];

	//���ݹ�ʽ a��b = |a|��|b|��cos��
	double result = sqrt(CA[0] * CA[0] + CA[1] * CA[1] + CA[2] * CA[2]);
	if (fabs(result)<TOLERANCE)
	{
		return 180;
	}

	result *= sqrt(CB[0] * CB[0] + CB[1] * CB[1] + CB[2] * CB[2]);
	if (fabs(result)<TOLERANCE)
	{
		return 180;
	}
	else
	{
		result = (CA[0] * CB[0] + CA[1] * CB[1] + CA[2] * CB[2]) / result;
		if (result>1.0) result = 1.0;
		if (result<-1.0) result = -1.0;
		result = acos(result) * 180 / PI;
		return result;
	}
}

// (106) ���������� ABC �����
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	GetAreaOfTri
// ��    ��:	���������� ABC �����
// ��    ��:	Vector3d A, Vector3d B, Vector3d C
//   (���):	A: �����εĶ���A
//				B: �����εĶ���B
//				C: �����εĶ���C
//   (����):	��
// ��    ��:	double: ������ ABC �����
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
double GetAreaOfTri(Vector3d A, Vector3d B, Vector3d C)
////GeoView_Tech_Mark_End
{
	//----------------------------------
	//                B
	//               / \
		//             /     \
		//           /         \
		//         /             \
		//       A --------------- C
	//----------------------------------
	// ��ʸ�� BA �� BC �Ĳ����ģ����ֵΪƽ���ı��ε�����������ε����Ϊ��һ��
	Vector3d BA, BC, P;
	AsubB(A, B, BA);
	AsubB(C, B, BC);
	AxB(BA, BC, P);
	// ���Ϊ��
	double area = sqrt(AdotB(P, P)) / 2.0;

	return area;
}

// (107) ����͹����ε����
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	GetAreaOfConvGon
// ��    ��:	����͹����ε����
// ��    ��:	Vector3d * pdData, unsigned int * ps
//   (���):	pfData:	Vector3d�͵�����
//				polygons: unsigned int �͵�����
//   (����):	��
// ��    ��:	double: ��ǰ͹����ε����
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
double GetAreaOfConvGon(Vector3d * pdData, unsigned int * ps)
////GeoView_Tech_Mark_End
{
	if ((!pdData) || (!ps))
	{
		return 0;
	}

	double area = 0;
	Vector3d A, B, C;
	SetAtoB(A, pdData[ps[1]]);
	for (unsigned int i = 2; i<ps[0]; i++)
	{
		SetAtoB(B, pdData[ps[i]]);
		SetAtoB(C, pdData[ps[i + 1]]);
		area += GetAreaOfTri(A, B, C);
	}

	return area;
}

// ����ƽ�����ε����
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	GetAreaOfPlaneGon
// ��    ��:	����ƽ�����ε����
// ��    ��:	std::vector<Vertex3d>& xyzs
//   (���):	xyzs:	ƽ�����ε�˳�ζ�������
//   (����):	��
// ��    ��:	double: ƽ�����ε����(�п���Ϊ����)
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	
// ��    ��:	
// ��������:	2006-3-18, Wengzp: add notes.
// �޸�����:	
//==================================================================//
double GetAreaOfPlaneGon(std::vector<Vertex3d>& xyzs)
////GeoView_Tech_Mark_End
{
	double dArea = 0.0;
	int nDots = xyzs.size();
	if (nDots<3)
	{
		return dArea;
	}

	Vector3d A = { xyzs[0].x, xyzs[0].y, xyzs[0].z },
		B = { xyzs[1].x, xyzs[1].y, xyzs[1].z },
		C = { xyzs[2].x, xyzs[2].y, xyzs[2].z };
	double dNormal[4] = { 0.0, 0.0, 0.0, 0.0 };
	GetTriNormal(dNormal, A, B, C, true);
	int nType = 2, i = 0;
	double dAbsX = fabs(dNormal[0]),
		dAbsY = fabs(dNormal[1]),
		dAbsZ = fabs(dNormal[2]);
	double dAbsMax = dAbsZ;
	if (dAbsX >= dAbsY && dAbsX >= dAbsZ)
	{
		nType = 0;
		dAbsMax = dAbsX;
	}
	else if (dAbsY >= dAbsX && dAbsY >= dAbsZ)
	{
		nType = 1;
		dAbsMax = dAbsY;
	}

	xyzs.push_back(xyzs[0]);
	switch (nType)
	{
	case 0:
	{
		for (i = 0; i<nDots; i++)
		{
			dArea += xyzs[i].y * (xyzs[i + 1].z - xyzs[i].z);
		}
	}
	break;
	case 1:
	{
		for (i = 0; i<nDots; i++)
		{
			dArea += xyzs[i].x * (xyzs[i + 1].z - xyzs[i].z);
		}
	}
	break;
	case 2:
	{
		for (i = 0; i<nDots; i++)
		{
			dArea += xyzs[i].x * (xyzs[i + 1].y - xyzs[i].y);
		}
	}
	break;
	default:;
	}
	xyzs.erase(xyzs.begin() + nDots);

	dArea = 0.5*dArea / dAbsMax;
	return dArea;
}

// (108) �������ľ���
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	GetDistance
// ��    ��:	�������ľ���
// ��    ��:	const Vector3d A, const Vector3d B
//   (���):	A: ��A
//				B: ��B
//   (����):	��
// ��    ��:	double: ��A���B֮��ľ���(ŷ�Ͼ���)
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
double GetDistance(const Vector3d A, const Vector3d B)
////GeoView_Tech_Mark_End
{
	if ((A == NULL) || (B == NULL))
	{
		return 0.0f;
	}

	double len = sqrt((A[0] - B[0])*(A[0] - B[0])
		+ (A[1] - B[1])*(A[1] - B[1])
		+ (A[2] - B[2])*(A[2] - B[2]));
	return float(len);
}

// (112) �� C ����ֱ�� AB �ϵĴ��㣬�������Ƿ����߶� AB ��
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	GetVPnt_C_On_AB
// ��    ��:	�� C ����ֱ�� AB �ϵĴ��㣬�������Ƿ����߶� AB ��
// ��    ��:	const Vector3d A, const Vector3d B,
//				const Vector3d C, Vector3d P = NULL
//   (���):	A: �߶�A�Ķ˵�
//				B: �߶�B����һ�˵�
//				C: �ռ�����һ��
//				P: ����(���ܷ���)
//				pdDist: �㵽�߶εľ���(���ܷ���)
//   (����):	P: ����(���ܷ���)
//				pdDist: �㵽�߶εľ���(���ܷ���)
// ��    ��:	true: �������߶� AB ��; false: �������߶� AB ��
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
bool GetVPnt_C_On_AB(const Vector3d A, const Vector3d B,
	const Vector3d C, Vector3d P, double *pdDist)
	////GeoView_Tech_Mark_End
{
	Vector3d v1, v2;
	v1[0] = A[0] - B[0], v1[1] = A[1] - B[1], v1[2] = A[2] - B[2];
	v2[0] = C[0] - B[0], v2[1] = C[1] - B[1], v2[2] = C[2] - B[2];
	double t = AdotB(v1, v1);
	if (fabs(t)<TOLERANCE)
	{
		t = 0;
	}
	else
	{
		t = AdotB(v1, v2) / t;
	}
	if (P)
	{
		P[0] = B[0] + v1[0] * t;
		P[1] = B[1] + v1[1] * t;
		P[2] = B[2] + v1[2] * t;
	}
	if (pdDist)
	{
		*pdDist = GetDistance(C, P);
	}
	if ((t<0) || (t>1))
	{
		return false;
	}

	return true;
}

bool GetVPnt_C_On_AB(const Vertex3d A, const Vertex3d B,
	const Vertex3d C, Vertex3d P, double *pdDist)
{
	bool bOn = false;
	Vector3d PA, PB, PC, PP;
	PA[0] = A.x;	PA[1] = A.y;	PA[2] = A.z;
	PB[0] = B.x;	PB[1] = B.y;	PB[2] = B.z;
	PC[0] = C.x;	PC[1] = C.y;	PC[2] = C.z;

	bOn = GetVPnt_C_On_AB(PA, PB, PC, PP, pdDist);
	P.x = PP[0];	P.y = PP[1];	P.z = PP[2];
	return bOn;
}

// (113) �� �߶� AB ��ƽ�� (a, b, c, d) �Ľ��㣬�������Ƿ����߶� AB ��
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	GetInterpoint
// ��    ��:	���߶� AB ��ƽ�� (a, b, c, d) �Ľ��㣬�������Ƿ����߶� AB ��
// ��    ��:	Vector3d A, Vector3d B,
//				double a, double b, double c, double d,
//				Vector3d p
//   (���):	A, B:		�߶ε�����
//				a, b, c, d:	ƽ����� (ax + by + cz + d = 0)
//				P:			����(����)
//   (����):	P:			����
// ��    ��:	-1, ��ʾ�޽���
//				0, ��ʾ������ AB ֮��
//				1, ��ʾ������� A
//				2, ��ʾ������� B
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
int GetInterpoint(Vector3d A, Vector3d B,
	double a, double b, double c, double d,
	Vector3d p)
	////GeoView_Tech_Mark_End
{
	double fTemp, fTempA, fTempB;
	fTempA = a*A[0] + b*A[1] + c*A[2] + d;
	fTempB = a*B[0] + b*B[1] + c*B[2] + d;
	if (fabs(fTempA - fTempB)<TOLERANCE) // ֱ�� AB ƽ����ƽ�� (a, b, c, d)
	{
		return -1;
	}

	fTemp = fTempB / (fTempB - fTempA);

	if (fabs(fTemp - 0.0f)<TOLERANCE)
	{
		p[0] = B[0];
		p[1] = B[1];
		p[2] = B[2];
		return 2;
	}
	else if (fabs(fTemp - 1.0f)<TOLERANCE)
	{
		p[0] = A[0];
		p[1] = A[1];
		p[2] = A[2];
		return 1;
	}
	else if ((fTemp>0.0f) && (fTemp<1.0f))
	{
		p[0] = fTemp*A[0] + (1 - fTemp)*B[0];
		p[1] = fTemp*A[1] + (1 - fTemp)*B[1];
		p[2] = fTemp*A[2] + (1 - fTemp)*B[2];
		return 0;
	}
	else
	{
		return -1;
	}
}
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	Degreesf
// ��    ��:	//��������ˮƽ����ͶӰ�ߵķ�λ��
// ��    ��:	Point3d v
//   (���):	v: ����()
//   (����):	��
// ��    ��:	double: ת����ĽǶ�ֵ
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:    ��������λ��,������yֵΪ��Ϊ������ֵ,���÷����Һ����������
//              ���ݻ��������Ƕ�
// ��    ��:
// ��������:	2006-1-23  �￨
// �޸�����:	2006-1-23
//==================================================================//
double VecsAzimuth(Vector3d v)
{
	VecNormalize(v);
	//���������Ļ���
	double Radian = 0;
	//Ȼ�󽫻���ת��Ϊ�Ƕ�
	double Degree = 0;

	//��һ����
	if (v[0] >= 0 && v[1] >= 0)
	{
		Radian = acos(v[0]);
	}
	//�ڶ�����
	if (v[0] <= 0 && v[1] >= 0)
	{
		Radian = acos(v[0]);
	}
	//��������,
	if (v[0] <= 0 && v[1] <= 0)
	{
		Radian = PI + acos(v[0]);
	}
	//��������
	if (v[0] >= 0 && v[1] <= 0)
	{
		Radian = PI + acos(v[0]);
	}
	//������ת��Ϊ�Ƕ�
	Degree = Degreesf(Radian);
	return Degree;
}
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	AzimuthsVec
// ��    ��:	��λ�ǵĵ�λ����
// ��    ��:	double Azimuth
//   (���):	Azimuth ��λ���ȣ�
//   (����):	��
// ��    ��:    Point3D v
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:    ��λ�ǵ�����ֵ��Ϊ��λ��������yֵ������ֵ��Ϊxֵ��zֵ��Ϊ0
// ��    ��:
// ��������:	2006-1-23  �￨
// �޸�����:	2006-1-23
//==================================================================//
void AzimuthsVec(double Azimuth, Vector3d Vec)
{
	Vec[0] = cos(Radiansf(Azimuth));
	Vec[1] = sin(Radiansf(Azimuth));
	Vec[2] = 0;
}
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	GetInterpoint
// ��    ��:	���߶� AB ��ƽ��LMN�Ľ���P, �������Ƿ����߶�AB��
// ��    ��:	Vector3d A, Vector3d B,
//				Vector3d L, Vector3d M, Vector3d N,
//				Vector3d P
//   (���):	A, B:		�߶ε�����
//				L, M, N:	ƽ���ϲ����ߵ�����
//				P:			����(����)
//   (����):	P:			����
// ��    ��:	-1, ��ʾ�޽���
//				0, ��ʾ������ AB ֮��
//				1, ��ʾ������� A
//				2, ��ʾ������� B
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
int GetInterpoint(Vector3d A, Vector3d B,
	Vector3d L, Vector3d M, Vector3d N,
	Vector3d P)
	////GeoView_Tech_Mark_End
{
	double a, b, c, d;
	Vector3d v, v1, v2;
	v1[0] = (float)(M[0] - L[0]);
	v1[1] = (float)(M[1] - L[1]);
	v1[2] = (float)(M[2] - L[2]);

	v2[0] = (float)(N[0] - L[0]);
	v2[1] = (float)(N[1] - L[1]);
	v2[2] = (float)(N[2] - L[2]);
	AxB(v1, v2, v);

	a = v[0];
	b = v[1];
	c = v[2];
	d = -(a*L[0] + b*L[1] + c*L[2]);

	return GetInterpoint(A, B, a, b, c, d, P);
}

// (113_2)�� ֱ�� AB ��ƽ��LMN�Ľ���P, �������Ƿ����߶�AB��
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	GetPntOfLine_Plane
// ��    ��:	��ֱ�� AB ��ƽ��LMN�Ľ���P, �������Ƿ����߶�AB��
// ��    ��:	Vector3d A, Vector3d B,
//				Vector3d L, Vector3d M, Vector3d N,
//				Vector3d P
//   (���):	A, B:		�߶ε�����
//				L, M, N:	ƽ���ϲ����ߵ�����
//				P:			����(����)
//   (����):	P:			����
// ��    ��:	-1: �޽���
//				0: �н���, ������AB��
//				1: �н���, ����AB��
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
int GetPntOfLine_Plane(Vector3d A, Vector3d B,
	Vector3d L, Vector3d M, Vector3d N,
	Vector3d P)
	////GeoView_Tech_Mark_End
{
	double a, b, c, d;
	Vector3d v, v1, v2;
	v1[0] = M[0] - L[0];
	v1[1] = M[1] - L[1];
	v1[2] = M[2] - L[2];

	v2[0] = N[0] - L[0];
	v2[1] = N[1] - L[1];
	v2[2] = N[2] - L[2];
	AxB(v1, v2, v);

	a = v[0];
	b = v[1];
	c = v[2];
	d = -(a*L[0] + b*L[1] + c*L[2]);

	double fTemp, fTempA, fTempB;
	fTempA = a*A[0] + b*A[1] + c*A[2] + d;
	fTempB = a*B[0] + b*B[1] + c*B[2] + d;
	if (fabs(fTempA - fTempB)<TOLERANCE) // ֱ�� AB ƽ����ƽ�� (a, b, c, d)
	{
		return -1;
	}

	fTemp = fTempB / (fTempB - fTempA);
	P[0] = fTemp*A[0] + (1 - fTemp)*B[0];
	P[1] = fTemp*A[1] + (1 - fTemp)*B[1];
	P[2] = fTemp*A[2] + (1 - fTemp)*B[2];
	if (fTemp>1.0f || fTemp<0.0f)
	{
		return 0;
	}
	return 1;
}

// (114) �� �� P ��ֱ�� AB(from A to B) ��ת �� �ǶȺ�ĵ�Q����
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	GetPntRotateOnLine
// ��    ��:	��� P ��ֱ�� AB(from A to B) ��ת �� �ǶȺ�ĵ�Q����
// ��    ��:	Vector3d A, Vector3d B, Vector3d P, Vector3d Q, float deltaRad
//   (���):	A, B:		ֱ��AB�ϵ�����
//				P:			��P
//				Q:			P����ת���λ�õ�Q(����)
//				deltaRad:	��ת�ĽǶ�(��λ: ����)
//   (����):	Q:			P����ת���λ�õ�Q(����)
// ��    ��:	��
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:	��ҹ㡶�����ͼ��ѧ��(Edition 3rd) page 372.
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
void GetPntRotateOnLine(Vector3d A, Vector3d B, Vector3d P,
	Vector3d Q, float deltaRad)
	////GeoView_Tech_Mark_End
{
	double a, b, c, v, distance, fb = 0.0f, fc = 1.0f;
	double cosa = (float)cos(deltaRad), sina = (float)sin(deltaRad);
	MATRIX4X4 Rx, InvRx, Ry, InvRy, Rz;
	Vector3d p, q;
	p[0] = P[0] - A[0];
	p[1] = P[1] - A[1];
	p[2] = P[2] - A[2];


	a = B[0] - A[0];
	b = B[1] - A[1];
	c = B[2] - A[2];
	distance = (float)(sqrt(a*a + b*b + c*c));
	a /= distance;
	b /= distance;
	c /= distance;
	v = sqrt(b*b + c*c);
	if (v>TOLERANCE)
	{
		fb = b / v;
		fc = c / v;
	}

	Rx[0][0] = 1.0f;	Rx[0][1] = 0.0f;		Rx[0][2] = 0.0f;		Rx[0][3] = 0.0f;
	Rx[1][0] = 0.0f;	Rx[1][1] = fc;			Rx[1][2] = -fb;			Rx[1][3] = 0.0f;
	Rx[2][0] = 0.0f;	Rx[2][1] = fb;			Rx[2][2] = fc;			Rx[2][3] = 0.0f;
	Rx[3][0] = 0.0f;	Rx[3][1] = 0.0f;		Rx[3][2] = 0.0f;		Rx[3][3] = 1.0f;

	Ry[0][0] = v;		Ry[0][1] = 0.0f;		Ry[0][2] = -a;			Ry[0][3] = 0.0f;
	Ry[1][0] = 0.0f;	Ry[1][1] = 1.0f;		Ry[1][2] = 0.0f;		Ry[1][3] = 0.0f;
	Ry[2][0] = a;		Ry[2][1] = 0.0f;		Ry[2][2] = v;			Ry[2][3] = 0.0f;
	Ry[3][0] = 0.0f;	Ry[3][1] = 0.0f;		Ry[3][2] = 0.0f;		Ry[3][3] = 1.0f;

	Rz[0][0] = cosa;	Rz[0][1] = -sina;		Rz[0][2] = 0.0f;		Rz[0][3] = 0.0f;
	Rz[1][0] = sina;	Rz[1][1] = cosa;		Rz[1][2] = 0.0f;		Rz[1][3] = 0.0f;
	Rz[2][0] = 0.0f;	Rz[2][1] = 0.0f;		Rz[2][2] = 1.0f;		Rz[2][3] = 0.0f;
	Rz[3][0] = 0.0f;	Rz[3][1] = 0.0f;		Rz[3][2] = 0.0f;		Rz[3][3] = 1.0f;

	MatrixCopy(Rx, InvRx);
	TransposeMatrix(InvRx);
	MatrixCopy(Ry, InvRy);
	TransposeMatrix(InvRy);

	VecTransform(p, q, Rx);
	VecTransform(q, p, Ry);
	VecTransform(p, q, Rz);
	VecTransform(q, p, InvRy);
	VecTransform(p, q, InvRx);

	Q[0] = A[0] + q[0];
	Q[1] = A[1] + q[1];
	Q[2] = A[2] + q[2];
}

// (115) ������黷�Ʒ��ߵ����� ////////////////////////+
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	GetPntsRotateNormal
// ��    ��:	������黷�Ʒ��ߵ�����
// ��    ��:	Vector3d Normal, Vector3d O, unsigned int wCnt, Vector3d* pData = NULL
//   (���):	wCnt:	��������ܵ���
//				pData:	����������(���ܷ���)
//				Normal:	���߷���
//				O:		�����ϵ�һ��
//   (����):	��
// ��    ��:	Vector3d*:	��ת��ĵ���������(�����, �û������ֶ��ͷ�)
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	�������������(xoyƽ���ϵ�), ���ؾ�������ָ��������ת�������
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
Vector3d* GetPntsRotateNormal(Vector3d N, Vector3d O, unsigned int wCnt, Vector3d* pData)
////GeoView_Tech_Mark_End
{
	if (pData == NULL)
	{// ���Ϊ��, ���Զ�����ȱʡ�ĵ�����
		pData = new Vector3d[wCnt];
		double dRad = 0.0, dTheta = 2 * PI / wCnt;
		for (unsigned int i = 0; i<wCnt; i++, dRad += dTheta)
		{
			pData[i][0] = cos(dRad);
			pData[i][1] = sin(dRad);
			pData[i][2] = 0.0;
		}
	}
	double dX = 0.0, dY = 0.0, dZ = 0.0;

	// ������ҹ㡶�����ͼ��ѧ��(Edition 3rd) page 372.
	double a, b, c, v, distance, fb = 0.0f, fc = 1.0f;
	MATRIX4X4 Rx, InvRx, Ry, InvRy;
	Vector3d p, q;

	a = N[0];
	b = N[1];
	c = N[2];
	distance = (float)(sqrt(a*a + b*b + c*c));
	a /= distance;
	b /= distance;
	c /= distance;
	v = sqrt(b*b + c*c);
	if (v>TOLERANCE)
	{
		fb = b / v;
		fc = c / v;
	}

	Rx[0][0] = 1.0f;	Rx[0][1] = 0.0f;		Rx[0][2] = 0.0f;		Rx[0][3] = 0.0f;
	Rx[1][0] = 0.0f;	Rx[1][1] = fc;			Rx[1][2] = -fb;			Rx[1][3] = 0.0f;
	Rx[2][0] = 0.0f;	Rx[2][1] = fb;			Rx[2][2] = fc;			Rx[2][3] = 0.0f;
	Rx[3][0] = 0.0f;	Rx[3][1] = 0.0f;		Rx[3][2] = 0.0f;		Rx[3][3] = 1.0f;

	Ry[0][0] = v;		Ry[0][1] = 0.0f;		Ry[0][2] = -a;			Ry[0][3] = 0.0f;
	Ry[1][0] = 0.0f;	Ry[1][1] = 1.0f;		Ry[1][2] = 0.0f;		Ry[1][3] = 0.0f;
	Ry[2][0] = a;		Ry[2][1] = 0.0f;		Ry[2][2] = v;			Ry[2][3] = 0.0f;
	Ry[3][0] = 0.0f;	Ry[3][1] = 0.0f;		Ry[3][2] = 0.0f;		Ry[3][3] = 1.0f;

	MatrixCopy(Rx, InvRx);
	TransposeMatrix(InvRx);
	MatrixCopy(Ry, InvRy);
	TransposeMatrix(InvRy);

	for (unsigned int i = 0; i<wCnt; i++)
	{
		p[0] = pData[i][0];
		p[1] = pData[i][1];
		p[2] = pData[i][2];
		VecTransform(p, q, InvRy);
		VecTransform(q, p, InvRx);
		pData[i][0] = O[0] + p[0];
		pData[i][1] = O[1] + p[1];
		pData[i][2] = O[2] + p[2];
	}

	return pData;
}

// (120) ��� P ����A��B��C����ȷ����ƽ��Ĵ���Q������
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	GetPntVerticalPlane
// ��    ��:	��� P ����A��B��C����ȷ����ƽ��Ĵ���Q������
// ��    ��:	Vector3d A, Vector3d B, Vector3d C, Vector3d P, Vector3d Q
//   (���):	A, B, C:	ƽ���ϲ����ߵ�����
//				P:			��P
//				Q:			��P����ƽ��ABC�Ĵ���(����)
//   (����):	Q:			��P����ƽ��ABC�Ĵ���
// ��    ��:	P��ƽ��ľ���
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	�㷨ԭ��:
//				N = (P2-P1)��(P3-P1)
//						--> Normalize
//				Distance = |v3��N|
//				PL = P - (v3��N)N 
// ��    ��:	���������ȼ��㡷�е��������
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
float GetPntVerticalPlane(Vector3d A, Vector3d B, Vector3d C,
	Vector3d P, Vector3d Q)
	////GeoView_Tech_Mark_End
{
	Vector3d v12, v13, N, v3;
	v12[0] = B[0] - A[0], v12[1] = B[1] - A[1], v12[2] = B[2] - A[2];
	v13[0] = C[0] - A[0], v13[1] = C[1] - A[1], v13[2] = C[2] - A[2];
	v3[0] = P[0] - A[0], v3[1] = P[1] - A[1], v3[2] = P[2] - A[2];

	AxB(v12, v13, N);
	VecNormalize(N);

	double fDistance = AdotB(v3, N);
	Q[0] = P[0] - fDistance*N[0];
	Q[1] = P[1] - fDistance*N[1];
	Q[2] = P[2] - fDistance*N[2];

	return (float)fabs(fDistance);
}

// (121) �жϵ�P�Ƿ��ڶ������(��άƽ����, �����ڶ���α���)
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	IsPntInGon
// ��    ��:	�жϵ� P �Ƿ��ڶ������
// ��    ��:	unsigned int wCnt, Vector3d * pData, Vector3d P
//   (���):	wCnt:	����εĵ���
//				pData:	����ε�����
//				P:		��P
//   (����):	��
// ��    ��:	true: ��P�ڶ������; false: ��P���ڶ������
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
bool IsPntInGon(unsigned int wCnt, Vector3d* pData, Vector3d P)
////GeoView_Tech_Mark_End
{
	/// Notes: �����Ϊ���ڶ���α��ϲ������ڶ������, ��Ա�����"// �ڱ���"�ȴ���
	///			return true; ��Ϊ return false;
	int nCCC = 0;	// ���㽻��ĸ���
	double x1, y1, x2, y2, y;

	for (unsigned int i = 0; i<wCnt; i++)
	{
		if (i == wCnt - 1)
		{
			x1 = pData[i][0]; y1 = pData[i][1];
			x2 = pData[0][0]; y2 = pData[0][1];
		}
		else
		{
			x1 = pData[i][0]; y1 = pData[i][1];
			x2 = pData[i + 1][0]; y2 = pData[i + 1][1];
		}

		if (x1 == x2)	// �ų���ֱ�����
		{
			if ((P[0] == x1) && ((P[1] - y1)*(P[1] - y2) <= 0.0f))// �ڱ���
				return true;
		}
		else if ((P[0] - x1)*(P[0] - x2) <= 0.0f)
		{
			if (P[0] == x1 && P[1] <= y1)
			{
				if (P[1] == y1)		// �ڱ���
					return true;
				if (x2 > P[0])
					nCCC++;
			}
			else if (P[0] == x2 && P[1] <= y2)
			{
				if (P[1] == y2)		// �ڱ���
					return true;
				if (x1 > P[0])
					nCCC++;
			}
			else
			{
				y = y1 + (P[0] - x1)*(y2 - y1) / (x2 - x1);
				if (y == P[1])			// �ڱ���
					return true;
				if (y > P[1])
					nCCC++;
			}
		}
	}

	// �������ĸ���Ϊ����, ��P�ڶ������
	if (fmod((double)nCCC, (double)2))
		return true;

	return false;
}

// (122) ���߶� AB ���߶� CD �Ľ��� P
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	GetIntersectOfTwoLine
// ��    ��:	���߶� AB ���߶� CD �Ľ��� P
// ��    ��:	Vector3d A, Vector3d B, Vector3d C, Vector3d D, Vector3d P
//   (���):	A, B:	�߶�AB�Ķ˵�
//				C, D:	�߶�CD�Ķ˵�
//				P:		�߶�AB���߶�CD�Ľ���
//   (����):	P:		�߶�AB���߶�CD�Ľ���
// ��    ��:	0: �޽���, 1: ������AB����CD��, 2: ����ֻ��AB��, 3: ����ֻ��CD��, 4: ���㲻��AB��CD��
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
int GetIntersectOfTwoLine(Vector3d A, Vector3d B,
	Vector3d C, Vector3d D,
	Vector3d P)
	////GeoView_Tech_Mark_End
{
	//// ����: 0: �޽���, 1: ������AB����CD��, 2: ����ֻ��AB��, 3: ����ֻ��CD��, 4: ���㲻��AB��CD��
	MATRIX4X4 MM;
	double x1 = A[0], y1 = A[1], z1 = A[2];
	double x2 = B[0], y2 = B[1], z2 = B[2];
	double x3 = C[0], y3 = C[1], z3 = C[2];
	double x4 = D[0], y4 = D[1], z4 = D[2];
	MM[0][0] = A[0], MM[0][1] = A[1], MM[0][2] = A[2], MM[0][3] = 1.0f;
	MM[1][0] = B[0], MM[1][1] = B[1], MM[1][2] = B[2], MM[1][3] = 1.0f;
	MM[2][0] = C[0], MM[2][1] = C[1], MM[2][2] = C[2], MM[2][3] = 1.0f;
	MM[3][0] = D[0], MM[3][1] = D[1], MM[3][2] = D[2], MM[3][3] = 1.0f;

	if (fabs(DetOfMatrix(MM))>TOLERANCE)	// ������
	{
		return 0;
	}

	double delta = (x3 - x4)*(y2 - y1) - (x2 - x1)*(y3 - y4);
	double u, v;
	if (fabs(delta)>TOLERANCE)
	{
		u = ((x3 - x1)*(y4 - y3) - (x4 - x3)*(y3 - y1)) / delta;
		v = ((x3 - x1)*(y2 - y1) - (x2 - x1)*(y3 - y1)) / delta;
	}
	else
	{
		delta = (x3 - x4)*(z2 - z1) - (x2 - x1)*(z3 - z4);
		if (fabs(delta)>TOLERANCE)
		{
			u = ((x3 - x1)*(z4 - z3) - (x4 - x3)*(z3 - z1)) / delta;
			v = ((x3 - x1)*(z2 - z1) - (x2 - x1)*(z3 - z1)) / delta;
		}
		else
		{
			delta = (y3 - y4)*(z2 - z1) - (y2 - y1)*(z3 - z4);
			if (fabs(delta)>TOLERANCE)
			{
				u = ((y3 - y1)*(z4 - z3) - (y4 - y3)*(z3 - z1)) / delta;
				v = ((y3 - y1)*(z2 - z1) - (y2 - y1)*(z3 - z1)) / delta;
			}
			else
				return 0;	// ƽ�л��غ�
		}
	}
	P[0] = (1.0f - u)*x1 + u*x2;
	P[1] = (1.0f - u)*y1 + u*y2;
	P[2] = (1.0f - u)*z1 + u*z2;
	/// ����: 0: �޽���, 1: ������AB����CD��, 2: ����ֻ��AB��, 3: ����ֻ��CD��, 4: ���㲻��AB��CD��
	bool bU = (u >= 0.0f && u <= 1.0f);
	bool bV = (v >= 0.0f && v <= 1.0f);
	if (bU && bV)
	{
		return 1;
	}
	if (bU && !bV)
	{
		return 2;
	}
	if (!bU && bV)
	{
		return 3;
	}
	return 4;
}

// (123) �����P��Ǧ������������ABC�Ľ���Q(��һ����ABC��)
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	GetIntersectOfTri
// ��    ��:	����� P ��Ǧ������������ ABC �Ľ���Q(��һ����ABC��)
// ��    ��:	Vector3d A, Vector3d B, Vector3d C, Vector3d P, Vector3d Q
//   (���):	A, B, C:	������ABC�Ķ���
//				P:			��P
//				Q:			���� P ��Ǧ������������ ABC �Ľ���(����)
//   (����):	Q:			���� P ��Ǧ������������ ABC �Ľ���
// ��    ��:	true: �н���;	false: �޽���
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
bool GetIntersectOfTri(Vector3d A, Vector3d B, Vector3d C,
	Vector3d P, Vector3d Q)
	////GeoView_Tech_Mark_End
{
	double x1 = A[0], y1 = A[1], z1 = A[2],
		x2 = B[0], y2 = B[1], z2 = B[2],
		x3 = C[0], y3 = C[1], z3 = C[2],
		x = P[0], y = P[1];

	MATRIX4X4 MM, MX, MY, deltaM;
	deltaM[0][0] = x1, deltaM[0][1] = y1, deltaM[0][2] = 1.0f, deltaM[0][3] = 0.0f;
	deltaM[1][0] = x2, deltaM[1][1] = y2, deltaM[1][2] = 1.0f, deltaM[1][3] = 0.0f;
	deltaM[2][0] = x3, deltaM[2][1] = y3, deltaM[2][2] = 1.0f, deltaM[2][3] = 0.0f;
	deltaM[3][0] = 0.0f, deltaM[3][1] = 0.0f, deltaM[3][2] = 0.0f, deltaM[3][3] = 1.0f;
	double detOfM = DetOfMatrix(deltaM);
	if (fabs(detOfM)<TOLERANCE)
	{
		return false;	// �޽���
	}
	MM[0][0] = x1, MM[0][1] = y1, MM[0][2] = z1, MM[0][3] = 0.0f;
	MM[1][0] = x2, MM[1][1] = y2, MM[1][2] = z2, MM[1][3] = 0.0f;
	MM[2][0] = x3, MM[2][1] = y3, MM[2][2] = z3, MM[2][3] = 0.0f;
	MM[3][0] = 0.0f, MM[3][1] = 0.0f, MM[3][2] = 0.0f, MM[3][3] = 1.0f;
	MX[0][0] = y1, MX[0][1] = z1, MX[0][2] = 1.0f, MX[0][3] = 0.0f;
	MX[1][0] = y2, MX[1][1] = z2, MX[1][2] = 1.0f, MX[1][3] = 0.0f;
	MX[2][0] = y3, MX[2][1] = z3, MX[2][2] = 1.0f, MX[2][3] = 0.0f;
	MX[3][0] = 0.0f, MX[3][1] = 0.0f, MX[3][2] = 0.0f, MX[3][3] = 1.0f;
	MY[0][0] = x1, MY[0][1] = z1, MY[0][2] = 1.0f, MY[0][3] = 0.0f;
	MY[1][0] = x2, MY[1][1] = z2, MY[1][2] = 1.0f, MY[1][3] = 0.0f;
	MY[2][0] = x3, MY[2][1] = z3, MY[2][2] = 1.0f, MY[2][3] = 0.0f;
	MY[3][0] = 0.0f, MY[3][1] = 0.0f, MY[3][2] = 0.0f, MY[3][3] = 1.0f;

	Q[0] = x;
	Q[1] = y;
	Q[2] = (DetOfMatrix(MM) - x*DetOfMatrix(MX) + y*DetOfMatrix(MY)) / detOfM;

	return true;
}

// (133) �������εķ��߷���
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
//�� �� ��: GetTriNormal
//��    ��: �������εķ��߷���
//��    ��: dCoef, A, B, C, bNormal
//    ���: A, B, C��Vector3d �����������ε���������
//			bNormal��bool�������Ƿ��һ��
//    ����: dCoef��double��Ԫ�����飬ƽ�淽��ϵ��
//��    ��: ��
//���÷���:	
//GeoView_Doc_Mark_End  											//
//��Ҫ˼·:
//��    ��:
//��������: 2004-3-8
//�޸�����: 2004-05-13�������䣬���ӷ��߷����ϵ�ƽ����.
//			������Ҫ�󣬴�NULLֵ��������ȱʡ
//==================================================================//
void GetTriNormal(double dCoef[4], const Vector3d A, const Vector3d B,
	const Vector3d C, const bool bNormal)
	////GeoView_Tech_Mark_End
{
	Vector3d v, v1, v2;

	AsubB(B, A, v1);
	AsubB(C, A, v2);
	AxB(v1, v2, v);

	if (bNormal)
	{
		double temp = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
		// ���������غϵ������εĳ���  [2006-4-20, 21:06]
		if (temp>TOLERANCE)
		{
			dCoef[0] = v[0] / temp;
			dCoef[1] = v[1] / temp;
			dCoef[2] = v[2] / temp;
		}
	}
	else
	{
		dCoef[0] = v[0];
		dCoef[1] = v[1];
		dCoef[2] = v[2];
	}
	dCoef[3] = -(dCoef[0] * A[0] + dCoef[1] * A[1] + dCoef[2] * A[2]);
}

// //(141)������������ƽ���ϵ�͹����ε����
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	GetAreaOfConvexityGon
// ��    ��:	������������ƽ���ϵ�͹����ε����
// ��    ��:	const Vector3d* ptdata, const unsigned int* polygons, int nprojection
//   (���):	ptData:		͹����εĶ�������
//				polygons:	͹����ε����˽ṹ
//				nprojection:ͶӰ��ʽ
//   (����):	��
// ��    ��:	double: ����ε����
// ���÷���:	Vector3d * pfData = new Vector3d [��];//Ҫ���þ�������
//				��¼����ε������
//				unsigned int * polygons = new unsigned int [��];//Ҫ���þ�������
//				��¼͹����λ�����������ɶ���ε����ӽṹ
//				float area = GetAreaOfConvexityGon(ptdata,polygons,0);
//				nprojection��¼������������һ��ƽ���ϵ��������0����XOY��1����YOZ��2����XOZ
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	�������㷨�������
// ��    ��:	��C������ơ�̷��ǿ	P201
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
double GetAreaOfConvexityGon(const Vector3d* ptdata, const unsigned int* polygons, int nprojection)
//GeoView_Tech_Mark_End
{
	if (nprojection >2 || nprojection<0)
		return 0;
	double area = 0.0;
	double s = 0.0;
	double sarea = 0.0;
	int i;
	Vector3d A, B;
	i = 0;
	int k = polygons[1] - 1;
	while (i <= k)
	{
		if (i == k)
		{
			if (nprojection == 0)
			{
				A[0] = ptdata[polygons[i + 2]][0];
				A[1] = ptdata[polygons[i + 2]][1];
				A[2] = ptdata[polygons[i + 2]][2];
				B[0] = ptdata[polygons[2]][0];
				B[1] = ptdata[polygons[2]][1];
				B[2] = ptdata[polygons[2]][2];
			}
			//���ͶӰ��YOZƽ��
			else if (nprojection == 1)
			{
				A[0] = ptdata[polygons[i + 2]][1];
				A[1] = ptdata[polygons[i + 2]][2];
				A[2] = ptdata[polygons[i + 2]][0];
				B[0] = ptdata[polygons[2]][1];
				B[1] = ptdata[polygons[2]][2];
				B[2] = ptdata[polygons[2]][0];
			}
			else
			{
				A[0] = ptdata[polygons[i + 2]][0];
				A[1] = ptdata[polygons[i + 2]][2];
				A[2] = ptdata[polygons[i + 2]][1];
				B[0] = ptdata[polygons[2]][0];
				B[1] = ptdata[polygons[2]][2];
				B[2] = ptdata[polygons[2]][1];
			}

		}
		else
		{
			if (nprojection == 0)
			{
				A[0] = ptdata[polygons[i + 2]][0];
				A[1] = ptdata[polygons[i + 2]][1];
				A[2] = ptdata[polygons[i + 2]][2];
				B[0] = ptdata[polygons[i + 3]][0];
				B[1] = ptdata[polygons[i + 3]][1];
				B[2] = ptdata[polygons[i + 3]][2];
			}
			else if (nprojection == 1)
			{
				A[0] = ptdata[polygons[i + 2]][1];
				A[1] = ptdata[polygons[i + 2]][2];
				A[2] = ptdata[polygons[i + 2]][0];
				B[0] = ptdata[polygons[i + 3]][1];
				B[1] = ptdata[polygons[i + 3]][2];
				B[2] = ptdata[polygons[i + 3]][0];
			}
			else
			{
				A[0] = ptdata[polygons[i + 2]][0];
				A[1] = ptdata[polygons[i + 2]][2];
				A[2] = ptdata[polygons[i + 2]][1];
				B[0] = ptdata[polygons[i + 3]][0];
				B[1] = ptdata[polygons[i + 3]][2];
				B[2] = ptdata[polygons[i + 3]][1];
			}
		}
		i++;
		//�������㷨����������������ֻ��������͹����Σ�
		sarea = (A[1] + B[1])*(B[0] - A[0]) / 2.0;
		area = area + sarea;
		// ��������ߵĻ���֪�����߷��̣�������������ʽ�ģ�
		//ת��Ϊ��ʽ�ķ����û��ֵķ����������,������Բ��ա�C������ơ�̷��ǿ	P201

	}
	area = fabs(area);
	return area;

}

//(142)��������������С������
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	GetMinMultiple
// ��    ��:	����������������С������
// ��    ��:	int a,int b
//   (���):	a, b:	��������
//   (����):	��
// ��    ��:	int: ��С������
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-3-8
// �޸�����:	2004-3-8
//==================================================================//
int GetMinMultiple(int a, int b)
////GeoView_Tech_Mark_End
{
	int m = (a <= b) ? a : b;
	int n = (a >= b) ? a : b;
	int nMinMul = 1;//��¼��С������
	double num = 1.0;
	int i = 1;
	//�������������С�������ܱ�����
	while (num > TOLERANCE)
	{
		nMinMul = i * n;
		num = fmod((double)nMinMul, (double)m);
		i++;
	}
	return nMinMul;
}

// (331) �ж���ȡ��������᷽��
//GeoView_Tech_Mark_Begin
//GeoView_Doc_Mark_Begin
//�� �� ����GetCmptAxis
//��    �ܣ��ж���ȡ��������᷽��
//��    ����p, q, v
//    ��ڣ�v, Vector3d����������η��߷���
//    ���ڣ�p, q, int����, ��Ч������
//��    �أ���
//���÷�����Vector3d v = ��;
//			int p = ��, q = ��;
//			GetCmptAxis(p, q, v);
//GeoView_Doc_Mark_End
//��Ҫ˼·��
//�������ڣ�2004/04/02	������
//�޸����ڣ�
void GetCmptAxis(int & p, int & q, const double v[4])
//GeoView_Tech_Mark_End
{
	p = 0;
	q = 1;

	// �����ֱ�� X �� (ƽ����YOZ ��)
	if ((fabs(v[1])<TOLERANCE) && (fabs(v[2])<TOLERANCE))
	{
		p = 1;
		q = 2;
	}
	// �����ֱ�� Y �� (ƽ����ZOX ��)
	else if ((fabs(v[0])<TOLERANCE) && (fabs(v[2])<TOLERANCE))
	{
		p = 0;
		q = 2;
	}
}

// (341) �� P �Ƿ��������� ABC �� (�ڱ���Ҳ��)
//GeoView_Tech_Mark_Begin
//GeoView_Doc_Mark_Begin
//�� �� ����IsInTri
//��    �ܣ��� P �Ƿ��������� ABC �� (�ڱ���Ҳ��)
//��    ����P, A, B, C
//    ��ڣ�P, Vector3d ������Ҫ�жϵĵ�
//		    A, B, C, Vector3d �����������ζ�������
//    ���ڣ���
//��    �أ�boolֵ���Ƿ����������� (�ڱ���Ҳ��)
//���÷�����Vector3d P = ����
//			Vector3d A = ��, B = ��, C = ����
//			IsInTri(P, A, B, C);
//GeoView_Doc_Mark_End
//��Ҫ˼·��
//�������ڣ�2004/03/17	������
//�޸����ڣ�
bool IsInTri(const Vector3d P, const Vector3d A, const Vector3d B,
	const Vector3d C)
	//GeoView_Tech_Mark_End
{
	Vector3d aa, bb, cc, v;
	int k = 0;
	double dTempA, dTempB;

	// �жϽ����Ƿ�����������
	AsubB(P, A, aa);
	AsubB(B, C, bb);
	AsubB(B, A, cc);

	v[0] = aa[0] * bb[1] - aa[1] * bb[0];
	v[1] = aa[0] * bb[2] - aa[2] * bb[0];
	v[2] = aa[1] * bb[2] - aa[2] * bb[1];

	k = 0;
	dTempA = fabs(v[0]);
	if (fabs(v[1])>dTempA)
	{
		dTempA = fabs(v[1]);
		k = 1;
	}
	if (fabs(v[2])>dTempA)
	{
		k = 2;
	}

	switch (k)
	{
	case 0:
		dTempA = (cc[0] * bb[1] - cc[1] * bb[0]) / v[0];
		dTempB = (aa[0] * cc[1] - aa[1] * cc[0]) / v[0];
		break;

	case 1:
		dTempA = (cc[0] * bb[2] - cc[2] * bb[0]) / v[1];
		dTempB = (aa[0] * cc[2] - aa[2] * cc[0]) / v[1];
		break;

	case 2:
	default:
		dTempA = (cc[1] * bb[2] - cc[2] * bb[1]) / v[2];
		dTempB = (aa[1] * cc[2] - aa[2] * cc[1]) / v[2];
		break;
	}

	// ��� dTempA С�� 1����ʾ����������
	// ��� dTempA ���� 1����ʾ�ڵ�������
	if ((dTempA - 1.0<-TOLERANCE) ||
		(dTempB<-TOLERANCE) ||
		(dTempB - 1.0>TOLERANCE))
		// if ( (dTempA<1)||(dTempB<0)||(dTempB>1) )
	{
		return false;
	}

	return true;
}

// (326) ���߶� MN �������� ABC �Ľ���
//GeoView_Tech_Mark_Begin
//GeoView_Doc_Mark_Begin
//�� �� ����GetPntOfMN_ABC
//��    �ܣ����߶� MN �������� ABC �Ľ���
//��    ����P, M, N, A, B, C, a, b, c, d
//          ��ڣ�P, Vector3d ��������������
//				  M��N��Vector3d�������߶� MN �Ķ˵�
//				  A��B��C��Vector3d������������ABC�Ķ���
//				  a, b, c, d, double������������ABC����ƽ�淽�̵�ϵ��
//          ���ڣ���
//��    �أ�boolֵ�������Ƿ�λ���߶�MN������������ABC�ڲ�
//���÷�����Vector3d P;
//			Vector3d M = ��, N = ����
//			Vector3d A = ��, B = ��, C = ����
//			double a = ����b = ����c = ����d = ����
//			GetPntOfMN_ABC(P, M, N, A, B, C, a, b, c, d);
//GeoView_Doc_Mark_End
//��Ҫ˼·��
//�������ڣ�2003/10/28	������
//�޸����ڣ�
bool GetPntOfMN_ABC(Vector3d P, const Vector3d M, const Vector3d N,
	const Vector3d A, const Vector3d B, const Vector3d C,
	const double dCoef[4])
	//GeoView_Tech_Mark_End
{
	Vector3d v;
	double dTempA, dTempB;

	// �ж��߶� MN �Ƿ���ƽ���ཻ
	AsubB(N, M, v);
	dTempA = dCoef[0] * v[0] + dCoef[1] * v[1] + dCoef[2] * v[2];
	// ֱ��ƽ����ƽ��
	if (fabs(dTempA)<TOLERANCE)
	{
		return false;
	}
	dTempB = (0 - dCoef[0] * M[0] - dCoef[1] * M[1] - dCoef[2] * M[2] - dCoef[3]) / dTempA;
	// ���㲻�� MN ��
	if ((dTempB<0) || (dTempB>1))
	{
		return false;
	}

	// ��������
	P[0] = M[0] + v[0] * dTempB;
	P[1] = M[1] + v[1] * dTempB;
	P[2] = M[2] + v[2] * dTempB;

	return IsInTri(P, A, B, C);
}

// (431) �ѵ� A �� B
//GeoView_Tech_Mark_Begin
//GeoView_Doc_Mark_Begin
//�� �� ����SetAtoB
//��    �ܣ��ѵ� A �� B
//��    ����B, A
//    ��ڣ�A, Vector3d ������Դ������
//    ���ڣ�B, Vector3d ������Ŀ��������ŵ�ַ
//��    �أ���
//���÷�����Vector3d A = ��, B;
//			SetAtoB(B, A);
//GeoView_Doc_Mark_End
//��Ҫ˼·��
//�������ڣ�2004/04/08	������
//�޸����ڣ�
void SetAtoB(Vector3d B, const Vector3d A)
//GeoView_Tech_Mark_End
{
	B[0] = A[0];
	B[1] = A[1];
	B[2] = A[2];
}


//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	GetVolumeOfPrism
// ��    ��:	����(б��������)�����������V
// ��    ��:	Vector3d A, Vector3d B, Vector3d C
//   (���):	A:	�������Ķ����,	Za >= 0;
//				B:	�������Ķ����,	Zb >= 0;
//				C:	�������Ķ����,	Zc >= 0;
//   (����):	��
// ��    ��:	double:	�����������
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	�������ĵ�����XOY�ϡ�
//				���ABCƽ���� XOY��(��б������), �� V = So * Za
//				���ABC��ƽ����XOY, ��Ҫ�������ıߴ�ֱ��XOY, �������������:
//					V = So * (Za + Zb + Zc)/3.0;
//				��֮, �����������������ʽ���Թ��Ϊ:
//
//					V = So * (Za + Zb + Zc)/3.0;
//
//				����SoΪ������ABC��ͶӰ���������
//				===================================================
//				Ӧ֤��"�򵥾�������", ���Zb = 0, Zc = 0, ���������׶��
// ��    ��:	
// ��������:	2004-8-23, Wengzp
// �޸�����:	
//==================================================================//
double GetVolumeOfPrism(Vector3d A, Vector3d B, Vector3d C)
////GeoView_Tech_Mark_End
{
	Vector3d Ao, Bo, Co;	// �ֱ�ΪABC��XOY�ϵ�ͶӰ
	double So, V;		// ͶӰ���������, ���������
	Ao[0] = A[0], Ao[1] = A[1], Ao[2] = 0.0;
	Bo[0] = B[0], Bo[1] = B[1], Bo[2] = 0.0;
	Co[0] = C[0], Co[1] = C[1], Co[2] = 0.0;

	So = GetAreaOfTri(Ao, Bo, Co);
	V = So*(A[2] + B[2] + C[2]) / 3.0;

	return V;
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	GetVolumeOfConvGon
// ��    ��:	��ȡ͹����ε����(����Ρ�������XOY��Χ�ɵĿռ�)
// ��    ��:	Vector3d * pdData, unsigned int * ps
//   (���):	pdData:		͹����ε�����
//				ps:			͹����ε����˽ṹ����
//   (����):	��
// ��    ��:	double:		�ÿռ�����
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	
// ��    ��:	
// ��������:	2004-8-28, Wengzp
// �޸�����:	
//==================================================================//
double GetVolumeOfConvGon(Vector3d * pdData, unsigned int * ps)
////GeoView_Tech_Mark_End
{
	if ((!pdData) || (!ps))
	{
		return 0;
	}

	double dVol = 0;
	Vector3d A, B, C;
	SetAtoB(A, pdData[ps[1]]);
	for (unsigned int i = 2; i<ps[0]; i++)
	{
		SetAtoB(B, pdData[ps[i]]);
		SetAtoB(C, pdData[ps[i + 1]]);
		dVol += GetVolumeOfPrism(A, B, C);
	}

	return dVol;
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	ReAdjustPointList
// ��    ��:	��������
// ��    ��:	unsigned int wCnt, Vector3d* pts, unsigned int wSize, std::vector<Vertex3d>& paAdjust, bool bSplineInterp
//   (���):	wCnt:			������
//				pts:			��������
//				wSize:			������ĵ�����
//				paAdjust:		������ĵ�������
//				bSplineInterp:	�Ƿ���Ҫ����������ֵ(ȱʡfalse)
//   (����):	paAdjust:		������ĵ�������
// ��    ��:	��
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	
// ��    ��:	
// ��������:	2005-1-19, Wengzp
// �޸�����:	
//==================================================================//
void ReAdjustPointList(unsigned int wCnt, Vector3d* pts, unsigned int wSize, std::vector<Vertex3d>& paAdjust, bool bSplineInterp)
////GeoView_Tech_Mark_End
{
	if (wCnt == 0 || wSize == 0)	// û���ı�Ҫ
		return;
	paAdjust.clear();
	paAdjust.resize(wSize);

	unsigned int i = 0, j = 0, k = 0;
	if (bSplineInterp)
	{// �����������ֵ

	}
	else
	{// ֱ�Ӵ����������ȡ"�м�ֵ��"
		if (wSize == wCnt)
		{// ֱ�Ӹ������ݼ���
			for (i = 0; i<wCnt; i++)
			{
				paAdjust[i].x = pts[i][0];
				paAdjust[i].y = pts[i][1];
				paAdjust[i].z = pts[i][2];
			}
			return;
		}

		// �Ծ���ΪȨ
		double dist = 0.0, length = 0.0;
		std::vector<double> daDist;
		std::vector<int> waRepeat;
		waRepeat.resize(wCnt);
		daDist.resize(wCnt);
		daDist[0] = 0.0;
		for (i = 1; i<wCnt; i++)
		{
			dist = sqrt((pts[i][0] - pts[i - 1][0])*(pts[i][0] - pts[i - 1][0]) +
				(pts[i][1] - pts[i - 1][1])*(pts[i][1] - pts[i - 1][1]) +
				(pts[i][2] - pts[i - 1][2])*(pts[i][2] - pts[i - 1][2]));
			daDist[i] = dist;
			length += dist;
		}

		if (wSize>wCnt)
		{// ��Ҫ�ظ�ĳЩ��Ҫ��
			int nReps = wSize - wCnt;
			for (i = 1, k = nReps; i<wCnt; i++)
			{
				j = Roundf(nReps*daDist[i] / length);
				j = j>k ? k : j;
				k -= j;
				waRepeat[i] = j + 1;
			}
			waRepeat[0] = k + 1;
			for (i = 0, k = 0; i<wCnt; i++)
			{
				for (j = 0; j<waRepeat[i]; j++, k++)
				{
					paAdjust[k].x = pts[i][0];
					paAdjust[k].y = pts[i][1];
					paAdjust[k].z = pts[i][2];
				}
			}
		}
		else// wSize<wCnt
		{
			for (j = 0; j<wSize; j++)
			{// ��������ĵĵ���Ϊ -1.0
				for (i = 1, k = 0, dist = daDist[0]; i<wCnt; i++)
				{
					if (daDist[i]>dist)
					{
						dist = daDist[i];
						k = i;
					}
				}
				daDist[k] = -1.0;
			}
			for (i = 0, k = 0; i<wCnt; i++, k++)
			{
				if (daDist[i]<0)
				{
					paAdjust[k].x = pts[i][0];
					paAdjust[k].y = pts[i][1];
					paAdjust[k].z = pts[i][2];
				}
			}
		}
		waRepeat.clear();
		daDist.clear();
	}
}

// �����������¶�(��λ: ��)
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	GetSlopeGradient
// ��    ��:	�����������¶�(��λ: ��)
// ��    ��:	Vector3d A, Vector3d B, Vector3d C
//   (���):	A: �����εĶ���
//				B: �����εĶ���
//				C: �����εĶ���
//   (����):	��
// ��    ��:	double: �¶�, ��λ: ��
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	�¶��ǵ����ķ�����Ǧ���ߵļнǡ�������������˵���¶Ⱦ�
//				�ǵ�λ���ߵ�Z�����ķ����ҡ����⣬�¶ȵķ�Χ��: 0�ȡ�90��
// ��    ��:	
// ��������:	2005-8-24, Wengzp: add notes.
// �޸�����:	
//==================================================================//
double GetSlopeGradient(Vector3d A, Vector3d B, Vector3d C)
////GeoView_Tech_Mark_End
{
	// ���������εĵ�λ����
	double dN[4] = { 1.0, 0.0, 0.0, 0.0 }, dGradient = 90.0;
	GetTriNormal(dN, A, B, C, true);
	if (dN[2]<0.0)
	{
		dN[2] = -dN[2];
	}

	// ���������ε��¶ȣ���λ: ����
	dGradient = acos(dN[2]);

	// ����ɶ�
	return Degreesf(dGradient);
}

// ��������������(��+x��Ϊ��׼, ��λ: ��)
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	GetSlopeDirection
// ��    ��:	��������������(��+x��Ϊ��׼, ��λ: ��)
// ��    ��:	Vector3d A, Vector3d B, Vector3d C
//   (���):	A: �����εĶ���
//				B: �����εĶ���
//				C: �����εĶ���
//   (����):	��
// ��    ��:	double: �¶�, ��λ: ��, ��+x��Ϊ��׼
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	�����������ε����Ϸ�����ˮƽ���ϵ�ͶӰ�����������ļн�, ����0��,
//				����90��, ��Χ: 0�ȡ�360��, ������+y��Ϊ����
// ��    ��:	
// ��������:	2005-8-24, Wengzp: add notes.
// �޸�����:	
//==================================================================//
double GetSlopeDirection(Vector3d A, Vector3d B, Vector3d C)
////GeoView_Tech_Mark_End
{
	// ���������εĵ�λ����, �������µ����ϵķ���
	double dN[4] = { 1.0, 0.0, 0.0, 0.0 }, dDirection = 0.0;
	GetTriNormal(dN, A, B, C, false);
	if (dN[2]<0.0)
	{
		dN[0] = -dN[0];
		dN[1] = -dN[1];
		dN[2] = -dN[2];
	}

	// ���������ε�����
	dDirection = Degreesf(atan2(dN[0], dN[1]));
	if (dDirection<0.0)
	{
		dDirection += 360.0;
	}

	return dDirection;
}

// �����߶ε��߶εľ���
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	GetDistanceSegment2Segment
// ��    ��:	�����߶ε��߶εľ���
// ��    ��:	const Vector3d A, const Vector3d B, 
//				const Vector3d C, const Vector3d D, 
//				Vector3d P, Vector3d Q
//   (���):	A, B: �߶�1�������˵�
//				C, D: �߶�2�������˵�
//				P:	  ��̾���ʱ�߶�1�ϵĵ�
//				Q:	  ��̾���ʱ�߶�2�ϵĵ�
//   (����):	P:	  ��̾���ʱ�߶�1�ϵĵ�
//				Q:	  ��̾���ʱ�߶�2�ϵĵ�
// ��    ��:	�����߶�֮��ľ���
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	���뺯��: Q(s,t) = as^2 + 2bst + ct^2 + 2ds + 2et + f (s��[0, 1], t��[0, 1])
//				��������߶β�ƽ��, ��Q��ͼ�����������档����s, t��ȡֵ, ����Ϊ:
//						4  |  3  |  2
//						--------------
//						5  |  0  |  1
//						--------------
//						6  |  7  |  8
//				��Ϊ����: 1) 0��;	2) 1,3,5,7; 3) 2,4,6,9
//				�����ݶ��Լ�ƫ������ֵԭ���Ƶ����߶�֮��ľ���
// ��    ��:	�������ͼ��ѧ���ι����㷨��⡷, page 310.
// ��������:	2005-12-21, Wengzp: add notes.
// �޸�����:	
//==================================================================//
double GetDistanceSegment2Segment(const Vector3d A, const Vector3d B,
	const Vector3d C, const Vector3d D,
	Vector3d P, Vector3d Q)
	////GeoView_Tech_Mark_End
{
	double dDist = 0.0;
	Vector3d M0, M1, M;
	AsubB(B, A, M0);// M0�Ǵ�Aָ��B��ʸ��
	AsubB(D, C, M1);// M1�Ǵ�Cָ��D��ʸ��
	AsubB(A, C, M);	// M�Ǵ��߶�2�Ķ˵�Cָ���߶�1�Ķ˵�A��ʸ��
	double a = AdotB(M0, M0), b = -AdotB(M0, M1), c = AdotB(M1, M1),
		d = AdotB(M0, M), e = -AdotB(M1, M), f = AdotB(M, M),
		s = 0.0, t = 0.0;

	double delta = a*c - b*b;
	if (delta>0.0)// �������߶β�ƽ��
	{
		s = (b*e - c*d) / delta;
		t = (b*d - a*e) / delta;
		if (s >= 0.0 && s <= 1.0)
		{
			if (t>1.0)	// Zone 3
			{
				s = -(b + d) / a;
			}
			else if (t<0.0)	// Zone 7
			{
				s = -d / a;
			}
		}
		else if (s>1.0)
		{
			if (t >= 0.0 && t <= 1.0)	// Zone 1
			{
				t = -(b + e) / c;
			}
			else if (t>1.0)	// Zone 2
			{
				if (a + b + d>0.0)	// if Qs(1, 1)>0.0, then t=1.0, ͬ3
				{
					s = -(b + d) / a;
				}
				else			// if Qt(1, 1)>0.0, then s=1.0, ͬ1
				{
					t = -(b + e) / c;
				}
			}
			else// t<0.0	// Zone 8
			{
				if (a + d>0.0)	// if Qs(1, 0)>0.0, then t=0.0, ͬ7
				{
					s = -d / a;
				}
				else	// if Qt(1, 0)>0.0, then s = 1.0, ͬ1
				{
					t = -(b + e) / c;
				}
			}
		}
		else// s<0.0
		{
			if (t >= 0.0 && t <= 1.0)	// Zone 5
			{
				t = -e / c;
			}
			else if (t>1.0)	// Zone 4
			{
				if (b + d>0.0)	// if Qs(0, 1)>0.0, then t=1.0, ͬ3
				{
					s = -(b + d) / a;;
				}
				else	// if Qt(0, 1)>0.0, then s=0.0, ͬ5
				{
					t = -e / c;
				}
			}
			else// t<0.0	// Zone 6
			{
				if (d>0.0)	// if Qs(0, 0)>0.0, then t=0.0, ͬ7
				{
					s = -d / a;
				}
				else	// if Qt(0, 0)>0.0, then s=0.0, ͬ5
				{
					t = -e / c;
				}
			}
		}
		if (s>1.0)
		{
			s = 1.0;
		}
		else if (s<0.0)
		{
			s = 0.0;
		}
		if (t>1.0)
		{
			t = 1.0;
		}
		else if (t<0.0)
		{
			t = 0.0;
		}

	}
	else// �����߶�ƽ��
	{
		if (b>0.0)
		{
			if (d >= 0.0)
			{
				s = 0.0;
				t = 0.0;
			}
			else if (-d <= a)
			{
				s = -d / a;
				t = 0.0;
			}
			else
			{
				s = 1.0;
				double tmp = -(a + d);
				if (tmp >= b)
				{
					t = 1.0;
				}
				else
				{
					t = tmp / b;
				}
			}
		}
		else
		{
			if (-d >= a)
			{
				s = 1.0;
				t = 0.0;
			}
			else if (d <= 0.0)
			{
				s = -d / a;
				t = 0.0;
			}
			else
			{
				s = 0.0;
				if (d >= -b)
				{
					t = 1.0;
				}
				else
				{
					t = -d / b;
				}
			}
		}
	}
	P[0] = A[0] + s*M0[0];	P[1] = A[1] + s*M0[1];	P[2] = A[2] + s*M0[2];
	Q[0] = C[0] + t*M1[0];	Q[1] = C[1] + t*M1[1];	Q[2] = C[2] + t*M1[2];
	return GetDistance(P, Q);
}

// ��ȡ�������Ŀ���(�㷨�����ݼ���ģ��֮���η�)
//		A:			�����ε�A��(û�о���������)
//		B:			�����ε�B��(û�о���������)
//		C:			�����ε�C��(û�о���������)
//		z:			����߳�
//		fpArea:		�Ӵ������(���ܷ���)
// ע:
//		��ϸ�㷨�ɼ�"6.4.2���ݼ���ģ�͡���6.4.2.2���η�"
//	����: ����
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	GetGrooveOfTriprism
// ��    ��:	��ȡ�������Ŀ���
// ��    ��:	Vector3d A, Vector3d B, Vector3d C, float z, double* fpArea
//   (���):	A:			�����ε�A��(û�о���������)
//				B:			�����ε�B��(û�о���������)
//				C:			�����ε�C��(û�о���������)
//				z:			����߳�
//				dpArea:		�Ӵ������(���ܷ���)
//   (����):	dpArea:		�Ӵ������
// ��    ��:	����
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:
// ��    ��:
// ��������:	2004-1-12
// �޸�����:	2004-1-12
//==================================================================//
double GetGrooveOfTriprism(Vector3d A, Vector3d B, Vector3d C, double z, double* dpArea)
////GeoView_Tech_Mark_End
{
	double vol = 0.0,	// ����
		dArea = 0.0;	// �Ӵ������

	// ����ʹ�� Za >= Zb >= Zc;
	double dTemp;
	if (A[2]<B[2])
	{
		dTemp = A[0]; A[0] = B[0]; B[0] = dTemp;
		dTemp = A[1]; A[1] = B[1]; B[1] = dTemp;
		dTemp = A[2]; A[2] = B[2]; B[2] = dTemp;
	}
	if (C[2]>A[2])
	{
		dTemp = A[0]; A[0] = C[0]; C[0] = dTemp;
		dTemp = A[1]; A[1] = C[1]; C[1] = dTemp;
		dTemp = A[2]; A[2] = C[2]; C[2] = dTemp;
	}
	if (C[2]>B[2])
	{
		dTemp = C[0]; C[0] = B[0]; B[0] = dTemp;
		dTemp = C[1]; C[1] = B[1]; B[1] = dTemp;
		dTemp = C[2]; C[2] = B[2]; B[2] = dTemp;
	}
	// �����ε����߳�
	double a = sqrt((B[0] - C[0])*(B[0] - C[0]) + (B[1] - C[1])*(B[1] - C[1]) + (B[2] - C[2])*(B[2] - C[2])),
		b = sqrt((A[0] - C[0])*(A[0] - C[0]) + (A[1] - C[1])*(A[1] - C[1]) + (A[2] - C[2])*(A[2] - C[2])),
		c = sqrt((B[0] - A[0])*(B[0] - A[0]) + (B[1] - A[1])*(B[1] - A[1]) + (B[2] - A[2])*(B[2] - A[2]));
	double l = (a + b + c) / 2.0f;
	double dAreaTri = sqrt(l*(l - a)*(l - b)*(l - c));	// �����ε����
	double Za = A[2], Zb = B[2], Zc = C[2],	// ���ڹ�ϵ: Za >= Zb >= Zc
		sinA = 2.0*dAreaTri / (b*c),
		sinC = 2.0*dAreaTri / (a*b),
		bh = 2.0*dAreaTri / b;

	Vector3d P = { C[0], C[1], z }, Q;
	double dH = GetPntVerticalPlane(A, B, C, P, Q);

	//////////////////////[2006-1-15, 13:50, Wengzp]//////////////+
	//TODO: ������д��Ӵ����Ŀ��
	Vector3d Ao, Bo, Co;	// �ֱ�ΪABC��XOY�ϵ�ͶӰ
	double So;		// ͶӰ���������
	Ao[0] = A[0], Ao[1] = A[1], Ao[2] = 0.0;
	Bo[0] = B[0], Bo[1] = B[1], Bo[2] = 0.0;
	Co[0] = C[0], Co[1] = C[1], Co[2] = 0.0;
	So = GetAreaOfTri(Ao, Bo, Co);
	//////////////////////[2006-1-15, 13:50, Wengzp]//////////////-

	if (So>TOLERANCE && dH > TOLERANCE)
	{
		if (z <= Zc)
		{
			dArea = 0.0f;
			vol = 0.0;
		}
		else if (z <= Zb)
		{
			dArea = a*b*(z - Zc)*(z - Zc)*sinC / (2.0*(Zb - Zc)*(Za - Zc));
			vol = dArea*dH / 3.0;
		}
		else if (z <= Za)
		{
			dArea = b*c*(1.0 - (Za - z)*(Za - z) / ((Za - Zb)*(Za - Zc)))*sinA / 2.0;
			vol = So*(Za - Zb + Za - Zc) / 3.0 - So*(Za - z)
				+ (Za - z)*(dAreaTri - dArea)*So / dAreaTri / 3.0;
		}
		else
		{
			dArea = dAreaTri;
			vol = So*(z - Za + z - Zb + z - Zc) / 3.0;
		}
	}

	if (dpArea)
		*dpArea = dArea;
	return vol;
}


//������ֵ
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	GetMiddle
// ��    ��:	ͳ����ֵ
// ��    ��:	std::vector<double>& daX
//   (���):	daX:	����������

//   (����):	��
// ��    ��:	double:	��ֵ
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	
// ��    ��:	
// ��������:	2006-4-3, zhangdan: add notes.
// �޸�����:	
//==================================================================//
double GetMiddle(std::vector<double>& daX)
////GeoView_Tech_Mark_End
{
	int i, j, n, m;
	double temp;
	n = daX.size() - 1;
	for (j = 0; j<n; j++)
	{
		m = n - j;
		for (i = 0; i<m; i++)
		{
			if (daX[i]>daX[i + 1])
			{
				temp = daX[i];
				daX[i] = daX[i + 1];
				daX[i + 1] = temp;
			}
		}
	}
	if (n % 2 == 0)
	{
		return (daX[n / 2 - 1] + daX[n / 2]) / 2;
	}
	else
		return daX[(n - 1) / 2];


}

//����һά������ֵ
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	GetMinMax
// ��    ��:    ����ֵ
// ��    ��:	std::vector<double>& daX
//   (���):	daX:	����������
//              dMinMax[2]: ����������С���ֵ

//   (����):	��
// ��    ��:	double:	��С���ֵ
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	
// ��    ��:	
// ��������:	2006-5-23, zhangdan: add notes.
// �޸�����:	
//==================================================================//
void GetMinMax(double dMinMax[2], std::vector<double>& daX)
//GeoView_Tech_Mark_End
{
	double dMin, dMax;
	int n = daX.size();
	dMin = dMax = daX[0];

	for (int i = 1; i<n; i++)
	{
		if (daX[i]>dMax)
		{
			dMax = daX[i];
		}

		if (daX[i]<dMin)
		{
			dMin = daX[i];
		}
	}

	dMinMax[0] = dMin;
	dMinMax[1] = dMax;
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	GetAverage
// ��    ��:	ͳ��ƽ��ֵ
// ��    ��:	std::vector<double>& daZ, std::vector<double>* paAlpha
//   (���):	daZ:		����������
//				paAlpha:	Ȩϵ������(����������������ͬ����С, ����ͼ�������ƽ��)ָ��, 
//							��ΪNULL, ���������ƽ��
//   (����):	��
// ��    ��:	double:	ƽ��ֵ
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	
// ��    ��:	
// ��������:	2006-1-18, Wengzp: add notes.
// �޸�����:	
//==================================================================//
double GetAverage(std::vector<double>& daZ, std::vector<double>* paAlpha)
////GeoView_Tech_Mark_End
{
	double dAverage = 0.0;
	int i = 0,
		n = daZ.size();
	if (n>0)
	{
		if (paAlpha && (*paAlpha).size() == n)
		{// ��Ȩƽ��
			for (i = 0; i<n; i++)
			{
				dAverage += (*paAlpha)[i] * daZ[i];
			}
		}
		else
		{// ����ƽ��
			for (i = 0; i<n; i++)
			{
				dAverage += daZ[i];
			}
			dAverage /= n;
		}
	}
	return dAverage;
}

// ���㷽��
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	GetVariance
// ��    ��:	���㷽��
// ��    ��:	std::vector<double>& daX
//   (���):	daX:	����������
//   (����):	��
// ��    ��:	double:	��������
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	������Щ�ǳ���(n-1), �����ǳ���n
// ��    ��:	http://www.huanggao.net/course/40315/ws/SX_13_01_020/
// ��������:	2006-1-18, Wengzp: add notes.
// �޸�����:	
//==================================================================//
double GetVariance(std::vector<double>& daX)
////GeoView_Tech_Mark_End
{
	double S2 = 0.0, dAver = 0.0;
	int i = 0, n = daX.size();
	if (n>0)
	{
		dAver = GetAverage(daX, 0);
		for (i = 0; i<n; i++)
		{
			S2 += daX[i] * daX[i];
		}
		S2 = S2 / n - dAver*dAver;
	}

	return S2;
}

// �����׼��
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	GetStandardDeviation
// ��    ��:	�����׼��
// ��    ��:	std::vector<double>& daX
//   (���):	daX:	����������
//   (����):	��
// ��    ��:	double:	������׼��
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	������Щ�ǳ���(n-1), �����ǳ���n
// ��    ��:	
// ��������:	2006-1-18, Wengzp: add notes.
// �޸�����:	
//==================================================================//
double GetStandardDeviation(std::vector<double>& daX)
////GeoView_Tech_Mark_End
{
	double S2 = GetVariance(daX);
	return sqrt(S2);
}


//٤����
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	lgam1
// ��    ��:	����٤����ֵ������ֵ��
// ��    ��:	 X
//   (���):	 X:	     ʵ����
//   (����):	 ��
// ��    ��:	double:	 ٤����ֵ
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	����٤����ֵ�����֣��������б�ѩ�����ʽ�ƽ�
// ��    ��:    ��ʿ��. C�����㷨����(Edition 2). ����: �廪��ѧ������.1996
// ��������:	2006-4-5, zhangdan: add notes.
// �޸�����:	
//==================================================================//
double lgam1(double x)
////GeoView_Tech_Mark_End
{
	int i;
	double y, t, s, u;
	static double a[11] = { 0.0000677106, -0.0003442342,
		0.0015397681, -0.0024467480, 0.0109736958,
		-0.0002109075, 0.0742379071, 0.0815782188,
		0.4118402518, 0.4227843370, 1.0 };
	if (x <= 0.0)
	{
		printf("err**x<=0!\n"); return(-1.0);
	}
	y = x;
	if (y <= 1.0)
	{
		t = 1.0 / (y*(y + 1.0)); y = y + 2.0;
	}
	else if (y <= 2.0)
	{
		t = 1.0 / y; y = y + 1.0;
	}
	else if (y <= 3.0) t = 1.0;
	else
	{
		t = 1.0;
		while (y>3.0)
		{
			y = y - 1.0; t = t*y;
		}
	}
	s = a[0]; u = y - 2.0;
	for (i = 1; i <= 10; i++)
		s = s*u + a[i];
	s = s*t;
	return(s);
}


//����ȫ٤����
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	lgam2
// ��    ��:	����٤����ֵ������ֵ��
// ��    ��:	 X
//   (���):	 a :    ʵ����
//               X :	ʵ����
//   (����):	 ��
// ��    ��:	double:	 ����ȫ٤����ֵ
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	
// ��    ��:    ��ʿ��. C�����㷨����(Edition 2). ����: �廪��ѧ������.1996
// ��������:	2006-4-5, zhangdan: add notes.
// �޸�����:	
//==================================================================//
double lgam2(double a, double x)
////GeoView_Tech_Mark_End
{
	int n;
	double p, q, d, s, s1, p0, q0, p1, q1, qq;
	//   extern double lgam1();
	if ((a <= 0.0) || (x<0.0))
	{
		if (a <= 0.0) printf("err**a<=0!\n");
		if (x<0.0) printf("err**x<0!\n");
		return(-1.0);
	}
	if (x + 1.0 == 1.0) return(0.0);
	if (x>1.0e+35) return(1.0);
	q = log(x); q = a*q; qq = exp(q);
	if (x<1.0 + a)
	{
		p = a; d = 1.0 / a; s = d;
		for (n = 1; n <= 100; n++)
		{
			p = 1.0 + p; d = d*x / p; s = s + d;
			if (fabs(d)<fabs(s)*1.0e-07)
			{
				s = s*exp(-x)*qq / lgam1(a);
				return(s);
			}
		}
	}
	else
	{
		s = 1.0 / x; p0 = 0.0; p1 = 1.0; q0 = 1.0; q1 = x;
		for (n = 1; n <= 100; n++)
		{
			p0 = p1 + (n - a)*p0; q0 = q1 + (n - a)*q0;
			p = x*p0 + n*p1; q = x*q0 + n*q1;
			if (fabs(q) + 1.0 != 1.0)
			{
				s1 = p / q; p1 = p; q1 = q;
				if (fabs((s1 - s) / s1)<1.0e-07)
				{
					s = s1*exp(-x)*qq / lgam1(a);
					return(1.0 - s);
				}
				s = s1;
			}
			p1 = p; q1 = q;
		}
	}
	printf("a too large !\n");
	s = 1.0 - s*exp(-x)*qq / lgam1(a);
	return(s);
}


// ����
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	lerrf
// ��    ��:	��������ֵ
// ��    ��:	X
//   (���): 	X:	 ʵ����
//   (����):	��
// ��    ��:	double:	���ֵ
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	
// ��    ��:	��ʿ��. C�����㷨����(Edition 2). ����: �廪��ѧ������.1996	
// ��������:	2006-4-5, zhangdan: add notes.
// �޸�����:	
//==================================================================//
double lerrf(double x)
////GeoView_Tech_Mark_End
{
	double y;
	//    extern double lgam2();
	if (x >= 0.0)
		y = lgam2(0.5, x*x);
	else
		y = -lgam2(0.5, x*x);
	return(y);

}

// ��̬�ֲ�����
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	lgass
// ��    ��:	������̬�ֲ�����ֵ(�۴λ��֣�
// ��    ��:	a , d, X
//   (���):	 X:	  ���������
//               a :  ��������ֵ��
//               d :  ��׼��
//   (����):	��
// ��    ��:	double:	��̬�ֲ�����ֵ
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	
// ��    ��:	
// ��������:	2006-4-5, zhangdan: add notes.
// �޸�����:	
//==================================================================//
double lgass(double a, double d, double x)
////GeoView_Tech_Mark_End
{
	double y;
	if (d <= 0.0) d = 1.0e-10;
	y = 0.5 + 0.5*lerrf((x - a) / (sqrt(2.0)*d));
	return(y);

}


// ��̬�ֲ�����
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	NormalFunction
// ��    ��:	������̬�ֲ�����ֵ
// ��    ��:	a , d, X
//   (���):	 X:	  ���������
//               a :  ��������ֵ��
//               d :  ��׼��
//   (����):	��
// ��    ��:	double:	��̬�ֲ�����ֵ
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	
// ��    ��:	
// ��������:	2006-4-6, zhangdan: add notes.
// �޸�����:	
//==================================================================//
double NormalFunction(double a, double d, double x)
////GeoView_Tech_Mark_End
{
	double y, pi;
	pi = 3.1415926;
	if (d <= 0)
		return 0;
	else
	{
		y = (exp(-(x - a)*(x - a) / (2 * d*d))) / (d*sqrt(2 * pi));
		return y;
	}

}


/*
generate a B-spline open knot vector with multiplicity
equal to the order at the ends.

c            = order of the basis function
n            = the number of defining polygon vertices
nplus2       = index of x() for the first occurence of the maximum knot vector value
nplusc       = maximum value of the knot vector -- $n + c$
x()          = array containing the knot vector
*/

void Knot(int n, int c, int * x)
{
	int nplusc, nplus2, i;
	nplusc = n + c;
	nplus2 = n + 2;

	x[1] = 0;
	for (i = 2; i <= nplusc; i++)
	{
		if ((i > c) && (i < nplus2))
			x[i] = x[i - 1] + 1;
		else
			x[i] = x[i - 1];
	}
}

/*  Subroutine to generate rational B-spline basis functions--open knot vector

C code for An Introduction to NURBS
by David F. Rogers. Copyright (C) 2000 David F. Rogers,
All rights reserved.

Name: rbais
Language: C
Subroutines called: none
Book reference: Chapter 4, Sec. 4. , p 296

c        = order of the B-spline basis function
d        = first term of the basis function recursion relation
e        = second term of the basis function recursion relation
h[]	     = array containing the homogeneous weights
npts     = number of defining polygon vertices
nplusc   = constant -- npts + c -- maximum number of knot values
r[]      = array containing the rationalbasis functions
r[1] contains the basis function associated with B1 etc.
t        = parameter value
temp[]   = temporary array
x[]      = knot vector
*/
void RBasis(int c, double t, int npts, int * x, double * h, double * r)
{
	int nplusc;
	int i, j, k;
	double d, e;
	double sum;
	double * temp = new double[npts + c + 1];

	nplusc = npts + c;

	//calculate the first order nonrational basis functions n[i]
	for (i = 1; i <= nplusc - 1; i++)
	{
		if ((t >= x[i]) && (t < x[i + 1]))
			temp[i] = 1;
		else
			temp[i] = 0;
	}

	//calculate the higher order nonrational basis functions
	for (k = 2; k <= c; k++)
	{
		for (i = 1; i <= nplusc - k; i++)
		{
			if (temp[i] != 0)    /* if the lower order basis function is zero skip the calculation */
				d = ((t - x[i])*temp[i]) / (x[i + k - 1] - x[i]);
			else
				d = 0;

			if (temp[i + 1] != 0)     /* if the lower order basis function is zero skip the calculation */
				e = ((x[i + k] - t)*temp[i + 1]) / (x[i + k] - x[i + 1]);
			else
				e = 0;

			temp[i] = d + e;
		}
	}

	if (t == (double)x[nplusc])
	{		/*    pick up last point	*/
		temp[npts] = 1;
	}

	//calculate sum for denominator of rational basis functions
	sum = 0.0;
	for (j = 1; j <= npts; j++)
	{
		sum = sum + temp[j] * h[j];
	}

	/* form rational basis functions and put in r vector */

	for (i = 1; i <= npts; i++)
	{
		if (sum != 0)
		{
			r[i] = (temp[i] * h[i]) / (sum);
		}
		else
			r[i] = 0;
	}
	if (temp)
	{
		delete[] temp;
		temp = NULL;
	}
}


//generate a rational B-spline curve using an uniform open knot vector

void CalRBSpline(int npts, int k, int p1, std::vector<double>& arConPt,
	std::vector<double> & arCurvePt)//std::vector<double> & xAr,
{
	/*  Subroutine to generate a rational B-spline curve using an uniform open knot vector

	C code for An Introduction to NURBS
	by David F. Rogers. Copyright (C) 2000 David F. Rogers,
	All rights reserved.

	Name: rbspline.c
	Language: C
	Subroutines called: knot.c, rbasis.c, fmtmul.c
	Book reference: Chapter 4, Alg. p. 297

	b[]         = array containing the defining polygon vertices
	b[1] contains the x-component of the vertex
	b[2] contains the y-component of the vertex
	b[3] contains the z-component of the vertex
	h[]			= array containing the homogeneous weighting factors
	k           = order of the B-spline basis function
	nbasis      = array containing the basis functions for a single value of t
	nplusc      = number of knot values
	npts        = number of defining polygon vertices
	p[,]        = array containing the curve points
	p[1] contains the x-component of the point
	p[2] contains the y-component of the point
	p[3] contains the z-component of the point
	p1          = number of points to be calculated on the curve
	t           = parameter value 0 <= t <= npts - k + 1
	x[]         = array containing the knot vector
	*/

	//we can calculate the NURBUS curve,from this function
	if (npts <= 0)
	{
		return;
	}
	int i = 0;
	int number = 0;
	number = arConPt.size();
	if (number != npts * 3)
	{
		return;
	}
	double * b = new double[number + 1];
	for (i = 1; i <= number; i++)
	{
		b[i] = arConPt[i - 1];
	}
	double * h = new double[npts + 1];
	for (i = 1; i <= npts; i++)
	{
		h[i] = 1.0;
	}
	double * p = new double[p1 * 3 + 1];
	for (i = 1; i <= p1 * 3; i++)
	{
		p[i] = 0.0;
	}

	//up to now, we had finished the ready work,now we will get the point in curve
	int j, icount, jcount;
	int i1;
	int * x = new int[npts + k + 1];		/* allows for 20 data points with basis function of order 5 */
	int nplusc;

	double step;
	double t;
	double * nbasis = new double[npts + 1];
	double temp;

	nplusc = npts + k;

	//zero and redimension the knot vector and the basis array
	for (i = 0; i <= npts; i++)
	{
		nbasis[i] = 0.0;
	}
	//	int nxAr = xAr.size();
	//	for (i=0;i<nxAr;i++)
	//	{
	//		x[i] = xAr.GetAt(i);
	//	}
	//generate the uniform open knot vector
	Knot(npts, k, x);
	icount = 0;
	/*    calculate the points on the rational B-spline curve */

	t = 0;
	step = ((double)x[nplusc]) / ((double)(p1 - 1));

	for (i1 = 1; i1 <= p1; i1++)
	{

		if ((double)x[nplusc] - t < 5e-6)
		{
			t = (double)x[nplusc];
		}

		RBasis(k, t, npts, x, h, nbasis);      /* generate the basis function for this value of t */
		for (j = 1; j <= 3; j++)
		{      /* generate a point on the curve */
			jcount = j;
			p[icount + j] = 0.0;

			for (i = 1; i <= npts; i++)
			{ /* Do local matrix multiplication */
				temp = nbasis[i] * b[jcount];
				p[icount + j] = p[icount + j] + temp;

				jcount = jcount + 3;
			}
		}

		icount = icount + 3;
		t = t + step;
	}

	//get the point coordinate
	arCurvePt.clear();
	for (i = 1; i <= p1 * 3; i = i + 3)
	{
		arCurvePt.push_back(p[i]);
		arCurvePt.push_back(p[i + 1]);
		arCurvePt.push_back(p[i + 2]);
	}
	int nNumber = arCurvePt.size();
	//	if ((nNumber%3) != 0)
	//	{
	//		AfxMessageBox("erro",MB_OK);
	//	}

	//clear the memery
	if (b)
	{
		delete[] b;
		b = NULL;
	}
	if (x)
	{
		delete[] x;
		x = NULL;
	}
	if (p)
	{
		delete[] p;
		p = NULL;
	}
	if (h)
	{
		delete[] h;
		h = NULL;
	}
	if (nbasis)
	{
		delete[] nbasis;
		nbasis = NULL;
	}

}


//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	IsPntInGon3D
// ��    ��:	�жϵ� pt �Ƿ��ڶ������
// ��    ��:	int nNum, Vector3d * pData, Vector3d pt
//				nNum	�ڵ����
//				pData	�ڵ�����
//				pt		���жϵĽڵ�
//   (���):	��
//   (����):	��
// ��    ��:	������ڲ��򷵻�true�����򷵻�false
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	ʸ�������жϽڵ��Ƿ��ڶ�����ڲ����ڱ���Ҳ��Ϊ���ڲ�
// ��    ��:	
// ��������:	2006-7-20, Zhangzhiting: add notes.
// �޸�����:	
//==================================================================//
bool IsPntInGon3D(int nNum, Vector3d * pData, Vector3d pt)
////GeoView_Tech_Mark_End
{
	//����ԭ��
	if (nNum < 3 || pData == NULL)
	{
		return false;
	}
	//
	//�ж��Ƿ��ڽڵ�λ��
	/*
	Vector3d ptA,ptB;
	Vector3d v1,v2,vv1,vv2;
	double lfDis = 0.0;
	for (int i=0;i<nNum;i++)
	{
	ptA[0] = pData[i][0],ptA[1] = pData[i][1],ptA[2] = pData[i][2];
	lfDis = GetDistance(ptA,pt);
	if (lfDis < 0.00001)
	{
	return true;
	}
	}

	//��ȡһ����ʼ��ʸ���������ж�
	ptA[0] = pData[0][0],ptA[1] = pData[0][1],ptA[2] = pData[0][2];
	ptB[0] = pData[nNum-1][0],ptB[1] = pData[nNum-1][1],ptB[2] = pData[nNum-1][2];
	AsubB(ptA,pt,vv1);
	AsubB(ptB,pt,vv2);
	AxB(vv1,vv2,v1);
	double lflenA = VecLen(v1);
	double lfAngle = 0.0;
	for (i=0;i<nNum-1;i++)
	{
	ptA[0] = pData[i][0],ptA[1] = pData[i][1],ptA[2] = pData[i][2];
	ptB[0] = pData[i+1][0],ptB[1] = pData[i+1][1],ptB[2] = pData[i+1][2];
	AsubB(ptA,pt,vv1);
	AsubB(ptB,pt,vv2);
	AxB(vv1,vv2,v2);

	lfAngle = AdotB(v1,v2);

	double lflenB = VecLen(v2);
	lflenA = lflenA/(lflenA*lflenB);
	if (lfAngle <= 0)
	{
	return false;
	}
	}
	return true;
	*/
	//�üнǺ��жϣ���������жϰ������
	Vector3d v1, v2, ptA, ptB;
	double lfAngle = 0.0;
	double lfAllAngle = 0.0;
	Vector3d vvvOne, vvv;
	int i = 0;//ѭ������
	for (i = 0; i<nNum; i++)
	{
		ptA[0] = pData[i][0], ptA[1] = pData[i][1], ptA[2] = pData[i][2];
		if (i == nNum - 1)
		{
			ptB[0] = pData[0][0], ptB[1] = pData[0][1], ptB[2] = pData[0][2];
		}
		else
		{
			ptB[0] = pData[i + 1][0], ptB[1] = pData[i + 1][1], ptB[2] = pData[i + 1][2];
		}
		AsubB(ptA, pt, v1);
		AsubB(ptB, pt, v2);
		VecNormalize(v1);
		VecNormalize(v2);
		AxB(v1, v2, vvvOne);
		VecNormalize(vvvOne);//������һ��
		if (VecLen(vvvOne)>0.000001)
		{
			break;
		}
	}
	for (i = 0; i<nNum; i++)
	{
		ptA[0] = pData[i][0], ptA[1] = pData[i][1], ptA[2] = pData[i][2];
		if (i == nNum - 1)
		{
			ptB[0] = pData[0][0], ptB[1] = pData[0][1], ptB[2] = pData[0][2];
		}
		else
		{
			ptB[0] = pData[i + 1][0], ptB[1] = pData[i + 1][1], ptB[2] = pData[i + 1][2];
		}
		AsubB(ptA, pt, v1);
		AsubB(ptB, pt, v2);
		VecNormalize(v1);
		VecNormalize(v2);
		lfAngle = AdotB(v1, v2);

		AxB(v1, v2, vvv);
		VecNormalize(vvv);//������һ��
		double dRes = AdotB(vvv, vvvOne);
		double fRadius = 0.0;
		if (lfAngle>-1.0 && lfAngle<1.0)
		{
			fRadius = acos(lfAngle);
		}
		else if (lfAngle <= -1.0)
		{
			fRadius = PI;
		}
		else if (lfAngle >= 1.0)
		{
			fRadius = 0;
		}
		if (fabs(dRes)<0.000001)
		{
			continue;
		}
		if (dRes<0)
		{
			lfAllAngle = lfAllAngle - fRadius;
		}
		else
		{
			lfAllAngle = lfAllAngle + fRadius;
		}
	}
	if (fabs(fabs(lfAllAngle) - 2 * PI) < 0.000001)
	{
		return true;
	}
	else
	{
		return false;
	}

}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	MayLineIntersect
// ��    ��:	�ж϶�άƽ���ϵ������Ƿ��п����ཻ���Լӿ���ֱ���󽻵��ٶ�
// ��    ��:	Vector3d A,Vector3d B,   �߶�AB�����˵�
//              Vector3d C,Vector3d D    �߶�CD�����˵�
//   (���):	��
//   (����):	��
// ��    ��:	�����ཻ:true, ������:false
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	����A��B ����ֱ�߹�ʽ��aX+bY+c=0;
//              ���(a*C[0]+b*C[1]+c)(a*D[0]+b*D[1]+c)<0����˵��C��D��AB�����࣬���п����ཻ
//                        
// ��    ��:	������ѧ�α�
// ��������:	2006-9-4, �￨, add notes.
// �޸�����:	
//==================================================================//
bool MayLineIntersect(Vector3d A, Vector3d B, Vector3d C, Vector3d D)
////GeoView_Tech_Mark_End
{
	//����AB���㣬����aX+bY+c=0����
	double a = 0;
	double b = 0;
	double c = 0;
	a = B[1] - A[1];
	b = A[0] - B[0];
	c = A[1] * B[0] - B[1] * A[0];
	double dResault = 0;
	dResault = (a*C[0] + b*C[1] + c)*(a*D[0] + b*D[1] + c);
	if (dResault<0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	XYZListToPnt3dList(std::vector<Vertex3d> XYZList,unsigned int uDataSize,Vector3d *Pnt3dList);
// ��    ��:	ʵ��Vertex3d������Vector3d����֮���ת��
// ��    ��:	��
//   (���):	��
//   (����):	��
// ��    ��:	��
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	
// ��    ��:	
// ��������:	2006-8-18, �￨, add notes.
// �޸�����:	
//==================================================================//
////GeoView_Tech_Mark_End
bool XYZListToPnt3dList(std::vector<Vertex3d> &XYZList, int nDataSize, Vector3d *Pnt3dList)
{
	if (XYZList.size()<nDataSize)
	{
		return false;
	}
	for (int i = 0; i<nDataSize; i++)
	{
		Pnt3dList[i][0] = XYZList[i].x;
		Pnt3dList[i][1] = XYZList[i].y;
		Pnt3dList[i][2] = XYZList[i].z;
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	Pnt3dListToXYZList(POINT *Pnt3dList,unsigned int uDataSize,std::vector<Vertex3d> XYZList);
// ��    ��:    ʵ��Vector3d����Vertex3d����֮���ת��	
// ��    ��:	��
//   (���):	��
//   (����):	��
// ��    ��:	��
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	
// ��    ��:	
// ��������:	2006-8-18, �￨, add notes.
// �޸�����:	
//==================================================================//
////GeoView_Tech_Mark_End
bool Pnt3dListToXYZList(Vector3d *Pnt3dList, int nDataSize, std::vector<Vertex3d> &XYZList)
{
	if (nDataSize <= 0)
	{
		return false;
	}
	for (int i = 0; i<nDataSize; i++)
	{
		Vertex3d pntTemp;
		pntTemp.x = Pnt3dList[i][0];
		pntTemp.y = Pnt3dList[i][1];
		pntTemp.z = Pnt3dList[i][2];
		XYZList.push_back(pntTemp);
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// �� �� ��:	
// ��    ��:	
// ��    ��:	��
//   (���):	��
//   (����):	��
// ��    ��:	��
// ���÷���:	
//GeoView_Doc_Mark_End  											//
// ��Ҫ˼·:	
// ��    ��:	
// ��������:	2006-9-8, �￨, add notes.
// �޸�����:	
//==================================================================//
////GeoView_Tech_Mark_End
void PntACopyB(Vector3d A, Vector3d B)
{
	//	A[0]=B[0];	
	//	A[1]=B[1;
	//	A[2]=B[2;
	memcpy(A, B, sizeof(Vector3d));
}
//
////////////////////////////////////////////////////////////////////////////
//////GeoView_Tech_Mark_Begin
////==================================================================//
////GeoView_Doc_Mark_Begin											//
//// �� �� ��:	IsSameEdge
//// ��    ��:	
//// ��    ��:	EDGE ed1,EDGE ed2
////   (���):	EDGE ed1,EDGE ed2
////   (����):	��
//// ��    ��:	bool
//// ���÷���:	
////GeoView_Doc_Mark_End  											//
//// ��Ҫ˼·:	
//// ��    ��:	
//// ��������:	2006-9-29, ZhangZhiting: add notes.
//// �޸�����:	
////==================================================================//
// bool IsSameEdge(EDGE ed1,EDGE ed2)
//////GeoView_Tech_Mark_End
//{
//	if (ed1.p1 == ed2.p1 && ed1.p2 == ed2.p2)
//	{
//		return true;
//	}
//	if (ed1.p1 == ed2.p2 && ed1.p2 == ed2.p1)
//	{
//		return true;
//	}
//	return false;
//}
//



/************************************************************************/
/*                            DMSToDegree()                             */
/************************************************************************/

static const char*sym = "NnEeSsWw";
static const double vm[] = { 1.0, 0.0166666666667, 0.00027777778 };
/************************************************************************/
/*                            proj_strtod()                             */
/************************************************************************/
static double proj_strtod(char *nptr, char **endptr)

{
	char c, *cp = nptr;
	double result;

	/*
	* Scan for characters which cause problems with VC++ strtod()
	*/
	while ((c = *cp) != '\0') {
		if (c == 'd' || c == 'D') {

			/*
			* Found one, so NUL it out, call strtod(),
			* then restore it and return
			*/
			*cp = '\0';
			result = strtod(nptr, endptr);
			*cp = c;
			return result;
		}
		++cp;
	}

	/* no offending characters, just handle normally */

	return strtod(nptr, endptr);
}
double DMSToDegree(const char *is)

{
	int sign, n, nl;
	char *p, *s, work[64];
	double v, tv;

	/* copy sting into work space */
	while (isspace((unsigned char)(sign = *is))) ++is;
	for (n = sizeof(work), s = work, p = (char *)is; isgraph(*p) && --n; )
		*s++ = *p++;
	*s = '\0';
	/* it is possible that a really odd input (like lots of leading
	zeros) could be truncated in copying into work.  But ... */
	sign = *(s = work);
	if (sign == '+' || sign == '-') s++;
	else sign = '+';
	for (v = 0., nl = 0; nl < 3; nl = n + 1) {
		if (!(isdigit(*s) || *s == '.')) break;
		if ((tv = proj_strtod(s, &s)) == HUGE_VAL)
			return tv;
		switch (*s) {
		case 'D': case 'd':
			n = 0; break;
		case '\'':
			n = 1; break;
		case '"':
			n = 2; break;
		case 'r': case 'R':
			if (nl) {
				return 0.0;
			}
			++s;
			v = tv;
			goto skip;
		default:
			v += tv * vm[nl];
		skip: n = 4;
			continue;
		}
		if (n < nl) {
			return 0.0;
		}
		v += tv * vm[n];
		++s;
	}
	/* postfix sign */
	if (*s && ((p = (char *)strchr(sym, *s))) != NULL) {
		sign = (p - sym) >= 4 ? '-' : '+';
		++s;
	}
	if (sign == '-')
		v = -v;

	return v;
}


/************************************************************************/
/*                            degreeToDMS()                             */
/*                                                                      */
/*      Translate a decimal degrees value to a DMS string with          */
/*      hemisphere.                                                     */
/************************************************************************/
const char *degreeToDMS(double dfAngle, const char * pszAxis,
	int nPrecision)

{ 
	int         nDegrees, nMinutes;
	double      dfSeconds, dfABSAngle, dfEpsilon;
	char        szFormat[30];
	const char  *pszHemisphere;
	static   char szBuffer[50] = { 0 };


	dfEpsilon = (0.5 / 3600.0) * pow(0.1, nPrecision);

	dfABSAngle = fabs(dfAngle) + dfEpsilon;
	if (dfABSAngle > 361)
	{
		return "Invalid angle";
	}

	nDegrees = (int)dfABSAngle;
	nMinutes = (int)((dfABSAngle - nDegrees) * 60);
	dfSeconds = dfABSAngle * 3600 - nDegrees * 3600 - nMinutes * 60;

	if (dfSeconds > dfEpsilon * 3600.0)
		dfSeconds -= dfEpsilon * 3600.0;

	if (cslIEqualString(pszAxis, "Long") && dfAngle < 0.0)
		pszHemisphere = "W";
	else if (cslIEqualString(pszAxis, "Long"))
		pszHemisphere = "E";
	else if (dfAngle < 0.0)
		pszHemisphere = "S";
	else
		pszHemisphere = "N";

	sprintf(szFormat, "%%3dd%%2d\'%%%d.%df\"%s", nPrecision + 3, nPrecision, pszHemisphere);
	sprintf(szBuffer, szFormat, nDegrees, nMinutes, dfSeconds);

	return(szBuffer);
}

/************************************************************************/
/*                         packedDMSToDegree()                          */
/************************************************************************/

/**
* Convert a packed DMS value (DDDMMMSSS.SS) into decimal degrees.
*
* This function converts a packed DMS angle to seconds. The standard
* packed DMS format is:
*
*  degrees * 1000000 + minutes * 1000 + seconds
*
* Example:     ang = 120025045.25 yields
*              deg = 120
*              min = 25
*              sec = 45.25
*
* The algorithm used for the conversion is as follows:
*
* 1.  The absolute value of the angle is used.
*
* 2.  The degrees are separated out:
*     deg = ang/1000000                    (fractional portion truncated)
*
* 3.  The minutes are separated out:
*     min = (ang - deg * 1000000) / 1000   (fractional portion truncated)
*
* 4.  The seconds are then computed:
*     sec = ang - deg * 1000000 - min * 1000
*
* 5.  The total angle in seconds is computed:
*     sec = deg * 3600.0 + min * 60.0 + sec
*
* 6.  The sign of sec is set to that of the input angle.
*
* Packed DMS values used by the USGS GCTP package and probably by other
* software.
*
* NOTE: This code does not validate input value. If you give the wrong
* value, you will get the wrong result.
*
* @param dfPacked Angle in packed DMS format.
*
* @return Angle in decimal degrees.
*
*/

double packedDMSToDegree(PACKED_DMS_TYPE dfPacked)
{
	double  dfDegrees, dfMinutes, dfSeconds, dfSign;

	dfSign = (dfPacked < 0.0) ? -1 : 1;

	dfSeconds = fabs(dfPacked);
	dfDegrees = floor(dfSeconds / 1000000.0);
	dfSeconds = dfSeconds - dfDegrees * 1000000.0;
	dfMinutes = floor(dfSeconds / 1000.0);
	dfSeconds = dfSeconds - dfMinutes * 1000.0;
	dfSeconds = dfSign * (dfDegrees * 3600.0 + dfMinutes * 60.0 + dfSeconds);
	dfDegrees = dfSeconds / 3600.0;

	return dfDegrees;
}

/************************************************************************/
/*                         degreeToPackedDMS()                          */
/************************************************************************/
/**
* Convert decimal degrees into packed DMS value (DDDMMMSSS.SS).
*
* This function converts a value, specified in decimal degrees into
* packed DMS angle. The standard packed DMS format is:
*
*  degrees * 1000000 + minutes * 1000 + seconds
*
* See also CPLPackedDMSToDec().
*
* @param dfDec Angle in decimal degrees.
*
* @return Angle in packed DMS format.
*
*/

double degreeToPackedDMS(double dfDec)
{
	double  dfDegrees, dfMinutes, dfSeconds, dfSign;

	dfSign = (dfDec < 0.0) ? -1 : 1;

	dfDec = fabs(dfDec);
	dfDegrees = floor(dfDec);
	dfMinutes = floor((dfDec - dfDegrees) * 60.0);
	dfSeconds = (dfDec - dfDegrees) * 3600.0 - dfMinutes * 60.0;

	return dfSign * (dfDegrees * 1000000.0 + dfMinutes * 1000.0 + dfSeconds);
}


end_gdb_namespace
end_gtl_namespace



 