#include "stdio.h"
#include "mem.h"
#include "type.h"
#include "const.h"
#include "protect.h"
#include "syscall.h"
#include "tty.h"
#include "func.h"
#include "proc.h"
#include "global.h"

PRIVATE void hd_handler();
PRIVATE void hd_cmd_out(struct hd_cmd * cmd);
PRIVATE int waitfor(int mask, int val, int timeout);
PRIVATE void interrupt_wait();
PRIVATE void print_identify_info(u16* hdinfo);
PRIVATE void hd_identify(int drive);

PRIVATE	u8 hdbuf[SECTOR_SIZE * 2];

PRIVATE void partition(int device, int style);
PRIVATE void print_hdinfo(struct hd_info *hdi);

PRIVATE void get_part_table(int drive, int sect_nr, struct part_ent *entry);

PRIVATE struct hd_info hd_info[1];
#define DRV_OF_DEV(dev) (dev <= MAX_PRIM? \
						 dev / NR_PRIM_PER_DRIVE : \
						 (dev - MINOR_hd1a) / NR_SUB_PER_DRIVE)

PUBLIC void Init_Hd(){
	int i;
	/* 从物理地址0x475处获取系统内硬盘数量 */
	u8 *pNrDrives = (u8*)(0x475);
	//disp_int(*pNrDrives);
	assert(*pNrDrives);
	
	put_irq_handler(NUM_AT_WINI_IRQ, hd_handler); //添加中断函数到 硬盘中断
	enable_irq(NUM_CASCADE_IRQ); //打开第二块8259A中端芯片
	enable_irq(NUM_AT_WINI_IRQ); //打开硬盘中断
	for(i=0;i<(sizeof(hd_info)/sizeof(hd_info[0]));i++){
		memset(&hd_info[i], 0, sizeof(hd_info[0]));
	}
	hd_info[0].open_cnt = 0;
}

PUBLIC void hd_open(int device){
	int drive = DRV_OF_DEV(device);
	assert(drive == 0);

	hd_identify(drive);
	if(hd_info[drive].open_cnt++ == 0){
		partition(drive * (NR_PART_PER_DRIVE + 1), P_PRIMARY);
		print_hdinfo(&hd_info[drive]);
	}
}

PRIVATE void partition(int device, int style){
	int i;
	int drive = DRV_OF_DEV(device);
	
	struct hd_info *hdi = &hd_info[drive];
	struct part_ent part_tbl[NR_SUB_PER_DRIVE];
	if(style == P_PRIMARY){
		get_part_table(drive, drive, part_tbl);
		
		int nr_prim_parts = 0;
		for(i = 0;i < NR_PART_PER_DRIVE;i++){
			if(part_tbl[i].sys_id == NO_PART)
				continue;
			nr_prim_parts++;
			int dev_nr = i + 1;
			hdi->primary[dev_nr].base = part_tbl[i].start_sect;
			hdi->primary[dev_nr].size = part_tbl[i].nr_sects;

			if (part_tbl[i].sys_id == EXT_PART) /* extended */
				partition(device + dev_nr, P_EXTENDED);
		}
		printf("parts %d \n", nr_prim_parts);
		assert(nr_prim_parts != 0);
	}else if(style == P_EXTENDED){
		
	}
}

PRIVATE void get_part_table(int drive, int sect_nr, struct part_ent *entry){
	struct hd_cmd cmd;
	cmd.features = 0;
	cmd.count = 1;
	cmd.lba_low = sect_nr & 0xFF;
	cmd.lba_mid = (sect_nr >> 8) & 0xFF;
	cmd.lba_high = (sect_nr >> 16) & 0xFF;
	cmd.device = MAKE_DEVICE_REG(1, drive, (sect_nr >> 24) & 0xF);
	cmd.command = ATA_READ;

	hd_cmd_out(&cmd);
	
	interrupt_wait();
		disp_str("--->>>>");

	port_read(REG_DATA, hdbuf, SECTOR_SIZE);
	print_identify_info((u16 *)hdbuf);
	
	memcpy(entry, 
		   hdbuf + PARTITION_TABLE_OFFSET,
		   sizeof(struct part_ent) * NR_PART_PER_DRIVE); 
}

