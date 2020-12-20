#include "./analog.h"

analog::analog(uint8_t pinCodeX,uint8_t pinCodeY, uint8_t pinCodeZ,int blind)
{
    this->blind = blind;
    this->pinCodeX = pinCodeX;
    this->pinCodeY = pinCodeY;
    this->pinCodeZ = pinCodeZ;
}

analog::~analog()
{
}
void analog::init(config data){
this->configData =data;
pinMode(this->pinCodeX,INPUT);
pinMode(this->pinCodeY,INPUT);
pinMode(this->pinCodeZ,INPUT);
}
int analog::getX(){
return analogRead(this->pinCodeX);
}
int analog::getY(){
return analogRead(this->pinCodeY);
}
int analog::getZ(){
return analogRead(this->pinCodeZ);
}

int analog::getXAxis(){
int x= this->getX();

if(abs(x-this->configData.axcen)>this->blind){
    if(x>this->configData.axcen){
            return ((float)x-(float)this->configData.axcen)/((float)this->configData.axmax-(float)this->configData.axcen+this->blind)*(-100.00);
    }else
    {
        return ((float)this->configData.axcen - (float)x)/((float)this->configData.axcen - (float)this->blind -(float)this->configData.axmin)*100.00;
    }
    
}else
{
    return 0;
}

}
int analog::getYAxis(){
int y= this->getY();
if(abs(y-this->configData.aycen)>this->blind){
    if(y>this->configData.aycen){
            return ((float)y-(float)this->configData.aycen)/((float)this->configData.aymax-(float)this->configData.aycen+this->blind)*(-100.00);
    }else
    {
        return ((float)this->configData.aycen - (float)y)/((float)this->configData.aycen - (float)this->blind -(float)this->configData.aymin)*100.00;
    }
    
}else
{
    return 0;
}
}
int analog::getZAxis(){
int z= this->getZ();
if(abs(z-this->configData.azcen)>this->blindZ){
    if(z>this->configData.azcen){
            return ((float)z-(float)this->configData.azcen)/((float)this->configData.azmax-(float)this->configData.azcen+this->blindZ)*(-100.00);
    }else
    {
        return ((float)this->configData.azcen - (float)z)/((float)this->configData.azcen - (float)this->blindZ -(float)this->configData.azmin)*100.00;
    }
    
}else
{
    return 0;
}
}