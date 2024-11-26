/*
*********************************************************************************************************
*
*	模块名称 : NRF24L01模块驱动
*	文件名称 : bsp_RF24l01.c
*	版    本 : V1.0
*	说    明 : NRF24l01驱动文件，根据实际连接IO修改.h文件中的宏定义
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2019-05-27  		  
*
*
*********************************************************************************************************
*/
#include "bsp.h"

const char *g_ErrorString = "RF24L01 is not find !...";

/* 宏定义 */
#ifndef __USE_SOFT_SPI_INTERFACE__
#define SPI_WAIT_TIMEOUT			((uint16_t)0xFFFF)		/** 硬件SPI超时 */
//#define 	__RF24L01_TX_TEST__							//**@@ 如果测试发送功能则需要定义该宏，如果测试接收则需要屏蔽该宏 **@@//

/* 变量定义 */
const char *g_Ashining = "ashining";
uint8_t g_TxMode = 0;				/* 发送模式默认是固定发送 */
uint8_t g_UartRxBuffer[ 100 ] = { 0 };
uint8_t g_RF24L01RxBuffer[ 32 ] = { 0 }; 

static void drv_spi_init( void );
static void NRF24L01_check( void );
static void NRF24L01_Gpio_Init( void );
static void RF24L01_Init( void );
/*
*********************************************************************************************************
*	函 数 名: bsp_Nrf24l01Init
*	功能说明: 初始化NRF24L01，包括GPIO，寄存器设置等
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_Nrf24l01Init(void)
{
	drv_spi_init();			/* 初始化NRF24L01所用的SPI，仅支持硬件SPI目前 */
	NRF24L01_Gpio_Init();	/* 初始化CE和IRQ脚 */
	NRF24L01_check();		/* NRF24L01自检 */
	RF24L01_Init();			/* 初始化NRF24L01 */	
	/* 默认配置成接收模式 */
//	RF24L01_Set_Mode( MODE_RX );		
}

/**
  * @brief :SPI初始化(硬件)
  * @param :无
  * @note  :无
  * @retval:无
  */ 
static void drv_spi_init( void )
{
	GPIO_InitTypeDef	SpiGpioInitStructer;
	SPI_InitTypeDef		SpiInitStructer;
	
	/** SPI引脚配置 */
	RCC_APB2PeriphClockCmd( SPI_CLK_GPIO_CLK | SPI_MISO_GPIO_CLK | SPI_MOSI_GPIO_CLK | SPI_NSS_GPIO_CLK, ENABLE );	//打开端口时钟
	
	//SCK MOSI MISO 配置为复用
	SpiGpioInitStructer.GPIO_Speed = GPIO_Speed_10MHz;
	SpiGpioInitStructer.GPIO_Mode = GPIO_Mode_AF_PP;
	
	SpiGpioInitStructer.GPIO_Pin = SPI_CLK_GPIO_PIN;
	GPIO_Init( SPI_CLK_GPIO_PORT, &SpiGpioInitStructer );		//初始化SCK
	
	SpiGpioInitStructer.GPIO_Pin = SPI_MOSI_GPIO_PIN;
	GPIO_Init( SPI_MOSI_GPIO_PORT, &SpiGpioInitStructer );		//初始化MOSI
	
	SpiGpioInitStructer.GPIO_Pin = SPI_MISO_GPIO_PIN;
	GPIO_Init( SPI_MISO_GPIO_PORT, &SpiGpioInitStructer );		//初始化MISO
	
	//NSS配置为推挽输出
	SpiGpioInitStructer.GPIO_Mode = GPIO_Mode_Out_PP;
	SpiGpioInitStructer.GPIO_Pin = SPI_NSS_GPIO_PIN;
	GPIO_Init( SPI_NSS_GPIO_PORT, &SpiGpioInitStructer );		//初始化NSS
	GPIO_SetBits( SPI_NSS_GPIO_PORT, SPI_NSS_GPIO_PIN );		//置高

	/* SPI配置 */
	SPI_I2S_DeInit( SPI_PORT );			//复位SPI
	
	if( SPI1 == SPI_PORT )				
	{
		RCC_APB2PeriphClockCmd( SPI_PORT_CLK, ENABLE );			//SPI1在APB2上，打开相应SPI时钟
	}
	else
	{
		RCC_APB1PeriphClockCmd( SPI_PORT_CLK, ENABLE );			//SPI2 3在APB1上
	}
	
	SPI_Cmd( SPI_PORT, DISABLE );		//关闭SPI外设，配置前关闭
	
	SpiInitStructer.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	//双线全双工
	SpiInitStructer.SPI_Mode = SPI_Mode_Master;							//主机模式
	SpiInitStructer.SPI_CPOL = SPI_CPOL_Low;							//空闲状态为低电平 
	SpiInitStructer.SPI_CPHA = SPI_CPHA_1Edge;							//第一个边沿采集数据
	SpiInitStructer.SPI_DataSize = SPI_DataSize_8b;						//8位数据
	SpiInitStructer.SPI_NSS = SPI_NSS_Soft;								//从机软件管理
	SpiInitStructer.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;	//32分频
	SpiInitStructer.SPI_FirstBit = SPI_FirstBit_MSB;					//最高位先发送
	SpiInitStructer.SPI_CRCPolynomial = 7;								//CRC多项式,默认不使用SPI自带CRC	 
	
	SPI_Init( SPI_PORT, &SpiInitStructer );
	SPI_Cmd( SPI_PORT, ENABLE );
}

