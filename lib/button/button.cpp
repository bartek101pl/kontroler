#include "button.h"

button::button(int pin,String Lable)
{
    this->pin = pin;
    this->lable = lable;
    
}

button::~button()
{
}
 bool button::getStatus(){
     return (digitalRead(this->pin) == LOW);
 }

String button::getLable(){
    return this->lable;
}
void button::Init(){
    pinMode(this->pin,INPUT_PULLUP);
}