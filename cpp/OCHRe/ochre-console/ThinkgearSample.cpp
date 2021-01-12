#include <iostream> // cerr, cout
#include <sstream>
#include <chrono>      // system_clock, 
#include <iomanip>     // put_time, setfill, setw

#include "include/ThinkgearSample.h"

#define READ_ALL_AVAILABLE_PACKETS -1

namespace tgc
{
    std::string TimeStamp()
    {
        using namespace std::chrono;
        system_clock::time_point nowTimePoint = system_clock::now();
        time_t currentTime = system_clock::to_time_t(nowTimePoint);
        // time_t loses millisecond resolution (it shouldn't but it's lost somewhere), nowMillis will hold onto it
        int nowMillis = duration_cast<milliseconds>(nowTimePoint.time_since_epoch()).count() % 1000;

        tm currentLocalTime;
        std::ostringstream ss;
        gmtime_s(&currentLocalTime, &currentTime); // swap these two arguments and drop the "_s" if not using MSVC

        ss << std::put_time(&currentLocalTime, "%Y-%m-%d %T") << " " << std::setfill('0') << std::setw(3) << nowMillis;

        return ss.str();
    }

    ThinkgearSample::ThinkgearSample()
    {
        GetNewConnectionId();
        Connect();
    }

    ThinkgearSample::~ThinkgearSample()
    {
        TG_Disconnect(connection_id);
        TG_FreeConnection(connection_id);
    }

    void ThinkgearSample::GetNewConnectionId()
    {
        connection_id = TG_GetNewConnectionId();
        if (connection_id < 0) {
            std::cerr << TimeStamp() << " ERROR: TG_GetNewConnectionId() returned " << connection_id << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    void ThinkgearSample::Connect()
    {
        if (int errCode = TG_Connect(connection_id, com_port_name.c_str(), TG_BAUD_57600, TG_STREAM_PACKETS); errCode != 0)
            std::cerr << TimeStamp() << " ERROR: TG_Connect() returned " << errCode << std::endl;
    }

    void ThinkgearSample::ReadPackets(std::chrono::seconds sec = std::chrono::seconds(15))
    {
        using namespace std::chrono;
        seconds finishTime = duration_cast<seconds>(system_clock::now().time_since_epoch()) + sec;
        system_clock::time_point currentTime;

        int packetsWereRead = TG_ReadPackets(connection_id, READ_ALL_AVAILABLE_PACKETS);
        do {
            if (packetsWereRead) {
                for (auto &[key, data_type] : data_types) {
                    if (!TG_GetValueStatus(connection_id, key))    // non-zero if updated by latest packet
                        data_type = TG_GetValue(connection_id, key);
                }
            }
            currentTime = system_clock::now();
        } while (currentTime.time_since_epoch().count() < finishTime.count());
    }

    int ThinkgearSample::GetConnectionId() const
    {
        return connection_id;
    }

    int ThinkgearSample::GetAttention() const
    {
        return esense_attention;
    }

    int ThinkgearSample::GetMeditation() const
    {
        return esense_meditation;
    }

    int ThinkgearSample::GetEegRaw() const
    {
        return eeg_raw;
    }
}