/**
  * @brief :SPI收发一个字节
  * @param :
  *			@TxByte: 发送的数据字节
  * @note  :非堵塞式，一旦等待超时，函数会自动退出
  * @retval:接收到的字节
  */
uint8_t drv_spi_read_write_byte( uint8_t TxByte )
{
	uint8_t l_Data = 0;
	uint16_t l_WaitTime = 0;
	
	while( RESET == SPI_I2S_GetFlagStatus( SPI_PORT, SPI_I2S_FLAG_TXE ) )		//等待发送缓冲区为空
	{
		if( SPI_WAIT_TIMEOUT == ++l_WaitTime )
		{
			break;			//如果等待超时则退出
		}
	}
	l_WaitTime = SPI_WAIT_TIMEOUT / 2;		//重新设置接收等待时间(因为SPI的速度很快，正常情况下在发送完成之后会立即收到数据，等待时间不需要过长)
	SPI_PORT->DR = TxByte;	//发送数据
	
	while( RESET == SPI_I2S_GetFlagStatus( SPI_PORT, SPI_I2S_FLAG_RXNE ) )		//等待接收缓冲区非空
	{
		if( SPI_WAIT_TIMEOUT == ++l_WaitTime )
		{
			break;			//如果等待超时则退出
		}
	}
	
	l_Data = (uint8_t)SPI_PORT->DR;		//读取接收数据
	
	return l_Data;		//返回
}

/**
  * @brief :SPI收发字符串
  * @param :
  *			@ReadBuffer: 接收数据缓冲区地址
  *			@WriteBuffer:发送字节缓冲区地址
  *			@Length:字节长度
  * @note  :非堵塞式，一旦等待超时，函数会自动退出
  * @retval:无
  */
void drv_spi_read_write_string( uint8_t* ReadBuffer, uint8_t* WriteBuffer, uint16_t Length )
{
	GPIO_ResetBits( SPI_NSS_GPIO_PORT, SPI_NSS_GPIO_PIN);			//拉低片选
	while( Length-- )
	{
		*ReadBuffer = drv_spi_read_write_byte( *WriteBuffer );		//收发数据
		ReadBuffer++;
		WriteBuffer++;				//读写地址加1
	}
	GPIO_SetBits( SPI_NSS_GPIO_PORT, SPI_NSS_GPIO_PIN);				//拉高片选
}

/** 硬件SPI */
#endif

/**
  * @brief :NRF24L01读寄存器
  * @param :
           @Addr:寄存器地址
  * @note  :地址在设备中有效
  * @retval:读取的数据
  */
