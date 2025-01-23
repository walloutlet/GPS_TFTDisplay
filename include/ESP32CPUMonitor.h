#include <Arduino.h>

class ESP32CPUMonitor {
private:
    uint32_t lastPrintTime = 0;
    const uint32_t printInterval;  // Interval in milliseconds
    bool enablePrintStats;
    
    // Variables for CPU usage calculation
    uint32_t lastMeasurementTime = 0;
    uint32_t busyTime = 0;
    uint32_t measurementStartTime = 0;
    float cpuUsage = 0;
    const uint32_t sampleInterval = 100; // Sample every 100ms
    bool isSampling = false;

    // Helper function to get task high water mark
    uint32_t getTaskHighWaterMark(TaskHandle_t task) {
        return uxTaskGetStackHighWaterMark(task);
    }

public:
    ESP32CPUMonitor(uint32_t interval = 5000, bool enablePrint = true) 
        : printInterval(interval), enablePrintStats(enablePrint) {}

    void begin() {
        lastPrintTime = millis();
        lastMeasurementTime = millis();
        measurementStartTime = millis();
    }

    void update() {
        uint32_t currentTime = millis();
        
        // Start a new measurement cycle
        if (!isSampling && (currentTime - lastMeasurementTime >= sampleInterval)) {
            measurementStartTime = currentTime;
            busyTime = 0;
            isSampling = true;
            lastMeasurementTime = currentTime;
        }
        
        // If we're in a sampling period, count busy time
        if (isSampling) {
            busyTime++;
            
            // End sampling period after sampleInterval
            if (currentTime - measurementStartTime >= sampleInterval) {
                // Calculate CPU usage as percentage of busy time
                float newUsage = (float)busyTime / (float)(currentTime - measurementStartTime) * 100.0f;
                
                // Smooth the measurements with a simple moving average
                cpuUsage = (cpuUsage * 0.7f) + (newUsage * 0.3f);
                
                // Clamp CPU usage to valid range
                if (cpuUsage < 0.0f) cpuUsage = 0.0f;
                if (cpuUsage > 100.0f) cpuUsage = 100.0f;
                
                isSampling = false;
            }
        }

        // Print stats if interval has elapsed
        if (currentTime - lastPrintTime >= printInterval) {
            if (enablePrintStats) {
                printStats();
            }
            lastPrintTime = currentTime;
        }
    }

    float getCPUUsage() {
        return cpuUsage;
    }

private:
    void printStats() {
        Serial.println("\n===== ESP32 CPU Statistics =====");
        Serial.printf("CPU Usage: %.2f%%\n", cpuUsage);
        Serial.printf("CPU Frequency: %dMHz\n", ESP.getCpuFreqMHz());
        Serial.printf("Free Heap: %d bytes\n", ESP.getFreeHeap());
        Serial.printf("Minimum Free Heap: %d bytes\n", ESP.getMinFreeHeap());
        Serial.println("===============================================");
        
        // Get information about currently running tasks
        TaskHandle_t taskIterator = NULL;
        UBaseType_t taskCount = uxTaskGetNumberOfTasks();
        
        for (UBaseType_t i = 0; i < taskCount; i++) {
            taskIterator = pxTaskGetNext(taskIterator);
            if (taskIterator != NULL) {
                char taskName[16];
                strlcpy(taskName, pcTaskGetTaskName(taskIterator), sizeof(taskName));
                UBaseType_t stackHighWaterMark = getTaskHighWaterMark(taskIterator);
                eTaskState taskState = eTaskGetState(taskIterator);
                
                const char* stateStr;
                switch (taskState) {
                    case eRunning: stateStr = "Running"; break;
                    case eReady: stateStr = "Ready"; break;
                    case eBlocked: stateStr = "Blocked"; break;
                    case eSuspended: stateStr = "Suspended"; break;
                    case eDeleted: stateStr = "Deleted"; break;
                    default: stateStr = "Unknown"; break;
                }
                
                Serial.printf("%-16s State: %-10s Stack: %5d\n", 
                            taskName, 
                            stateStr,
                            stackHighWaterMark);
            }
        }
        Serial.println("===============================================\n");
    }
};