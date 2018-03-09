#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include "MS5611.h"
#include "MS5611_I2C.h"

#define NOKIA_DC 10
#define NOKIA_CS 6
#define NOKIA_RST 11



//SOFTWARE SPI:
//Adafruit_PCD8544 display = Adafruit_PCD8544(SCK, MOSI, NOKIA_DC, NOKIA_CS, NOKIA_RST);


//HARDWARE SPI:
Adafruit_PCD8544 display = Adafruit_PCD8544(NOKIA_DC, NOKIA_CS, NOKIA_RST);
// Note with hardware SPI MISO and SS pins aren't used but will still be read
// and written to during SPI transfer.  Be careful sharing these pins!



#define MS5611_CSB                    13    // Chip/Slave Select Pin
#define D1_OSR                         5    // (Default pressure OSR mode 5) 
#define D2_OSR                         2    // (Default temperature OSR mode 2)

MS5611_SPI MS5611;
MS5611_I2C MS5611_I2C; 

float temperatureF = 0;


void setup()   {
  Serial.begin(115200);

  display.begin();
  display.setContrast(100);

  display.display(); // show splashscreen
  delay(2000);
  display.clearDisplay();   // clears the screen and buffer

  // draw a single pixel -- (to prove that the LCD is working) --
  display.drawPixel(10, 10, BLACK);
  display.display();
  delay(2000);
  display.clearDisplay();


  //MS5611.begin(MS5611_CSB);
  MS5611_I2C.begin();
  MS5611_I2C.setOversampling(MS5611_HIGH_RES);

}


void loop() {


      //====MS5611=================================================================/

        //TEMPERATURE:
        //temperatureF = MS5611.getTemperatureF(D2_OSR);
        temperatureF = MS5611_I2C.readTemperature(),


        //DEBUG:
        Serial.println(temperatureF); 


        
  
      //====NOKIA5110==============================================================/

        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(BLACK);
        display.setCursor(0,0);
        display.println(temperatureF);
        display.display();

        
}
