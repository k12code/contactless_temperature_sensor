#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>

#define OLED_ADDR 0x3C

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void display(int x, int y, String text, bool clear = false, bool flush = false) {
  if (clear) {
    oled.clearDisplay();
  }
  oled.setCursor(x, y);
  oled.print(text);
  if (flush) {
    oled.display();
  }
}

String formatData(String prefix, float value, String suffix, int len = 6, int minWidth = 3, int precision = 1) {
  char chars[len];
  String result = prefix;

  dtostrf(value, minWidth, precision, chars);

  for (int i = 0; i < sizeof(chars); i++) {
    result += chars[i];
  }

  result += suffix;
  return result;
}

void setup() {
  oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  oled.setFont(&FreeSans9pt7b);
  //oled.setTextSize(1);
  oled.setTextColor(WHITE);

  mlx.begin();

  while (!Serial) { }

  Serial.println("Device Started.");

  display(0, 14, "Device started.", true, true);
  delay(2000);
}

void loop() {
  float target = mlx.readObjectTempC();
  float ambient = mlx.readAmbientTempC();

  oled.clearDisplay();
  display(0, 14, "Temperature", true, false);
  display(0, 38, formatData("obj:  ", target, " *C"), false, false);
  display(0, 61, formatData("env: ", ambient, " *C"), false, true);
}
