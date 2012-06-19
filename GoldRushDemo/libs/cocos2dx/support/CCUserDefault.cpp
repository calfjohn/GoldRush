/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#include "CCUserDefault.h"
#include "platform/CCFileUtils.h"

#include <libxml/parser.h>
#include <libxml/tree.h>

// root name of xml
#define USERDEFAULT_ROOT_NAME	"userDefaultRoot"

#define XML_FILE_NAME "UserDefault.xml"

using namespace std;

NS_CC_BEGIN;

/**
 * define the functions here because we don't want to
 * export xmlNodePtr and other types in "CCUserDefault.h"
 */

static xmlNodePtr findChild(const char* pKey, xmlNodePtr *rootNode)
{
	xmlNodePtr curNode = NULL;

	if (NULL == rootNode) 
	{
		return curNode;
	}
	
	curNode = (*rootNode)->xmlChildrenNode;
	while (NULL != curNode)
	{		
		if (! xmlStrcmp(curNode->name, BAD_CAST pKey))
		{
			break;
		}
		xmlNodePtr nodeTemp = findChild(pKey, &curNode);
		if (NULL != nodeTemp)
		{
			curNode = nodeTemp;
			break;
		}
		
		curNode = curNode->next;
	}
	
	return curNode;
}

/*
static xmlNodePtr getXMLNodeForKey(const char* pKey, xmlNodePtr *rootNode, xmlDocPtr *doc)
{
	xmlNodePtr curNode = NULL;

	// check the key value
	if (! pKey)
	{
		return NULL;
	}

	do 
	{
		// read doc
		*doc = xmlReadFile(CCUserDefault::sharedUserDefault()->getXMLFilePath().c_str(), "utf-8", XML_PARSE_RECOVER);
		if (NULL == *doc)
		{
			CCLOG("can not read xml file");
			break;
		}

		// get root node
		*rootNode = xmlDocGetRootElement(*doc);
		if (NULL == *rootNode)
		{
			CCLOG("read root node error");
			break;
		}

		// find the node
		curNode = findChild(pKey, rootNode);
		

	} while (0);

	return curNode;
}*/

static xmlNodePtr getXMLNodeForKey(const char* pSubNode, const char* pKey, xmlNodePtr *rootNode, xmlDocPtr *doc)
{
	xmlNodePtr curNode = NULL;
	
	// check the key value
	if (! pKey)
	{
		return NULL;
	}
	
	do 
	{
		// read doc
		*doc = xmlReadFile(CCUserDefault::sharedUserDefault()->getXMLFilePath().c_str(), "utf-8", XML_PARSE_RECOVER);
		if (NULL == *doc)
		{
			CCLOG("can not read xml file");
			break;
		}
		
		// get root node
		*rootNode = xmlDocGetRootElement(*doc);
		if (NULL == *rootNode)
		{
			CCLOG("read root node error");
			break;
		}
		
		// find the node
		if (pSubNode) 
		{
			curNode = findChild(pSubNode, rootNode);
			if (NULL != curNode)
			{
				curNode = findChild(pKey, &curNode);
			}
		}
		else 
		{
			curNode = findChild(pKey, rootNode);
		}		

	} while (0);
	
	return curNode;
}


const char* getValueForKey(const char* pKey, const char *pSubNode = NULL)
{
	const char* ret = NULL;
	xmlNodePtr rootNode;
	xmlDocPtr doc;
	xmlNodePtr node = getXMLNodeForKey(pSubNode, pKey, &rootNode, &doc);
	
	// find the node
	if (node)
	{
		ret = (const char*)xmlNodeGetContent(node);
	}

	// free doc
	if (doc)
	{
		xmlFreeDoc(doc);
	}

	return ret;
}

void setValueForKey(const char* pKey, const char* pValue, const char *pSubNode = NULL)
{
	xmlNodePtr rootNode;
	xmlDocPtr doc;
	xmlNodePtr node;

	// check the params
	if (! pKey || ! pValue)
	{
		return;
	}

	// find the node
	node = getXMLNodeForKey(pSubNode, pKey, &rootNode, &doc);

	// if node exist, change the content
	if (node)
	{
		xmlNodeSetContent(node, BAD_CAST pValue);
	}
	else
	{
		if (rootNode)
		{
			// the node doesn't exist, add a new one after subNode
			// libxml in android donesn't support xmlNewTextChild, so use this approach
			node = getXMLNodeForKey(NULL, pSubNode, &rootNode, &doc);
			if (!node) 
			{
				node = xmlNewNode(NULL, BAD_CAST pSubNode);
				xmlAddChild(rootNode, node);
				
				xmlNodePtr tmpNode = xmlNewNode(NULL, BAD_CAST pKey);
				xmlNodePtr content = xmlNewText(BAD_CAST pValue);
				xmlAddChild(node, tmpNode);
				xmlAddChild(tmpNode, content);
			}
			else
			{
				xmlNodePtr tmpNode = xmlNewNode(NULL, BAD_CAST pKey);
				xmlNodePtr content = xmlNewText(BAD_CAST pValue);
				xmlAddChild(node, tmpNode);
				xmlAddChild(tmpNode, content);
			}
		}	
	}

	// save file and free doc
	if (doc)
	{
		xmlSaveFile(CCUserDefault::sharedUserDefault()->getXMLFilePath().c_str(),doc);
		xmlFreeDoc(doc);
	}
}

/**
 * implements of CCUserDefault
 */

CCUserDefault* CCUserDefault::m_spUserDefault = 0;
string CCUserDefault::m_sFilePath = string("");
bool CCUserDefault::m_sbIsFilePathInitialized = false;