uint8_t NRF24L01_Read_Reg( uint8_t RegAddr )
{
    uint8_t btmp;
	
    RF24L01_SET_CS_LOW( );			//片选
	
    drv_spi_read_write_byte( NRF_READ_REG | RegAddr );	//读命令 地址
    btmp = drv_spi_read_write_byte( 0xFF );				//读数据
	
    RF24L01_SET_CS_HIGH( );			//取消片选
	
    return btmp;
}

/**
  * @brief :NRF24L01读指定长度的数据
  * @param :
  *			@reg:地址
  *			@pBuf:数据存放地址
  *			@len:数据长度
  * @note  :数据长度不超过255，地址在设备中有效
  * @retval:读取状态
  */
void NRF24L01_Read_Buf( uint8_t RegAddr, uint8_t *pBuf, uint8_t len )
{
    uint8_t btmp;
	
    RF24L01_SET_CS_LOW( );			//片选
	
    drv_spi_read_write_byte( NRF_READ_REG | RegAddr );	//读命令 地址
    for( btmp = 0; btmp < len; btmp ++ )
    {
        *( pBuf + btmp ) = drv_spi_read_write_byte( 0xFF );	//读数据
    }
    RF24L01_SET_CS_HIGH( );		//取消片选
}

/**
  * @brief :NRF24L01写寄存器
  * @param :无
  * @note  :地址在设备中有效
  * @retval:读写状态
  */
void NRF24L01_Write_Reg( uint8_t RegAddr, uint8_t Value )
{
    RF24L01_SET_CS_LOW( );		//片选
	
    drv_spi_read_write_byte( NRF_WRITE_REG | RegAddr );	//写命令 地址
    drv_spi_read_write_byte( Value );			//写数据
	
    RF24L01_SET_CS_HIGH( );		//取消片选
}

/**
  * @brief :NRF24L01写指定长度的数据
  * @param :
  *			@reg:地址
  *			@pBuf:写入的数据地址
  *			@len:数据长度
  * @note  :数据长度不超过255，地址在设备中有效
  * @retval:写状态
  */
void NRF24L01_Write_Buf( uint8_t RegAddr, uint8_t *pBuf, uint8_t len )
{
    uint8_t i;
	
    RF24L01_SET_CS_LOW( );		//片选
	
    drv_spi_read_write_byte( NRF_WRITE_REG | RegAddr );	//写命令 地址
    for( i = 0; i < len; i ++ )
    {
        drv_spi_read_write_byte( *( pBuf + i ) );		//写数据
    }
	
    RF24L01_SET_CS_HIGH( );		//取消片选
}

/**
  * @brief :清空TX缓冲区
  * @param :无
  * @note  :无
  * @retval:无
  */
void NRF24L01_Flush_Tx_Fifo ( void )
{
    RF24L01_SET_CS_LOW( );		//片选
	
    drv_spi_read_write_byte( FLUSH_TX );	//清TX FIFO命令
	
    RF24L01_SET_CS_HIGH( );		//取消片选
}

/**
  * @brief :清空RX缓冲区
  * @param :无
  * @note  :无
  * @retval:无
  */
void NRF24L01_Flush_Rx_Fifo( void )
{
    RF24L01_SET_CS_LOW( );		//片选
	
    drv_spi_read_write_byte( FLUSH_RX );	//清RX FIFO命令
	
    RF24L01_SET_CS_HIGH( );		//取消片选
}

/**
  * @brief :重新使用上一包数据
  * @param :无
  * @note  :无
  * @retval:无
  */
void NRF24L01_Reuse_Tx_Payload( void )
{
    RF24L01_SET_CS_LOW( );		//片选
	
    drv_spi_read_write_byte( REUSE_TX_PL );		//重新使用上一包命令
	
    RF24L01_SET_CS_HIGH( );		//取消片选
}

/**
  * @brief :NRF24L01空操作
  * @param :无
  * @note  :无
  * @retval:无
  */
