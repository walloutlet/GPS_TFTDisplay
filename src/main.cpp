#include <Arduino.h>
#include <Preferences.h>
#include <TinyGPS++.h>
#include <TFT_eSPI.h>
#include "config.h"

// Create a Preferences object
Preferences prefs;

// Initialize the TFT object
TFT_eSPI tft = TFT_eSPI();

// Initialize GPS object
TinyGPSPlus gps;

// HardwareSerial for GPS
HardwareSerial gpsSerial(1);

// Flags
bool newGPSSpeed = false;
bool enableSpeedSmoothing = true;

// Error counters
int gpsSpeedInvalid = 0;

// Speed Smoothing variables
const int numReadings = 5;   // Number of readings to store, default: 5 (smaller = more responsive to changes in speed but jumpy / larger smoother but less responsive)
float readings[numReadings]; // Readings from the analog input
int readIndex = 0;           // Index of the current reading
float total = 0;             // Running total
float speedGPSReturned = 0;  // Raw speed returned from GPS read
float speedAvg = 0;          // Smoothed speed

class LC76GGPS {
private:
    // Calculate NMEA checksum
    String calculateChecksum(const String& sentence) {
        uint8_t checksum = 0;
        // Calculate for characters between $ and *
        for (int i = 1; i < sentence.length(); i++) {
            checksum ^= sentence[i];
        }
        char checksumStr[3];
        sprintf(checksumStr, "%02X", checksum);
        return String(checksumStr);
    }

    // Send command and wait for acknowledgment
    bool sendCommand(const String& command, int timeout = 1000) {
        Serial.println("Sending command: " + command);  // Debug output
        gpsSerial.print(command);
        
        unsigned long startTime = millis();
        String response;
        
        while (millis() - startTime < timeout) {
            if (gpsSerial.available()) {
                char c = gpsSerial.read();
                response += c;
                if (c == '\n') {
                    Serial.println("Response: " + response);  // Debug output
                    response = "";
                }
            }
        }
        return true;  // For LC76G we assume success if no error response
    }

public:
    void begin() {
        gpsSerial.begin(GPS_BAUDRATE, SERIAL_8N1, GPS_RX, GPS_TX);
        delay(100);  // Allow module to stabilize
    }

    // Set update rate to 10Hz (100ms interval)
    bool setUpdateRate10Hz() {
        // Create command to set 100ms positioning interval
        String posFreqCmd = "$PAIR050,100";
        posFreqCmd += "*" + calculateChecksum(posFreqCmd);
        posFreqCmd += "\r\n";

        // Send the command
        if (!sendCommand(posFreqCmd)) {
            return false;
        }

        // Wait a bit before sending next command
        delay(100);

        // Set output configuration for RMC and GGA at high rate
        // Type 0 = GGA, Type 1 = RMC
        String configGGACmd = "$PAIR062,0,1";
        configGGACmd += "*" + calculateChecksum(configGGACmd);
        configGGACmd += "\r\n";
        
        if (!sendCommand(configGGACmd)) {
            return false;
        }

        delay(100);

        String configRMCCmd = "$PAIR062,1,1";
        configRMCCmd += "*" + calculateChecksum(configRMCCmd);
        configRMCCmd += "\r\n";
        
        return sendCommand(configRMCCmd);
    }
};

// Function to save the values to NVS (Non-Volatile Storage)
void updatePrefs(void) {
  prefs.putInt("speedErr", gpsSpeedInvalid);             // Save error count
}

void displayErrorCount(int errorCount) {
  tft.fillScreen(TFT_BLACK);  // Clear the screen
  tft.setTextSize(2);  // Set text size
  tft.setTextColor(TFT_WHITE, TFT_BLACK);  // Set text color to white with black background

  // Calculate the position to center the text
  String errorStr = "Errors: " + String(errorCount);  // Convert error count to string
  int textWidth = tft.textWidth(errorStr);            // Get the width of the text
  int x = (240 - textWidth) / 2;                      // Calculate x position to center the text
  int y = 100;                                        // Y position for the text

  // Display the error count
  tft.setCursor(x, y);
  tft.print(errorStr);
}

void displayNoGPSSignal() {
  tft.fillScreen(TFT_BLACK);  // Clear the screen

  tft.setTextSize(2);  // Set text size
  tft.setTextColor(TFT_WHITE, TFT_BLACK);  // Set text color to white with black background

  // Calculate the position to center the text
  String message = "No GPS Signal";
  int textWidth = tft.textWidth(message);  // Get the width of the text
  int x = (240 - textWidth) / 2;           // Calculate x position to center the text
  int y = 100;                             // Calculate y position to center the text

  // Display the message
  tft.setCursor(x, y);
  tft.print(message);
}

void displayNoGPSSpeedSignal() {
  tft.fillScreen(TFT_BLACK);  // Clear the screen

  tft.setTextSize(2);  // Set text size
  tft.setTextColor(TFT_WHITE, TFT_BLACK);  // Set text color to white with black background

  // Calculate the position to center the text
  String message = "No GPS Speed";
  int textWidth = tft.textWidth(message);  // Get the width of the text
  int x = (240 - textWidth) / 2;           // Calculate x position to center the text
  int y = 100;                             // Calculate y position to center the text

  // Display the message
  tft.setCursor(x, y);
  tft.print(message);
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
      Serial.print("  Err count: ");
      Serial.print(gpsSpeedInvalid);
      if (enableSpeedSmoothing) {
        Serial.print("  Smoothed Speed: ");
        Serial.print(speedAvg);
        Serial.print(" mph");
      }
      Serial.print("\n");
    #endif
  }
  else {
    if (newGPSSpeed) {
      gpsSpeedInvalid++;
      updatePrefs();
    }
    displayNoGPSSpeedSignal();
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

  displayErrorCount(prefs.getInt("speedErr", 0));
  delay(5000);

  #ifdef DEBUG
    Serial.print("[DEBUG] Previous GPS Speed Errors: ");
    Serial.println(prefs.getInt("speedErr", 0));
    delay(5000);
  #endif

  // Reset memory values
  updatePrefs();

  // Initialize GPS Serial
  // gpsSerial.begin(GPS_BAUDRATE, SERIAL_8N1, GPS_RX, GPS_TX);
  // Initialize the GPS object
  LC76GGPS lc76g;
  lc76g.begin();
  
  // Set update rate to 10Hz
  if (lc76g.setUpdateRate10Hz()) {
      Serial.println("[DEBUG] Successfully set update rate to 10Hz");
  } else {
      Serial.println("[DEBUG] Failed to set update rate");
  }

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
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  // Check if GPS Data is Valid
  if (gps.location.isValid()) {
    tft.fillScreen(TFT_BLACK);                  // Fill the screen with black color
    getGPSSpeed();                              // Process GPS Speed Information
  } else {
    displayNoGPSSignal();
    #ifdef DEBUG
      Serial.println("[DEBUG] No GPS Signal!");
    #endif
  }

  // Check if the boot button is pressed
  if (digitalRead(BOOT_BUTTON_PIN) == LOW) {
    displayErrorCount(gpsSpeedInvalid);         // Display the error count on the TFT screen
    delay(5000);                                // Debounce delay
    tft.fillScreen(TFT_BLACK);                  // Fill the screen with black color
  }

  delay(100);
}