/**
 * If the user invoke delete CCUserDefault::sharedUserDefault(), should set m_spUserDefault
 * to null to avoid error when he invoke CCUserDefault::sharedUserDefault() later.
 */
CCUserDefault::~CCUserDefault()
{
	m_spUserDefault = NULL;
}

void CCUserDefault::purgeSharedUserDefault()
{
	CC_SAFE_DELETE(m_spUserDefault);
	m_spUserDefault = NULL;
}

bool CCUserDefault::getBoolForKey(const char* pKey, bool defaultValue, const char *pSubNode)
{
	const char* value = getValueForKey(pKey, pSubNode);
	bool ret = defaultValue;

	if (value)
	{
		ret = (! strcmp(value, "true"));
		xmlFree((void*)value);
	}

	return ret;
}

int CCUserDefault::getIntegerForKey(const char* pKey, int defaultValue, const char *pSubNode)
{
	const char* value = getValueForKey(pKey, pSubNode);
	int ret = defaultValue;

	if (value)
	{
		ret = atoi(value);
		xmlFree((void*)value);
	}

	return ret;
}

float CCUserDefault::getFloatForKey(const char* pKey, float defaultValue, const char *pSubNode)
{
	float ret = (float)getDoubleForKey(pKey, (double)defaultValue, pSubNode);
 
	return ret;
}

double CCUserDefault::getDoubleForKey(const char* pKey, double defaultValue, const char *pSubNode)
{
	const char* value = getValueForKey(pKey, pSubNode);
	double ret = defaultValue;

	if (value)
	{
		ret = atof(value);
		xmlFree((void*)value);
	}

	return ret;
}

string CCUserDefault::getStringForKey(const char* pKey, const std::string & defaultValue, const char *pSubNode)
{
	const char* value = getValueForKey(pKey, pSubNode);
	string ret = defaultValue;

	if (value)
	{
		ret = string(value);
		xmlFree((void*)value);
	}

	return ret;
}

void CCUserDefault::setBoolForKey(const char* pKey, bool value, const char *pSubNode)
{
    // save bool value as sring

	if (true == value)
	{
		setStringForKey(pKey, "true", pSubNode);
	}
	else
	{
		setStringForKey(pKey, "false", pSubNode);
	}
}

void CCUserDefault::setIntegerForKey(const char* pKey, int value, const char *pSubNode)
{
	// check key
	if (! pKey)
	{
		return;
	}

	// format the value
	char tmp[50];
	memset(tmp, 0, 50);
	sprintf(tmp, "%d", value);

	setValueForKey(pKey, tmp, pSubNode);
}

void CCUserDefault::setFloatForKey(const char* pKey, float value, const char *pSubNode)
{
	setDoubleForKey(pKey, value, pSubNode);
}

void CCUserDefault::setDoubleForKey(const char* pKey, double value, const char *pSubNode)
{
	// check key
	if (! pKey)
	{
		return;
	}

	// format the value
    char tmp[50];
	memset(tmp, 0, 50);
	sprintf(tmp, "%f", value);

	setValueForKey(pKey, tmp, pSubNode);
}

void CCUserDefault::setStringForKey(const char* pKey, const std::string & value, const char *pSubNode)
{
	// check key
	if (! pKey)
	{
		return;
	}

	setValueForKey(pKey, value.c_str(), pSubNode);
}

CCUserDefault* CCUserDefault::sharedUserDefault()
{
	initXMLFilePath();

	// only create xml file one time
	// the file exists after the programe exit
	if ((! isXMLFileExist()) && (! createXMLFile()))
	{
		return NULL;
	}

	if (! m_spUserDefault)
	{
		m_spUserDefault = new CCUserDefault();
	}

	return m_spUserDefault;
}

bool CCUserDefault::isXMLFileExist()
{
	FILE *fp = fopen(m_sFilePath.c_str(), "r");
	bool bRet = false;

	if (fp)
	{
		bRet = true;
		fclose(fp);
	}

	return bRet;
}

void CCUserDefault::initXMLFilePath()
{
	if (! m_sbIsFilePathInitialized)
	{
		m_sFilePath += CCFileUtils::getWriteablePath() + XML_FILE_NAME;
		m_sbIsFilePathInitialized = true;
	}	
}

// create new xml file
bool CCUserDefault::createXMLFile()
{
	bool bRet = false;
    xmlDocPtr doc = NULL;

	do 
	{
		// new doc
		doc = xmlNewDoc(BAD_CAST"1.0");
		if (doc == NULL)
		{
			CCLOG("can not create xml doc");
			break;
		}

		// new root node
		xmlNodePtr rootNode = xmlNewNode(NULL, BAD_CAST USERDEFAULT_ROOT_NAME);
		if (rootNode == NULL)
		{
			CCLOG("can not create root node");
			break;
		}

		// set root node
		xmlDocSetRootElement(doc, rootNode);

		// save xml file
		xmlSaveFile(m_sFilePath.c_str(), doc);

		bRet = true;
	} while (0);

	// if doc is not null, free it
	if (doc)
	{
		xmlFreeDoc(doc);
	}

	return bRet;
}

const string& CCUserDefault::getXMLFilePath()
{
	return m_sFilePath;
}

bool CCUserDefault::openXMLFile(std::string fileName)
{
	FILE *fp = fopen(fileName.c_str(), "r");
	bool bRet = false;
	
	if (fp)
	{
		m_sFilePath = fileName;
		m_sbIsFilePathInitialized = true;
		bRet = true;
		fclose(fp);
	}
	
	return bRet;
}

NS_CC_END;
