/* *****************************************************************
 *
 
 * Blinker 库下载地址:
 * https://github.com/blinker-iot/blinker-library/archive/master.zip
 * 
 * Blinker 是一套跨硬件、跨平台的物联网解决方案，提供APP端、设备端、
 * 服务器端支持，使用公有云服务进行数据传输存储。可用于智能家居、
 * 数据监测等领域，可以帮助用户更好更快地搭建物联网项目。
 * 
 * 如果使用 ESP8266 接入 Blinker,
 * 请确保安装了 2.7.4 或更新的 ESP8266/Arduino 支持包。
 * https://github.com/esp8266/Arduino/releases
 * 
 * 如果使用 ESP32 接入 Blinker,
 * 请确保安装了 1.0.4 或更新的 ESP32/Arduino 支持包。
 * https://github.com/espressif/arduino-esp32/releases
 * 
 * 文档: https://diandeng.tech/doc
 *       
 * 
 * *****************************************************************/

#define BLINKER_WIFI
#define BLINKER_ALIGENIE_SENSOR
#include <SoftwareSerial.h>
//SoftwareSerial SIMserial(D8,D7); // 建立SoftwareSerial对象，RX引脚2, TX引脚3
 
  

#define BLINKER_MIOT_SENSOR



#include <Blinker.h>

char auth[] = "88888888";//设备密钥
char ssid[] = "8888";//你的WiFi名
char pswd[] = "12345678";//你的WiFi密码

#define BUZZER  13
//#define alarm_lamp  13  //灯和蜂鸣器并联用一个IO控制
#define jie_dian_hua  12

#define mqDPin  16
#define mqAPin  A0
//int mqAPin=A0;
//int mqDPin=D0;
int mqBite=0;
float mqVal=0.0;
float mqVot=0.0;
float Vrl=0;
float co_ppm=0;
int counter=0;
//BlinkerButton Button1("cha-CO");
BlinkerNumber Number1("co_ppm");
BlinkerNumber chaafz("mqVal");

//void aligenieQuery(int32_t queryCode)
//{
//    BLINKER_LOG("AliGenie Query codes: ", queryCode);
//
//    switch (queryCode)
//    {
//        case BLINKER_CMD_QUERY_ALL_NUMBER :
//            BLINKER_LOG("AliGenie Query All");
//            BlinkerAliGenie.temp(20);
//            BlinkerAliGenie.humi(20);
//            BlinkerAliGenie.pm25(20);
//            BlinkerAliGenie.print();
//            break;
//        default :
//            BlinkerAliGenie.temp(20);
//            BlinkerAliGenie.humi(20);
//            BlinkerAliGenie.pm25(20);
//            BlinkerAliGenie.print();
//            break;
//    }
//}
void miotQuery(int32_t queryCode)
{
    BLINKER_LOG("MIOT Query codes: ", queryCode);

    switch (queryCode)
    {
        case BLINKER_CMD_QUERY_PM25_NUMBER :
            BLINKER_LOG("MIOT Query PM25");
            BlinkerMIOT.pm25(20);
            BlinkerMIOT.print();
            break;
        case BLINKER_CMD_QUERY_CO2_NUMBER :
            BLINKER_LOG("MIOT Query CO");
            BlinkerMIOT.co2(co_ppm);
            BlinkerMIOT.print();
            break;
        case BLINKER_CMD_QUERY_HUMI_NUMBER :
            BLINKER_LOG("MIOT Query HUMI");
            BlinkerMIOT.humi(20);
            BlinkerMIOT.print();
            break;
        case BLINKER_CMD_QUERY_TEMP_NUMBER :
            BLINKER_LOG("MIOT Query TEMP");
            BlinkerMIOT.temp(20);
            BlinkerMIOT.print();
            break;
        case BLINKER_CMD_QUERY_TIME_NUMBER :
            BLINKER_LOG("MIOT Query Time");
           // BlinkerMIOT.time(millis());
            BlinkerMIOT.print();
            break;
        default :
            BlinkerMIOT.temp(20);
            BlinkerMIOT.humi(20);
            BlinkerMIOT.pm25(20);
            BlinkerMIOT.co2(co_ppm);
            BlinkerMIOT.print();
            break;
    }
}
void dataStorage()
{
    Blinker.dataStorage("mqVal", mqVal);
    Blinker.dataStorage("co_ppm", co_ppm);
}
void dataRead(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);

    Blinker.vibrate();
    
    uint32_t BlinkerTime = millis();
    
    Blinker.print("millis", BlinkerTime);
    Number1.print(co_ppm);
}
void heartbeat()
{
    Number1.print(co_ppm);
   chaafz.print(mqVal);
    
}

