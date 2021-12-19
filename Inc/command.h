#ifndef __COMMAND_H
#define __COMMAND_H
/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"
#include "stm32g0xx_ll_system.h"
#include "stdint.h"
/*Micro definition -----------------------------------------------------------*/
#define  LOGIN                         0
#define  READY                         1
#define  STAY_INJECT                   2
#define  HALF_INJECT                   3
#define  FULL_INJECT                   4
#define  TAIL_INJECT                   5
#define  FINISH                        6

#define  TAIL_VALUE                    100000


#define	 T_2MS								    		 0
#define  T_LED                         1
#define  T_HAF                         2
#define  T_3                           3
#define  T_HBT                         4  
#define  TIMER_MAX                     5

#define  GUN_ON                        0
#define  GUN_OFF                       1

#define  u8                            uint8_t
#define  u32                           uint32_t

#define  FREQ_5K                       5000
#define  FREQ_6K                       6000

#define  id                            0

#define  READY_FILL                    0 //"123"//#READY_(id)    //#(READY__(READY_(id)))       
#define  startfill                     1
#define  endfill                       2
#define  breakfill                     3
#define  cancelfill                    4
#define  cycledata                     5

#define  SEND_EN                       1
#define  SEND_OVER                     0

#define  UN_REC_REPLY                  0
#define  REC_REPLY                     1
#define  REPLY_ERR                     2 

#define  TASK_ID                       0
#define  TASK_TIME                     1
#define  TASK_VLM                      2
#define  TASK_TYPE                     3
#define  TASK_NO                       4
#define  TASK_BUSI_TAIL                5
#define  TASK_PRICE                    6
#define  TASK_PULSE_COUNT              7
#define  TASK_PULSE_TAIL               8
#define  TASK_FINISH                   9


 

/*Structure definition -------------------------------------------------------*/
typedef struct	_SW_TIMER_STRUCT
{
	int	old_ms;
	int	timeout_ms;
	int	timeout_over;
	int	count_en;
	int over_ms;
	
} SW_TIMER_STRUCT;

typedef	struct	_SCAN_DATA_STRUCT
{
	__IO	u32	data;
	__IO	u32	data_f1;
	__IO	u32	data_f2;
	__IO	u32	data_f3;
	__IO	u32	data_f4;
	__IO	u32	data_and;
	__IO	u32	data_or;
} SCAN_DATA_STRUCT;

typedef struct	_UREA_PARA
{
	char  message_id[30];
	int   timestamp;
	int   status;
	int 	pulse_equ;
	float open_price;
	char  QR_Code[30];
	float conctr;
	
} UREA_PARA_STRUCT;

typedef struct	_TASK_PARA
{
	char  message_id[30];
	int   timestamp;
	int   task_vlm;
	int 	task_type;
  char  task_no[30];
	int   busi_tail;
	float price;
	int   pulse_count;
	int   pulse_tail;
	
} TASK_PARA_STRUCT;

#endif 