PRIVATE void print_hdinfo(struct hd_info *hdi){
	int i;
	for(i=0;i<NR_PART_PER_DRIVE + 1;i++){
		
	}
}
PRIVATE void hd_identify(int drive){
	struct hd_cmd cmd;
	cmd.device = MAKE_DEVICE_REG(0, drive, 0);
		
	cmd.command = ATA_IDENTIFY;
	hd_cmd_out(&cmd);

	interrupt_wait();

	port_read(REG_DATA, hdbuf, SECTOR_SIZE);
	
	print_identify_info((u16 *)hdbuf);
}

PRIVATE void print_identify_info(u16* hdinfo){
	int i, k;
	char s[64];
	struct iden_info_ascii{
		int idx;
		int len;
		char * desc;
	}iinfo[]={
				{10, 20, "HD SN"},
				{27, 40, "HD Model"}
			};
	for(k = 0;k < sizeof(iinfo)/sizeof(iinfo[0]);k++){
		char * p = (char*) &hdinfo[iinfo[k].idx];
		for(i=0;i<iinfo[k].len/2;i++){
			s[i*2+1] = *p++;
			s[i*2]   = *p++;
		}
		s[i*2] = 0;
		disp_str(iinfo[k].desc);
		disp_str(" -- ");
		disp_str(s);  
		disp_str("\n");
	}
 	int capabilities = hdinfo[49];
	disp_str("LBA supported: ");
	disp_str((capabilities & 0x0200) ? "Yes\n" : "No\n");

	int cmd_set_supported = hdinfo[83];
	disp_str("LBA48 supported: ");
	disp_str((cmd_set_supported & 0x0400) ? "Yes\n" : "No\n");

	int sectors = ((int)hdinfo[61] << 16) + hdinfo[60];
	disp_str("HD size: ");
	disp_int(sectors * 512 / 1000000);  
	disp_str("MB\n");
}

PRIVATE void interrupt_wait(){
	MESSAGE msg;
	sendrec(RECEIVE, INTERRUPT, &msg);
}

PRIVATE void hd_cmd_out(struct hd_cmd * cmd){
	if(!waitfor(STATUS_BSY, 0, HD_TIMEOUT))
		panic("hd error!");
	out_port(REG_DEV_CTRL, 0);
	out_port(REG_FEATURES, cmd->features);
	out_port(REG_SECTOR_COUNT, cmd->count);
	out_port(REG_LBA_LOW, cmd->lba_low);
	out_port(REG_LBA_MID, cmd->lba_mid);
	out_port(REG_LBA_HIGH, cmd->lba_high);
	out_port(REG_DEVICE, cmd->device);

	out_port(REG_CMD, cmd->command); 
}
/**
 * <Ring 0> 等待硬盘空闲 
 * 在它内部操作期间，它将 0x1f7 端口的第 7 位置“1”，表明自己很忙。一旦硬盘系统准备就绪，它再将此位清零，说明自己已经忙完了，同时将第 3位置“1”，意思是准备好了，请求主机发送或者接收数据
 *
 *@return 成功 1 失败 0
 */
PRIVATE int waitfor(int mask, int val, int timeout){
	int t = get_ticks();
	while(((get_ticks() - t) * 1000 / HZ) < timeout)
		if((in_port(REG_STATUS) & mask) == val)
			return 1;
	return 0;
}
/**
 * <Ring 0> Interrupt handler 
 *
 *
 *
 */
PRIVATE void hd_handler(){
	disp_str("this is a int!\n");
	u8 hd_status = in_port(REG_STATUS);
	
	inform_int(TASK_HD);
}