#include <Arduino.h>
#include <Preferences.h>
#include <LC76G.h>
#include <TinyGPS++.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include <ui.h>
#include "config.h"

// Define the LVGL screen buffer size
#define DRAW_BUF_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT / 10 * (LV_COLOR_DEPTH / 16))

// Create a Preferences object
Preferences prefs;

// Initialize the TFT object
TFT_eSPI tft = TFT_eSPI();

// Initialize the LC76G GPS hardware
LC76G lc76g;

// Initialize GPS object
TinyGPSPlus gps;

//lvgl draw buffer
uint32_t draw_buf[DRAW_BUF_SIZE / 4];

// Variable to store the last PPS time
volatile bool ppsTriggered = false;  // Flag to indicate PPS signal
volatile unsigned long ppsTime = 0;  // Time when PPS signal was received
volatile bool sysClockSet = false;   // System clock set flag
unsigned long lastSyncTime = 0;      // Add a variable to store the last time the system clock was synchronized

// State variables
bool gpsSignalAcquired = false;
volatile float currentSpeed = 888; // Initial speed value
volatile bool newSpeedAvailable = false; // Flag to indicate new speed data

// State variables for number of satelittes
volatile bool newSatNumAvailable = false; // Flag to indicate new satelitte number data
volatile int currentSats; // Buffer to store the current satelitte number

// States variables to store the last latitude and longitude
volatile bool newLatCoordAvailable = false;
volatile bool newLngCoordAvailable = false;
volatile float currentLat = 0.0;
volatile float currentLng = 0.0;
float lastLatitude = 0.0;
float lastLongitude = 0.0;

// State variables for heading
volatile bool newHeadingAvailable = false; // Flag to indicate new heading data
char currentHeading[4]; // Buffer to store the current heading data

// State variable for GPS timer fire
volatile bool gpsReady = false;

// Buffers to hold the formatted date and time strings from system clock
char sysDateBuffer[11]; // YYYY/MM/DD + null terminator
char sysTimeBuffer[9];  // HH:MM:SS + null terminator

// Speed Smoothing variables
bool enableSmoothing = true;  // Flag to enable or disable smoothing
const int numReadings = 5;    // Number of readings to store, default: 5 (smaller = more responsive to changes in speed but jumpy / larger smoother but less responsive)
float readings[numReadings];  // Readings from the analog input
int readIndex = 0;            // Index of the current reading
float total = 0;              // Running total
float average = 0;            // The average

// Variable to store the last time the button was pressed
unsigned long lastDebounceTime = 0;

// Timer interrupt variables
hw_timer_t *timer0 = NULL;
hw_timer_t *timer1 = NULL;

void processSatNum(void) {
  if (gps.satellites.isValid()) {
    int sats = gps.satellites.value();

    if (sats != (int)currentSats) {
      currentSats = sats;
      newSatNumAvailable = true;
    }
  }
}

void processCoords(void) {
  if (gps.satellites.isValid()) {
    float lat = gps.location.lat();
    float lng = gps.location.lng();

    if (lat != currentLat) {
      currentLat = lat;
      newLatCoordAvailable = true;
    }
    if (lng != currentLng) {
      currentLng = lng;
      newLngCoordAvailable = true;
    }
  }
}

void processHeading(void) {
  if (gps.satellites.isValid()) {
    float heading = gps.course.deg();
    const char* directions[] = {"N", "NNE", "NE", "ENE", "E", "ESE", "SE", "SSE", "S", "SSW", "SW", "WSW", "W", "WNW", "NW", "NNW"};
    int index = (int)((heading / 22.5) + 0.5) % 16;

    if (directions[index] != currentHeading) {
      strncpy(currentHeading, directions[index], sizeof(currentHeading) - 1);
      currentHeading[sizeof(currentHeading) - 1] = '\0'; // Ensure null-termination
      newHeadingAvailable = true;
    }
  }
}

