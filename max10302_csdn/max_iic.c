#include "max_iic.h"

void I2C_delay(void)
{
    uint16_t i=160; //Set delay time value
    while(i)
    {
        i--;
    }
}
/**
 * @brief I2C Start
 * @param None
 * @retval None
 */
bool I2C_Start(void)
{
    SDA_H;
    I2C_delay();
    SCL_H;
    I2C_delay();
    if(!SDA_read)return FALSE;	//SDA Always low return FALSE
    SDA_L;
    I2C_delay();
    if(SDA_read) return FALSE;	//SDA Always high return FALSE
    SCL_L;
    I2C_delay();
    return TRUE;
}
/**
 * @brief I2C Stop
 * @param None
 * @retval None
 */
void I2C_Stop(void)
{
    SCL_L;
    I2C_delay();
    SDA_L;
    I2C_delay();
    SCL_H;
    I2C_delay();
    SDA_H;
    I2C_delay();
}
/**
 * @brief I2C Ack
 * @param None
 * @retval None
 */
void I2C_Ack(void)
{
    SCL_L;
    I2C_delay();
    SDA_L;
    I2C_delay();
    SCL_H;
    I2C_delay();
    SCL_L;
    I2C_delay();


}
/**
 * @brief I2C No Ack
 * @param None
 * @retval None
 */
void I2C_NoAck(void)
{
    SCL_L;
    I2C_delay();
    SDA_H;
    I2C_delay();
    SCL_H;
    I2C_delay();
    SCL_L;
    I2C_delay();
}
/**
 * @brief  Wait Ack
 * @param  None
 * @retval bool FALSE:1--->no ACK
 *              TRUE :0--->ACK
 */
uint8_t I2C_WaitAck(void)
{
    uint8_t re;
    SCL_L;
    I2C_delay();
    SDA_H;
    I2C_delay();
    SCL_H;
    I2C_delay();
    if(SDA_read)
    {
        re=1;
    }
    else re=0;
    SCL_L;
    return re;
}
/**
 * @brief Send one Byte
 * @param uint8_t SendByte
 * @retval None
 */
void I2C_SendByte(uint8_t SendByte)
{
    uint8_t i=8;

    while(i--)
    {
        SCL_L;
        I2C_delay();
        if((SendByte&0x80))
        {
            SDA_H;
        }
        else
        {
            SDA_L;
        }
        SendByte<<=1;
        I2C_delay();
        SCL_H;
        I2C_delay();

    }
    SCL_L;
}
/**
 * @brief Receive one Byte
 * @param uint8_t ack
 * @retval receive  receive one byte
 */
uint8_t I2C_ReceiveByte(uint8_t ack)
{
    unsigned char i=8,receive=0;
    SDA_H;
    while(i--)
    {
        receive<<=1;
        SCL_L;
        I2C_delay();
        SCL_H;
        I2C_delay();
        if(SDA_read)
        {
            receive|=0x01;
        }
    }
    SCL_L;
    if (!ack)
        I2C_NoAck();  //Send Nack
        else
            I2C_Ack();   //Send ack
            return receive;
}
/**
 * @brief Write a Byte to the device
 * @param uint8_t WriteAddr,uint8_t WriteData
 * @retval bool FALSE: 0
 *              TRUE : 1
 */
bool I2C_Write(uint8_t dev,uint8_t WriteAddr,uint8_t WriteData)
{
    I2C_Start();
    I2C_SendByte(dev); //Send write cmd
    I2C_WaitAck();
    I2C_SendByte(WriteAddr); //Send addr
    I2C_WaitAck();
    I2C_SendByte(WriteData); //Send data
    I2C_WaitAck();
    I2C_Stop();		   	//iic stop
    return TRUE;
}
/********************************************************************/
bool I2C_Write2(uint8_t WriteAddr,uint8_t WriteData)
{
    if (!I2C_Start()) return FALSE;
    I2C_SendByte(0x10);//设置器件地址+段地址
    if (!I2C_WaitAck())
    {
        I2C_Stop();
        return FALSE;
    }
    I2C_SendByte(WriteAddr);   //设置段内地址
    I2C_WaitAck();

    I2C_SendByte(WriteData);
    I2C_WaitAck();
    I2C_Stop();

    return TRUE;
}
/************************************************************************/
/**
 * @brief Read a byte from the device
 * @param uint8_t WriteAddr
 * @retval temp  Return the read byte
 */
