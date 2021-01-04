// ochre-console.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>    // cout, cerr, endl
#include <string>      // string
#include <ctime>       // time
#include <chrono>      // system_clock, duration_cast, milliseconds
#include <iomanip>     // put_time, setfill, setw
#include <sstream>
#include "thinkgear.h"

double getAvgDataValue(int, int, int);

/**
 * Return an ISO-8601 timestamp
 * TODO: Make this its own class so that data types aren't created and destroyed every time.
 */
std::string timeStamp()
{
    std::chrono::system_clock::time_point nowTimePoint = std::chrono::system_clock::now();
    time_t currentTime = std::chrono::system_clock::to_time_t(nowTimePoint);
    int nowMillis = std::chrono::duration_cast<std::chrono::milliseconds>(nowTimePoint.time_since_epoch()).count() % 1000;

    tm currentLocalTime;
    std::ostringstream ss;
    gmtime_s(&currentLocalTime, &currentTime);

    ss << std::put_time(&currentLocalTime, "%Y-%m-%d %T") << "." << std::setfill('0') << std::setw(3) << nowMillis << " ";

    return ss.str();
}

/**
 * Prompts and waits for the user to press ENTER.
 */
void wait()
{
    std::cout << std::endl << "Press ENTER..." << std::endl;
    while (std::cin.get() != '\n');
}

int main(int argc, char **argv)
{
    /* NOTE: On Windows, COM10 and higher must be preceded by \\.\, as in
     *       "\\\\.\\COM12" (must escape backslashes in strings).
     *       COM9 and lower do not require the \\.\, but are allowed to include them.
     */
    std::string comSix = "\\\\.\\COM6";
    int packetsRead = 0;
    int errCode = 0;

    double secondsToRun = 1;
    time_t startTime = 0;
    time_t currTime = 0;
    int set_filter_flag = 0;
    int packetCount = 0;

    float rawValue = 0;
    float attentionValue = 0;
    float meditationValue = 0;

    std::cout << timeStamp() << "Stream SDK version: " << TG_GetVersion() << std::endl;

    // Get a connection ID handle to ThinkGear
    int connectionId = TG_GetNewConnectionId();
    if (connectionId < 0)
    {
        std::cerr << timeStamp() << "ERROR: TG_GetNewConnectionId() returned " << connectionId << std::endl;
        wait();
        exit(EXIT_FAILURE);
    }

    // Attempt to connect the connection ID handle to serial port "COM6"
    std::cout << timeStamp() << "comPortName: " << comSix << std::endl;
    errCode = TG_Connect(connectionId, comSix.c_str(), TG_BAUD_57600, TG_STREAM_PACKETS);
    if (errCode < 0)
    {
        std::cerr << timeStamp() << "ERROR: TG_Connect() returned " << errCode << std::endl;
        wait();
        exit(EXIT_FAILURE);
    }

    startTime = time(0);
    while (difftime(time(0), startTime) < secondsToRun)
    {
        do
        {
            // Read a single Packet from the connection
            packetsRead = TG_ReadPackets(connectionId, 1);
            if (packetsRead == 1 && TG_GetValueStatus(connectionId, TG_DATA_RAW) != 0)
            {
                // Get and print out the new raw value
                rawValue = TG_GetValue(connectionId, TG_DATA_RAW);
                std::cout << timeStamp() << "raw: " << rawValue << std::endl;
                packetCount++;
            }
        } while (packetsRead > 0);
    }

    std::cout << timeStamp() << "data: " << getAvgDataValue(connectionId, TG_DATA_ATTENTION, 5) << std::endl;

    std::cout << timeStamp() << "Total packets read: " << packetCount << std::endl;
    //std::cout << timeStamp() << "Max reading: " << min << std::endl; 
    //std::cout << timeStamp() << "Min reading: " << max << std::endl;

    TG_Disconnect(connectionId); // Disconnect
    TG_FreeConnection(connectionId); // Clean up

    return 0;
}

double getAvgDataValue(int connectionId, int dataType, int seconds)
{
    time_t startTime = time(0);
    float value = 0;
    float sumOfValues = 0;
    int packetCount = 0;
    int packetRead = 0;

    while (difftime(time(0), startTime) < seconds)
    {
        do
        {
            packetRead = TG_ReadPackets(connectionId, 1);
            if (packetRead == 1 && TG_GetValueStatus(connectionId, dataType) != 0)
            {
                value = TG_GetValue(connectionId, dataType);
                sumOfValues += value;
                packetCount++;
                std::cout << timeStamp() << " Value (" << packetCount << "): " << value << std::endl;
            }
        } while (packetRead > 0);
    }
    return (double)sumOfValues / packetCount;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
