
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>
#include <libxml/parser.h>
#include <libxml/xmlmemory.h>
#include "deftype.h"
#include "debug.h"

/*! 文件命令 */
#define XML_NAME        "./config/hardware.xml"


/* 解析节点属性 */
static int parse_node_attr(xmlNodePtr pNode, xmlAttrPtr pAttr)
{
    xmlChar *pStr;

    while(NULL != pAttr)
    {
        /* 获取节点属性值   */
        pStr = xmlGetProp(pNode, (const xmlChar *)(pAttr->name));
        /* 打印节点属性 */
        DEBUG_MSG("D:%s node attr %s=%s\r\n", pNode->name, pAttr->name, pStr);
        xmlFree(pStr);
        /* 指向下一个 */
        pAttr = pAttr->next;
    }

    return 0;
}

/* 主函数 */
int main(int argc, char *argv[])
{
    int res = 0;
    xmlDocPtr doc;
    xmlNodePtr cur, curchildren;
    xmlChar *str;
    xmlAttrPtr attrPtr;
    char *filedir;

    /* 输入参数检查 */
    if(2 != argc)
    {
        DEBUG_MSG("E:input param error.\r\n");
        exit(0);
    }
    filedir = malloc(strlen(argv[1])+strlen("./config/")+4);
    strcpy(filedir, "./config/");
    strcat(filedir, argv[1]);
    DEBUG_MSG("D:file dir = %s\r\n", filedir);

    /* 载入xml文件 */
    DEBUG_MSG("D:parse xml file.\r\n");
    doc = xmlParseFile(filedir);
    if(NULL == doc)
    {
        free(filedir);
        DEBUG_MSG("E:document not parsed successfully.\r\n");
        exit(0);
    }
    free(filedir);
    /* 获取根节点 */
    cur = xmlDocGetRootElement(doc);
    if(NULL == cur)
    {
        DEBUG_MSG("E:empty document.\r\n");
        xmlFreeDoc(doc);
        exit(0);
    }
    /* 根节点名称判断 */
    if(xmlStrcmp(cur->name, (const char *)"root"))
    {
        DEBUG_MSG("E:document of the wrong type, root node != root.\r\n");
        xmlFreeDoc(doc);
        exit(0);
    }

    /* root子节点 */
    cur = cur->children;
    while(NULL != cur)
    {
        /* 节点名称判断 */
        //if(0 == xmlStrcmp(cur->name, (const xmlChar *)"items"))
        {
            /* 节点属性 */
            attrPtr = cur->properties;
            /* 节点属性解析 */
            parse_node_attr(cur, attrPtr);

            /* 子节点解析 */
            curchildren = cur->children;
            while(NULL != curchildren)
            {
                /* 子节点属性 */
                attrPtr = curchildren->properties;
                /* 子节点属性解析 */
                parse_node_attr(curchildren, attrPtr);
                
                curchildren = curchildren->next;
            }
        }
        /* 指向下一个兄弟节点 */
        cur = cur->next;
    }

    /* 释放xml文档 */
    xmlFreeDoc(doc);
    DEBUG_MSG("D:parse xml file ok.\r\n");

    exit(0);
}



