#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <math.h>
#include <SimpleKalmanFilter.h>

#define FLASH_MEMORY_SIZE 512
SimpleKalmanFilter locnhieu(10, 10, 0.1);
float zerotime =0;
float spantime =0;`
float zero;
float span; 
const int LEDPIN = 14; // Định nghĩa chân kết nối LED
const int BUZZER = 32;
const int LOADCELL_DOUT_PIN = 4;
int t=100;
int status = 0; 

LiquidCrystal_I2C lcd(0x27,16,2);

double weightFilt =0;
double weightfiltPre =0;


void blinkledandplaybuzzercalib()
{
  digitalWrite(LEDPIN,HIGH);
  digitalWrite(BUZZER,HIGH);
  delay(2000);
  digitalWrite(LEDPIN,LOW);
  digitalWrite(BUZZER,LOW);
}

void blinkledandplaybuzzerreset()
{
  digitalWrite(BUZZER,HIGH);
  digitalWrite(LEDPIN,HIGH);
  delay(1000);
  digitalWrite(LEDPIN,LOW);
  digitalWrite(BUZZER,LOW);
}

void blinkledandplaybuzzercalibzero()
{
  digitalWrite(LEDPIN,HIGH);
  digitalWrite(BUZZER,HIGH);
  delay(500);
  digitalWrite(LEDPIN,LOW);
  delay(500);
  digitalWrite(BUZZER,HIGH);
  delay(500);
  digitalWrite(LEDPIN,LOW);
  digitalWrite(BUZZER,LOW);
}

void blinkledandplaybuzzercalibspan()
{
  digitalWrite(LEDPIN,HIGH);
  digitalWrite(BUZZER,HIGH);
  delay(500);
  digitalWrite(LEDPIN,LOW);
  delay(500);
  digitalWrite(LEDPIN,HIGH);
  delay(500);
  digitalWrite(LEDPIN,LOW);
  delay(500);
  digitalWrite(LEDPIN,HIGH);
  delay(500);
  digitalWrite(LEDPIN,LOW);
  digitalWrite(BUZZER,LOW);
}

void setup() {
  Serial.begin(115200);
  
  pinMode(21, OUTPUT);
  pinMode(22, OUTPUT);
  lcd.init(); // initialize the lcd
  lcd.backlight();

  EEPROM.begin(FLASH_MEMORY_SIZE);
  pinMode(LEDPIN, OUTPUT);
  pinMode(BUZZER, OUTPUT); 
  digitalWrite(LEDPIN,LOW);
  digitalWrite(BUZZER,LOW);
  int EEPROM_address1 = 0;
  int EEPROM_address2 = sizeof(double);
  EEPROM.get(EEPROM_address1, zero);
  delay(100);
  EEPROM.get(EEPROM_address2, span);
  delay(100);
  //Print 
  lcd.setCursor(0, 0);  
          lcd.print("    LOADCELL    ");
          lcd.setCursor(0, 1);  
          lcd.print("     UA741      ");  
          delay(1500);
          lcd.clear();
          
  
          
          lcd.setCursor(0, 0);  
          lcd.print("Designed by:     ");
          delay(800);
          lcd.clear();
   
           lcd.setCursor(0, 0);lcd.print("  M             ");delay(t);
           lcd.setCursor(0, 0);lcd.print("  MA            ");delay(t);         
           lcd.setCursor(0, 0);lcd.print("  MAI           ");delay(t);
           lcd.setCursor(0, 0);lcd.print("  MAI T         ");delay(t);
           lcd.setCursor(0, 0);lcd.print("  MAI TH        ");delay(t);
           lcd.setCursor(0, 0);lcd.print("  MAI THA       ");delay(t);
           lcd.setCursor(0, 0);lcd.print("  MAI THAN      ");delay(t);
           lcd.setCursor(0, 0);lcd.print("  MAI THANH     ");delay(t);                                                  
           lcd.setCursor(0, 0);lcd.print("  MAI THANH D   ");delay(t);          
           lcd.setCursor(0, 0);lcd.print("  MAI THANH DA  ");delay(t); 
           lcd.setCursor(0, 0);lcd.print("  MAI THANH DA  ");delay(t);
           lcd.setCursor(0, 0);lcd.print("  MAI THANH DAT ");delay(t);         

           lcd.setCursor(0, 1);lcd.print("  NP            ");delay(t);
           lcd.setCursor(0, 1);lcd.print("  NP B          ");delay(t);
           lcd.setCursor(0, 1);lcd.print("  NP BA         ");delay(t);          
           lcd.setCursor(0, 1);lcd.print("  NP BAO        ");delay(t);          
           lcd.setCursor(0, 1);lcd.print("  NP BAO N      ");delay(t);          
           lcd.setCursor(0, 1);lcd.print("  NP BAO N      ");delay(t);          
           lcd.setCursor(0, 1);lcd.print("  NP BAO NG     ");delay(t);          
           lcd.setCursor(0, 1);lcd.print("  NP BAO NGU    ");delay(t);          
           lcd.setCursor(0, 1);lcd.print("  NP BAO NGUY   ");delay(t);
           lcd.setCursor(0, 1);lcd.print("  NP BAO NGUYEN ");delay(t);
           delay(500);
           lcd.clear();
           delay(500);
           lcd.clear();

}

void loop() {
  //receiver data
  String dulieu = "";
  while(Serial.available()>0){
    char c = Serial.read();
    dulieu += c;
    delay(5);
  }

  dulieu.trim();
  String command = dulieu.substring(0,5);
  String data = dulieu.substring(5);

  if(command == "CALIB"){
  int EEPROM_address1 = 0;
  int EEPROM_address2 = sizeof(double);
  EEPROM.get(EEPROM_address1, zero);
  delay(100);
  EEPROM.get(EEPROM_address2, span);
  delay(100);
  if((zero!=0)&&(span!=0)){
  blinkledandplaybuzzercalib();
  }
  };

  if(command == "RESET"){
    status = 1; 
    int EEPROM_address1 = 0;
    int EEPROM_address2 = sizeof(double);
    digitalWrite(LEDPIN, LOW);
    blinkledandplaybuzzerreset();
    zerotime = 0;
    spantime = 0;
    int address = 0;
    EEPROM.put(EEPROM_address1, zerotime);
    delay(100);
    EEPROM.commit();
    // Lưu số xxxxxx.x tại địa chỉ EEPROM 8 (kích thước double là 8 byte)
    EEPROM.put(EEPROM_address2, spantime);
    delay(100);
    EEPROM.commit();
  }

  if(command == "CZERO"){
    zerotime = data.toDouble();
    zero = zerotime;
    int EEPROM_address1 = 0;
    int EEPROM_address2 = sizeof(double);
    blinkledandplaybuzzercalibzero();
    int address = 0;
    EEPROM.put(EEPROM_address1, zerotime);
    delay(100);
    EEPROM.commit();
  }

    if(command == "CSPAN"){
    spantime = data.toDouble();
    span = spantime;
    int EEPROM_address1 = 0;
    int EEPROM_address2 = sizeof(double);
    blinkledandplaybuzzercalibspan();
    EEPROM.put(EEPROM_address2, spantime);
    delay(100);
    EEPROM.commit();
  }
    

    double reading = analogRead(LOADCELL_DOUT_PIN);
    weightFilt = reading;
    // weightFilt = 0.854*weightFilt + 0.0728*reading + 0.0728*weightfiltPre;
    weightFilt = locnhieu.updateEstimate(reading)*3.3/4095;  
    double weight = (weightFilt-zero)/span;  
    Serial.println(weightFilt, 5);                                                                     
    lcd.setCursor(0, 0);  
    lcd.print("  CAN DIEN TU  ");
    lcd.setCursor(11, 1);  
    lcd.print("KG");  
    lcd.setCursor(3, 1);  

    
    if(weight<0.01){
      lcd.print(0.00);
    }
    else if(weight > 1){
      lcd.print("inf0");
    }
    else{
      lcd.print(weight);
    }
    
  // print message
   delay(100);
}