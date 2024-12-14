#include <stdio.h>
#include <stdlib.h>

#include "graph_data.h"
#include "role_manager.h"

extern PDisplayDevice g_ptOledDev;

//�ָ�����
static void recover_data(void *arg)
{
	struct GameRole *ptRole = (struct GameRole *)arg;

	ptRole->iCurrentX = ptRole->memento.iOriginX;
	ptRole->iCurrentY = ptRole->memento.iOriginY;
	ptRole->iCurrentFlag = ptRole->memento.iOriginFlag;
}

//extern int flush_start;
/*******************
CLOUD
*******************/
static int clear_cloud(void *arg)
{
	struct GameRole *ptRole = (struct GameRole *)arg;
	
    //printf("clear cloud.\r\n");
	//��˸��ԭ��᲻��������������,һ֡�����30ms,��һ֡��,�ڶ�֡��û��,����֡����,�����������˸����?
    //����option_test�еĽ����,��˸ȷʵ�ǲ�����ɵ�
    //DMA����ʱ,�������ź���������ʾ����,��ȻDMA�ڴ��䣬��DMA�е����ݲ�����,�ͻ���˸.
	ClearGraphInDisplayDev(g_ptOledDev,ptRole->iCurrentX,ptRole->iCurrentY,ptRole->ptGraphBitMap);
    
    return 0;
}

static int move_cloud(void *arg)
{
	struct GameRole *ptRole = (struct GameRole *)arg;
	
	ptRole->iCurrentX -= 1;
	if(ptRole->iCurrentX < -24){
		ptRole->iCurrentX = 128+rand()%128;
		ptRole->iCurrentY = (rand()%24)+9;
	}
    
    return 0;
}

static int load_cloud(void *arg)
{
	struct GameRole *ptRole = (struct GameRole *)arg;
	ShowGraphInDisplayDev(g_ptOledDev,ptRole->iCurrentX,ptRole->iCurrentY,ptRole->ptGraphBitMap);

	//printf("load cloud.\r\n");
    //flush_start =1;
    return 0;
}

static int excute_cloud(void *arg)
{
	struct GameRole *ptRole = (struct GameRole *)arg;

	clear_cloud(ptRole);
	move_cloud(ptRole);
	load_cloud(ptRole);
    return 0;
}


/* λͼ */
GraphBitMap g_tRoleGraphCloud = {
	.iWidth = 24,
	.iHeight = 8,
	.pbuffer = (unsigned char *)CLOUD
};

/* CloudRole */	
/* ��Ҫ����iCurrentX,iCurrentY */
struct GameRole g_tGameRoleCloud1 = {
	.name = "cloud1",
	.iCurrentFlag = FLAG_ENABLE | FLAG_NEUTRAL,
	.ptGraphBitMap = &g_tRoleGraphCloud,
	.iCurrentX = 55,
	.iCurrentY = 16,

	.excute_ops = excute_cloud,
	.recover_data = recover_data
};

struct GameRole g_tGameRoleCloud2 = {
	.name = "cloud2",
	.iCurrentFlag = FLAG_ENABLE | FLAG_NEUTRAL,
	.ptGraphBitMap = &g_tRoleGraphCloud,
	.iCurrentX = 8,
	.iCurrentY = 128,
	
	.excute_ops = excute_cloud,
	.recover_data = recover_data
};


/*******************
GROUND
*******************/
static int clear_ground(void *arg)
{
	return 0;
}

static int move_ground(void *arg)
{
	struct GameRole *ptRole = (struct GameRole *)arg;
	
	ptRole->ptGraphBitMap->index += 3;

	if(ptRole->ptGraphBitMap->index == 450){
		ptRole->ptGraphBitMap->index = 0;
	}
	return 0;
}

static int load_ground(void *arg)
{
	struct GameRole *ptRole = (struct GameRole *)arg;
	
	ShowGraphInDisplayDev(g_ptOledDev,ptRole->iCurrentX,ptRole->iCurrentY,ptRole->ptGraphBitMap);
	return 0;
}

static int excute_ground(void *arg)
{
	struct GameRole *ptRole = (struct GameRole *)arg;
	
	clear_ground(ptRole);
	move_ground(ptRole);
	load_ground(ptRole);
	return 0;
}

/* λͼ */
GraphBitMap g_tRoleGraphGround = {
	.iWidth = 128,
	.iHeight = 8,
	.pbuffer = (unsigned char *)GROUND
};

