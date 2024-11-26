/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"			/* FatFs lower layer API */
#include "bsp.h"			/* 底层驱动， 提供SD, USB, NAND 驱动 */

//#define ff_printf	printf
#define ff_printf(...)

#define SD_SECTOR_SIZE		512	/* SD卡扇区大小必须为512 */

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status 
(
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = STA_NOINIT;

	switch (pdrv)
	{
		case FS_SD :
			stat = 0;
			break;
		
		case FS_SPIFLASH :	
			/* 获取芯片信息 */
			sf_ReadInfo();		
			/* 识别ID是否正确 */
			if(g_tSF.ChipID == N25Q128A_ID)
			{
				stat &= ~STA_NOINIT;
			}
			else 
			{
				stat = STA_NOINIT;
			}
			// translate the reslut code here
			break;


		default:
			break;
	}
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat = STA_NOINIT;

	switch (pdrv)
	{
		case FS_SD :		/* SD卡 */
			if (SD_Init() == SD_OK)
			{
				stat = RES_OK;
			}
			else
			{
				stat = STA_NODISK;
			}
			break;
			
		case FS_SPIFLASH :
			bsp_InitSFlash();		/* 初始化FLASH,引脚上只初始化CS，对应SPI口线，在bsp初始化时已经初始化过了 */
			// translate the reslut code here
			stat = disk_status(pdrv);
			break;

		default :
			break;
	}
	return stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res = RES_PARERR;
	SD_Error Status;

	switch (pdrv)
	{
		case FS_SD :
			if (count == 1)
			{
				Status = SD_ReadBlock(buff, sector << 9 , SD_SECTOR_SIZE);
			}
			else
			{
				Status = SD_ReadMultiBlocks(buff, sector << 9 , SD_SECTOR_SIZE, count+1);
			}
#ifdef  SD_DMA_MODE			
			/* SDIO工作在DMA模式，需要检查操作DMA传输是否完成 */
			Status = SD_WaitReadOperation();
			if (Status != SD_OK)
			{
				return RES_ERROR;
			}
			
			//while(SD_GetStatus() != SD_TRANSFER_OK); 如果不插卡，则此处会死机
			{
				uint32_t i;
				SDTransferState status;
				
				for (i = 0; i < 100000; i++)
				{									
					status = SD_GetStatus();
					if (status == SD_TRANSFER_ERROR)
					{
						res = RES_ERROR;
						break;
					}
					else if (status == SD_TRANSFER_OK)
					{
						res = RES_OK;
						break;
					}
					//  SD_TRANSFER_BUSY 则继续等待 
				}
			}
#endif
			break;
			
		case FS_SPIFLASH :
			// translate the arguments here
			/* 
				由于SPI_FLASH一个扇区大小为4096字节，
				sector是扇区号，读第1个扇区时，实际时从地址1*4096开始读取
				count时读出扇区的数量，每个扇区4096字节，读1个扇区时，实际读字节数为1*4096
			*/
			sf_ReadBuffer(buff, sector << 12, count<<12);	
			// translate the reslut code here
			res = RES_OK;
			break;

		default:
			res = RES_PARERR;
			break;
	}
	return res;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write 
(
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res = RES_OK;
	SD_Error Status;
	uint32_t j;
	BYTE *p;

	switch (pdrv)
	{
		case FS_SD :
			if (count == 1)
			{
				Status = SD_WriteBlock((uint8_t *)buff, sector << 9 , SD_SECTOR_SIZE);
			}
			else
			{
				Status = SD_WriteMultiBlocks((uint8_t *)buff, sector << 9 ,SD_SECTOR_SIZE, count+1);				
			}
			
			if (Status != SD_OK)
			{
				return RES_ERROR;
			}
#ifdef  SD_DMA_MODE
			/* SDIO工作在DMA模式，需要检查操作DMA传输是否完成 */
			Status = SD_WaitWriteOperation();
			if (Status != SD_OK)
			{
				return RES_ERROR;
			}
			
			//while(SD_GetStatus() != SD_TRANSFER_OK); 如果不插卡，则此处会死机
			{
				uint32_t i;
				SDTransferState status;
				
				for (i = 0; i < 100000; i++)
				{									
					status = SD_GetStatus();
					if (status == SD_TRANSFER_ERROR)
					{
						res = RES_ERROR;
						break;
					}
					else if (status == SD_TRANSFER_OK)
					{
						res = RES_OK;
						break;
					}
					//  SD_TRANSFER_BUSY 则继续等待 
				}
			}
#endif
			break;
			
		case FS_SPIFLASH :
			// translate the arguments here
			/* 
				由于SPI_FLASH一个扇区大小为4096字节，
				sector是扇区号，写入第1个扇区时，实际时从地址1*4096开始写入
				count是写入扇区的数量，每个扇区4096字节，写入1个扇区时，实际写入数量为1*4096
			*/		
			p = (BYTE *)buff;
			for(j = 0; j < count; j++)
			{
				/* 每次写入一个扇区 */
				sf_WriteBuffer((uint8_t *)buff, sector << 12, 4096);
				p += 4096;					
			}

			// translate the reslut code here
			res = RES_OK;
			return res;	
		

		default:
			res = RES_PARERR;
			break;
	}
	return res;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res = RES_PARERR;

	switch (pdrv) 
	{
		case FS_SD :
			res = RES_ERROR;
			switch (cmd)
			{
				/* SD卡磁盘容量： SDCardInfo.CardCapacity */
				case CTRL_SYNC :		/* Wait for end of internal write process of the drive */
					res = RES_OK;
					break;

				// Get R/W sector size (WORD) 
				case GET_SECTOR_SIZE :    
					*(WORD * )buff = SD_SECTOR_SIZE;
					res = RES_OK;
					break;
				
				case GET_SECTOR_COUNT :	/* Get drive capacity in unit of sector (DWORD) */
					*(DWORD*)buff = SDCardInfo.CardCapacity / SDCardInfo.CardBlockSize;
					res = RES_OK;
					break;

				case GET_BLOCK_SIZE :	/* Get erase block size in unit of sector (DWORD) */
					*(WORD*)buff = 512;
					res = RES_OK;
					break;

				case CTRL_ERASE_SECTOR: /* Erase a block of sectors (used when _USE_ERASE == 1) */
					break;
				
				default:
					res = RES_PARERR;
					break;
			}
			break;

		case FS_SPIFLASH :
				// pre-process here
				switch (cmd) 
				{
					/* SPI Flash不需要同步 */
					case CTRL_SYNC :  
						break;
					
					/* 扇区数量: 4096*4096/1024/1024=16(MB) */
					case GET_SECTOR_COUNT:
						*(DWORD * )buff = 4096;
						break;
					
					/* 扇区大小 */
					case GET_SECTOR_SIZE :
						*(WORD * )buff = 4096;
						break;
					
					/* 同时擦除扇区个数 */
					case GET_BLOCK_SIZE :
						*(DWORD * )buff = 8;
						break;
				}
				// post-process here
				res = RES_OK;
				break;

		default:
			res = RES_PARERR;
			break;
	}
	return res;
}
#endif

/*
*********************************************************************************************************
*	函 数 名: get_fattime
*	功能说明: 获得系统时间，用于改写文件的创建和修改时间。客户可以自行移植和系统的RTC关联起来
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
DWORD get_fattime (void)
{
	/* 如果有全局时钟，可按下面的格式进行时钟转换. 这个例子是2014-07-02 00:00:00 */
#if 0
	RTC_ReadClock();
	return  ((DWORD)(g_tRTC.Year - 1980) << 25)		/* Year  */
			| ((DWORD)g_tRTC.Mon << 21)				/* Month   */
			| ((DWORD)g_tRTC.Day << 16)				/* Day_m  1*/
			| ((DWORD)g_tRTC.Hour << 11)			/* Hour  */
			| ((DWORD)g_tRTC.Min << 5)				/* Min  */
			| ((DWORD)g_tRTC.Sec >> 1);				/* Sec  */
	
#else
	return	  ((DWORD)(2014 - 1980) << 25)	/* Year = 2014 */
			| ((DWORD)7 << 21)				/* Month = 7 */
			| ((DWORD)2 << 16)				/* Day_m = 2*/
			| ((DWORD)0 << 11)				/* Hour = 0 */
			| ((DWORD)0 << 5)				/* Min = 0 */
			| ((DWORD)0 >> 1);				/* Sec = 0 */
#endif	
}


