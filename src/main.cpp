#include <Arduino.h>  
 
void setup() {
  Serial.begin(115200);  
  Serial.println("ADC Example - Agafia SG0+");
  analogReadResolution(12); // Set ADC resolution to 12 bits (0-4095)
}

 
 
void loop() {
  uint16_t adcValue = analogRead(A0); // Read ADC value from pin A0
  Serial.print("ADC Value: ");
  Serial.println(adcValue);  
}
