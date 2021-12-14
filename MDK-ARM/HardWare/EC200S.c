#include "ec200s.h"
#include "stdio.h"
#include "cJSON.h"
//#include "oled.h"
char *strx;
IWDG_HandleTypeDef hiwdg;
uint8_t tempstr[255];
//GPS模块的经纬度数据值
struct 
{
char Latitude[10];//经度原数据
char longitude[9];//纬度源数据
char Latitudess[3];//整数部分
char longitudess[2];
char Latitudedd[7];//小数点部分
char longitudedd[7];
float TrueLatitude;//转换过数据
float Truelongitude;//转换过数据	
char buffer[200];//存储转换的经纬度数据
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
	 HAL_IWDG_Refresh(&hiwdg);//喂狗
}
void Clear_Buffer(void)//清空缓存
{
    uint16_t i;
//    Uart1_SendStr((char*)RxBuffer);
    for(i=0;i<Rxcouter;i++)
    RxBuffer[i]=0;//缓存
    Rxcouter=0;
    IWDG_Feed();//喂狗
}
void EC200S_Init(void)
{
    printf("AT\r\n"); 
    HAL_Delay(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
    Clear_Buffer();	
    while(strx==NULL)
    {
        Clear_Buffer();	
        printf("AT\r\n"); 
        HAL_Delay(300);
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
    }
		printf("ATE0&W\r\n");//
    HAL_Delay(300);
	  printf("AT+QMTDISC=0\r\n");//关闭连接
    HAL_Delay(300);
    printf("AT+CIMI\r\n");//获取卡号，类似是否存在卡的意思，比较重要。
    HAL_Delay(300);
    strx=strstr((const char*)RxBuffer,(const char*)"460");//返460，表明识别到卡了
    Clear_Buffer();	
    while(strx==NULL)
    {
        Clear_Buffer();	
        printf("AT+CIMI\r\n");//获取卡号，类似是否存在卡的意思，比较重要。
        HAL_Delay(300);
        strx=strstr((const char*)RxBuffer,(const char*)"460");//返回OK,说明卡是存在的
    }
    printf("AT+QICSGP=1,1,\"CMNET\",\"\",\"\",0\r\n");//接入APN，无用户名和密码
    HAL_Delay(300);
    Clear_Buffer();
		printf("AT+CGATT?\r\n");//查询激活状态
		HAL_Delay(300);
		strx=strstr((const char*)RxBuffer,(const char*)"+CGATT: 1");//返1
		Clear_Buffer();	
		while(strx==NULL)
		{
            Clear_Buffer();	
            printf("AT+CGATT?\r\n");//获取激活状态
            HAL_Delay(300);
            strx=strstr((const char*)RxBuffer,(const char*)"+CGATT: 1");//返回1,表明注网成功
		}
		printf("AT+CSQ\r\n");//查看获取CSQ值
    HAL_Delay(300);
    Clear_Buffer();
		printf("AT+QIACT=1\r\n");//激活
    HAL_Delay(300);
    Clear_Buffer();
    printf("AT+QIACT?\r\n");//获取当前卡的IP地址
    HAL_Delay(300);
    Clear_Buffer();
		printf("AT+QGPS=1\r\n"); //对GNSS开机
    HAL_Delay(300);
		printf("AT+QGPS?\r\n"); //对GNSS开机
    HAL_Delay(300);
    Clear_Buffer();	
}



void EC200U_GETGNSSdata(void)//获取GNSS数据
{

	  printf("AT+QGPSGNMEA=\"RMC\"\r\n"); //获取定位信息
		HAL_Delay(300);
	  strx=strstr((char*)RxBuffer,(char*)"+QGPSGNMEA:");//是否获取到GPS正确发送
    if(strx)
		{
     Getdata_Change();//解析GPS数据
		}

 // Clear_Buffer();
			if(uart2_getok)//空闲的时候，将数组归0
		{
		    Rxcouter=0;
		}
		
}


void EC200S_CreateTCPSokcet(void)//创建sokcet
{
    printf("AT+QIOPEN=1,0,\"TCP\",\"47.92.146.210\",10000,0,1\r\n");//创建连接TCP,输入IP以及服务器端口号码 ,采用直接吐出的方式
    HAL_Delay(300);//TCP连接模式，会检测连接状态
		strx=strstr((const char*)RxBuffer,(const char*)"+QIOPEN: 0,0");//检查是否登陆成功
 	while(strx==NULL)
		{
			strx=strstr((const char*)RxBuffer,(const char*)"+QIOPEN: 0,0");//检查是否登陆成功
			HAL_Delay(100);
		}  

    Clear_Buffer();	
    
}


void  MQTT_Init(void)
{
    //printf("AT+QMTCFG=\"aliauth\",0,\"%s\",\"%s\",\"%s\"\r\n",ProductKey,DeviceName,DeviceSecret);  //ProductKey== jzj DeviceName==id  DeviceSecret可去掉
	  printf("AT+QMTCFG=\"SSL\",0,1,2\r\n");
    HAL_Delay(300);
    printf("AT+QMTOPEN=0,\"mqtt.anlan.tech\",8883\r\n");//通过TCP方式去连接MQTT阿里云服务器 
    HAL_Delay(300);
    strx=strstr((const char*)RxBuffer,(const char*)"+QMTOPEN: 0,0");//看下返回状态
  while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTOPEN: 0,0");//确认返回值正确
    }
    Clear_Buffer();
    HAL_Delay(1000);		
    printf("AT+QMTCONN=0,\"tsttest004\"\r\n");//去登录MQTT服务器，设备ID，产品ID,鉴权信息。用户根据实际需要进行更改
    HAL_Delay(300);
    strx=strstr((const char*)RxBuffer,(const char*)"+QMTCONN: 0,0,0");//看下返回状态
  while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTCONN: 0,0,0");//看下返回状态
    }
    Clear_Buffer(); 
    HAL_Delay(1000);		
    printf("AT+QMTSUB=0,1,\"hello\",0\r\n");//去登录MQTT服务器，设备ID，产品ID,鉴权信息。用户根据实际需要进行更改
    HAL_Delay(300);
    strx=strstr((const char*)RxBuffer,(const char*)"+QMTSUB: 0,1,0,0");//看下返回状态
  while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTSUB: 0,1,0,0");//看下返回状态
    }
    Clear_Buffer(); 

}



