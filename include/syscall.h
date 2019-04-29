#ifndef	_TTAO_SYSCALL_H_
#define _TTAO_SYSCALL_H_

struct mess1{
	int mli1;
	int mli2;
	int mli3;
	int mli4;
};

struct mess2{
	void* m2p1;
	void* m2p2;
	void* m2p3;
	void* m2p4;
};

struct mess3{
	int m3il;
	int m3i2;
	int m3i3;
	int m3i4;
	u64 m3l1;
	u64 m3l2;
	void* m3p1;
	void* m3p2;
};

typedef struct{
	int source;
	int type;
	union{
		struct mess1 m1;
		struct mess2 m2;
		struct mess3 m3;
	}u;
}MESSAGE;


PUBLIC void sendrec(int function, int src_desc, MESSAGE * msg);
#endif