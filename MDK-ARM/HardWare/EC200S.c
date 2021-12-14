#include "ec200s.h"
#include "stdio.h"
#include "cJSON.h"
//#include "oled.h"
char *strx;
IWDG_HandleTypeDef hiwdg;
uint8_t tempstr[255];
//GPSģ��ľ�γ������ֵ
struct 
{
char Latitude[10];//����ԭ����
char longitude[9];//γ��Դ����
char Latitudess[3];//��������
char longitudess[2];
char Latitudedd[7];//С���㲿��
char longitudedd[7];
float TrueLatitude;//ת��������
float Truelongitude;//ת��������	
char buffer[200];//�洢ת���ľ�γ������
char data_len[10];	
}LongLatidata;
 void MX_IWDG_Init(void)
{

  /* USER CODE BEGIN IWDG_Init 0 */

  /* USER CODE END IWDG_Init 0 */

  /* USER CODE BEGIN IWDG_Init 1 */

  /* USER CODE END IWDG_Init 1 */
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_64;
  hiwdg.Init.Window = 4000;
  hiwdg.Init.Reload = 4000;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */

}
void IWDG_Feed(void)
{
	 HAL_IWDG_Refresh(&hiwdg);//ι��
}
void Clear_Buffer(void)//��ջ���
{
    uint16_t i;
//    Uart1_SendStr((char*)RxBuffer);
    for(i=0;i<Rxcouter;i++)
    RxBuffer[i]=0;//����
    Rxcouter=0;
    IWDG_Feed();//ι��
}
void EC200S_Init(void)
{
    printf("AT\r\n"); 
    HAL_Delay(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    Clear_Buffer();	
    while(strx==NULL)
    {
        Clear_Buffer();	
        printf("AT\r\n"); 
        HAL_Delay(300);
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    }
		printf("ATE0&W\r\n");//
    HAL_Delay(300);
	  printf("AT+QMTDISC=0\r\n");//�ر�����
    HAL_Delay(300);
    printf("AT+CIMI\r\n");//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
    HAL_Delay(300);
    strx=strstr((const char*)RxBuffer,(const char*)"460");//��460������ʶ�𵽿���
    Clear_Buffer();	
    while(strx==NULL)
    {
        Clear_Buffer();	
        printf("AT+CIMI\r\n");//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
        HAL_Delay(300);
        strx=strstr((const char*)RxBuffer,(const char*)"460");//����OK,˵�����Ǵ��ڵ�
    }
    printf("AT+QICSGP=1,1,\"CMNET\",\"\",\"\",0\r\n");//����APN�����û���������
    HAL_Delay(300);
    Clear_Buffer();
		printf("AT+CGATT?\r\n");//��ѯ����״̬
		HAL_Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+CGATT: 1");//��1
		Clear_Buffer();	
		while(strx==NULL)
		{
            Clear_Buffer();	
            printf("AT+CGATT?\r\n");//��ȡ����״̬
            HAL_Delay(300);
            strx=strstr((const char*)RxBuffer,(const char*)"+CGATT: 1");//����1,����ע���ɹ�
		}
		printf("AT+CSQ\r\n");//�鿴��ȡCSQֵ
    HAL_Delay(300);
    Clear_Buffer();
		printf("AT+QIACT=1\r\n");//����
    HAL_Delay(300);
    Clear_Buffer();
    printf("AT+QIACT?\r\n");//��ȡ��ǰ����IP��ַ
    HAL_Delay(300);
    Clear_Buffer();
		printf("AT+QGPS=1\r\n"); //��GNSS����
    HAL_Delay(300);
		printf("AT+QGPS?\r\n"); //��GNSS����
    HAL_Delay(300);
    Clear_Buffer();	
}



void EC200U_GETGNSSdata(void)//��ȡGNSS����
{

	  printf("AT+QGPSGNMEA=\"RMC\"\r\n"); //��ȡ��λ��Ϣ
		HAL_Delay(300);
	  strx=strstr((char*)RxBuffer,(char*)"+QGPSGNMEA:");//�Ƿ��ȡ��GPS��ȷ����
    if(strx)
		{
     Getdata_Change();//����GPS����
		}

 // Clear_Buffer();
			if(uart2_getok)//���е�ʱ�򣬽������0
		{
		    Rxcouter=0;
		}
		
}


void EC200S_CreateTCPSokcet(void)//����sokcet
{
    printf("AT+QIOPEN=1,0,\"TCP\",\"47.92.146.210\",10000,0,1\r\n");//��������TCP,����IP�Լ��������˿ں��� ,����ֱ���³��ķ�ʽ
    HAL_Delay(300);//TCP����ģʽ����������״̬
		strx=strstr((const char*)RxBuffer,(const char*)"+QIOPEN: 0,0");//����Ƿ��½�ɹ�
 	while(strx==NULL)
		{
			strx=strstr((const char*)RxBuffer,(const char*)"+QIOPEN: 0,0");//����Ƿ��½�ɹ�
			HAL_Delay(100);
		}  

    Clear_Buffer();	
    
}


void  MQTT_Init(void)
{
    //printf("AT+QMTCFG=\"aliauth\",0,\"%s\",\"%s\",\"%s\"\r\n",ProductKey,DeviceName,DeviceSecret);  //ProductKey== jzj DeviceName==id  DeviceSecret��ȥ��
	  printf("AT+QMTCFG=\"SSL\",0,1,2\r\n");
    HAL_Delay(300);
    printf("AT+QMTOPEN=0,\"mqtt.anlan.tech\",8883\r\n");//ͨ��TCP��ʽȥ����MQTT�����Ʒ����� 
    HAL_Delay(300);
    strx=strstr((const char*)RxBuffer,(const char*)"+QMTOPEN: 0,0");//���·���״̬
  while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTOPEN: 0,0");//ȷ�Ϸ���ֵ��ȷ
    }
    Clear_Buffer();
    HAL_Delay(1000);		
    printf("AT+QMTCONN=0,\"tsttest004\"\r\n");//ȥ��¼MQTT���������豸ID����ƷID,��Ȩ��Ϣ���û�����ʵ����Ҫ���и���
    HAL_Delay(300);
    strx=strstr((const char*)RxBuffer,(const char*)"+QMTCONN: 0,0,0");//���·���״̬
  while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTCONN: 0,0,0");//���·���״̬
    }
    Clear_Buffer(); 
    HAL_Delay(1000);		
    printf("AT+QMTSUB=0,1,\"hello\",0\r\n");//ȥ��¼MQTT���������豸ID����ƷID,��Ȩ��Ϣ���û�����ʵ����Ҫ���и���
    HAL_Delay(300);
    strx=strstr((const char*)RxBuffer,(const char*)"+QMTSUB: 0,1,0,0");//���·���״̬
  while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTSUB: 0,1,0,0");//���·���״̬
    }
    Clear_Buffer(); 

}