uint8_t I2C_Read(uint8_t dev,uint8_t WriteAddr)
{
    uint8_t temp=0;
    I2C_Start();
    I2C_SendByte(dev);  //Send write cmd
    I2C_WaitAck();
    I2C_SendByte(WriteAddr); //Send addr
    I2C_WaitAck();

    I2C_Start();
    I2C_SendByte(dev|1);  //Send read cmd
    I2C_WaitAck();
    temp=I2C_ReceiveByte(0);
    I2C_Stop();
    return temp;
}
/*********************************************************************************/
//读出1串数据
uint8_t I2C_Read2(uint8_t WriteAddr)
{
    uint8_t tempDat=0;
    if (!I2C_Start()) return FALSE;
    I2C_SendByte(0x77);//设置器件地址+段地址

    if (!I2C_WaitAck())
    {
        I2C_Stop();
        return FALSE;
    }
    I2C_SendByte(WriteAddr);   //设置低起始地址
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte(0x77 | 0x01);
    I2C_WaitAck();
    tempDat = I2C_ReceiveByte(0);
    I2C_Stop();

    return tempDat;
}
/**
 * @brief Read continuously
 * @param uint8_t devaddr  device addr
 *        uint8_t addr     Start addr
 *        uint8_t len      read data length
 *        uint8_t *rbuf    read data buf
 * @retval None
 */
void I2C_DevRead(uint8_t devaddr,uint8_t addr,uint8_t len,uint8_t *rbuf)
{
    int i=0;
    I2C_Start();
    I2C_SendByte(devaddr);
    if(!I2C_WaitAck())
    {
        I2C_Stop();
        return ;
    }
    I2C_SendByte(addr);  //address ++
    if(!I2C_WaitAck())
    {
        I2C_Stop();
        return ;
    }
    I2C_Start();
    I2C_SendByte(devaddr|0x01);
    if(!I2C_WaitAck())
    {
        I2C_Stop();
        return ;
    }
    for(i=0; i<len; i++)
    {

        if(i==len-1)
        {
            rbuf[i]=I2C_ReceiveByte(0);  //The last byte does not answer
        }
        else
            rbuf[i]=I2C_ReceiveByte(1);
    }
    I2C_Stop( );
}

/**
 * @brief Write continuously
 * @param uint8_t devaddr  device addr
 *        uint8_t addr     Start addr
 *        uint8_t len      read data length
 *        uint8_t *rbuf    read data buf
 * @retval None
 */
void I2C_DevWrite(uint8_t devaddr,uint8_t addr,uint8_t len,uint8_t *wbuf)
{
    int i=0;
    I2C_Start();
    I2C_SendByte(devaddr);
    I2C_WaitAck();
    I2C_SendByte(addr);  //address ++
    I2C_WaitAck();
    for(i=0; i<len; i++)
    {
        I2C_SendByte(wbuf[i]);
        I2C_WaitAck();
    }
    I2C_Stop( );
}

//==========================================================

void IIC_WriteBytes(uint8_t WriteAddr,uint8_t* data,uint8_t dataLength)
{
    uint8_t i;
    I2C_Start();

    I2C_SendByte(WriteAddr);	    //发送写命令
    I2C_WaitAck();

    for(i=0;i<dataLength;i++)
    {
        I2C_SendByte(data[i]);
        I2C_WaitAck();
    }
    I2C_Stop();//产生一个停止条件
    I2C_delay();
}

void IIC_ReadBytes(uint8_t deviceAddr, uint8_t writeAddr,uint8_t* data,uint8_t dataLength)
{
    uint8_t i;
    I2C_Start();

    I2C_SendByte(deviceAddr);	    //发送写命令
    I2C_WaitAck();
    I2C_SendByte(writeAddr);
    I2C_WaitAck();
    I2C_SendByte(deviceAddr|0X01);//进入接收模式
    I2C_WaitAck();

    for(i=0;i<dataLength-1;i++)
    {
        data[i] = I2C_ReceiveByte(1);
    }
    data[dataLength-1] = I2C_ReceiveByte(0);
    I2C_Stop();//产生一个停止条件
    I2C_delay();
}

void IIC_Read_One_Byte(uint8_t daddr,uint8_t addr,uint8_t* data)
{
    I2C_Start();

    I2C_SendByte(daddr);	   //发送写命令
    I2C_WaitAck();
    I2C_SendByte(addr);//发送地址
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte(daddr|0X01);//进入接收模式
    I2C_WaitAck();
    *data = I2C_ReceiveByte(0);
    I2C_Stop();//产生一个停止条件
}

void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data)
{
    I2C_Start();

    I2C_SendByte(daddr);	    //发送写命令
    I2C_WaitAck();
    I2C_SendByte(addr);//发送地址
    I2C_WaitAck();
    I2C_SendByte(data);     //发送字节
    I2C_WaitAck();
    I2C_Stop();//产生一个停止条件
    I2C_delay();
}
