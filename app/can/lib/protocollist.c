/* name: canrecthread.c
* description: CAN接收线程
* author: zuokongxiao
* date: 2014-08-28
*/
#include <stdio.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <linux/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "deftype.h"
#include "debug.h"
#include "protocollist.h"

//plist_create 
int plist_create(struct protocol_list *pstHead)
{
    if(INVALID_POINTER(pstHead))
    {
        DEBUG_MSG("E:input param error!\r\n");
        return -1;
    }
    INIT_LIST_HEAD(&(pstHead->list));
    return 0;
}

//plist_add 发送时创建
int plist_add(char cmdtype, char cmdnum, char cmdcontent, struct protocol_list *pstHead)
{
    struct protocol_list *pstNew = NULL;
    int res = 0;
    struct timespec ts;
    
    if(INVALID_POINTER(pstHead))
    {
        DEBUG_MSG("E:input param error!\r\n");
        return -1;
    }
    /* 申请内存 */
    pstNew = (struct protocol_list *)malloc(sizeof(struct protocol_list));
    if(NULL == pstNew)
    {
        ERROR_MSG("E:malloc failed!\r\n");
        return -1;
    }
    memset(pstNew, 0, sizeof(struct protocol_list));
    /* 命令 */
    pstNew->ucCmdType = cmdtype;
    pstNew->ucCmdNum = cmdnum;
    pstNew->ucCmdContent = cmdcontent;
    /* 创建信号量 */
    res = sem_init(&(pstNew->semfd), 0, 0);
    if(0 > res)
    {
        ERROR_MSG("E:sem_init failed!\r\n");
        free(pstNew);
        return -1;
    }
    /* 将新节点加入链表 */
    list_add_tail(&(pstNew->list), &(pstHead->list));
    
    /* 信号量等待 */
    if(0 > clock_gettime(CLOCK_REALTIME, &ts))
    {
        ERROR_MSG("E:clock_gettime!\r\n");
        free(pstNew);
        sem_destroy(&(pstNew->semfd));
        return -1;
    }
    ts.tv_sec += 20;
    while((res = sem_timedwait(&(pstNew->semfd), &ts)) == -1 && errno == EINTR)
    {
        continue; /* Restart if interrupted by handler */
    }
    /* Check what happened */
    if(-1 == res)
    {
        if(errno == ETIMEDOUT)
        {
            pstNew->semret = SEM_TIMEOUT_RET;
        }
        else
        {
            pstNew->semret = SEM_ERROR_RET;
        }
        ERROR_MSG("E:sem_timedwait ret error!\r\n");
    }
    else
    {
        pstNew->semret = SEM_NORMAL_RET;
    }
    
    return 0;
}

//plist_del 命令返回时删除 删除分配的内存
int plist_del(char cmdtype, char cmdnum, char cmdcontent, struct protocol_list *pstHead)
{
    struct protocol_list *pstTemp = NULL;
    struct list_head *pos;
    int res = 0;
    
    if(INVALID_POINTER(pstHead))
    {
        DEBUG_MSG("E:input param error!\r\n");
        return -1;
    }
    /* 遍历链表 */
    res = -1;
    list_for_each(pos, &(pstHead->list))
    {
        pstTemp = list_entry(pos, struct protocol_list, list);
        if((cmdtype == pstTemp->ucCmdType) && (cmdnum == pstTemp->ucCmdNum) \
            && (cmdcontent == pstTemp->ucCmdContent))
        {
            list_del(pos); /* 删除节点 */
            sem_destroy(&(pstTemp->semfd)); /* 信号量删除 */
            free(pstTemp); /* 释放内存 */
            res = 0;
            break;
        }
    }
    //if(list_empty(&(pstHead->list)))
    //printf("now the list if empty\n");
    
    return res;
}

//plist_match 判断结束的命令是否在链表中
int plist_match(char cmdtype, char cmdnum, char cmdcontent, char *param, char paramlen, struct protocol_list *pstHead)
{
    struct protocol_list *pstTemp = NULL;
    struct list_head *pos;
    int res = 0, i = 0;
    
    if(INVALID_POINTER(pstHead))
    {
        DEBUG_MSG("E:input param error!\r\n");
        return -1;
    }
    /* 遍历链表 */
    res = -1;
    list_for_each(pos, &(pstHead->list))
    {
        pstTemp = list_entry(pos, struct protocol_list, list);
        if((cmdtype == pstTemp->ucCmdType) && (cmdnum == pstTemp->ucCmdNum) \
            && (cmdcontent == pstTemp->ucCmdContent))
        {
            /* 参数复制 */
            if(NULL != param)
            {
                memcpy(pstTemp->aucCmdParam, param, paramlen);
            }
            pstTemp->ucCmdParamLen = paramlen;
            /* 信号量发送 */
            sem_post(&(pstTemp->semfd));
            res = 0;
            break;
        }
    }
    
    return res;
}



