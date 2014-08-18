#include <ctype.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include "common.h"
#include "deftype.h"
#include "debug.h"


int main(int argc, char *argv[])
{
    int fd;
    int funcs, addr = 0xa0;

    /* open */
    fd = open("/dev/i2c-0", O_RDONLY);
    if(-1 == fd)
    {
        ERROR_MSG("open /dev/i2c-0 error!\r\n");
        exit(-1);
    }
    DEBUG_MSG("open /dev/i2c-0 ok! fd=%d\r\n", fd);

#if 1
    // get funcs list
    if((ioctl(fd, I2C_FUNCS, &funcs) < 0))
    {
        ERROR_MSG("write i2c funs!\r\n");
        return -1;
    }
#endif
    // check for req funcs
    /*CHECK_I2C_FUNC(funcs, I2C_FUNC_SMBUS_READ_BYTE);
    CHECK_I2C_FUNC(funcs, I2C_FUNC_SMBUS_WRITE_BYTE);
    CHECK_I2C_FUNC(funcs, I2C_FUNC_SMBUS_READ_BYTE_DATA);
    CHECK_I2C_FUNC(funcs, I2C_FUNC_SMBUS_WRITE_BYTE_DATA);
    CHECK_I2C_FUNC(funcs, I2C_FUNC_SMBUS_READ_WORD_DATA);
    CHECK_I2C_FUNC(funcs, I2C_FUNC_SMBUS_WRITE_WORD_DATA);*/

    if(ioctl(fd, I2C_SLAVE, addr) < 0)
    {
        ERROR_MSG("write i2c addr!\r\n");
        //exit(-1);
    }

    close(fd);
    DEBUG_MSG("close /dev/i2c-0!\r\n");
    exit(0);
}