//访问阿里云需要提交JSON数据
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
    strx=strstr((const char*)RxBuffer,(const char*)"+QMTRECV:");//返回+QMTRECV:，表明接收到onenet平台
	      //+QMTRECV: 0,0,"$creq/a54ff306-8993-52e1-9cbf-5ef099f4c0c8","1"
	if(strx)
	{
			strx=strstr((const char*)RxBuffer,(const char*)"}\"");
			//  有}"出现代表数据接收完成
			if(strx)
			{
				memcpy(recdata, RxBuffer, Rxcouter);//接收到的数据保存 
				CParsejson(recdata); 
				Clear_Buffer();  
		 
			 }
				memset(recdata, 0, 250);
	 
	} 
	if(uart2_getok)//空闲的时候，将数组归0
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
     //从缓冲区中解析json结构
    jsonstrx=strstr((const char*)data,(const char*)"{"); //判断到有"{出现,代表json产生 
    if(jsonstrx)
    {
         while(1)
        {
            if(jsonstrx[i]==0x0D)//找到结束符，就退出  
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
        if(status)//开继电器
         HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
       else//关继电器
     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
      
    }
		
		 itemName = cJSON_GetObjectItem(order, "Buzzer");
    if (itemName)
    {
      status= itemName->valueint;//
        if(status)//开BEEP
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
       else//关BEEP
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
    }
    //将json结构占用的数据空间释放
    memset(jsondata, 0, 250);
    cJSON_Delete(root);
    
}


//访问阿里云需要提交JSON数据将状态值返回
void Mqttaliyun_SaveREADdata(void)
{
	  char json[]="{\"id\":\"26\",\"version\":\"1.0\",\"sys\":{\"ack\":0},\"params\":{\"NightLightSwitch\":%d,\"Buzzer\":%d},\"method\":\"thing.event.property.post\"}";	 //     char json[]="{\"datastreams\":[{\"id\":\"location\",\"datapoints\":[{\"value\":{\"lon\":%2.6f,\"lat\":%2.6f}}]}]}";
    char t_json[200];
		uint8_t BEEPstatus, SWTCTRstatus;
    unsigned short json_len;
	    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2))//高电平
				SWTCTRstatus=0;
			else
				SWTCTRstatus=1;
			if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15))//高电平
				BEEPstatus=1;
			else
				BEEPstatus=0;
     sprintf(t_json, json, SWTCTRstatus,BEEPstatus);
     json_len = strlen(t_json)/sizeof(char);
     t_json[json_len]=0;
	   printf("AT+QMTPUBEX=0,0,0,0,\"%s\",%d\r\n",PubTopic,json_len);//发布消息
		 HAL_Delay(100);
		 printf("%s",t_json);
		strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUBEX: 0,0,0");//发布成功   +QMTPUBEX: 0,0,0
		while(strx==NULL)
		{
			strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUBEX: 0,0,0");//发布成功   +QMTPUBEX: 0,0,0
		}
    HAL_Delay(300);
}

