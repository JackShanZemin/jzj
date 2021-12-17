#ifndef __COMMAND_H
#define __COMMAND_H
/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"
#include "stm32g0xx_ll_system.h"
#include "stdint.h"
/*Micro definition -----------------------------------------------------------*/
#define  READY                         0
#define  STAY_INJECT                   1
#define  HALF_INJECT                   2
#define  FULL_INJECT                   3
#define  TAIL_INJECT                   4
#define  FINISH                        5

#define  TAIL_VALUE                    100000


#define	 T_2MS								    		 0
#define  T_LED                         1
#define  T_HAF                         2
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
 

/*Structure definition -------------------------------------------------------*/
typedef struct	_SW_TIMER_STRUCT
{
	u32	old_ms;
	u32	timeout_ms;
	u32	timeout_over;
	int	count_en;
	u32 over_ms;
	
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



#endif 

