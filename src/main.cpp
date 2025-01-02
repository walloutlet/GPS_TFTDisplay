#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <TinyGPSPlus.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include <ui.h>
#include "config.h"

#define DRAW_BUF_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT / 10 * (LV_COLOR_DEPTH / 16))

uint32_t draw_buf[DRAW_BUF_SIZE / 4];

// Structure to hold GPS data
struct GPSData {
    float speed;
    float latitude;
    float longitude;
    int sats;
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
};

GPSData gpsData;
TinyGPSPlus gps;                                         // Initialize the GPS object
TFT_eSPI tft = TFT_eSPI();                               // Initialize the TFT display object

bool satFix = false;

#if LV_USE_LOG != 0
void lv_print_logs( lv_log_level_t level, const char * buf )
{
    LV_UNUSED(level);
    Serial.println(buf);
    Serial.flush();
}
#endif

/**
 * @brief Flushes a rectangular area of the display with a single color.
 * 
 * This function is used to fill a specified area of the display with a single color.
 * 
 * @param disp    Pointer to the LVGL display.
 * @param area    Pointer to the rectangular area to be flushed. The area is defined by its top-left (x1, y1) and bottom-right (x2, y2) coordinates.
 * @param color_p Pointer to the color to be used for flushing. The color is represented by an lv_color_t structure.
 */
void display_flush(lv_display_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)&color_p, w * h, true);
  tft.endWrite();

  lv_display_flush_ready(disp);

}

static uint32_t lvgl_tick(void) {
    return millis();
}

// Task function to read GPS data
void gpsReaderTask(void *pvParameters) {
    for (;;) {
        while (Serial2.available()) {
            gps.encode(Serial2.read());
        }
        
        // Update GPS data structure
        if (gps.satellites.isValid()) {
            gpsData.sats = gps.satellites.value();
        }
        if (gps.speed.isValid()) {
            gpsData.speed = gps.speed.mph();
        }
        if (gps.location.isValid()) {
            gpsData.latitude = gps.location.lat();
            gpsData.longitude = gps.location.lng();
        }
        if (gps.date.isValid()) {
            gpsData.year = gps.date.year();
            gpsData.month = gps.date.month();
            gpsData.day = gps.date.day();
        }
        if (gps.time.isValid()) {
            gpsData.hour = gps.time.hour();
            gpsData.minute = gps.time.minute();
            gpsData.second = gps.time.second();
        }
        
        vTaskDelay(100 / portTICK_RATE_MS); // delay 100ms
    }
}

/**
 * @brief Prints GPS data as text to the Serial Monitor.
 * 
 * @param gps The TinyGPSPlus object containing the GPS data.
 */
void printTextGPSData_Serial(TinyGPSPlus gps) {
  char dateBuffer[11];
  char timeBuffer[9];

  snprintf(dateBuffer, sizeof(dateBuffer), "%02d/%02d/%04d", gps.date.month(), gps.date.day(), gps.date.year());
  snprintf(timeBuffer, sizeof(timeBuffer), "%02d:%02d:%02d", gps.time.hour(), gps.time.minute(), gps.time.second());

  Serial.println("GPS Data: ");
  Serial.print("Latitude: ");
  Serial.println(gps.location.lat(), 6);
  Serial.print("Longitude: ");
  Serial.println(gps.location.lng(), 6);
  Serial.print("Altitude: ");
  Serial.print(gps.altitude.meters());
  Serial.println(" meters");
  Serial.print("Speed: ");
  Serial.print((int)gps.speed.mph());
  Serial.println(" mph");
  Serial.print("Course: ");
  Serial.println(gps.course.deg());
  Serial.print("Satellites: ");
  Serial.println(gps.satellites.value());
  Serial.print("Date: ");
  Serial.println(dateBuffer);
  Serial.print("Time (UTC): ");
  Serial.println(timeBuffer);
  Serial.println();
}

/**
 * @brief Prints GPS data as text to the TFT display.
 * 
 * @param gps The TinyGPSPlus object containing the GPS data.
 */
void printTextGPSData_TFT(TinyGPSPlus gps) {
  static u_int8_t fontType = 1;
  static u_int8_t lineSpacing = 2;
  int16_t x_pos = 0;
  int16_t y_pos = 90;
  char dateBuffer[11];
  char timeBuffer[9];

  // Format the time string into 24 hour clock and US date format
  snprintf(dateBuffer, sizeof(dateBuffer), "%02d/%02d/%04d", gps.date.month(), gps.date.day(), gps.date.year());
  snprintf(timeBuffer, sizeof(timeBuffer), "%02d:%02d:%02d", gps.time.hour(), gps.time.minute(), gps.time.second());

  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  x_pos = (SCREEN_WIDTH - tft.textWidth(" GPS Time: " + String(timeBuffer) + " UTC")) / 2;
  tft.setCursor(x_pos, y_pos, fontType);
  x_pos = tft.getCursorX();
  tft.println(" GPS Time: " + String(timeBuffer) + " UTC");
  tft.setCursor(x_pos, tft.getCursorY() + lineSpacing, fontType);
  tft.println(" GPS Date: " + String(dateBuffer));
  tft.setCursor(x_pos, tft.getCursorY() + lineSpacing, fontType);
  tft.println("GPS Speed:" + String(gps.speed.mph(), 0) + " mph");
  tft.setCursor(x_pos, tft.getCursorY() + lineSpacing, fontType);
  tft.println(" Latitude: " + String(gps.location.lat(), 6));
  tft.setCursor(x_pos, tft.getCursorY() + lineSpacing, fontType);
  tft.println("Longitude: " + String(gps.location.lng(), 6));
  tft.setCursor(x_pos, tft.getCursorY() + lineSpacing, fontType);
  tft.println("# of Satellites: " + String(gps.satellites.value()));
}