void NRF24L01_Nop( void )
{
    RF24L01_SET_CS_LOW( );		//片选
	
    drv_spi_read_write_byte( NOP );		//空操作命令
	
    RF24L01_SET_CS_HIGH( );		//取消片选
}

/**
  * @brief :NRF24L01读状态寄存器
  * @param :无
  * @note  :无
  * @retval:RF24L01状态
  */
uint8_t NRF24L01_Read_Status_Register( void )
{
    uint8_t Status;
	
    RF24L01_SET_CS_LOW( );		//片选
	
    Status = drv_spi_read_write_byte( NRF_READ_REG + STATUS );	//读状态寄存器
	
    RF24L01_SET_CS_HIGH( );		//取消片选
	
    return Status;
}

/**
  * @brief :NRF24L01清中断
  * @param :
           @IRQ_Source:中断源
  * @note  :无
  * @retval:清除后状态寄存器的值
  */
uint8_t NRF24L01_Clear_IRQ_Flag( uint8_t IRQ_Source )
{
    uint8_t btmp = 0;

    IRQ_Source &= ( 1 << RX_DR ) | ( 1 << TX_DS ) | ( 1 << MAX_RT );	//中断标志处理
    btmp = NRF24L01_Read_Status_Register( );			//读状态寄存器
			
    RF24L01_SET_CS_LOW( );			//片选
    drv_spi_read_write_byte( NRF_WRITE_REG + STATUS );	//写状态寄存器命令
    drv_spi_read_write_byte( IRQ_Source | btmp );		//清相应中断标志
    RF24L01_SET_CS_HIGH( );			//取消片选
	
    return ( NRF24L01_Read_Status_Register( ));			//返回状态寄存器状态
}

/**
  * @brief :读RF24L01中断状态
  * @param :无
  * @note  :无
  * @retval:中断状态
  */
uint8_t RF24L01_Read_IRQ_Status( void )
{
    return ( NRF24L01_Read_Status_Register( ) & (( 1 << RX_DR ) | ( 1 << TX_DS ) | ( 1 << MAX_RT )));	//返回中断状态
}
 
 /**
  * @brief :读FIFO中数据宽度
  * @param :无
  * @note  :无
  * @retval:数据宽度
  */
uint8_t NRF24L01_Read_Top_Fifo_Width( void )
{
    uint8_t btmp;
	
    RF24L01_SET_CS_LOW( );		//片选
	
    drv_spi_read_write_byte( R_RX_PL_WID );	//读FIFO中数据宽度命令
    btmp = drv_spi_read_write_byte( 0xFF );	//读数据
	
    RF24L01_SET_CS_HIGH( );		//取消片选
	
    return btmp;
}

 /**
  * @brief :读接收到的数据
  * @param :无
  * @note  :无
  * @retval:
           @pRxBuf:数据存放地址首地址
  */
uint8_t NRF24L01_Read_Rx_Payload( uint8_t *pRxBuf )
{
    uint8_t Width, PipeNum;
	
    PipeNum = ( NRF24L01_Read_Reg( STATUS ) >> 1 ) & 0x07;	//读接收状态
    Width = NRF24L01_Read_Top_Fifo_Width( );		//读接收数据个数

    RF24L01_SET_CS_LOW( );		//片选
    drv_spi_read_write_byte( RD_RX_PLOAD );			//读有效数据命令
	
    for( PipeNum = 0; PipeNum < Width; PipeNum ++ )
    {
        *( pRxBuf + PipeNum ) = drv_spi_read_write_byte( 0xFF );		//读数据
    }
    RF24L01_SET_CS_HIGH( );		//取消片选
    NRF24L01_Flush_Rx_Fifo( );	//清空RX FIFO
	
    return Width;
}

 /**
  * @brief :发送数据（带应答）
  * @param :
  *			@pTxBuf:发送数据地址
  *			@len:长度
  * @note  :一次不超过32个字节
  * @retval:无
  */
