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
#include <linux/can.h>
#include <linux/can/error.h>
#include <linux/can/raw.h>
#include <pthread.h>
#include <semaphore.h>
#include "deftype.h"
#include "debug.h"
#include "can.h"


void *can_rec_thread(void *arg)
{
    struct can_frame stCanFrame;

    while(1)
    {
        can_read(&stCanFrame);
    }
}





