/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"			/* FatFs lower layer API */
#include "bsp.h"			/* �ײ������� �ṩSD, USB, NAND ���� */

//#define ff_printf	printf
#define ff_printf(...)

#define SD_SECTOR_SIZE		512	/* SD��������С����Ϊ512 */

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
			/* ��ȡоƬ��Ϣ */
			sf_ReadInfo();		
			/* ʶ��ID�Ƿ���ȷ */
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
		case FS_SD :		/* SD�� */
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
			bsp_InitSFlash();		/* ��ʼ��FLASH,������ֻ��ʼ��CS����ӦSPI���ߣ���bsp��ʼ��ʱ�Ѿ���ʼ������ */
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
			/* SDIO������DMAģʽ����Ҫ������DMA�����Ƿ���� */
			Status = SD_WaitReadOperation();
			if (Status != SD_OK)
			{
				return RES_ERROR;
			}
			
			//while(SD_GetStatus() != SD_TRANSFER_OK); ������忨����˴�������
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
					//  SD_TRANSFER_BUSY ������ȴ� 
				}
			}
#endif
			break;
			
		case FS_SPIFLASH :
			// translate the arguments here
			/* 
				����SPI_FLASHһ��������СΪ4096�ֽڣ�
				sector�������ţ�����1������ʱ��ʵ��ʱ�ӵ�ַ1*4096��ʼ��ȡ
				countʱ����������������ÿ������4096�ֽڣ���1������ʱ��ʵ�ʶ��ֽ���Ϊ1*4096
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
			/* SDIO������DMAģʽ����Ҫ������DMA�����Ƿ���� */
			Status = SD_WaitWriteOperation();
			if (Status != SD_OK)
			{
				return RES_ERROR;
			}
			
			//while(SD_GetStatus() != SD_TRANSFER_OK); ������忨����˴�������
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
					//  SD_TRANSFER_BUSY ������ȴ� 
				}
			}
#endif
			break;
			
		case FS_SPIFLASH :
			// translate the arguments here
			/* 
				����SPI_FLASHһ��������СΪ4096�ֽڣ�
				sector�������ţ�д���1������ʱ��ʵ��ʱ�ӵ�ַ1*4096��ʼд��
				count��д��������������ÿ������4096�ֽڣ�д��1������ʱ��ʵ��д������Ϊ1*4096
			*/		
			p = (BYTE *)buff;
			for(j = 0; j < count; j++)
			{
				/* ÿ��д��һ������ */
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
				/* SD������������ SDCardInfo.CardCapacity */
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
					/* SPI Flash����Ҫͬ�� */
					case CTRL_SYNC :  
						break;
					
					/* ��������: 4096*4096/1024/1024=16(MB) */
					case GET_SECTOR_COUNT:
						*(DWORD * )buff = 4096;
						break;
					
					/* ������С */
					case GET_SECTOR_SIZE :
						*(WORD * )buff = 4096;
						break;
					
					/* ͬʱ������������ */
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
*	�� �� ��: get_fattime
*	����˵��: ���ϵͳʱ�䣬���ڸ�д�ļ��Ĵ������޸�ʱ�䡣�ͻ�����������ֲ��ϵͳ��RTC��������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
DWORD get_fattime (void)
{
	/* �����ȫ��ʱ�ӣ��ɰ�����ĸ�ʽ����ʱ��ת��. ���������2014-07-02 00:00:00 */
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


