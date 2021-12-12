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
#define  TIMER_MAX                     3

#define  GUN_ON                        0
#define  GUN_OFF                       1

#define  u8                            uint8_t
#define  u32                           uint32_t

#define  FREQ_5K                       5000
#define  FREQ_6K                       6000

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



#endif 