void setup()
{
    Serial.begin(115200);
    // SIMserial.begin(115200); // HC-06 默认波特率 9600
    BLINKER_DEBUG.stream(Serial);

    pinMode(BUZZER, OUTPUT);
    pinMode(jie_dian_hua, INPUT_PULLUP);
    //pinMode(LED_BUILTIN, OUTPUT);
   // digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(BUZZER, HIGH);
    
    
    pinMode(mqAPin, INPUT);
    pinMode(mqDPin, INPUT);

    Blinker.begin(auth, ssid, pswd);
    Blinker.attachData(dataRead);
    Blinker.attachDataStorage(dataStorage);
     Blinker.attachHeartbeat(heartbeat);
    //BlinkerAliGenie.attachQuery(aligenieQuery);
    BlinkerMIOT.attachQuery(miotQuery);
    
}
delay (10000);

void loop()
{   
    int jt=1;
    Blinker.run();
    dataStorage();
    if(counter>240){counter=0;}
    mqVal = analogRead(mqAPin);
// 将ADC输出值转换为模拟电压值
 //mqVot = mqVal*0.0049;
//mqVot = mqVal * 4.9;
Vrl = mqVal*5/1024 ;

//每个传感器的计算公式不同根据实际测量传感器参数设计
//MQ系列传感器电压与浓度转换关系https://blog.csdn.net/qq_35952136/article/details/95589074
co_ppm=pow(((((5-Vrl)/(Vrl/976))/19276)/0.9425),-0.5718);

 
//Serial.println(mqVot);
mqBite = digitalRead(mqDPin);
jt=digitalRead(jie_dian_hua);
//mqBite = 1;

if (mqBite == HIGH){
  digitalWrite(BUZZER, LOW);
//   Serial.println("mqVal=");
//    Serial.println(mqVal);
//     Serial.println("Vrl=");
//    Serial.println(Vrl);
//    Serial.println("co_ppm=");
//    Serial.println(co_ppm);
//    delay(1000);
   if (jt == LOW){
  Serial.println("ATA");
}  
//Serial.println("ATA");
//Serial.println(mqVot);
     delay(100);
     counter=0;
//Serial.println("一氧化碳浓度正常！");
//Serial.println("AT\r\n");
}
else{
//Serial.println("一氧化碳浓度超标！！！");

//SIMserial.println("AT\r\n");
//Serial.println("AT\r\n");
//SIMserial.print("ATD15327240974;\r\n");  //拨打电话
//int i=counter/60;
//Serial.println("ATD");
switch(counter){
    case 10:
        Serial.println("ATD15327240974;\r\n");//把ATD后边的电话号码改成你的电话号码
        break;
    case 60:
        Serial.println("ATD18354360937;\r\n");
        break;
    case 120:
        Serial.println("ATD18364903602;\r\n");
        break;
        case 180:
        Serial.println("ATD18364903975;\r\n");
        break;
}
//Serial.println("\r\n");


 for(int i=4;i>0;i--){
 digitalWrite(BUZZER, !digitalRead(BUZZER));
 // digitalWrite(alarm_lamp,!digitalRead(alarm_lamp));
 delay(250);
 }
 counter++;
// Serial.println("ATD18364903975;\r\n");
// for(int i=1;i>0;i--){
//digitalWrite(BUZZER, !digitalRead(BUZZER));
// digitalWrite(alarm_lamp,!digitalRead(alarm_lamp));
// delay(500);
// }
//  Serial.println("ATD18354360937;\r\n");
// for(int i=1;i>0;i--){
// digitalWrite(BUZZER, !digitalRead(BUZZER));
// digitalWrite(alarm_lamp,!digitalRead(alarm_lamp));
// delay(500);
// }
}
//delay(1000);


}
