#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_timer.h"

class ESP32CPUMonitor {
private:
    uint32_t lastPrintTime = 0;
    const uint32_t printInterval;  // Interval in milliseconds
    
    // Statistics tracking
    uint64_t lastIdleTime = 0;
    float cpuUsage = 0;

    // Flag to enable/disable printing stats
    bool enablePrintStats;

    // Helper function to get task high water mark
    uint32_t getTaskHighWaterMark(TaskHandle_t task) {
        return uxTaskGetStackHighWaterMark(task);
    }

public:
    ESP32CPUMonitor(uint32_t interval = 5000, bool enablePrint = true) 
        : printInterval(interval), enablePrintStats(enablePrint) {}

    void begin() {
        lastPrintTime = millis();
        lastIdleTime = esp_timer_get_time();
    }

    void update() {
        if (millis() - lastPrintTime >= printInterval) {
            calculateCPUUsage();
            if (enablePrintStats) {
                printStats();
            }
            lastPrintTime = millis();
        }
    }

    float getCPUUsage() {
        return cpuUsage;
    }

private:
    void calculateCPUUsage() {
        uint64_t currentIdleTime = esp_timer_get_time();
        
        if (lastIdleTime > 0) {
            uint64_t totalTime = printInterval * 1000;  // Convert to microseconds
            uint64_t idleTimeDelta = currentIdleTime - lastIdleTime;
            
            if (totalTime > 0) {
                // Calculate CPU usage as percentage of non-idle time
                cpuUsage = 100.0f - ((float)idleTimeDelta * 100.0f / (float)totalTime);
                
                // Clamp CPU usage to valid range
                if (cpuUsage < 0.0f) cpuUsage = 0.0f;
                if (cpuUsage > 100.0f) cpuUsage = 100.0f;
            }
        }

        lastIdleTime = currentIdleTime;
    }

    void printStats() {
        Serial.println("\n===== ESP32 CPU Statistics =====");
        Serial.printf("CPU Usage: %.2f%%\n", cpuUsage);
        Serial.printf("CPU Frequency: %dMHz\n", ESP.getCpuFreqMHz());
        Serial.printf("Free Heap: %d bytes\n", ESP.getFreeHeap());
        Serial.printf("Minimum Free Heap: %d bytes\n", ESP.getMinFreeHeap());
        Serial.printf("Max Alloc Heap: %d bytes\n", ESP.getMaxAllocHeap());
        
        // Print information about key tasks
        Serial.println("\nTask Stack Info (Free Stack Space):");
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