/*************将原始数据解析出经纬度数据*******************/	
void Getdata_Change(void)
{
	unsigned char i;
	char *strx;
  unsigned char paydata[20];
	char json[]="{lon:%d.%06d:lat:%d.%06d}";
	strx=strstr((const char*)RxBuffer,(const char*)"N,");//返回N，表明经纬度数据被正确获取了
		if(strx)
		{
			for(i=0;i<9;i++)
			LongLatidata.longitude[i]=strx[i-10];//纬度获取到3151.0237
		  strx=strstr((const char*)RxBuffer,(const char*)"E,");//返回E，读取纬度数据11702.5641
			if(strx)
			{
				for(i=0;i<10;i++)
				LongLatidata.Latitude[i]=strx[i-11];//经度数据获取到	
				for(i=0;i<3;i++)
				LongLatidata.Latitudess[i]=LongLatidata.Latitude[i];
				for(i=3;i<10;i++)
				LongLatidata.Latitudedd[i-3]=LongLatidata.Latitude[i];
				LongLatidata.Truelongitude=(LongLatidata.Latitudess[0]-0x30)*100+(LongLatidata.Latitudess[1]-0x30)*10+(LongLatidata.Latitudess[2]-0x30)\
				+((LongLatidata.Latitudedd[0]-0x30)*10+(LongLatidata.Latitudedd[1]-0x30)+(float)(LongLatidata.Latitudedd[3]-0x30)/10+\
				(float)(LongLatidata.Latitudedd[4]-0x30)/100+(float)(LongLatidata.Latitudedd[5]-0x30)/1000+(float)(LongLatidata.Latitudedd[6]-0x30)/10000)/60.0;//获取完整的数据
				//			///////////////////////////////////////////
				for(i=0;i<2;i++)
				LongLatidata.longitudess[i]=LongLatidata.longitude[i];
				for(i=2;i<9;i++)
				LongLatidata.longitudedd[i-2]=LongLatidata.longitude[i];	
				LongLatidata.TrueLatitude=(LongLatidata.longitudess[0]-0x30)*10+(LongLatidata.longitudess[1]-0x30)\
				+((LongLatidata.longitudedd[0]-0x30)*10+(LongLatidata.longitudedd[1]-0x30)+(float)(LongLatidata.longitudedd[3]-0x30)/10+\
				(float)(LongLatidata.longitudedd[4]-0x30)/100+(float)(LongLatidata.longitudedd[5]-0x30)/1000+(float)(LongLatidata.longitudedd[6]-0x30)/10000)/60.0;//获取完整的数据b
				sprintf(LongLatidata.buffer, json, (unsigned int )LongLatidata.Truelongitude,(unsigned int)((LongLatidata.Truelongitude-(unsigned int )LongLatidata.Truelongitude)*1000000)\
				,(unsigned int)LongLatidata.TrueLatitude,(unsigned int)((LongLatidata.TrueLatitude-(unsigned int )LongLatidata.TrueLatitude)*1000000));//浮点型数据转成字符串数据
				//sprintf(LongLatidata.data_len,"%d",strlen(LongLatidata.buffer)/sizeof(char));//长度转成字符串
				//EC200U_TCPSend((uint8_t*)LongLatidata.buffer);
				for(i=0;i<10;i++)
				paydata[i]=LongLatidata.buffer[i+4];//获取经度数据
				paydata[10]=0;
				// OLED_ShowString(0+37,4,paydata,16);//显示在OLED上
				for(i=0;i<9;i++)
				paydata[i]=LongLatidata.buffer[i+20];//获取纬度数据
				paydata[9]=0;
				//OLED_ShowString(0+37,6,paydata,16);//显示在OLED上
		  }
	  }
		else
		{
		//	EC200U_TCPSend((uint8_t*)"0000.00000");
//			OLED_ShowString(0+36,4,(u8*)":000.000000",16);
//			OLED_ShowString(0+36,6,(u8*)":00.000000",16);

		}
		
	
}




