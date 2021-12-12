#ifndef __EC200S_H
#define __EC200S_H
#include "usart.h"
#include "string.h"
#include "stm32g0xx_hal.h"
void Clear_Buffer(void);
void EC200S_Init(void);
void EC200S_CreateUDPSokcet(void);
void EC200S_Senddata(uint8_t *len,uint8_t *data);
void EC200S_Senddatahex(uint8_t *data);
void EC200S_CreateTCPSokcet(void);
void IWDG_Feed(void);
void MX_IWDG_Init(void);
void  MQTT_Init(void);
int MqttOnenet_Savedata(uint8_t *t_payload,uint8_t temp,uint8_t humi,float adcvalue);
int MqttOnenet_UPdataCtr(void);
void Control_LED(void);
void  mzh_checkstr(uint8_t *buff,uint8_t len);
void EC200S_ONENETRECData(void);
uint16_t Mqttaliyun_Savedata(uint8_t *t_payload,uint8_t temp,uint8_t humi);
void EC200S_RECData(void);
void CParsejson(char *data);
void Mqttaliyun_SaveREADdata(void);
void EC200U_GETGNSSdata(void);
void Getdata_Change(void);
////阿里云的三元素,在设备里面都可以查询到//////////////////////
#define ProductKey     "a1ZMkwhevG9"             //产品KEY
#define DeviceName          "mzh001"      //
#define DeviceSecret    "cc96840980ac49570acc47e2c0195b5c"  //
#define PubTopic         "/sys/a1ZMkwhevG9/mzh001/thing/event/property/post"
#define SubTopic        "/sys/a1SZ2mtWIur/m001/thing/service/property/set"

#endif