/* GroundRole */	
struct GameRole g_tGameRoleGround = {
	.name = "ground",
	.iCurrentFlag = FLAG_ENABLE | FLAG_NEUTRAL,
	.ptGraphBitMap = &g_tRoleGraphGround,
	.iCurrentX = 0,
	.iCurrentY =64,

	.excute_ops = excute_ground,
	.recover_data = recover_data
};

/*******************
CACTUS
*******************/
static int clear_cactus(void *arg)
{
	struct GameRole *ptRole = (struct GameRole *)arg;
	
	ClearGraphInDisplayDev(g_ptOledDev,ptRole->iCurrentX,ptRole->iCurrentY,ptRole->ptGraphBitMap);
    
    return 0;
}

static int cactus1_position = 0;
static int cactus2_position = 0;
static int move_cactus1(void *arg)
{
	struct GameRole *ptRole = (struct GameRole *)arg;
	
	ptRole->iCurrentX -= 3;
	cactus1_position = ptRole->iCurrentX;
	
	if(ptRole->iCurrentX < -16){
		if(cactus2_position < 92){
			ptRole->iCurrentX = 128+rand()%40; //36��֤��������²��������ڶ���������
		}else{
			ptRole->iCurrentX = cactus2_position+40+rand()%64; 
		}
	}
    return 0;
}

static int move_cactus2(void *arg)
{
	struct GameRole *ptRole = (struct GameRole *)arg;
	
	ptRole->iCurrentX -= 3;
	cactus2_position = ptRole->iCurrentX;
	
	if(ptRole->iCurrentX < -16){
		if(cactus1_position < 92){
			ptRole->iCurrentX = 128+rand()%40; //36��֤��������²��������ڶ���������
		}else{
			ptRole->iCurrentX = cactus1_position+40+rand()%64; 
		}
	}
    return 0;
}
static int load_cactus(void *arg)
{
	struct GameRole *ptRole = (struct GameRole *)arg;
	
	ShowGraphInDisplayDev(g_ptOledDev,ptRole->iCurrentX,ptRole->iCurrentY,ptRole->ptGraphBitMap);
    
    return 0;
}

static int excute_cactus1(void *arg)
{
	struct GameRole *ptRole = (struct GameRole *)arg;

	clear_cactus(ptRole);
	move_cactus1(ptRole);
	load_cactus(ptRole);
    
    return 0;
}

static int excute_cactus2(void *arg)
{
	struct GameRole *ptRole = (struct GameRole *)arg;

	clear_cactus(ptRole);
	move_cactus2(ptRole);
	load_cactus(ptRole);
    
    return 0;
}
	
/* λͼ */
GraphBitMap g_tRoleGraphCactus1 = {
	.iWidth = 8,
	.iHeight = 16,
	.pbuffer = (unsigned char *)CACTUS_1
};

/* λͼ2 */
GraphBitMap g_tRoleGraphCactus2 = {
	.iWidth = 16,
	.iHeight = 16,
	.pbuffer = (unsigned char *)CACTUS_2
};
	
/* CactusRole: 1*/	
/* ��Ҫ����iCurrentX,iCurrentY */
struct GameRole g_tGameRoleCactus1 = {
	.name = "cactus1",
	.iCurrentFlag = FLAG_ENABLE | FLAG_ENEMY,
	.ptGraphBitMap = &g_tRoleGraphCactus1,
	.iCurrentX = 70,
	.iCurrentY = 64,

	.excute_ops = excute_cactus1,
	.recover_data = recover_data
};

/* CactusRole: 2*/	
/* ��Ҫ����iCurrentX,iCurrentY */
struct GameRole g_tGameRoleCactus2 = {
	.name = "cactus2",
	.iCurrentFlag = FLAG_ENABLE | FLAG_ENEMY,
	.ptGraphBitMap = &g_tRoleGraphCactus2,
	.iCurrentX = 128,
	.iCurrentY = 64,

	.excute_ops = excute_cactus2,
	.recover_data = recover_data
};

/*******************
DINOSAUR
*******************/
//uint16_t role_height = 0;							//dino�ƶ��ľ���,��ʼֵΪ0

