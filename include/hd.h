#ifndef	_TTAO_HD_H_
#define _TTAO_HD_H_
//LBA(逻辑扇区号)=磁头数 × 每磁道扇区数 × 当前所在柱面号 + 每磁道扇区数 × 当前所在磁头号 + 当前所在扇区号 – 1
#define REG_STATUS	0x1F7 /* 0号硬盘状态寄存器（读时）、0号硬盘命令寄存器（写时） */

#define REG_DATA 			0x1F0
#define REG_FEATURES 		0x1F1
#define REG_ERROR			REG_FEATURES
#define REG_SECTOR_COUNT	0x1F2
#define REG_LBA_LOW			0x1F3
#define REG_LBA_MID			0x1F4
#define REG_LBA_HIGH		0x1F5
#define	REG_DEVICE			0x1F6


#define REG_CMD				REG_STATUS

#define STATUS_BSY			0x80
#define	STATUS_READY		0x08
#define STATUS_WAIT			0x88

#define	STATUS_DRDY			0x40
/*设备块控制寄存器*/
#define REG_DEV_CTRL		0x3F6

struct hd_cmd{
	u8 features;
	u8 count;
	u8 lba_low;
	u8 lba_mid;
	u8 lba_high;
	u8 device;
	u8 command;
}HDCMD;

PUBLIC void hd_identify(int drive);

#define HD_TIMEOUT		10000
#define ATA_IDENTIFY 	0xEC /* 发送0xEC的ATA命令可以得到硬盘的详细参数 */
#define MAKE_DEVICE_REG(lba, drv, lba_highest)(((lba) << 6) 		| \
												((drv) << 4)		| \
												(lba_highest & 0xF) | \
												 0xA0) 

/* HARD Drive*/
#define SECTOR_SIZE 	512
#define SECTOR_BITS		(512 * 8)
#endif