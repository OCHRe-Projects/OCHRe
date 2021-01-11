#ifndef THINKGEAR_SAMPLE_H
#define THINKGEAR_SAMPLE_H

#include <string>
#include <array>
#include <chrono> // system_clock, duration_cast, milliseconds
#include <cstdint>
#include <vector>

#include "include/thinkgear.h"

class ThinkgearSample {
private:
    int connection_id;
    std::string com_port_name;

    std::chrono::system_clock::time_point timestamp;
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



public:
    ThinkgearSample();
    ~ThinkgearSample();

    void GetNewConnectionId();
    void Connect();

    void ReadPacketsForDuration(std::chrono::seconds);

    int GetAttention();
    int GetMeditation();

    int GetEegRaw();

    


};

#endif // !THINKGEAR_SAMPLE_H