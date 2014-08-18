/*! \file  debug.h
* \b 接口说明：
*
*
* \b 功能描述:
*
*       调试信息打印
*
* \b 历史记录:
*
*     <作者>        <时间>      <修改记录> <br>
*     zuokongxiao  2014-06-12   创建文件   <br>
*/

#ifndef __DEBUG_H
#define __DEBUG_H

/******************************************************************************/
/*                            头文件                                          */
/******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

/******************************************************************************/
/*                           宏定义                                           */
/******************************************************************************/
/*! 配置是否输出调试信息 */
#define PRINTF_MSG_EN                   (1)
/*! 配置是否输出调试信息 */
#define DEBUG_MSG_EN                    (2)
/*! 配置是否输出错误信息 */
#define ERROR_MSG_EN                    (1)

/*! printf 宏定义 */
#if PRINTF_MSG_EN > 0
 #define PRINTF_MSG(arg...)     printf(##arg)
#else
 #define PRINTF_MSG(arg...)     ((void)0)
#endif

/*! 调试信息 */
#if DEBUG_MSG_EN == 1 //普通打印
 #define PRINTF_MSG(arg...)     printf(##arg)
#elif DEBUG_MSG_EN == 2 //打印文件名、行号
 #define DEBUG_MSG(format,arg...) do{ \
         print_run_time(__FILE__, __LINE__); \
         printf(format, ##arg); \
        } while(0)
#elif DEBUG_MSG_EN == 3 //打印文件名、行号、函数名
 #define DEBUG_MSG(format,arg...) do{ \
         print_run_time(__FILE__, __LINE__); \
         printf("[F:%s] "format, __FUNCTION__, ##arg); \
        } while(0)
#else
 #define DEBUG_MSG(arg)      ((void)0)
#endif

/*! 错误信息 */
#if ERROR_MSG_EN == 1
 #define ERROR_MSG(format,arg...) do{ \
         print_run_time(__FILE__, __LINE__); \
         printf("[E:%s] "format, strerror(errno), ##arg); \
        } while(0)
#else
 #define ERROR_MSG()        ((void)0)
#endif

/******************************************************************************/
/*                          外部接口函数声明                                  */
/******************************************************************************/
void print_run_time(char *pFile, int lLine);

#endif /*!< end __DEBUG_H */

/********************************end of file***********************************/

