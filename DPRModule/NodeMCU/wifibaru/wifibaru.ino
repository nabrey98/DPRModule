#include <ESP8266WiFi.h>                                                      //memasukkan library ESP8266WiFi.h
#include <Wire.h>                                                             //memasukkan library wire.h untuk pengaturan LCDI2C
#include <LiquidCrystal_I2C.h>                                                //memasukkan library LiquidCrystal_I2C.h
LiquidCrystal_I2C lcd(0x27,16,2);                                             //mendeklarasikan alamat LCD I2C
#include <FirebaseArduino.h>                                                  //memasukkan library FirebaseArduino.h

// setting firebase dan wifi
#define FIREBASE_HOST "korina-1.firebaseio.com"                               //copy dan paste url https://korina-1.firebaseio.com/ tanpa https://
#define FIREBASE_AUTH "TpEjUot2jOzFajlQw7c2x3f2rcxxcHbLngnTdjAm"              //copy dan paste kode rahasia databse yang ada pada menu setting > Akun layanan > Rahasia Database
#define WIFI_SSID "Sina-IoT1"                                                 //memasukkan nama wifi yang digunakan
#define WIFI_PASSWORD "sina615i"                                              //memasukkan password wifi yang digunakan

unsigned long curTime, oldTime, gantirt, gantidt, sisa, sisar, sisaa, sisab;  //mendeklarasikan variabel untuk penghitungan konversi waktu 
unsigned long getTime=6000;
int j, m, s, rts, rt, dt, dts, jr, mr, sr, ja, ma, sa, jb, mb, sb;            //mendeklarasikan variabel penempatan jam menit detik
String downa, b;                                                              //mendeklarasikan variabel untuk downtime dan runtime
int bufPPM;                                                                   //mendeklarasikan data ppm

void setup()
{
  Serial.begin(115200);

  //    connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");  
    delay(500);}

  delay(3000);                                                                //menunggu wifi tersambung selama 3 deik
  pinMode(D6, OUTPUT);                                                        //mendeklarasikan pin D6 sebagai output ke relay
  digitalWrite(D6, LOW);                                                      //perintah menyambungkan terminal 30A ke pin D3 pada relay
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); 

  //inisialisasi LCD 16x2
  lcd.init();
  lcd.backlight();
  lcd.clear();
  //inisialisasi pin D3 dan D4 sebagai input
  pinMode(D3, INPUT);
  digitalWrite(D3, HIGH);
  pinMode(D4, INPUT);
  digitalWrite(D4, HIGH);
}

void loop()
{   
  lcd.setCursor(0,0);
  lcd.print("PPM:");  
  
// Program Timer
  if(digitalRead(D3) == LOW)
  {
     
    unsigned long pb8 = millis();                                             //mendeklararikan variabel menjadi millis
    
    while(digitalRead(D4) == HIGH)
    {
      unsigned long pb9 = millis();//function to start recording when the button was pressed to print on the LCD      

 //////////////////////////////////////////////program downtime///////////////////////////////////////////
        //perhitungan jam
        dts = int((pb9 - pb8) / 1000);     
        ja = int(dts / 3600);
        //perhitungan menit
        sisaa = (dts - (3600*ja));
        ma = int(sisaa / 60);
        //perhitungan detik
        sisaa = (sisaa - (60*ma));
        sa = int(sisaa / 1);  
    
      lcd.setCursor(0, 1);
      downa = String(ja) + ":" + String(ma) + ":" + String(sa);
      lcd.print(String(downa) + ("  "));
      delay(1000);

      lcd.setCursor(8, 1);
      lcd.print("D");
      
      Firebase.setInt("Trigger",0);                                           //mengirim data 0 sebagai trigger agar NodeMCU tidak sleep saat downtime
    }
    lcd.setCursor(9, 1);                                                
    lcd.print(String(downa) + ("  "));                                        //menampilkan waktu downtime pada lcd

    Firebase.pushString("Pillowpack/downtime",downa);                         //mengirim data downtime ke firebase
   
  }
 

  if(digitalRead(D4) == LOW)//run
  {

      
    unsigned long pr8 = millis();
    //////////////////////////////////////////////Program RunTime////////////////////////////////////////////////////////////////////////////     
    while(digitalRead(D3) == HIGH)
    {
      
      unsigned long pr9 = millis();//function to start recording when the button was pressed to print on the LCD
      
      //perhitungan jam
      rts = int((pr9 - pr8) / 1000);
      jb = int(rts / 3600);
      //perhitungan menit
      sisab = (rts - (3600*jb));
      mb = int(sisab / 60);
      //perhitungan detik
      sisab = (sisab - (60*mb));
      sb = int(sisab / 1);
    
      delay(1000);

      b = String(jb) + ":" + String(mb) + ":" + String(sb);
   
      lcd.setCursor(8, 0);
      lcd.print("R");

      lcd.setCursor(0, 1);
      lcd.print(String(b) + ("    "));                                        
            

/////////////////////////////////////////Parsing data ppm dari UNO//////////////////////////////////////
    Firebase.setInt("Trigger",0);                                             //mengirim data 0 ke firebase

    bufPPM = Serial.parseInt();
     if(bufPPM >0){   
        Serial.println(bufPPM);
        lcd.setCursor(4,0);                                                  //menampilkan PPM pada lcd
        lcd.print("   ");
        lcd.setCursor(4,0);
        lcd.print(bufPPM);
      Firebase.pushInt("Pillowpack/PPM",bufPPM);                              //mengirim data runtime ke firebase
        delay(100);
      }       
    }
    Firebase.pushString("Pillowpack/runtime",b);                              //mengirim data runtime ke firebase
    lcd.setCursor(9, 0);
    lcd.print(String(b) + ("    "));                                          //menampilkan waktu runtime pada lcd

       
  }
   
}   
    