//���ʰ�������Ҫ�ύJSON����
uint16_t Mqttaliyun_Savedata(uint8_t *t_payload,uint8_t temp,uint8_t humi)
{
    char json[]="{\"id\":\"26\",\"version\":\"1.0\",\"sys\":{\"ack\":0},\"params\":{\"CurrentTemperature\":{\"value\":%d},\"RelativeHumidity\":{\"value\":%d},\"GeoLocation\":{\"Latitude\":%d.%06d,\"Longitude\":%d.%06d}},\"method\":\"thing.event.property.post\"}";	 //     char json[]="{\"datastreams\":[{\"id\":\"location\",\"datapoints\":[{\"value\":{\"lon\":%2.6f,\"lat\":%2.6f}}]}]}";
    char t_json[300];
    unsigned short json_len;
    sprintf(t_json, json, temp, humi,(unsigned int )LongLatidata.TrueLatitude,(unsigned int)((LongLatidata.TrueLatitude-(unsigned int )LongLatidata.TrueLatitude)*1000000),(unsigned int)	LongLatidata.Truelongitude,(unsigned int)((LongLatidata.Truelongitude-(unsigned int )LongLatidata.Truelongitude)*1000000));
    json_len = strlen(t_json)/sizeof(char);
  	memcpy(t_payload, t_json, json_len);
    return json_len;
}


