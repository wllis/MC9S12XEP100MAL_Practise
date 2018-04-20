
/**********************(c) Copyright 20017-2018, CIDI **********************
	File： LCD_12864.h
	Description：  

    Author：wllis
    Date: 04/17/2018
      
    Note:
  
  Version: 1.0

**************************************************************************/

#ifndef __CAN_H
#define __CAN_H

/*CAN0 Acceptance Code Definitions */ 
#define ACC_CAN0_EX_ID ConvertID2MSCAN_ID(CAN0_ID)//|(0X00180000)&(0xFFFFFFFE))//确保帧类型为扩展数据帧
#define ACC_CAN0_EX_ID_R0 ((ACC_CAN0_EX_ID&0xFF000000)>>24)
#define ACC_CAN0_EX_ID_R1 ((ACC_CAN0_EX_ID&0x00FF0000)>>16)
#define ACC_CAN0_EX_ID_R2 ((ACC_CAN0_EX_ID&0x0000FF00)>>8)
#define ACC_CAN0_EX_ID_R3  (ACC_CAN0_EX_ID&0x000000FF)

/*CAN0 Mask Code Definitions */
#define MASK_CAN0_EX_ID 0x00070000
#define MASK_CAN0_EX_ID_R0 ((MASK_CAN0_EX_ID&0xFF000000)>>24)
#define MASK_CAN0_EX_ID_R1 ((MASK_CAN0_EX_ID&0x00FF0000)>>16)
#define MASK_CAN0_EX_ID_R2 ((MASK_CAN0_EX_ID&0x0000FF00)>>8)
#define MASK_CAN0_EX_ID_R3  (MASK_CAN0_EX_ID&0x000000FF)

/*CAN4 Acceptance Code Definitions */ 
#define ACC_CAN4_EX_ID ConvertID2MSCAN_ID(CAN4_ID)//|(0X00180000)&(0xFFFFFFFE))
#define ACC_CAN4_EX_ID_R0 ((ACC_CAN4_EX_ID&0xFF000000)>>24)
#define ACC_CAN4_EX_ID_R1 ((ACC_CAN4_EX_ID&0x00FF0000)>>16)
#define ACC_CAN4_EX_ID_R2 ((ACC_CAN4_EX_ID&0x0000FF00)>>8)
#define ACC_CAN4_EX_ID_R3  (ACC_CAN4_EX_ID&0x000000FF)

/*CAN4 Mask Code Definitions */
#define MASK_CAN4_EX_ID 0x00070000
#define MASK_CAN4_EX_ID_R0 ((MASK_CAN4_EX_ID&0xFF000000)>>24)
#define MASK_CAN4_EX_ID_R1 ((MASK_CAN4_EX_ID&0x00FF0000)>>16)
#define MASK_CAN4_EX_ID_R2 ((MASK_CAN4_EX_ID&0x0000FF00)>>8)
#define MASK_CAN4_EX_ID_R3  (MASK_CAN4_EX_ID&0x000000FF)

unsigned long ConvertID2MSCAN_ID(unsigned long OID);
//void InitPorts(void); 
void InitCAN0(void); 
int CAN0SendFrame(unsigned long id, unsigned char priority, unsigned char length, unsigned char *txdata );
void InitCAN4(void); 
int CAN4SendFrame(unsigned long id, unsigned char priority, unsigned char length, unsigned char *txdata );
//void setbusclock(void);


extern unsigned long CAN0_ID;
extern unsigned long CAN4_ID;


#endif

/********************************* end of file ***********************************/