void NRF24L01_Write_Tx_Payload_Ack( uint8_t *pTxBuf, uint8_t len )
{
    uint8_t btmp;
    uint8_t length = ( len > 32 ) ? 32 : len;		//数据长达大约32 则只发送32个

    NRF24L01_Flush_Tx_Fifo( );		//清TX FIFO
	
    RF24L01_SET_CS_LOW( );			//片选
    drv_spi_read_write_byte( WR_TX_PLOAD );	//发送命令
	
    for( btmp = 0; btmp < length; btmp ++ )
    {
        drv_spi_read_write_byte( *( pTxBuf + btmp ) );	//发送数据
    }
    RF24L01_SET_CS_HIGH( );			//取消片选
}

 /**
  * @brief :发送数据（不带应答）
  * @param :
  *			@pTxBuf:发送数据地址
  *			@len:长度
  * @note  :一次不超过32个字节
  * @retval:无
  */
void NRF24L01_Write_Tx_Payload_NoAck( uint8_t *pTxBuf, uint8_t len )
{
    if( len > 32 || len == 0 )
    {
        return ;		//数据长度大于32 或者等于0 不执行
    }
	
    RF24L01_SET_CS_LOW( );	//片选
    drv_spi_read_write_byte( WR_TX_PLOAD_NACK );	//发送命令
    while( len-- )
    {
        drv_spi_read_write_byte( *pTxBuf );			//发送数据
		pTxBuf++;
    }
    RF24L01_SET_CS_HIGH( );		//取消片选
}

 /**
  * @brief :在接收模式下向TX FIFO写数据(带ACK)
  * @param :
  *			@pData:数据地址
  *			@len:长度
  * @note  :一次不超过32个字节
  * @retval:无
  */
void NRF24L01_Write_Tx_Payload_InAck( uint8_t *pData, uint8_t len )
{
    uint8_t btmp;
	
	len = ( len > 32 ) ? 32 : len;		//数据长度大于32个则只写32个字节

    RF24L01_SET_CS_LOW( );			//片选
    drv_spi_read_write_byte( W_ACK_PLOAD );		//命令
    for( btmp = 0; btmp < len; btmp ++ )
    {
        drv_spi_read_write_byte( *( pData + btmp ) );	//写数据
    }
    RF24L01_SET_CS_HIGH( );			//取消片选
}

 /**
  * @brief :设置发送地址
  * @param :
  *			@pAddr:地址存放地址
  *			@len:长度
  * @note  :无
  * @retval:无
  */
void NRF24L01_Set_TxAddr( uint8_t *pAddr, uint8_t len )
{
	len = ( len > 5 ) ? 5 : len;					//地址不能大于5个字节
    NRF24L01_Write_Buf( TX_ADDR, pAddr, len );	//写地址
}

 /**
  * @brief :设置接收通道地址
  * @param :
  *			@PipeNum:通道
  *			@pAddr:地址存肥着地址
  *			@Len:长度
  * @note  :通道不大于5 地址长度不大于5个字节
  * @retval:无
  */
void NRF24L01_Set_RxAddr( uint8_t PipeNum, uint8_t *pAddr, uint8_t Len )
{
    Len = ( Len > 5 ) ? 5 : Len;
    PipeNum = ( PipeNum > 5 ) ? 5 : PipeNum;		//通道不大于5 地址长度不大于5个字节

    NRF24L01_Write_Buf( RX_ADDR_P0 + PipeNum, pAddr, Len );	//写入地址
}

 /**
  * @brief :设置通信速度
  * @param :
  *			@Speed:速度
  * @note  :无
  * @retval:无
  */
void NRF24L01_Set_Speed( nRf24l01SpeedType Speed )
{
	uint8_t btmp = 0;
	
	btmp = NRF24L01_Read_Reg( RF_SETUP );
	btmp &= ~( ( 1<<5 ) | ( 1<<3 ) );
	
	if( Speed == SPEED_250K )		//250K
	{
		btmp |= ( 1<<5 );
	}
	else if( Speed == SPEED_1M )   //1M
	{
   		btmp &= ~( ( 1<<5 ) | ( 1<<3 ) );
	}
	else if( Speed == SPEED_2M )   //2M
	{
		btmp |= ( 1<<3 );
	}

	NRF24L01_Write_Reg( RF_SETUP, btmp );
}

 /**
  * @brief :设置功率
  * @param :
  *			@Speed:速度
  * @note  :无
  * @retval:无
  */