const char* directionHeading(TinyGPSPlus gps) {
  // Determine Direction based on Heading
  const char* directions[] = {"N", "NNE", "NE", "ENE", "E", "ESE", "SE", "SSE", "S", "SSW", "SW", "WSW", "W", "WNW", "NW", "NNW"};
  int index = (int) ((gps.course.deg() / 22.5) + 0.5) % 16;

  return directions[index];
}

void IRAM_ATTR onTimer() {
    lv_tick_inc(1); // Increment LVGL's tick by 1ms
}

int counter() {
    static int count = 0;
    static bool increasing = true;

    if (increasing) {
        if (count < 140) {
            return count++;
        } else {
            increasing = false;
            return count;
        }
    } else {
        if (count > 0) {
            return count--;
        } else {
            increasing = true;
            return count;
        }
    }
}

void setup() {
  Serial.begin(SERIAL_BAUDRATE);
  Serial2.begin(GPS_BAUDRATE);
  
  xTaskCreate(gpsReaderTask, "GPS Reader Task", 2048, NULL, 1, NULL);

  // Initliaze the TFT display
  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true);

  // Initialize LVGL
  lv_init();

  // Set a custom tick source so that LVGL will know how much time elapsed.
  lv_tick_set_cb(lvgl_tick);

  // Register print function for debugging
//  #if LV_USE_LOG != 0
//    lv_log_register_print_cb( lv_print_logs );
//  #endif

  lv_display_t * disp;
  disp = lv_tft_espi_create(SCREEN_WIDTH, SCREEN_HEIGHT, draw_buf, sizeof(draw_buf));
  lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_0);
  lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), LV_STATE_DEFAULT);

  // Initialize the UI library from SquareLine
  ui_init();

  // Wait for all hardware to finish initializing
  delay(100);
}

void loop() {
  char dateBuffer[11];
  char timeBuffer[9];

  // Let the LVGL GUI do its work and run every 5 ms
  lv_timer_handler();
//  delay(5);

  // Read data from the GPS module using TinyGPSPlus
//  Serial.print("Serial2 bytes available: ");
//  Serial.println(Serial2.available());
//  while (Serial2.available() > 0) {
//    gps.encode(Serial2.read());
//  }

  // Output the GPS data to the Serial Monitor
//  printTextGPSData_Serial(gps);

  // Output the GPS data to the TFT display
  // printTextGPSData_TFT(gps);
// You can access gpsData here
//  Serial.println("GPS Sats: " + String(gpsData.sats));
//  Serial.println("GPS Speed: " + String((int)gpsData.speed) + " mph");
//  Serial.println("GPS Coordinates: " + String(gpsData.latitude) + ", " + String(gpsData.longitude));
//  Serial.println("GPS Date and Time: " + String(gpsData.year) + "-" + String(gpsData.month) + "-" + String(gpsData.day) + " " + String(gpsData.hour) + ":" + String(gpsData.minute) + ":" + String(gpsData.second));

  // Format the time string into 24 hour clock and US date format
  int adjustedHour = gpsData.hour + TZ_OFFSET;
  if (adjustedHour < 0) {
      adjustedHour += 24;
  }
  snprintf(dateBuffer, sizeof(dateBuffer), "%02d/%02d/%04d", gpsData.month, gpsData.day, gpsData.year);
  snprintf(timeBuffer, sizeof(timeBuffer), "%02d:%02d:%02d", adjustedHour, gpsData.minute, gpsData.second);
//  snprintf(timeBuffer, sizeof(timeBuffer), "%02d:%02d", adjustedHour, gps.time.minute());

  lv_label_set_text(ui_Date, dateBuffer);
  lv_label_set_text(ui_Time, timeBuffer);

  if(gps.sentencesWithFix() > 0 && satFix == false) {
    lv_obj_clear_flag(ui_SatImg, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_SatNumBackGround, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_SatNum, LV_OBJ_FLAG_HIDDEN);
    satFix = true;
//    lv_obj_set_style_bg_color(ui_textSpeed, lv_color_hex(0x009427), LV_PART_MAIN);
//    lv_obj_set_style_bg_color(ui_textHeading, lv_color_hex(0x009427), LV_PART_MAIN);
//    lv_obj_set_style_bg_color(ui_labelSats, lv_color_hex(0x323136), LV_PART_MAIN);
  } 

  static char bufSats[16];
  std::snprintf(bufSats, sizeof(bufSats), "%02d", gpsData.sats);
  lv_label_set_text(ui_SatNum, bufSats);

  static char bufSpeed[16];
  std::snprintf(bufSpeed, sizeof(bufSpeed), "%03d", (int)gpsData.speed);
//  std::snprintf(bufSpeed, sizeof(bufSpeed), "%03d", counter());
  lv_label_set_text(ui_SpeedNum, bufSpeed);
//  lv_arc_set_value(ui_SpeedArc, counter());

//  lv_label_set_text(ui_Heading, directionHeading(gps));

  // Set refresh rate
  delay(REFRESH_RATE);

}