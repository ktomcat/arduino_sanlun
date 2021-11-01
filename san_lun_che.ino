#include <IRremote.h>
#define LR_PIN 11
#define STBY 2
#define PWM_R 9
#define RIN_1 4
#define RIN_2 5
#define PWM_L 6
#define LIN_1 7
#define LIN_2 8


IRrecv irrecv(LR_PIN);
decode_results results;
long ir_item;
long cmd = 0x00;
int spd_l = 50;
int spd_h = 100;
int spd = 50;
int wave = 0;

void setup(){
  Serial.begin(9600);
  Serial.print("start");
  irrecv.enableIRIn();
  initPins();
  
      digitalWrite(STBY,HIGH);
      
//      analogWrite(PWM_L,spd);
//      digitalWrite(LIN_1,LOW);
//      digitalWrite(LIN_2,HIGH);//后
      
//      analogWrite(PWM_R,spd);
//      digitalWrite(RIN_1,LOW);
//      digitalWrite(RIN_2,HIGH);//前
}

void initPins(){
  pinMode(STBY,OUTPUT);
  pinMode(PWM_L,OUTPUT);
  pinMode(LIN_1,OUTPUT);
  pinMode(LIN_2,OUTPUT);
  pinMode(PWM_R,OUTPUT);
  pinMode(RIN_1,OUTPUT);
  pinMode(RIN_2,OUTPUT);
  analogWrite(PWM_L,spd);
  analogWrite(PWM_R,spd);
  digitalWrite(STBY,LOW);
  digitalWrite(LIN_1,LOW);
  digitalWrite(LIN_2,LOW);
  digitalWrite(RIN_1,LOW);
  digitalWrite(RIN_2,LOW);
}


void loop(){
  if(irrecv.decode(&results)){
    ir_item = results.value;
    String tp="UNKNOW";
    String typelist[14] = {"UNKNOW","NEC","SONY","RC5","RC6","DISH","SHARP","PANASONIC","JVC","SANYOw","MITSUBISHI","SAMSUNG","LG","WHYNTER"};
    if(results.decode_type>=1 && results.decode_type<=13){
      tp=typelist[results.decode_type];
    }
    Serial.print("IR TYPE："+tp+" ");
    Serial.println(ir_item,HEX);
    if(cmd != ir_item){
      switch(ir_item){
        //暂停
        case 0xEBFD8B21:
        case 0x53576295:
        case 0x565A44F8:
          digitalWrite(STBY,LOW);
          digitalWrite(LIN_1,LOW);
          digitalWrite(LIN_2,LOW);
          digitalWrite(RIN_1,LOW);
          digitalWrite(RIN_2,LOW);
          cmd = ir_item;
          break;
        
        //右转
        case 0xE05993FD:
        case 0x6161FD34:
          analogWrite(PWM_L,spd);
          analogWrite(PWM_R,spd);
          digitalWrite(STBY,HIGH);
          digitalWrite(STBY,HIGH);
          digitalWrite(LIN_1,HIGH);
          digitalWrite(LIN_2,LOW);
          digitalWrite(RIN_1,HIGH);
          digitalWrite(RIN_2,LOW);
          cmd = ir_item;
          break;
        
        //左转
        case 0x5371C298:
        case 0xf2EFACADD:
          analogWrite(PWM_L,spd);
          analogWrite(PWM_R,spd);
          digitalWrite(STBY,HIGH);
          digitalWrite(LIN_1,LOW);
          digitalWrite(LIN_2,HIGH);
          digitalWrite(RIN_1,LOW);
          digitalWrite(RIN_2,HIGH);
          cmd = ir_item;
          break;
        
        //后退
        case 0x73E3EAC9:
        case 0xCDD7A668:
          analogWrite(PWM_L,spd);
          analogWrite(PWM_R,spd);
          digitalWrite(STBY,HIGH);
          digitalWrite(LIN_1,LOW);
          digitalWrite(LIN_2,HIGH);
          digitalWrite(RIN_1,HIGH);
          digitalWrite(RIN_2,LOW);
          cmd = ir_item;
          break;
        
        //前进
        case 0x75C0FF8:
        case 0x3A5F8139:
          analogWrite(PWM_L,spd);
          analogWrite(PWM_R,spd);
          digitalWrite(STBY,HIGH);
          digitalWrite(LIN_1,HIGH);
          digitalWrite(LIN_2,LOW);
          digitalWrite(RIN_1,LOW);
          digitalWrite(RIN_2,HIGH);
          cmd = ir_item;
          break;
          
        //加速
        case 0x98A027A0:
        case 0xB7C389AD:
          spd += 10;
          if(spd>spd_h){
            spd = spd_h;
          }
          analogWrite(PWM_L,spd);
          analogWrite(PWM_R,spd);
          break;
          
        //减速
        case 0x5280271:
        case 0x4B3BAEDC:
          spd -= 10;
          if(spd<spd_l){
            spd = spd_l;
          }
          analogWrite(PWM_L,spd);
          analogWrite(PWM_R,spd);
          break;
        //扭腰
        case 0xD3BE4D6C:
        case 0xD5F35A21:
          digitalWrite(STBY,HIGH);
          digitalWrite(LIN_1,HIGH);
          digitalWrite(LIN_2,LOW);
          digitalWrite(RIN_1,LOW);
          digitalWrite(RIN_2,HIGH);
          cmd = ir_item;
          break;
        //左前
        case 0xD60DBA24:
        case 0xAC5ED351:
          analogWrite(PWM_L,spd_l);
          analogWrite(PWM_R,spd_h);
          digitalWrite(STBY,HIGH);
          digitalWrite(LIN_1,HIGH);
          digitalWrite(LIN_2,LOW);
          digitalWrite(RIN_1,LOW);
          digitalWrite(RIN_2,HIGH);
          cmd = ir_item;
          break;
        //右前
        case 0x719DABA5:
        case 0xDEC605A8:
          analogWrite(PWM_L,spd_h);
          analogWrite(PWM_R,spd_l);
          digitalWrite(STBY,HIGH);
          digitalWrite(LIN_1,HIGH);
          digitalWrite(LIN_2,LOW);
          digitalWrite(RIN_1,LOW);
          digitalWrite(RIN_2,HIGH);
          cmd = ir_item;
          break;
      }
    }
    irrecv.resume();
  }
  if(ir_item==0xD3BE4D6C || ir_item==0xD5F35A21){
      if(wave<120){
          analogWrite(PWM_L,spd_h);
          analogWrite(PWM_R,spd_l);
      }else{
          analogWrite(PWM_L,spd_l);
          analogWrite(PWM_R,spd_h);
      }
      Serial.print(wave);
      wave ++;
      if(wave>=200){
        wave = 0;
      }
    }
}