void NRF24L01_Set_Power( nRf24l01PowerType Power )
{
    uint8_t btmp;
	
	btmp = NRF24L01_Read_Reg( RF_SETUP ) & ~0x07;
    switch( Power )
    {
        case POWER_F18DBM:
            btmp |= PWR_18DB;
            break;
        case POWER_F12DBM:
            btmp |= PWR_12DB;
            break;
        case POWER_F6DBM:
            btmp |= PWR_6DB;
            break;
        case POWER_0DBM:
            btmp |= PWR_0DB;
            break;
        default:
            break;
    }
    NRF24L01_Write_Reg( RF_SETUP, btmp );
}

 /**
  * @brief :设置频率
  * @param :
  *			@FreqPoint:频率设置参数
  * @note  :值不大于127
  * @retval:无
  */
void RF24LL01_Write_Hopping_Point( uint8_t FreqPoint )
{
    NRF24L01_Write_Reg(  RF_CH, FreqPoint & 0x7F );
}

/**
  * @brief :NRF24L01检测
  * @param :无
  * @note  :无
  * @retval:无
  */ 
static void NRF24L01_check( void )
{
	uint8_t i;
	uint8_t buf[5]={ 0XA5, 0XA5, 0XA5, 0XA5, 0XA5 };
	uint8_t read_buf[ 5 ] = { 0 };
	 
	while( 1 )
	{
		NRF24L01_Write_Buf( TX_ADDR, buf, 5 );			//写入5个字节的地址
		NRF24L01_Read_Buf( TX_ADDR, read_buf, 5 );		//读出写入的地址  
		for( i = 0; i < 5; i++ )
		{
			if( buf[ i ] != read_buf[ i ] )
			{
				break;
			}	
		} 
		
		if( 5 == i )
		{
			break;
		}
		else
		{
			comSendBuf(COM1,(uint8_t *)g_ErrorString,26);
		}
		bsp_DelayMS(1000);
	}
}

 /**
  * @brief :设置模式
  * @param :
  *			@Mode:模式发送模式或接收模式
  * @note  :无
  * @retval:无
  */
void RF24L01_Set_Mode( nRf24l01ModeType Mode )
{
    uint8_t controlreg = 0;
	controlreg = NRF24L01_Read_Reg( CONFIG );
	
    if( Mode == MODE_TX )       
	{
		controlreg &= ~( 1<< PRIM_RX );
	}
    else 
	{
		if( Mode == MODE_RX )  
		{ 
			controlreg |= ( 1<< PRIM_RX ); 
		}
	}

    NRF24L01_Write_Reg( CONFIG, controlreg );
}

/**
  * @brief :NRF24L01发送一次数据
  * @param :
  *			@txbuf:待发送数据首地址
  *			@Length:发送数据长度
  * @note  :无
  * @retval:
  *			MAX_TX：达到最大重发次数
  *			TX_OK：发送完成
  *			0xFF:其他原因
  */ 
uint8_t NRF24L01_TxPacket( uint8_t *txbuf, uint8_t Length )
{
	uint8_t l_Status = 0;
	uint16_t l_MsTimes = 0;
	
	RF24L01_SET_CS_LOW( );		//片选
	drv_spi_read_write_byte( FLUSH_TX );
	RF24L01_SET_CS_HIGH( );
	
	RF24L01_SET_CE_LOW( );		
	NRF24L01_Write_Buf( WR_TX_PLOAD, txbuf, Length );	//写数据到TX BUF 32字节  TX_PLOAD_WIDTH
	RF24L01_SET_CE_HIGH( );			//启动发送
	while( 0 != RF24L01_GET_IRQ_STATUS( ))
	{
		bsp_DelayMS(1);
		if( 500 == l_MsTimes++ )						//500ms还没有发送成功，重新初始化设备
		{
			NRF24L01_Gpio_Init( );
			RF24L01_Init( );
			RF24L01_Set_Mode( MODE_TX );
			break;
		}
	}
	l_Status = NRF24L01_Read_Reg(STATUS);						//读状态寄存器
	NRF24L01_Write_Reg( STATUS, l_Status );						//清除TX_DS或MAX_RT中断标志
	
	if( l_Status & MAX_TX )	//达到最大重发次数
	{
		NRF24L01_Write_Reg( FLUSH_TX,0xff );	//清除TX FIFO寄存器
		return MAX_TX; 
	}
	if( l_Status & TX_OK )	//发送完成
	{
		return TX_OK;
	}
	
	return 0xFF;	//其他原因发送失败
}