void processGPSSpeed(void) {
  if (gps.speed.isValid()) {
    float speed = gps.speed.mph();

    if (enableSmoothing) {
      // Smooth the speed data using a simple moving average function
      total = total - readings[readIndex];
      readings[readIndex] = speed;
      total = total + readings[readIndex];
      readIndex = (readIndex + 1) % numReadings;
      average = total / numReadings;
      // Round the average to the nearest whole number
      average = round(average);
    }
    else {
      average = speed;
    }

    // Catch any GPS data corruption, set speed to 0 if a negative number is calculated
    if (average < 0) {
      average = 0.0f;
    }

    // Update the speed value if it has changed
    if (average != currentSpeed) {
      currentSpeed = average;
      newSpeedAvailable = true;
    }
  }
}

void checkBootButton() {
  // Check if the button is pressed (LOW) and if enough time has passed since the last press
  if (digitalRead(BOOT_BUTTON_PIN) == LOW && (millis() - lastDebounceTime) > DEBOUNCE_TIME) {
    // Update the last debounce time
    lastDebounceTime = millis();

    // Toggle the enableSmoothing flag
    enableSmoothing = !enableSmoothing;
    if (enableSmoothing) {
      lv_obj_remove_flag(ui_SmoothingLabel, LV_OBJ_FLAG_HIDDEN);
    } else {
      lv_obj_add_flag(ui_SmoothingLabel, LV_OBJ_FLAG_HIDDEN);
    }

    #ifdef DEBUG
      Serial.print("[DEBUG] Button Press Detected! Smoothing is now ");
      Serial.println(enableSmoothing ? "Enabled" : "Disabled");
      delay(1000);
    #endif
  }
}

// Function to update the LVGL labels with the current date and time
void formatSysDateTime() {
    time_t rawtime;
    struct tm *timeinfo;

    // Get the current time
    time(&rawtime);

    // Adjust for the time zone offset
    rawtime += TZ_OFFSET * 3600;

    // Convert to local time
    timeinfo = localtime(&rawtime);

    // Format the date as 'YYYY/MM/DD'
    strftime(sysDateBuffer, sizeof(sysDateBuffer), "%Y/%m/%d", timeinfo);

    // Format the time as 'HH:MM:SS'
    strftime(sysTimeBuffer, sizeof(sysTimeBuffer), "%H:%M:%S", timeinfo);
}

void setSysTime() {
  // Check if the GPS time is valid
  if (gps.time.isValid() && gps.date.isValid()) {
    // Create a struct tm to hold the time components
    struct tm tm;
    tm.tm_year = gps.date.year() - 1900;        // Years since 1900
    tm.tm_mon = gps.date.month() - 1;           // Months since January (0-11)
    tm.tm_mday = gps.date.day();                // Day of the month (1-31)
    tm.tm_hour = gps.time.hour();               // Hours since midnight (0-23)
    tm.tm_min = gps.time.minute();              // Minutes after the hour (0-59)
    tm.tm_sec = gps.time.second();              // Seconds after the minute (0-59)

    // Convert struct tm to time_t (seconds since the Unix epoch)
    time_t t = mktime(&tm);

    // Adjust for latency between PPS signal and time update
    unsigned long currentTime = millis();
    unsigned long latency = currentTime - ppsTime;  // Latency in milliseconds
    t += (latency / 1000);                          // Add latency in seconds

    // Create a struct timeval to hold the time in seconds and microseconds
    struct timeval tv;
    tv.tv_sec = t;
    tv.tv_usec = (latency % 1000) * 1000;           // Add remaining microseconds

    // Set the system clock
    if (settimeofday(&tv, NULL) == 0) {
        sysClockSet = true;
        lastSyncTime = millis();  // Record the time when the clock was last synchronized
        formatSysDateTime();
        #ifdef DEBUG
          Serial.println("[DEBUG] System clock set successfully.");
          delay(5000);
        #endif
    } else {
        #ifdef DEBUG
          Serial.println("[DEBUG] Failed to set system clock.");
          delay(5000);
        #endif
    }
  } else {
      #ifdef DEBUG
        Serial.println("[DEBUG] Waiting for valid GPS time...");
        delay(1000);
      #endif
  }
}

