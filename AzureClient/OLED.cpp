#include "OLED.h"


void OLED::updateDisplay(){
  display.clearDisplay();

  // text display tests
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("T:");
  display.println(_sensor->temperature);
  display.print("P:");
  display.println(_sensor->pressure);
  display.print("H:");
  display.println(_sensor->humidity);
   display.print("M:");
  display.println(ESP.getFreeHeap());
     display.print("I:");
  display.println(_sensor->msgId);

  display.display();
}
