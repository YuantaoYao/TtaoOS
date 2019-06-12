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


#define MAX_DRIVES			2 //最大设备数
#define NR_PART_PER_DRIVE	4 //每块设备最多分区数
#define NR_SUB_PER_PART		16 //每个分区最多逻辑分区数
#define NR_SUB_PER_DRIVE	(NR_SUB_PER_PART * NR_PART_PER_DRIVE)
#define NR_PRIM_PER_DRIVE	(NR_PART_PER_DRIVE + 1) //每块设备最多引导分区数

#define MAX_PRIM		(NR_PRIM_PER_DRIVE * MAX_DRIVES -1)

#define MINOR_hd1a			0x10							//第一块设备的第一个分区的第一个逻辑分区
#define MINOR_hd2a			(MINOR_hd1a + NR_SUB_PER_PART)  //第一块设备的第二个分区的第一个逻辑分区

#define REG_CMD				REG_STATUS

#define STATUS_BSY			0x80
#define	STATUS_READY		0x08
#define STATUS_WAIT			0x88

#define	STATUS_DRDY			0x40
/*设备块控制寄存器*/
#define REG_DEV_CTRL		0x3F6

#define	P_PRIMARY	0 		//主分区
#define	P_EXTENDED	1		//扩展分区

#define NO_PART		0x00
#define EXT_PART	0x05

#define NR_FILES		64
#define NR_FILE_DESC	64
#define NR_INODE		64
#define NR_SUPER_BLOCK	8

struct hd_cmd{
	u8 features;
	u8 count;
	u8 lba_low;
	u8 lba_mid;
	u8 lba_high;
	u8 device;
	u8 command;
}HDCMD;

struct part_info{
	u32 base; /* 开始扇区 */
	u32 size; /* how many sector in this partition*/
};

/* main drive struct, one entry per drive */
struct hd_info{
	int 	open_cnt;
	struct part_info	primary[NR_PRIM_PER_DRIVE];//引导分区的扇区
	struct part_info	logical[NR_SUB_PER_DRIVE]; //逻辑分区的扇区
};

struct part_ent{
	u8 boot_ind;    /*引导标识*/
	
	u8 start_head;	/*起始柱面*/
	u8 start_sector;/*起始扇区*/
	u8 start_cyl;	/*起始磁道*/
	u8 sys_id;		/*文件系统编号ID*/
	u8 end_head;    /*结束柱面*/
	u8 end_sector;  /*结束扇区*/
	u8 end_cyl;     /*结束磁道*/
	u32 start_sect;
	u32 nr_sects;	/*分区中的扇区总数*/
}PARTITION_ENTRY;

PUBLIC void hd_open(int device);
#define HD_TIMEOUT		10000
#define PARTITION_TABLE_OFFSET	0x1BE
#define ATA_READ		0x20
#define ATA_WRITE		0x30
#define ATA_IDENTIFY 	0xEC /* 发送0xEC的ATA命令可以得到硬盘的详细参数 */
#define MAKE_DEVICE_REG(lba, drv, lba_highest)(((lba) << 6) 		| \
												((drv) << 4)		| \
												(lba_highest & 0xF) | \
												 0xA0) 

/* HARD Drive*/
#define SECTOR_SIZE 	512
#define SECTOR_BITS		(512 * 8)
#endif