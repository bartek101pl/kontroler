#include <Arduino.h>

#include "../lib/analog/analog.h"
#include "../lib/button/button.h"

#include <EEPROM.h>
#include "../lib/RF24/RF24.h"

uint8_t getValue(bool data[8]);
uint8_t binaryV[8] = {128,64,32,16,8,4,2,1};
analog an1{A0,A1,A2,18};
analog an2{A3,A4,A5,70};
analog an3{A6,A7,A8,18};

button bg{29,"Button green"},
        by{33,"Button yellow"},
        br{31,"Button red"},
        w1{26,"Winch-> przełącznik góra"},
        w2{28,"Winch->przełącznik dół"},
        speed1{25,"Speed switch->1"},
        speed2{27,"Speed switch->2"},
        ONOFFL{24,"On/Off switch-> LEFT"},
        ONOFFR{23,"On/Off switch->RIGHT"};

struct senddata{
    int16_t a1x;
    int16_t a1y;
    int16_t a1z;
    int16_t a2x;
    int16_t a2y;
    int16_t a2z;
    int16_t a3x;
    int16_t a3y;
    int16_t a3z;
    uint8_t button;
    uint8_t code = 18;
};

config confA1,confA2,confA3;
senddata datax;
bool status = false,pin = false;

RF24 myRadio (7, 8);
byte addres[][6]={"stero","kont"};

void setup() {

    Serial1.begin(9600);
    pinMode(13,OUTPUT);
    digitalWrite(13,HIGH);

    bg.Init();
    br.Init();
    by.Init();
    w1.Init();
    w2.Init();
    speed1.Init();
    speed2.Init();
    ONOFFL.Init();
    ONOFFR.Init();


    EEPROM.get(0,confA1);
    EEPROM.get(sizeof(config),confA2);
    EEPROM.get((sizeof(config)*2),confA3);

    an1.init(confA1);
    an2.init(confA2);
    an3.init(confA3);

    pinMode(2,INPUT);

    if(! myRadio.begin())
        digitalWrite(13,LOW);

    myRadio.setDataRate(RF24_250KBPS);
    myRadio.setAutoAck(false);
    myRadio.setChannel(108);
    myRadio.setPALevel(RF24_PA_MAX);
    myRadio.openWritingPipe(addres[0]);

}