void EC200S_RECData(void)
{
	  uint8_t recdata[250];
//		Uart1_SendStr(RxBuffer);
    strx=strstr((const char*)RxBuffer,(const char*)"+QMTRECV:");//����+QMTRECV:���������յ�onenetƽ̨
	      //+QMTRECV: 0,0,"$creq/a54ff306-8993-52e1-9cbf-5ef099f4c0c8","1"
	if(strx)
	{
			strx=strstr((const char*)RxBuffer,(const char*)"}\"");
			//  ��}"���ִ������ݽ������
			if(strx)
			{
				memcpy(recdata, RxBuffer, Rxcouter);//���յ������ݱ��� 
				CParsejson(recdata); 
				Clear_Buffer();  
		 
			 }
				memset(recdata, 0, 250);
	 
	} 
	if(uart2_getok)//���е�ʱ�򣬽������0
	{
		Rxcouter=0;
	}
    
    
}


void CParsejson(char *data)
{
    uint8_t i=0,*jsonstrx;
		uint8_t jsondata[250];
		uint8_t status;
  // char* data = "{\"method\":\"thing.service.property.set\",\"id\":\"1686637400\",\"params\":{\"WorkSwitch\":1},\"version\":\"1.0.0\"}";
     cJSON * root, *itemName,*itemAge,*order;
     //�ӻ������н���json�ṹ
    jsonstrx=strstr((const char*)data,(const char*)"{"); //�жϵ���"{����,����json���� 
    if(jsonstrx)
    {
         while(1)
        {
            if(jsonstrx[i]==0x0D)//�ҵ������������˳�  
            {   
                jsondata[i-1]=0;
                i=0;
                break;
            }
            jsondata[i]=jsonstrx[i];
            i++;
        }
        
    }

//    Uart1_SendStr(jsondata);
    root = cJSON_Parse(jsondata);   
    if (jsondata == NULL)
    {
        return;
    }

    order = cJSON_GetObjectItem(root, "params");
	if (order == NULL) {
		return ;
	}
    itemName = cJSON_GetObjectItem(order, "NightLightSwitch");

    if (itemName)
    {
      status= itemName->valueint;//
        if(status)//���̵���
         HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
       else//�ؼ̵���
     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
      
    }
		
		 itemName = cJSON_GetObjectItem(order, "Buzzer");
    if (itemName)
    {
      status= itemName->valueint;//
        if(status)//��BEEP
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
       else//��BEEP
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
    }
    //��json�ṹռ�õ����ݿռ��ͷ�
    memset(jsondata, 0, 250);
    cJSON_Delete(root);
    
}


//���ʰ�������Ҫ�ύJSON���ݽ�״ֵ̬����
void Mqttaliyun_SaveREADdata(void)
{
	  char json[]="{\"id\":\"26\",\"version\":\"1.0\",\"sys\":{\"ack\":0},\"params\":{\"NightLightSwitch\":%d,\"Buzzer\":%d},\"method\":\"thing.event.property.post\"}";	 //     char json[]="{\"datastreams\":[{\"id\":\"location\",\"datapoints\":[{\"value\":{\"lon\":%2.6f,\"lat\":%2.6f}}]}]}";
    char t_json[200];
		uint8_t BEEPstatus, SWTCTRstatus;
    unsigned short json_len;
	    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2))//�ߵ�ƽ
				SWTCTRstatus=0;
			else
				SWTCTRstatus=1;
			if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15))//�ߵ�ƽ
				BEEPstatus=1;
			else
				BEEPstatus=0;
     sprintf(t_json, json, SWTCTRstatus,BEEPstatus);
     json_len = strlen(t_json)/sizeof(char);
     t_json[json_len]=0;
	   printf("AT+QMTPUBEX=0,0,0,0,\"%s\",%d\r\n",PubTopic,json_len);//������Ϣ
		 HAL_Delay(100);
		 printf("%s",t_json);
		strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUBEX: 0,0,0");//�����ɹ�   +QMTPUBEX: 0,0,0
		while(strx==NULL)
		{
			strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUBEX: 0,0,0");//�����ɹ�   +QMTPUBEX: 0,0,0
		}
    HAL_Delay(300);
}