static int clear_dinosaur(void *arg)
{
	struct GameRole *ptRole = (struct GameRole *)arg;
	
	ClearGraphInDisplayDev(g_ptOledDev,ptRole->iCurrentX,ptRole->iCurrentY,ptRole->ptGraphBitMap);
    
    return 0;
}

static char speed_table[] = {1, 1, 3, 3, 4, 4, 5, 6, 7};
int speed_index = sizeof(speed_table);		//�ٶ�����,��ʼֵΪ9

static int move_dinosaur(void *arg)
{	
	struct GameRole *ptRole = (struct GameRole *)arg;

	/* ����Ϊ����״̬ */
	if(ptRole->iCurrentFlag & FLAG_CONTROL_UP){			
		ptRole->iCurrentY -= speed_table[speed_index - 1];	//�����߶�,����һ��9֡,9�ε���,һ��34
		speed_index--;
		
		/* ��������ߴ�,role_height = 34 */
		if(speed_index == 0){							
			ptRole->iCurrentFlag &= ~FLAG_CONTROL_UP;		//���������־λ
			ptRole->iCurrentFlag |= FLAG_CONTROL_DOWN;		//��Ϊ����״̬
		}
	}
	/* ����Ϊ����״̬ */
	else if(ptRole->iCurrentFlag & FLAG_CONTROL_DOWN){
		ptRole->iCurrentY += speed_table[speed_index];		//�������,�½�һ��9֡,9�ε���,һ��34
		speed_index++;

		/* ���䵽���� */
		if(speed_index == 9){						
			ptRole->iCurrentFlag &= ~FLAG_CONTROL_DOWN;		//��������־λ
			ptRole->iCurrentFlag |= FLAG_CONTROL_DEFAULT;	//��ΪĬ��״̬
		}
	}
	/* ���ܶ��� */
	ptRole->ptGraphBitMap->index = (ptRole->ptGraphBitMap->index + 32)%64; 
    
	return 0;
}

static int load_dinosaur(void *arg)
{
	struct GameRole *ptRole = (struct GameRole *)arg;
	
	ShowGraphInDisplayDev(g_ptOledDev,ptRole->iCurrentX,ptRole->iCurrentY,ptRole->ptGraphBitMap);
	return 0;
}

static int excute_dinosaur(void *arg)
{
	struct GameRole *ptRole = (struct GameRole *)arg;
	
	clear_dinosaur(ptRole);
	move_dinosaur(ptRole);
	load_dinosaur(ptRole);
	return 0;
}

static int control_dinosaur(void *role,int cmd)
{
	struct GameRole *ptRole = (struct GameRole *)role;

	if(cmd == CONTROL_OPS_CMD_UP){
		//�ܹ�������
		if(ptRole->iCurrentFlag & FLAG_CONTROL_DEFAULT){
			ptRole->iCurrentFlag &= ~FLAG_CONTROL_DEFAULT;		//���Ĭ��״̬��־λ
			ptRole->iCurrentFlag |= FLAG_CONTROL_UP;			//��һ����״̬��־λ
		}
		
	}
	else if(cmd == CONTROL_OPS_CMD_ENABLE){
		//�����������
		ptRole->iCurrentFlag |= FLAG_ENABLE;
	}
	else{
		ptRole->iCurrentFlag |= FLAG_DISABLE;
	}
	return 0;
}

/* λͼ */
GraphBitMap g_tRoleGraphDinosaur = {
	.iWidth = 16,
	.iHeight = 16,
	.pbuffer = (unsigned char *)DINOSAUR
};

/* DinosaurRole */	
struct GameRole g_tGameRoleDinosaur = {
	.name = "dinosaur",
	.iCurrentFlag = FLAG_ENABLE | FLAG_CONTROL_DEFAULT,
	.ptGraphBitMap = &g_tRoleGraphDinosaur,
	.iCurrentX = 12,
	.iCurrentY = 56,

	.control_ops = control_dinosaur,
	.excute_ops  = excute_dinosaur,
	.recover_data = recover_data
};

void AddRoleToManager(void)
{
	RoleRegister(&g_tGameRoleCloud1);
	RoleRegister(&g_tGameRoleCloud2);
	RoleRegister(&g_tGameRoleGround);
	RoleRegister(&g_tGameRoleCactus1);
	RoleRegister(&g_tGameRoleCactus2);
    RoleRegister(&g_tGameRoleDinosaur);
	//RoleRegister(&g_tGameRoleDinosaur2);
}