/**
  * @brief :NRF24L01接收数据
  * @param :
  *			@rxbuf:接收数据存放地址
  * @note  :无
  * @retval:接收的数据个数
  */ 
uint8_t NRF24L01_RxPacket( uint8_t *rxbuf )
{
	uint8_t l_Status = 0, l_RxLength = 0, l_100MsTimes = 0;
	
	RF24L01_SET_CS_LOW( );		//片选
	drv_spi_read_write_byte( FLUSH_RX );
	RF24L01_SET_CS_HIGH( );
	
	while( 0 != RF24L01_GET_IRQ_STATUS( ))
	{
		bsp_DelayMS(100);
		
		if( 30 == l_100MsTimes++ )		//3s没接收过数据，重新初始化模块
		{
			NRF24L01_Gpio_Init( );
			RF24L01_Init( );
			RF24L01_Set_Mode( MODE_RX );
			break;
		}
	}	
	l_Status = NRF24L01_Read_Reg( STATUS );		//读状态寄存器
	NRF24L01_Write_Reg( STATUS,l_Status );		//清中断标志
	if( l_Status & RX_OK)	//接收到数据
	{
		l_RxLength = NRF24L01_Read_Reg( R_RX_PL_WID );		//读取接收到的数据个数
		NRF24L01_Read_Buf( RD_RX_PLOAD,rxbuf,l_RxLength );	//接收到数据 
		NRF24L01_Write_Reg( FLUSH_RX,0xff );				//清除RX FIFO
		return l_RxLength; 
	}		
	return 0;				//没有收到数据	
}

 /**
  * @brief :RF24L01引脚初始化
  * @param :无
  * @note  :无
  * @retval:无
  */
static void NRF24L01_Gpio_Init( void )
{
	GPIO_InitTypeDef	RF24L01_GpioInitStructer;
	
	RCC_APB2PeriphClockCmd( RF24L01_CE_GPIO_CLK | RF24L01_IRQ_GPIO_CLK, ENABLE );	//?? CE IRQ????  CS SCK MISO MOSI?SPI?????
	
	//设置CE
	RF24L01_GpioInitStructer.GPIO_Mode = GPIO_Mode_Out_PP;
	RF24L01_GpioInitStructer.GPIO_Speed = GPIO_Speed_50MHz;
	RF24L01_GpioInitStructer.GPIO_Pin = RF24L01_CE_GPIO_PIN;
	GPIO_Init( RF24L01_CE_GPIO_PORT, &RF24L01_GpioInitStructer );
	//CE拉高
	GPIO_SetBits( RF24L01_CE_GPIO_PORT, RF24L01_CE_GPIO_PIN);
	
	//配置IRQ
	RF24L01_GpioInitStructer.GPIO_Mode = GPIO_Mode_IPU;
	RF24L01_GpioInitStructer.GPIO_Speed = GPIO_Speed_50MHz;
	RF24L01_GpioInitStructer.GPIO_Pin = RF24L01_IRQ_GPIO_PIN;
	GPIO_Init( RF24L01_IRQ_GPIO_PORT, &RF24L01_GpioInitStructer );
	//IRQ拉高
	GPIO_SetBits( RF24L01_IRQ_GPIO_PORT, RF24L01_IRQ_GPIO_PIN);
	
	RF24L01_SET_CE_LOW( );		//??24L01
	RF24L01_SET_CS_HIGH( );		//??SPI??
}

 /**
  * @brief :RF24L01模块初始化
  * @param :无
  * @note  :无
  * @retval:无
  */