void processGPSSignalLoss() {
  gpsSignalAcquired = false;
  lv_obj_add_flag(ui_SatImg, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(ui_SatNumBackGround, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(ui_SatNum, LV_OBJ_FLAG_HIDDEN);

  currentSpeed = 0;
  strncpy(currentHeading, "---", sizeof(currentHeading) - 1);
  currentHeading[sizeof(currentHeading) - 1] = '\0'; // Ensure null-termination

  newSatNumAvailable = true;
  newSpeedAvailable = true;
  newHeadingAvailable = true;

  #ifdef DEBUG
    Serial.println("[DEBUG] No GPS Signal!");
    Serial.println("[DEBUG] Pausing for 5 seconds.");
    delay(5000);
  #endif
}

// Function to save latitude and longitude to Preferences
void saveCoordinates(float lat, float lng) {
  prefs.putFloat("lat", lat);  // Save latitude
  prefs.putFloat("lng", lng);  // Save longitude
}

// Function to load latitude and longitude from Preferences
void loadCoordinates() {
  lastLatitude = prefs.getFloat("lat", 0.0);   // Load latitude (default 0.0 if not found)
  lastLongitude = prefs.getFloat("lng", 0.0);  // Load longitude (default 0.0 if not found)
}

void display_flush(lv_display_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)&color_p, w * h, true);
  tft.endWrite();

  lv_display_flush_ready(disp);
}

// Interrupt Service Routine (ISR) for LVGL screen update (Timer 0)
void IRAM_ATTR lvglISR() {
  lv_tick_inc(SCREEN_REFRESH_RATE);  // Convert microseconds to milliseconds and update lvgl tick clock
}

// Interrupt Service Routine (ISR) for GPS data update (Timer 1)
void IRAM_ATTR gpsISR() {
  gpsReady = true;
}

// Interrupt Service Routine (ISR) for the PPS signal
void IRAM_ATTR ppsISR() {
  ppsTriggered = true;
  ppsTime = millis();
}

void setup() {
  #ifdef DEBUG
    // Initialize Serial for debugging
    Serial.begin(SERIAL_BAUDRATE);
  #endif

  // Initialize Preferences
  prefs.begin("gps_data", false);  // Open the "gps_data" namespace in read/write mode

  // Load the last saved coordinates
  loadCoordinates();

  #ifdef DEBUG
    Serial.print("[DEBUG] Stored Lat: ");
    Serial.print(lastLatitude, 6);
    Serial.print(" Stored Lng: ");
    Serial.println(lastLongitude, 6);
    delay(2000);
  #endif

  // Initialize TFT Display
  tft.begin();
  tft.setRotation(SCREEN_ROTATION); // Set rotation to 0 degrees
  tft.fillScreen(TFT_BLACK);

  // Initialize the GPS hardware
  lc76g.begin(GPS_RX, SERIAL_8N1, GPS_TX, GPS_BAUDRATE);
  if (lc76g.setUpdateRate(GPS_REFRESH_RATE)) {
    #ifdef DEBUG
      Serial.println("[DEBUG] Successfully set LC76G GPS update rate to " + String(GPS_REFRESH_RATE) + " ms");
      delay(5000);
    #endif
  } else {
    #ifdef DEBUG
      Serial.println("[DEBUG] Failed to set LC76G GPS update rate");
      delay(5000);
    #endif
  }

  // Configure the PPS pin as input
  pinMode(GPS_PPS, INPUT);

  // Initialize the boot button pin
  pinMode(BOOT_BUTTON_PIN, INPUT_PULLUP);

  // Attach the interrupt to the PPS pin
  attachInterrupt(digitalPinToInterrupt(GPS_PPS), ppsISR, RISING);

  // Initialize the LVGL timer interrupt
  timer0 = timerBegin(0, 80, true); // Timer 0, prescaler 80 (1 MHz), count up
  timerAttachInterrupt(timer0, &lvglISR, true); // Attach the ISR
  timerAlarmWrite(timer0, SCREEN_REFRESH_RATE * 1000, true); // 10 ms interval, auto-reload
  timerAlarmEnable(timer0); // Enable the timer

  // Initialize the GPS timer interrupt
  timer1 = timerBegin(1, 80, true); // Timer 0, prescaler 80 (1 MHz), count up
  timerAttachInterrupt(timer1, &gpsISR, true); // Attach the ISR
  timerAlarmWrite(timer1, GPS_REFRESH_RATE * 1000, true); // 100 ms interval, auto-reload
  timerAlarmEnable(timer1); // Enable the timer

  // Initialize LVGL
  lv_init();

  lv_display_t * disp;
  disp = lv_tft_espi_create(SCREEN_WIDTH, SCREEN_HEIGHT, draw_buf, sizeof(draw_buf));
  lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_0);
  lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), LV_STATE_DEFAULT);

  // Initialize the UI library from SquareLine
  ui_init();

  // Initialize the GPS speed readings array to zero
  for (int i = 0; i < numReadings; i++) {
      readings[i] = 0.0f;
  }
}

