#ifndef THINKGEAR_SAMPLE_H
#define THINKGEAR_SAMPLE_H

#include <string>
#include <array>
#include <chrono> // system_clock, duration_cast, milliseconds
#include <cstdint>
#include <vector>
#include <map>

#include "include/thinkgear.h"


namespace tgc
{
    /* NOTE: On Windows, COM10 and higher must be preceded by \\.\, as in
    *       "\\\\.\\COM12" (must escape backslashes in strings).
    *       COM9 and lower do not require the \\.\, but are allowed to include them.
    */
    static const std::string DEFAULT_COM_NAME = "\\\\.\\COM6";
    //static const std::array<int, 12> TG_DATA_TYPES{ TG_DATA_POOR_SIGNAL, TG_DATA_ATTENTION, TG_DATA_MEDITATION, TG_DATA_RAW,
    //    TG_DATA_DELTA, TG_DATA_THETA, TG_DATA_ALPHA1, TG_DATA_ALPHA2, TG_DATA_BETA1, TG_DATA_BETA2,
    //    TG_DATA_GAMMA1, TG_DATA_GAMMA2 };

    class ThinkgearSample {
    private:
        int connection_id;
        std::string com_port_name{ DEFAULT_COM_NAME };

        uint8_t poor_signal_quality; // 0 best, 255 worst. 200 = no skin contact
        uint8_t esense_attention;    // 0-100
        uint8_t esense_meditation;   // 0-100

        int16_t eeg_raw; // 16-bit for TGAM (ThinkGearASIC Module), big-endian
        // this will go in the .cpp:
        // uint8_t raw_data_lsb = raw_data & 0x00ff;   // lsb is the high-order byte
        // uint8_t raw_data_msb = raw_data >> 8;       // msb is the low-order byte
        // int16_t raw_eeg = (raw_data_lsb << 8) | raw_data_msb;

        // std::array<uint32_t, 8> eeg_power;
        // They're all 3-byte big-endian unsigned ints
        uint32_t eeg_delta;     // 0.5 - 2.75Hz
        uint32_t eeg_theta;     // 3.5 - 6.75Hz
        uint32_t eeg_alpha_1;   // 7.5 - 9.25Hz
        uint32_t eeg_alpha_2;   // 10 - 11.75Hz
        uint32_t eeg_beta_1;    // 13 - 16.75Hz
        uint32_t eeg_beta_2;    // 18 - 29.75Hz
        uint32_t eeg_gamma_1;   // 31 - 39.75Hz
        uint32_t eeg_gamma_2;   // 42 - 49.75Hz

        std::map<int, uint32_t> data_types = {
            {TG_DATA_POOR_SIGNAL, poor_signal_quality},
            {TG_DATA_ATTENTION, esense_attention},
            {TG_DATA_MEDITATION, esense_meditation},
            {TG_DATA_RAW, eeg_raw},
            {TG_DATA_DELTA, eeg_delta},
            {TG_DATA_THETA, eeg_theta},
            {TG_DATA_ALPHA1, eeg_alpha_1},
            {TG_DATA_ALPHA2, eeg_alpha_2},
            {TG_DATA_BETA1, eeg_beta_1},
            {TG_DATA_BETA2, eeg_beta_2},
            {TG_DATA_GAMMA1, eeg_gamma_1},
            {TG_DATA_GAMMA2, eeg_gamma_2},
        };

    public:
        ThinkgearSample();
        ~ThinkgearSample();

        void GetNewConnectionId();
        void Connect();


        void ReadPackets(std::chrono::seconds);

        int GetConnectionId() const;
        int GetAttention() const;
        int GetMeditation() const;

        int GetEegRaw() const;

        //auto GetEegFreqBands();


    };

    /**
    * Return an ISO-8601 style timestamp.
    * Leave alone until compolers start implementing C++20 spec
    */
    std::string TimeStamp();

}
#endif // !THINKGEAR_SAMPLE_H