static void RF24L01_Init( void )
{
    uint8_t addr[5] = {INIT_ADDR};

    RF24L01_SET_CE_HIGH( );
    NRF24L01_Clear_IRQ_Flag( IRQ_ALL );
	
#if DYNAMIC_PACKET == 1
    NRF24L01_Write_Reg( DYNPD, ( 1 << 0 ) ); 	//使能通道1动态数据长度
    NRF24L01_Write_Reg( FEATRUE, 0x07 );
    NRF24L01_Read_Reg( DYNPD );
    NRF24L01_Read_Reg( FEATRUE );
	
#elif DYNAMIC_PACKET == 0
    
    L01_WriteSingleReg( L01REG_RX_PW_P0, FIXED_PACKET_LEN );	//固定数据长度
	
#endif	//DYNAMIC_PACKET

    NRF24L01_Write_Reg( CONFIG, /*( 1<<MASK_RX_DR ) |*/		//接收中断
                                      ( 1 << EN_CRC ) |     //使能CRC 1个字节
                                      ( 1 << PWR_UP ) );    //开启设备
    NRF24L01_Write_Reg( EN_AA, ( 1 << ENAA_P0 ) );   		//通道0自动应答
    NRF24L01_Write_Reg( EN_RXADDR, ( 1 << ERX_P0 ) );		//通道0接收
    NRF24L01_Write_Reg( SETUP_AW, AW_5BYTES );     			//地址宽度 5个字节
    NRF24L01_Write_Reg( SETUP_RETR, ARD_4000US |
                        ( REPEAT_CNT & 0x0F ) );         	//重复等待时间 250us
    NRF24L01_Write_Reg( RF_CH, 60 );             			//初始化通道
    NRF24L01_Write_Reg( RF_SETUP, 0x26 );

    NRF24L01_Set_TxAddr( &addr[0], 5 );                      //设置TX地址
    NRF24L01_Set_RxAddr( 0, &addr[0], 5 );                   //设置RX地址
}

/*
*********************************************************************************************************
*	函 数 名: Nrf24l01_RxTxTest
*	功能说明: NRF24L01发送接收测试
*	形    参：_mode：测试模式
*	返 回 值: 无
*********************************************************************************************************
*/
void Nrf24l01_RxTxTest(nRf24l01ModeType _mode)
{
	uint8_t ucData;
	
	RF24L01_Set_Mode( _mode );		/* 设置模式 */
	while(1)
	{
/****************************** 发送模式 ******************************************/	
		if(_mode == MODE_TX)
		{
//			printf("当前为发送测试\r\n");
			if(comGetChar(COM1,&ucData))
			{
				NRF24L01_TxPacket(&ucData, 1 );
				bsp_LedToggle(3);
			}
		}	
			
/****************************** 接收模式 ******************************************/
		else if(_mode == MODE_RX)
		{
//			printf("当前为接收测试\r\n");
			uint8_t ucLen = NRF24L01_RxPacket( g_RF24L01RxBuffer );		//接收字节
			if( ucLen != 0)
			{	
				comSendBuf(COM1,g_RF24L01RxBuffer,ucLen);		//输出接收到的字节
//				uint16_t ucData = (((uint16_t)g_RF24L01RxBuffer[0]<<8) & 0xff00 | ((uint16_t)g_RF24L01RxBuffer[1])&0x00ff);
//				printf("ucData = %d\r\n",ucData);
				bsp_LedToggle(2);
				bsp_DelayMS(200);
				bsp_LedToggle(2);
				bsp_DelayMS(200);
			}
		}
		
		switch(bsp_GetKey())
		{
			case KEY_DOWN_K1: 	
					goto Exti_while;	/* 退出测试 */ 
			
			case KEY_DOWN_K2:
					break;
			
			default:
					break;
		}
	}
Exti_while:
	printf("退出测试\r\n");
}
