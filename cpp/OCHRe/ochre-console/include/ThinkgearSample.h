#ifndef THINKGEAR_SAMPLE_H
#define THINKGEAR_SAMPLE_H

#include <chrono>      // system_clock, duration_cast, milliseconds
#include <vector>
#include <cstdint>
#include <array>
#include "include/thinkgear.h"

class ThinkgearSample {
private:
    std::chrono::system_clock::time_point timestamp;
    uint8_t poor_signal_quality;
    uint8_t esense_attention;
    uint8_t esense_meditation;

    int16_t raw_data; // 16-bit for TGAM (ThinkGearASIC Module), big-endian
    // this will go in the .cpp:
    //uint8_t raw_data_lsb = raw_data & 0x00ff;   // lsb is the high-order byte
    //uint8_t raw_data_msb = raw_data >> 8;       // msb is the low-order byte
    //int16_t raw_eeg = (raw_data_lsb << 8) | raw_data_msb;

    std::array<uint32_t, 8> eeg_power;


};

#endif // !THINKGEAR_SAMPLE_H