/*************��ԭʼ���ݽ�������γ������*******************/	
void Getdata_Change(void)
{
	unsigned char i;
	char *strx;
  unsigned char paydata[20];
	char json[]="{lon:%d.%06d:lat:%d.%06d}";
	strx=strstr((const char*)RxBuffer,(const char*)"N,");//����N��������γ�����ݱ���ȷ��ȡ��
		if(strx)
		{
			for(i=0;i<9;i++)
			LongLatidata.longitude[i]=strx[i-10];//γ�Ȼ�ȡ��3151.0237
		  strx=strstr((const char*)RxBuffer,(const char*)"E,");//����E����ȡγ������11702.5641
			if(strx)
			{
				for(i=0;i<10;i++)
				LongLatidata.Latitude[i]=strx[i-11];//�������ݻ�ȡ��	
				for(i=0;i<3;i++)
				LongLatidata.Latitudess[i]=LongLatidata.Latitude[i];
				for(i=3;i<10;i++)
				LongLatidata.Latitudedd[i-3]=LongLatidata.Latitude[i];
				LongLatidata.Truelongitude=(LongLatidata.Latitudess[0]-0x30)*100+(LongLatidata.Latitudess[1]-0x30)*10+(LongLatidata.Latitudess[2]-0x30)\
				+((LongLatidata.Latitudedd[0]-0x30)*10+(LongLatidata.Latitudedd[1]-0x30)+(float)(LongLatidata.Latitudedd[3]-0x30)/10+\
				(float)(LongLatidata.Latitudedd[4]-0x30)/100+(float)(LongLatidata.Latitudedd[5]-0x30)/1000+(float)(LongLatidata.Latitudedd[6]-0x30)/10000)/60.0;//��ȡ����������
				//			///////////////////////////////////////////
				for(i=0;i<2;i++)
				LongLatidata.longitudess[i]=LongLatidata.longitude[i];
				for(i=2;i<9;i++)
				LongLatidata.longitudedd[i-2]=LongLatidata.longitude[i];	
				LongLatidata.TrueLatitude=(LongLatidata.longitudess[0]-0x30)*10+(LongLatidata.longitudess[1]-0x30)\
				+((LongLatidata.longitudedd[0]-0x30)*10+(LongLatidata.longitudedd[1]-0x30)+(float)(LongLatidata.longitudedd[3]-0x30)/10+\
				(float)(LongLatidata.longitudedd[4]-0x30)/100+(float)(LongLatidata.longitudedd[5]-0x30)/1000+(float)(LongLatidata.longitudedd[6]-0x30)/10000)/60.0;//��ȡ����������b
				sprintf(LongLatidata.buffer, json, (unsigned int )LongLatidata.Truelongitude,(unsigned int)((LongLatidata.Truelongitude-(unsigned int )LongLatidata.Truelongitude)*1000000)\
				,(unsigned int)LongLatidata.TrueLatitude,(unsigned int)((LongLatidata.TrueLatitude-(unsigned int )LongLatidata.TrueLatitude)*1000000));//����������ת���ַ�������
				//sprintf(LongLatidata.data_len,"%d",strlen(LongLatidata.buffer)/sizeof(char));//����ת���ַ���
				//EC200U_TCPSend((uint8_t*)LongLatidata.buffer);
				for(i=0;i<10;i++)
				paydata[i]=LongLatidata.buffer[i+4];//��ȡ��������
				paydata[10]=0;
				// OLED_ShowString(0+37,4,paydata,16);//��ʾ��OLED��
				for(i=0;i<9;i++)
				paydata[i]=LongLatidata.buffer[i+20];//��ȡγ������
				paydata[9]=0;
				//OLED_ShowString(0+37,6,paydata,16);//��ʾ��OLED��
		  }
	  }
		else
		{
		//	EC200U_TCPSend((uint8_t*)"0000.00000");
//			OLED_ShowString(0+36,4,(u8*)":000.000000",16);
//			OLED_ShowString(0+36,6,(u8*)":00.000000",16);

		}
		
	
}