void loop() {

    if(digitalRead(2)==HIGH && !status)
    {
        pinMode(22,INPUT_PULLUP);
        Serial.println("Wykryto");
        delay(1000);
        Serial1.println("Polaczon!");
        Serial1.println("Podaj pin");
        status = true;
    }else if(digitalRead(2)==LOW && status){
        status = false;
        pin = false;
    }
    if(Serial1.available()&&digitalRead(2)==HIGH){
        if(pin==false){

            String cpin = Serial1.readString();

            if(cpin == "1234\n"){
                Serial1.println("Pin przyjety");
                pin = true;
            }else
            {
                Serial1.println("Bledny pin");
            }

        }else
        {
            String data = Serial1.readString();
            if(data == "analog status\n")
            {
                Serial1.println("------------------------------------------------------");
                Serial1.println("");
                Serial1.println("Status analogow : ");
                Serial1.print("analog 1: ");
                Serial1.print(an1.getX());
                Serial1.print(" | ");
                Serial1.print(an1.getY());
                Serial1.print(" | ");
                Serial1.print(an1.getZ());
                Serial1.print("\n");
                Serial1.print("analog 2: ");
                Serial1.print(an2.getX());
                Serial1.print(" | ");
                Serial1.print(an2.getY());
                Serial1.print(" | ");
                Serial1.print(an2.getZ());
                Serial1.print("\n");
                Serial1.print("analog 3: ");
                Serial1.print(an3.getX());
                Serial1.print(" | ");
                Serial1.print(an3.getY());
                Serial1.print(" | ");
                Serial1.print(an3.getZ());
                Serial1.print("\n");
                Serial1.print("\n");
                Serial1.println("------------------------------------------------------");
            }else if(data == "help\n"){
                Serial1.println("\n");
                Serial1.println("------------------------------------------------------");
                Serial1.println("HELP: ");
                Serial1.println("\n");
                Serial1.println("analog status");
                Serial1.println("analog new config");
                Serial1.println("analog config");
                Serial1.println("radio config");
                Serial1.println("radio new config");
                Serial1.println("logout");
                Serial1.println("------------------------------------------------------");
            }else if(data == "logout\n"){
                pin = false;
                status = false;
                Serial1.println("Wylogowano!");
            }else if(data == "analog new config\n"){
                config configA1,configA2,configA3;
                Serial1.println("Rozpoczynanie ustawiania nowych polozen analogow.");
                Serial1.println("\n");
                Serial1.println("------------------------------------------------------");
                Serial1.println("Przebieg: ");
                Serial1.println("Zostanie wyswietlony napis w którym bedzie podany numer analoga oraz w ktora strone nalezy go wychylic lub obrucic.");
                Serial1.println("Po wychyleniu nalezy wpisac komede \"next\"");
                Serial1.println("Przykladowo:");
                Serial1.println("Analog 1 wychyl do przodu");
                do{
                    while (!Serial1.available())
                    {
                        /* code */
                    }
                    data = Serial1.readString();
                }while (data != "next\n");
                Serial1.println("Poprawnie ");
                Serial1.println("Aby rozpoczac mapisz start");
                do{
                    while (!Serial1.available())
                    {
                        /* code */
                    }
                    data = Serial1.readString();
                }while (data != "start\n");
                Serial1.println("Rozpoczynamy");
                Serial1.println("\n");
                Serial1.println("Analog 1 pozycja srodkowa");
                do{
                    while (!Serial1.available())
                    {
                        /* code */
                    }
                    data = Serial1.readString();
                }while (data != "next\n");
                configA1.axcen = an1.getX();
                configA1.aycen = an1.getY();
                configA1.azcen = an1.getZ();
                Serial1.println("\n");
                Serial1.println("Analog 1 do przodu");
                do{
                    while (!Serial1.available())
                    {
                        /* code */
                    }
                    data = Serial1.readString();
                }while (data != "next\n");
                configA1.axmax = an1.getX();
                Serial1.println("\n");
                Serial1.println("Analog 1 do tylu");
                do{
                    while (!Serial1.available())
                    {
                        /* code */
                    }
                    data = Serial1.readString();
                }while (data != "next\n");
                configA1.axmin = an1.getX();
                Serial1.println("\n");
                Serial1.println("Analog 1 w lewo");
                do{
                    while (!Serial1.available())
                    {
                        /* code */
                    }
                    data = Serial1.readString();
                }while (data != "next\n");
                configA1.aymin = an1.getY();
                Serial1.println("\n");
                Serial1.println("Analog 1 w prawo");
                do{
                    while (!Serial1.available())
                    {
                        /* code */
                    }
                    data = Serial1.readString();
                }while (data != "next\n");
                configA1.aymax = an1.getY();
                Serial1.println("\n");
                Serial1.println("Analog 1 obrot w lewo");
                do{
                    while (!Serial1.available())
                    {
                        /* code */
                    }
                    data = Serial1.readString();
                }while (data != "next\n");
                configA1.azmax = an1.getZ();
                Serial1.println("\n");
                Serial1.println("Analog 1 obrot w prawo");
                do{
                    while (!Serial1.available())
                    {
                        /* code */
                    }
                    data = Serial1.readString();
                }while (data != "next\n");
                configA1.azmin = an1.getZ();
                //Analog 2
                Serial1.println("\n");
                Serial1.println("Analog 2 pozycja srodkowa");
                do{
                    while (!Serial1.available())
                    {
                        /* code */
                    }
                    data = Serial1.readString();
                }while (data != "next\n");
                configA2.axcen = an2.getX();
                configA2.aycen = an2.getY();
                configA2.azcen = an2.getZ();
                Serial1.println("\n");
                Serial1.println("Analog 2 do przodu");
                do{
                    while (!Serial1.available())
                    {
                        /* code */
                    }
                    data = Serial1.readString();
                }while (data != "next\n");
                configA2.axmax = an2.getX();
                Serial1.println("\n");
                Serial1.println("Analog 2 do tylu");
                do{
                    while (!Serial1.available())
                    {
                        /* code */
                    }
                    data = Serial1.readString();
                }while (data != "next\n");
                configA2.axmin = an2.getX();
                Serial1.println("\n");
                Serial1.println("Analog 2 w lewo");
                do{
                    while (!Serial1.available())
                    {
                        /* code */
                    }
                    data = Serial1.readString();
                }while (data != "next\n");
                configA2.aymin = an2.getY();
                Serial1.println("\n");
                Serial1.println("Analog 2 w prawo");
                do{
                    while (!Serial1.available())
                    {
                        /* code */
                    }
                    data = Serial1.readString();
                }while (data != "next\n");
                configA2.aymax = an2.getY();

                Serial1.println("\n");
                Serial1.println("Analog 2 obrot w lewo");
                do{
                    while (!Serial1.available())
                    {
                        /* code */
                    }
                    data = Serial1.readString();
                }while (data != "next\n");
                configA2.azmin = an2.getZ();
                Serial1.println("\n");
                Serial1.println("Analog 2 obrot w prawo");
                do{
                    while (!Serial1.available())
                    {
                        /* code */
                    }
                    data = Serial1.readString();
                }while (data != "next\n");
                configA2.azmax = an2.getZ();
                //analog 3
                Serial1.println("\n");
                Serial1.println("Analog 3 pozycja srodkowa");
                do{
                    while (!Serial1.available())
                    {
                        /* code */
                    }
                    data = Serial1.readString();
                }while (data != "next\n");
                configA3.axcen = an3.getX();
                configA3.aycen = an3.getY();
                configA3.azcen = an3.getZ();
                Serial1.println("\n");
                Serial1.println("Analog 3 do przodu");
                do{
                    while (!Serial1.available())
                    {
                        /* code */
                    }
                    data = Serial1.readString();
                }while (data != "next\n");
                configA3.axmax = an3.getX();
                Serial1.println("\n");
                Serial1.println("Analog 3 do tylu");
                do{
                    while (!Serial1.available())
                    {
                        /* code */
                    }
                    data = Serial1.readString();
                }while (data != "next\n");
                configA3.axmin = an3.getX();
                Serial1.println("\n");
                Serial1.println("Analog 3 w lewo");
                do{
                    while (!Serial1.available())
                    {
                        /* code */
                    }
                    data = Serial1.readString();
                }while (data != "next\n");
                configA3.aymin = an3.getY();
                Serial1.println("\n");
                Serial1.println("Analog 3 w prawo");
                do{
                    while (!Serial1.available())
                    {
                        /* code */
                    }
                    data = Serial1.readString();
                }while (data != "next\n");
                configA3.aymax = an3.getY();

                Serial1.println("\n");
                Serial1.println("Analog 3 obrot w lewo");
                do{
                    while (!Serial1.available())
                    {
                        /* code */
                    }
                    data = Serial1.readString();
                }while (data != "next\n");
                configA3.azmin = an3.getZ();
                Serial1.println("\n");
                Serial1.println("Analog 3 obrot w prawo");
                do{
                    while (!Serial1.available())
                    {
                        /* code */
                    }
                    data = Serial1.readString();
                }while (data != "next\n");
                configA3.azmax = an3.getZ();
                Serial1.println("\n");
                Serial1.println("Konfiguracja zakonczona!");
                Serial1.println("Nowe konfiguracje");
                Serial1.println("Analog 1:");
                Serial1.print("os x: ");
                Serial1.print(configA1.axmin);
                Serial1.print(" | ");
                Serial1.print(configA1.axcen);
                Serial1.print(" | ");
                Serial1.print(configA1.axmax);
                Serial1.print("\n");
                Serial1.print("os y: ");
                Serial1.print(configA1.aymin);
                Serial1.print(" | ");
                Serial1.print(configA1.aycen);
                Serial1.print(" | ");
                Serial1.print(configA1.aymax);
                Serial1.print("\n");
                Serial1.print("os z: ");
                Serial1.print(configA1.azmin);
                Serial1.print(" | ");
                Serial1.print(configA1.azcen);
                Serial1.print(" | ");
                Serial1.print(configA1.azmax);
                Serial1.print("\n");
                Serial1.print("\n");
                Serial1.println("Analog 2:");
                Serial1.print("os x: ");
                Serial1.print(configA2.axmin);
                Serial1.print(" | ");
                Serial1.print(configA2.axcen);
                Serial1.print(" | ");
                Serial1.print(configA2.axmax);
                Serial1.print("\n");
                Serial1.print("os y: ");
                Serial1.print(configA2.aymin);
                Serial1.print(" | ");
                Serial1.print(configA2.aycen);
                Serial1.print(" | ");
                Serial1.print(configA2.aymax);
                Serial1.print("\n");
                Serial1.print("os z: ");
                Serial1.print(configA2.azmin);
                Serial1.print(" | ");
                Serial1.print(configA2.azcen);
                Serial1.print(" | ");
                Serial1.print(configA2.azmax);
                Serial1.print("\n");
                Serial1.print("\n");
                Serial1.println("Analog 3:");
                Serial1.print("os x: ");
                Serial1.print(configA3.axmin);
                Serial1.print(" | ");
                Serial1.print(configA3.axcen);
                Serial1.print(" | ");
                Serial1.print(configA3.axmax);
                Serial1.print("\n");
                Serial1.print("os y: ");
                Serial1.print(configA3.aymin);
                Serial1.print(" | ");
                Serial1.print(configA3.aycen);
                Serial1.print(" | ");
                Serial1.print(configA3.aymax);
                Serial1.print("\n");
                Serial1.print("os z: ");
                Serial1.print(configA3.azmin);
                Serial1.print(" | ");
                Serial1.print(configA3.azcen);
                Serial1.print(" | ");
                Serial1.print(configA3.azmax);
                Serial1.print("\n");
                EEPROM.put(0,configA1);
                EEPROM.put(sizeof(config),configA2);
                EEPROM.put((sizeof(config)*2),configA3);
                Serial1.print("---------------------------------------------------------");
                Serial1.print("\n");
                Serial1.print("Uruchom ponownie kontroler aby zastosowac zmiany\n");
                Serial1.print("---------------------------------------------------------");
                Serial1.print("\n");
            }else if(data == "analog config\n"){
                Serial1.print("---------------------------------------------------------");
                Serial1.print("\n");
                Serial1.println("Obecna konfiguracja analogow");
                Serial1.println("Analog 1:");
                Serial1.print("os x: ");
                Serial1.print(confA1.axmin);
                Serial1.print(" | ");
                Serial1.print(confA1.axcen);
                Serial1.print(" | ");
                Serial1.print(confA1.axmax);
                Serial1.print("\n");
                Serial1.print("os y: ");
                Serial1.print(confA1.aymin);
                Serial1.print(" | ");
                Serial1.print(confA1.aycen);
                Serial1.print(" | ");
                Serial1.print(confA1.aymax);
                Serial1.print("\n");
                Serial1.print("os z: ");
                Serial1.print(confA1.azmin);
                Serial1.print(" | ");
                Serial1.print(confA1.azcen);
                Serial1.print(" | ");
                Serial1.print(confA1.azmax);
                Serial1.print("\n");
                Serial1.print("\n");
                Serial1.println("Analog 2:");
                Serial1.print("os x: ");
                Serial1.print(confA2.axmin);
                Serial1.print(" | ");
                Serial1.print(confA2.axcen);
                Serial1.print(" | ");
                Serial1.print(confA2.axmax);
                Serial1.print("\n");
                Serial1.print("os y: ");
                Serial1.print(confA2.aymin);
                Serial1.print(" | ");
                Serial1.print(confA2.aycen);
                Serial1.print(" | ");
                Serial1.print(confA2.aymax);
                Serial1.print("\n");
                Serial1.print("os z: ");
                Serial1.print(confA2.azmin);
                Serial1.print(" | ");
                Serial1.print(confA2.azcen);
                Serial1.print(" | ");
                Serial1.print(confA2.azmax);
                Serial1.print("\n");
                Serial1.print("\n");
                Serial1.println("Analog 3:");
                Serial1.print("os x: ");
                Serial1.print(confA3.axmin);
                Serial1.print(" | ");
                Serial1.print(confA3.axcen);
                Serial1.print(" | ");
                Serial1.print(confA3.axmax);
                Serial1.print("\n");
                Serial1.print("os y: ");
                Serial1.print(confA3.aymin);
                Serial1.print(" | ");
                Serial1.print(confA3.aycen);
                Serial1.print(" | ");
                Serial1.print(confA3.aymax);
                Serial1.print("\n");
                Serial1.print("os z: ");
                Serial1.print(confA3.azmin);
                Serial1.print(" | ");
                Serial1.print(confA3.azcen);
                Serial1.print(" | ");
                Serial1.print(confA3.azmax);
                Serial1.print("\n");
                Serial1.print("---------------------------------------\n");
            }
            else{
                Serial1.println("Nie rozpoznana komda : "+data);
            }
        }

    }

    datax.a1x = (speed2.getStatus())?(an1.getXAxis()/2):an1.getXAxis();
    datax.a1y = (speed2.getStatus())?(an1.getYAxis()/2):an1.getYAxis();
    datax.a1z = -(speed2.getStatus())?(an1.getZAxis()/2):an1.getZAxis();
    datax.a2x = (speed2.getStatus())?(an2.getXAxis()/2):an2.getXAxis();
    datax.a2y = (speed2.getStatus())?(an2.getYAxis()/2):an2.getYAxis();
    datax.a2z = (speed2.getStatus())?(an2.getZAxis()/2):an2.getZAxis();
    datax.a3x = (speed2.getStatus())?(an3.getXAxis()/2):an3.getXAxis();
    datax.a3y = (speed2.getStatus())?(an3.getYAxis()/2):an3.getYAxis();
    datax.a3z = (speed2.getStatus())?(an3.getZAxis()/2):an3.getZAxis();

    bool h2 = false;

    if(speed2.getStatus() == LOW && speed1.getStatus() == true)
        h2 = true;

    bool h[8] ={bg.getStatus(),br.getStatus(),by.getStatus(),w1.getStatus(),w2.getStatus(),ONOFFL.getStatus(),!ONOFFR.getStatus(),h2};

    datax.button = getValue(h);
    datax.code = 31;


    myRadio.writeFast(&datax,sizeof(datax));
    delay(50);

}

uint8_t getValue(bool data[8]){
    uint8_t parse = 0;

    for(short i =0;i<8;i++){
        parse+=(data[i]*binaryV[i]);
    }

    return parse;
}