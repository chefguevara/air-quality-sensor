#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>



// TFT <-> NodeMCU pinout
#define TFT_CS         D8
#define TFT_RST        D4                                            
#define TFT_DC         D3
#define TFT_MOSI       D6 
#define TFT_SCLK       D5

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// Color Theme
#define COLOR2 ST7735_WHITE
#define COLOR1 ST7735_BLACK

int text_color_air_quality;

void drawTable() {
  // Table
  tft.fillScreen(ST77XX_WHITE);
  tft.drawRect(0, 0, 127, 159, COLOR1);
  tft.drawLine(0, 50, 127, 50, COLOR1);
  tft.drawLine(0, 100, 127, 100, COLOR1);
}

void setupLCD(){
    tft.initR(INITR_GREENTAB);      // Init ST7735S chip, black tab
    delay(3000);
    drawTable();
}



// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t Color565(uint8_t r, uint8_t g, uint8_t b)
{
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

void fix_number_position(float number)
{
  if ((number >= -40) && (number < -9.9)) {
      ;
  }

  if ((number >= -9.9) && (number < 0.0)) tft.print(" ");

  if ((number >= 0.0) && (number < 9.9)) tft.print(" ");

  if ((number >= 9.9) && (number < 99.9)) tft.print(" ");
  
  if ((number >= 99.9) && (number < 151))  tft.print("");
}


void get_air_quality_color_warning_message(float score)
{
    if ((score >= 00) && (score < 50)) {
        text_color_air_quality = Color565(0, 137, 0); // dark green
    } 

    if ((score >= 51) && (score < 100)){
        text_color_air_quality = Color565(76, 255, 0); // light green
    }

    if ((score >= 101) && (score < 150)){
        text_color_air_quality = Color565(255, 255, 0); // yellow
    } 

    if ((score >= 151) && (score < 200)){
        text_color_air_quality = Color565(255, 140, 0); // light orange
    } 

    if ((score >= 201) && (score < 300)){
        text_color_air_quality = Color565(221, 128, 0); // dark orange
    } 

    if ((score >= 300)){
        text_color_air_quality = Color565(255, 0, 0); // red
    } 
}

void testdrawtext(char* text, uint16_t color)
{
    tft.setCursor(0, 0);
    tft.setTextColor(color);
    tft.setTextWrap(true);
    tft.print(text);
}

String CalculateIAQ(float score) {
  if      (score >= 301)                  return "Out!!";
  else if (score >= 201 && score <= 300 ) return "Worse";
  else if (score >= 151 && score <= 200 ) return "Bad";
  else if (score >= 101 && score <= 150 ) return "Poor";
  else if (score >=  51 && score <= 100 ) return "Avg";
  else if (score >=  00 && score <=  50 ) return "Good";
  return "--";
}

void temperature_to_lcd(float temperature_c, unsigned char text_position){
  tft.setCursor(4, text_position);
  tft.setTextColor(ST7735_BLACK, COLOR2);
  tft.setTextSize(1);

  int text_color = Color565(255, 50, 0);
  
  if(temperature_c > 0) {
    text_color = Color565(0, 50, 255);
  }

  tft.print("Temperature:");
  tft.setTextSize(3);

  tft.setCursor(1, text_position + 20);
  fix_number_position(temperature_c);
  tft.setTextColor(text_color, COLOR2);
  
  tft.print(temperature_c, 1);
  tft.setCursor(108, text_position + 20);
  tft.print("C");
  tft.drawChar(90, text_position + 20, 247, text_color, COLOR2, 2); //degree symbol
}

//outputs humidity to LCD

void humidity_to_lcd(float humidity_value, unsigned char text_position)
{
  tft.setCursor(4, text_position);
  tft.setTextColor(ST7735_BLACK, COLOR2);
  tft.setTextSize(1);

  tft.println("Humidity:");
  tft.setTextSize(3);

  tft.setCursor(1, text_position + 20);
  fix_number_position(humidity_value);
  
  tft.print(humidity_value, 1);
  tft.print(" %");
}


void gas_to_lcd(float air_quality_score, unsigned char text_position)
{
  get_air_quality_color_warning_message(air_quality_score); //text_color_air_quality
  
  tft.setCursor(4, text_position);
  tft.setTextColor(text_color_air_quality, COLOR2);
  tft.setTextSize(1);
  
  tft.println("Air quality:");
  tft.setTextSize(3);
  
  tft.setCursor(20, text_position + 20);
  tft.print(CalculateIAQ(air_quality_score));
}