void loop() {
  // Run the LVGL GUI
  lv_timer_handler();

  // Check if the boot button is pressed to toggle smoothing
  checkBootButton();

  // Check if GPS data is ready to be processed
  if (gpsReady) {
    gpsReady = false; // Reset the flag

    // Process GPS data
    while (Serial2.available() > 0) {
      gps.encode(Serial2.read());
    }

    // Check if GPS data is valid
    if (gps.location.isValid()) {
      if (!gpsSignalAcquired) {
        lv_obj_clear_flag(ui_SatImg, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_SatNumBackGround, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(ui_SatNum, LV_OBJ_FLAG_HIDDEN);
        gpsSignalAcquired = true;
      }
      if (!sysClockSet){
        setSysTime();     // Set the system clock from GPS data
      }
      processSatNum();    // Process the satelittle number information
      processGPSSpeed();  // Process speed information
      processHeading();   // Process direction heading information
      processCoords();    // Process coordinate information
    } else {
        // Handle case where GPS signal is lost
        gpsSignalAcquired = false;
        processGPSSignalLoss();
    }
  #ifdef DEBUG
    if (gpsSignalAcquired && sysClockSet) {
      Serial.print("[DEBUG] ");
      Serial.print("Speed: ");
      Serial.print(currentSpeed);
      Serial.print(" mph");
      Serial.print("  Heading: ");
      Serial.print(currentHeading);
      Serial.print("  Date: ");
      Serial.print(sysDateBuffer);
      Serial.print("  Time: ");
      Serial.print(sysTimeBuffer);
      Serial.print("  Sats: ");
      Serial.print(currentSats);
      Serial.print("  Lat: ");
      Serial.print(currentLat, 6);
      Serial.print("  Lng: ");
      Serial.print(currentLng, 6);
      Serial.print("\n");
    }
  #endif
  }

  if (sysClockSet) {
    formatSysDateTime();
    lv_label_set_text(ui_Date, sysDateBuffer);
    lv_label_set_text(ui_Time, sysTimeBuffer);
    if (millis() - lastSyncTime >= (GPS_CLOCK_SYNC * 1000)) {   
      sysClockSet = false;     // Re-sync the system clock to the GPS time
    }
  }

  if (newSatNumAvailable){
    newSatNumAvailable = false;
    static char bufSats[4];
    std::snprintf(bufSats, sizeof(bufSats), "%02d", currentSats);
    lv_label_set_text(ui_SatNum, bufSats);
  }

  if (newSpeedAvailable) {
    newSpeedAvailable = false;
    static char bufSpeed[4];
    std::snprintf(bufSpeed, sizeof(bufSpeed), "%03d", (int)currentSpeed);
    lv_label_set_text(ui_SpeedNum, bufSpeed);
  }

  if (newHeadingAvailable){
    newHeadingAvailable = false;
    lv_label_set_text(ui_Heading, currentHeading);
  }

  if (newLatCoordAvailable || newLngCoordAvailable)
  {
    newLatCoordAvailable = false;
    newLngCoordAvailable = false;
    saveCoordinates(currentLat, currentLng);   // Save the current coordinates to Preferences
  }


  if (ppsTriggered) {
    lv_chart_set_next_value(ui_SpeedChart, ui_SpeedChart_series_1, (int)currentSpeed);
    lv_chart_set_next_value(ui_SpeedChart, ui_SpeedChart_series_2, (int)currentSats);
    ppsTriggered = false;
  }

}