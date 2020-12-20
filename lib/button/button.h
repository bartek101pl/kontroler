#if !defined(BUTTON)
#define BUTTON
#include <Arduino.h>
class button
{
private:
   int pin;
   String lable;
public:
    button(int pin,String Lable);
    ~button();
    bool getStatus();
    String getLable();
    void Init();
};





#endif // BUTTON
