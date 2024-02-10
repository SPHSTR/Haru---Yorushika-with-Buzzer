#include <Arduino.h>

#define Buzzer 27
#define Button 18
#define LED 16 

double NOTEC_DO = 1047;
double NOTECs = 1109;
double NOTED_RE = 1175;
double NOTEEb = 1245;
double NOTEE_MI = 1319;
double NOTEF_FA = 1397;
double NOTEFs = 1480;
double NOTEG_SOL = 1568;
double NOTEGs = 1661;
double NOTEA_LA = 1760;
double NOTEBb = 1865;
double NOTEB_SI = 1976;
double NOTEB_SIL = 246.9;
double REST = 1;


String SongName = "[ Haru ] - Yorushika";
double NOTE[] = {NOTED_RE,  NOTEB_SI,REST,NOTEB_SI,NOTEA_LA,  NOTEA_LA,REST,NOTEF_FA,NOTEF_FA,  NOTEA_LA,NOTEF_FA,NOTEA_LA,NOTEB_SI,
              NOTEA_LA,REST,NOTEE_MI,NOTED_RE,  NOTEB_SIL,REST,NOTEF_FA,NOTEE_MI,  NOTEE_MI,NOTED_RE,NOTEE_MI,NOTEF_FA,
              NOTEF_FA,NOTEA_LA,  NOTEA_LA,REST,NOTED_RE,  NOTEB_SI,REST,NOTEB_SI,NOTEA_LA,  NOTEA_LA,REST,NOTEF_FA,
              NOTEA_LA,NOTEF_FA,NOTEA_LA,NOTEB_SI,  REST,NOTED_RE,NOTEE_MI,NOTEF_FA,  NOTEF_FA,NOTEG_SOL,NOTEA_LA,NOTEE_MI,
              NOTEE_MI,NOTED_RE,NOTEE_MI,NOTEE_MI,  NOTED_RE};

double BEAT[] = {2,  1.5,0.5,1,1,  1.5,0.5,1,1,  1,1,1,1,
              1,1,1,1,  1.5,0.5,1,1,  1,1,1,1,
              1,3,  1,1,2,  1.5,0.5,1,1,  1.5,0.5,2,  
              1,1,1,1,  1,1,1,1,  1,1,1,1,  
              1,1,1.5,0.5,  4};

hw_timer_t *My_timer = NULL;

void IRAM_ATTR onTimer(){
  digitalWrite(Buzzer, !digitalRead(Buzzer));
}

int Speedindicator = 0;
double Speed[] = {0.75 , 1 , 1.25 , 1.5 , 1.75};
void IRAM_ATTR ButtonPress(){
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 500){
  Speedindicator += 1;
  digitalWrite(LED, !digitalRead(LED));
  }
  last_interrupt_time = interrupt_time;
}

void setup() {
  Serial.begin(9600);
  pinMode(Buzzer,OUTPUT);
  pinMode(Button,INPUT);
  pinMode(LED,OUTPUT);

  attachInterrupt(Button, &ButtonPress, FALLING);

  My_timer = timerBegin(0, 80, true); 
  timerAttachInterrupt(My_timer, &onTimer, true);
}

double NOTEHZ;
double Duration;
int PlaySpeed;

void loop() {
  delay(1000);
  Serial.print("Now playing : ");
  Serial.println(SongName);

  int Size = sizeof(BEAT)/sizeof(BEAT[0]);
  for(int i = 0; i < Size ; i+=1){
    PlaySpeed = Speedindicator%(sizeof(Speed)/sizeof(Speed[0]));
    Serial.print(i+1);
    Serial.print("  Beat = ");
    Serial.print(BEAT[i]);
    Serial.print("  ");
    Serial.print("Speed = ");
    Serial.println((Speed[PlaySpeed]));
    NOTEHZ = 1000000/(2*NOTE[i]);
    Duration = (BEAT[i]*300)/((Speed[PlaySpeed]));

    timerAlarmWrite(My_timer, NOTEHZ, true);
    delay(Duration);

    timerAlarmWrite(My_timer, 800000, true);
    delay(Duration * 0.05);

    timerAlarmEnable(My_timer);
  }
  }
