#include <Arduino.h>
#include <Preferences.h>
#include <TinyGPS++.h>
#include <LC76G.h>
#include <TFT_eSPI.h>
#include "config.h"

// Create a Preferences object
Preferences prefs;

// Initialize the TFT object
TFT_eSPI tft = TFT_eSPI();

// Initialize the LC76G GPS instance
LC76G lc76g;

// Initialize GPS object
TinyGPSPlus gps;

// Flags
bool newGPSSpeed = false;
bool enableSpeedSmoothing = false;

// Error counters
int gpsSpeedInvalid = 0;

// Speed Smoothing variables
const int numReadings = 5;   // Number of readings to store, default: 5 (smaller = more responsive to changes in speed but jumpy / larger smoother but less responsive)
float readings[numReadings]; // Readings from the analog input
int readIndex = 0;           // Index of the current reading
float total = 0;             // Running total
float speedGPSReturned = 0;  // Raw speed returned from GPS read
float speedAvg = 0;          // Smoothed speed

// Function to save the values to NVS (Non-Volatile Storage)
void updatePrefs(void) {
  prefs.putInt("speedErr", gpsSpeedInvalid);  // Save error count
}

void displayMessage(String msg) {
  tft.fillScreen(TFT_BLACK);  // Clear the screen

  tft.setTextSize(2);  // Set text size
  tft.setTextColor(TFT_WHITE, TFT_BLACK);  // Set text color to white with black background

  // Calculate the position to center the text
  int textWidth = tft.textWidth(msg);  // Get the width of the text
  int x = (240 - textWidth) / 2;           // Calculate x position to center the text
  int y = 100;                             // Calculate y position to center the text

  // Display the message
  tft.setCursor(x, y);
  tft.print(msg);
}

// Function to display the speed on the TFT screen
void displaySpeed(float speed) {
  tft.setTextSize(2);  // Set text size
  tft.setTextColor(TFT_WHITE, TFT_BLACK);  // Set text color to white with black background

  // Clear the previous speed display by drawing a black rectangle over it
  tft.fillRect(0, 100, 240, 40, TFT_BLACK);  // Adjust the position and size as needed

  // Calculate the position to center the text
  if (enableSpeedSmoothing) {
    String speedStr = String(speed, 2) + " mph (S)";  // Convert speed to string with 2 decimal place
    int textWidth = tft.textWidth(speedStr);          // Get the width of the text
    int x = (240 - textWidth) / 2;                    // Calculate x position to center the text
    int y = 100;                                      // Y position for the text
    // Display the speed
    tft.setCursor(x, y);
    tft.print(speedStr);
  }
  else {
    String speedStr = String(speed, 2) + " mph (NS)";  // Convert speed to string with 2 decimal place
    int textWidth = tft.textWidth(speedStr);          // Get the width of the text
    int x = (240 - textWidth) / 2;                    // Calculate x position to center the text
    int y = 100;                                      // Y position for the text
    // Display the speed
    tft.setCursor(x, y);
    tft.print(speedStr);
  }
}

void getGPSSpeed(void) {
  if (gps.speed.isValid()) {
    newGPSSpeed = true;
    if (enableSpeedSmoothing) {
      // Smooth the speed data using a simple moving average function
      total = total - readings[readIndex];
      readings[readIndex] = gps.speed.mph();
      total = total + readings[readIndex];
      readIndex = (readIndex + 1) % numReadings;
      speedAvg = total / numReadings;
    }
    else {
      speedAvg = gps.speed.mph();
    }

    // Display the updated speed on the TFT screen
    displaySpeed(speedAvg);

    #ifdef DEBUG
      Serial.print("[DEBUG] GPS Speed: ");
      Serial.print(gps.speed.mph());
      Serial.print(" mph");
      if (enableSpeedSmoothing) {
        Serial.print("  Smoothed Speed: ");
        Serial.print(speedAvg);
        Serial.print(" mph");
      }
      Serial.print("  Err count: ");
      Serial.print(gpsSpeedInvalid);
      Serial.print("\n");
    #endif
  }
  else {
    if (newGPSSpeed) {
      gpsSpeedInvalid++;
      updatePrefs();
    }
    displayMessage("No GPS Speed");
    #ifdef DEBUG
      Serial.println("[DEBUG] No GPS Speed!");
    #endif
  }
}

void setup() {
  #ifdef DEBUG
    // Initialize Serial for debugging
    Serial.begin(SERIAL_BAUDRATE);
  #endif

  // Initialize Preferences
  prefs.begin("gpsData", false);

  // Initialize TFT Display
  tft.begin();
  tft.setRotation(SCREEN_ROTATION); // Set rotation to 0 degrees
  tft.fillScreen(TFT_BLACK);

  // Initialize the GPS hardware
  lc76g.begin(GPS_RX, SERIAL_8N1, GPS_TX, GPS_BAUDRATE);
  lc76g.sendCommand("$PAIR051");
  if (lc76g.setUpdateRate(GPS_REFRESH_RATE)) {
    displayMessage("GPS Module Updated!");
    delay(2000);
    #ifdef DEBUG
      Serial.println("[DEBUG] Successfully set LC76G GPS update rate to " + String(GPS_REFRESH_RATE) + " ms");
      delay(5000);
    #endif
  } else {
    displayMessage("No GPS Module!");
    delay(10000);
    #ifdef DEBUG
      Serial.println("[DEBUG] Failed to set LC76G GPS update rate");
      delay(5000);
    #endif
  }

  displayMessage("Errors: " + prefs.getInt("speedErr", 0));
  delay(2000);

  #ifdef DEBUG
    Serial.print("[DEBUG] Previous GPS Speed Errors: ");
    Serial.println(prefs.getInt("speedErr", 0));
    delay(5000);
  #endif

  // Reset memory values
  updatePrefs();

  // Configure the PPS pin as input
  pinMode(GPS_PPS, INPUT);

  // Initialize the boot button pin
  pinMode(BOOT_BUTTON_PIN, INPUT_PULLUP);

  // Initialize the GPS speed readings array to zero
  for (int i = 0; i < numReadings; i++) {
      readings[i] = 0.0f;
  }
}

void loop() {
  // Process GPS Data
  while (Serial2.available() > 0) {
    gps.encode(Serial2.read());
  }

  // Check if GPS Data is Valid
  if (gps.location.isValid()) {
    tft.fillScreen(TFT_BLACK);                  // Fill the screen with black color
    getGPSSpeed();                              // Process GPS Speed Information
  } else {
    displayMessage("No GPS Signal!");
    #ifdef DEBUG
      Serial.println("[DEBUG] No GPS Signal!");
    #endif
  }

  // Check if the boot button is pressed
  if (digitalRead(BOOT_BUTTON_PIN) == LOW) {
    displayMessage("Errors: " + gpsSpeedInvalid); // Display the error count on the TFT screen      
    delay(5000);                                // Debounce delay
    tft.fillScreen(TFT_BLACK);                  // Fill the screen with black color
  }

  delay(GPS_REFRESH_RATE);
}