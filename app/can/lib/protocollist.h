/* name: protocollist.h
 * description: 协议链表操作
 * author: zuokongxiao
 * date: 2014-09-04
 */

#ifndef __PROTOCOLLIST_H
#define __PROTOCOLLIST_H

#include "./list.h"

/* 信号量返回值定义 */
enum sem_ret_e
{
    SEM_NORMAL_RET = 0,
    SEM_TIMEOUT_RET,
    SEM_ERROR_RET
};

/* 协议链表结构体定义 */
struct protocol_list
{
    sem_t semfd; /* 信号量标识符 */
    int semret; /* 信号量返回值 */
    uint8 ucCmdFlag; /* 命令标志 */
    uint8 ucCmdType; /* 命令类型 */
    uint8 ucCmdNum; /* 命令对象 */
    uint8 ucCmdContent; /* 命令内容 */
    uint8 ucCmdParamLen; /* 参数长度 */
    uint8 aucCmdParam[32]; /* 命令参数 */
    struct list_head list;
};

int plist_create(struct protocol_list *pstHead);
int plist_add(char cmdtype, char cmdnum, char cmdcontent, struct protocol_list *pstHead);
int plist_del(char cmdtype, char cmdnum, char cmdcontent, struct protocol_list *pstHead);
int plist_match(char cmdtype, char cmdnum, char cmdcontent, char *param, char paramlen, struct protocol_list *pstHead);

#endif

