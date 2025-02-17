/*!
*
 * \file shared.h
 * Used by both cores
 *
 */

#pragma once
#include "string.h"
#include "pico/float.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/printf.h"
#include "pico/flash.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "hardware/flash.h"
#include "hardware/gpio.h"
#include "hardware/watchdog.h"
#include "hardware/exception.h"

/**
 * @brief Logs a message with a specified log level.
 *
 * This macro logs a message if the specified log level is less than or equal to the global LOGLEVEL.
 * The log message includes the core number and the time since boot in milliseconds.
 *
 * @param level The log level of the message.
 * @param ... The format string and arguments for the log message.
 */
#define LOG(level, ...) { \
    if (level <= LOGLEVEL) { \
        printf("[core%u @ %u ms] ", get_core_num(), to_ms_since_boot(get_absolute_time())); \
        printf(__VA_ARGS__); \
    } \
}

/**
 * @def _125M
 * @brief Constant value representing 125 million (125 x 10^6).
 */
#define _125M 125000000

/**
 * @enum error_t
 * @brief Enumeration of error codes used in the system.
 *
 * This enumeration defines various error codes that can be encountered
 * during the execution of the program. Each error code is represented
 * as a bit flag, allowing multiple errors to be combined using bitwise
 * operations.
 *
 */
typedef enum {
   FLASH_SAFE_EXECUTE_CORE_INIT_FAILURE = (1<<0), /**< Indicates a failure during the initialization of the flash safe execute mode. */
   FLASH_SAFE_EXECUTE_ERASE_FAILURE = (1<<1), /**< Indicates a failure during the erase operation in flash safe execute mode. */
   FLASH_SAFE_EXECUTE_PROGRAM_FAILURE = (1<<2), /**< Indicates a failure during the program operation in flash safe execute mode. */
   FLASH_SIZE_READBACK_FAILURE = (1<<3), /**< Indicates a failure in reading back the flash size. */
   STDIO_INIT_FAILURE = (1<<4), /**< Indicates a failure during the initialization of stdio. */
   REBOOT_BY_WATCHDOG = (1<<5), /**< Indicates that the system rebooted due to the watchdog timer. */
   INVALID_DIRECTION = (1<<6), /**< Indicates an invalid direction was specified. */
} error_t;


/**
 * @enum direction_t
 * @brief Enumeration for motor direction.
 *
 */
typedef enum {
   DIRECTION_FORWARD = 1, /**< Represents the forward direction */
   DIRECTION_REVERSE = 0, /**< Represents the reverse direction. */
} direction_t;

/** 
 * @brief Enum to define all speed steps.
 * 
 */
typedef enum {
    SPEED_STEP_REVERSE_STOP = 0x00, /**< 00000000bin 000dec */
    SPEED_STEP_REVERSE_EMERGENCY_STOP = 0x01, /**< 00000001bin 001dec */
    SPEED_STEP_REVERSE_001 = 0x02, /**< 00000010bin 002dec */
    SPEED_STEP_REVERSE_002 = 0x03, /**< 00000011bin 003dec */
    SPEED_STEP_REVERSE_003 = 0x04, /**< 00000100bin 004dec */
    SPEED_STEP_REVERSE_004 = 0x05, /**< 00000101bin 005dec */
    SPEED_STEP_REVERSE_005 = 0x06, /**< 00000110bin 006dec */
    SPEED_STEP_REVERSE_006 = 0x07, /**< 00000111bin 007dec */
    SPEED_STEP_REVERSE_007 = 0x08, /**< 00001000bin 008dec */
    SPEED_STEP_REVERSE_008 = 0x09, /**< 00001001bin 009dec */
    SPEED_STEP_REVERSE_009 = 0x0A, /**< 00001010bin 010dec */
    SPEED_STEP_REVERSE_010 = 0x0B, /**< 00001011bin 011dec */
    SPEED_STEP_REVERSE_011 = 0x0C, /**< 00001100bin 012dec */
    SPEED_STEP_REVERSE_012 = 0x0D, /**< 00001101bin 013dec */
    SPEED_STEP_REVERSE_013 = 0x0E, /**< 00001110bin 014dec */
    SPEED_STEP_REVERSE_014 = 0x0F, /**< 00001111bin 015dec */
    SPEED_STEP_REVERSE_015 = 0x10, /**< 00010000bin 016dec */
    SPEED_STEP_REVERSE_016 = 0x11, /**< 00010001bin 017dec */
    SPEED_STEP_REVERSE_017 = 0x12, /**< 00010010bin 018dec */
    SPEED_STEP_REVERSE_018 = 0x13, /**< 00010011bin 019dec */
    SPEED_STEP_REVERSE_019 = 0x14, /**< 00010100bin 020dec */
    SPEED_STEP_REVERSE_020 = 0x15, /**< 00010101bin 021dec */
    SPEED_STEP_REVERSE_021 = 0x16, /**< 00010110bin 022dec */
    SPEED_STEP_REVERSE_022 = 0x17, /**< 00010111bin 023dec */
    SPEED_STEP_REVERSE_023 = 0x18, /**< 00011000bin 024dec */
    SPEED_STEP_REVERSE_024 = 0x19, /**< 00011001bin 025dec */
    SPEED_STEP_REVERSE_025 = 0x1A, /**< 00011010bin 026dec */
    SPEED_STEP_REVERSE_026 = 0x1B, /**< 00011011bin 027dec */
    SPEED_STEP_REVERSE_027 = 0x1C, /**< 00011100bin 028dec */
    SPEED_STEP_REVERSE_028 = 0x1D, /**< 00011101bin 029dec */
    SPEED_STEP_REVERSE_029 = 0x1E, /**< 00011110bin 030dec */
    SPEED_STEP_REVERSE_030 = 0x1F, /**< 00011111bin 031dec */
    SPEED_STEP_REVERSE_031 = 0x20, /**< 00100000bin 032dec */
    SPEED_STEP_REVERSE_032 = 0x21, /**< 00100001bin 033dec */
    SPEED_STEP_REVERSE_033 = 0x22, /**< 00100010bin 034dec */
    SPEED_STEP_REVERSE_034 = 0x23, /**< 00100011bin 035dec */
    SPEED_STEP_REVERSE_035 = 0x24, /**< 00100100bin 036dec */
    SPEED_STEP_REVERSE_036 = 0x25, /**< 00100101bin 037dec */
    SPEED_STEP_REVERSE_037 = 0x26, /**< 00100110bin 038dec */
    SPEED_STEP_REVERSE_038 = 0x27, /**< 00100111bin 039dec */
    SPEED_STEP_REVERSE_039 = 0x28, /**< 00101000bin 040dec */
    SPEED_STEP_REVERSE_040 = 0x29, /**< 00101001bin 041dec */
    SPEED_STEP_REVERSE_041 = 0x2A, /**< 00101010bin 042dec */
    SPEED_STEP_REVERSE_042 = 0x2B, /**< 00101011bin 043dec */
    SPEED_STEP_REVERSE_043 = 0x2C, /**< 00101100bin 044dec */
    SPEED_STEP_REVERSE_044 = 0x2D, /**< 00101101bin 045dec */
    SPEED_STEP_REVERSE_045 = 0x2E, /**< 00101110bin 046dec */
    SPEED_STEP_REVERSE_046 = 0x2F, /**< 00101111bin 047dec */
    SPEED_STEP_REVERSE_047 = 0x30, /**< 00110000bin 048dec */
    SPEED_STEP_REVERSE_048 = 0x31, /**< 00110001bin 049dec */
    SPEED_STEP_REVERSE_049 = 0x32, /**< 00110010bin 050dec */
    SPEED_STEP_REVERSE_050 = 0x33, /**< 00110011bin 051dec */
    SPEED_STEP_REVERSE_051 = 0x34, /**< 00110100bin 052dec */
    SPEED_STEP_REVERSE_052 = 0x35, /**< 00110101bin 053dec */
    SPEED_STEP_REVERSE_053 = 0x36, /**< 00110110bin 054dec */
    SPEED_STEP_REVERSE_054 = 0x37, /**< 00110111bin 055dec */
    SPEED_STEP_REVERSE_055 = 0x38, /**< 00111000bin 056dec */
    SPEED_STEP_REVERSE_056 = 0x39, /**< 00111001bin 057dec */
    SPEED_STEP_REVERSE_057 = 0x3A, /**< 00111010bin 058dec */
    SPEED_STEP_REVERSE_058 = 0x3B, /**< 00111011bin 059dec */
    SPEED_STEP_REVERSE_059 = 0x3C, /**< 00111100bin 060dec */
    SPEED_STEP_REVERSE_060 = 0x3D, /**< 00111101bin 061dec */
    SPEED_STEP_REVERSE_061 = 0x3E, /**< 00111110bin 062dec */
    SPEED_STEP_REVERSE_062 = 0x3F, /**< 00111111bin 063dec */
    SPEED_STEP_REVERSE_063 = 0x40, /**< 01000000bin 064dec */
    SPEED_STEP_REVERSE_064 = 0x41, /**< 01000001bin 065dec */
    SPEED_STEP_REVERSE_065 = 0x42, /**< 01000010bin 066dec */
    SPEED_STEP_REVERSE_066 = 0x43, /**< 01000011bin 067dec */
    SPEED_STEP_REVERSE_067 = 0x44, /**< 01000100bin 068dec */
    SPEED_STEP_REVERSE_068 = 0x45, /**< 01000101bin 069dec */
    SPEED_STEP_REVERSE_069 = 0x46, /**< 01000110bin 070dec */
    SPEED_STEP_REVERSE_070 = 0x47, /**< 01000111bin 071dec */
    SPEED_STEP_REVERSE_071 = 0x48, /**< 01001000bin 072dec */
    SPEED_STEP_REVERSE_072 = 0x49, /**< 01001001bin 073dec */
    SPEED_STEP_REVERSE_073 = 0x4A, /**< 01001010bin 074dec */
    SPEED_STEP_REVERSE_074 = 0x4B, /**< 01001011bin 075dec */
    SPEED_STEP_REVERSE_075 = 0x4C, /**< 01001100bin 076dec */
    SPEED_STEP_REVERSE_076 = 0x4D, /**< 01001101bin 077dec */
    SPEED_STEP_REVERSE_077 = 0x4E, /**< 01001110bin 078dec */
    SPEED_STEP_REVERSE_078 = 0x4F, /**< 01001111bin 079dec */
    SPEED_STEP_REVERSE_079 = 0x50, /**< 01010000bin 080dec */
    SPEED_STEP_REVERSE_080 = 0x51, /**< 01010001bin 081dec */
    SPEED_STEP_REVERSE_081 = 0x52, /**< 01010010bin 082dec */
    SPEED_STEP_REVERSE_082 = 0x53, /**< 01010011bin 083dec */
    SPEED_STEP_REVERSE_083 = 0x54, /**< 01010100bin 084dec */
    SPEED_STEP_REVERSE_084 = 0x55, /**< 01010101bin 085dec */
    SPEED_STEP_REVERSE_085 = 0x56, /**< 01010110bin 086dec */
    SPEED_STEP_REVERSE_086 = 0x57, /**< 01010111bin 087dec */
    SPEED_STEP_REVERSE_087 = 0x58, /**< 01011000bin 088dec */
    SPEED_STEP_REVERSE_088 = 0x59, /**< 01011001bin 089dec */
    SPEED_STEP_REVERSE_089 = 0x5A, /**< 01011010bin 090dec */
    SPEED_STEP_REVERSE_090 = 0x5B, /**< 01011011bin 091dec */
    SPEED_STEP_REVERSE_091 = 0x5C, /**< 01011100bin 092dec */
    SPEED_STEP_REVERSE_092 = 0x5D, /**< 01011101bin 093dec */
    SPEED_STEP_REVERSE_093 = 0x5E, /**< 01011110bin 094dec */
    SPEED_STEP_REVERSE_094 = 0x5F, /**< 01011111bin 095dec */
    SPEED_STEP_REVERSE_095 = 0x60, /**< 01100000bin 096dec */
    SPEED_STEP_REVERSE_096 = 0x61, /**< 01100001bin 097dec */
    SPEED_STEP_REVERSE_097 = 0x62, /**< 01100010bin 098dec */
    SPEED_STEP_REVERSE_098 = 0x63, /**< 01100011bin 099dec */
    SPEED_STEP_REVERSE_099 = 0x64, /**< 01100100bin 100dec */
    SPEED_STEP_REVERSE_100 = 0x65, /**< 01100101bin 101dec */
    SPEED_STEP_REVERSE_101 = 0x66, /**< 01100110bin 102dec */
    SPEED_STEP_REVERSE_102 = 0x67, /**< 01100111bin 103dec */
    SPEED_STEP_REVERSE_103 = 0x68, /**< 01101000bin 104dec */
    SPEED_STEP_REVERSE_104 = 0x69, /**< 01101001bin 105dec */
    SPEED_STEP_REVERSE_105 = 0x6A, /**< 01101010bin 106dec */
    SPEED_STEP_REVERSE_106 = 0x6B, /**< 01101011bin 107dec */
    SPEED_STEP_REVERSE_107 = 0x6C, /**< 01101100bin 108dec */
    SPEED_STEP_REVERSE_108 = 0x6D, /**< 01101101bin 109dec */
    SPEED_STEP_REVERSE_109 = 0x6E, /**< 01101110bin 110dec */
    SPEED_STEP_REVERSE_110 = 0x6F, /**< 01101111bin 111dec */
    SPEED_STEP_REVERSE_111 = 0x70, /**< 01110000bin 112dec */
    SPEED_STEP_REVERSE_112 = 0x71, /**< 01110001bin 113dec */
    SPEED_STEP_REVERSE_113 = 0x72, /**< 01110010bin 114dec */
    SPEED_STEP_REVERSE_114 = 0x73, /**< 01110011bin 115dec */
    SPEED_STEP_REVERSE_115 = 0x74, /**< 01110100bin 116dec */
    SPEED_STEP_REVERSE_116 = 0x75, /**< 01110101bin 117dec */
    SPEED_STEP_REVERSE_117 = 0x76, /**< 01110110bin 118dec */
    SPEED_STEP_REVERSE_118 = 0x77, /**< 01110111bin 119dec */
    SPEED_STEP_REVERSE_119 = 0x78, /**< 01111000bin 120dec */
    SPEED_STEP_REVERSE_120 = 0x79, /**< 01111001bin 121dec */
    SPEED_STEP_REVERSE_121 = 0x7A, /**< 01111010bin 122dec */
    SPEED_STEP_REVERSE_122 = 0x7B, /**< 01111011bin 123dec */
    SPEED_STEP_REVERSE_123 = 0x7C, /**< 01111100bin 124dec */
    SPEED_STEP_REVERSE_124 = 0x7D, /**< 01111101bin 125dec */
    SPEED_STEP_REVERSE_125 = 0x7E, /**< 01111110bin 126dec */
    SPEED_STEP_REVERSE_126 = 0x7F, /**< 01111111bin 127dec */
    SPEED_STEP_FORWARD_STOP = 0x80, /**< 10000000bin 128dec */
    SPEED_STEP_FORWARD_EMERGENCY_STOP = 0x81, /**< 10000001bin 129dec */
    SPEED_STEP_FORWARD_001 = 0x82, /**< 10000010bin 130dec */
    SPEED_STEP_FORWARD_002 = 0x83, /**< 10000011bin 131dec */
    SPEED_STEP_FORWARD_003 = 0x84, /**< 10000100bin 132dec */
    SPEED_STEP_FORWARD_004 = 0x85, /**< 10000101bin 133dec */
    SPEED_STEP_FORWARD_005 = 0x86, /**< 10000110bin 134dec */
    SPEED_STEP_FORWARD_006 = 0x87, /**< 10000111bin 135dec */
    SPEED_STEP_FORWARD_007 = 0x88, /**< 10001000bin 136dec */
    SPEED_STEP_FORWARD_008 = 0x89, /**< 10001001bin 137dec */
    SPEED_STEP_FORWARD_009 = 0x8A, /**< 10001010bin 138dec */
    SPEED_STEP_FORWARD_010 = 0x8B, /**< 10001011bin 139dec */
    SPEED_STEP_FORWARD_011 = 0x8C, /**< 10001100bin 140dec */
    SPEED_STEP_FORWARD_012 = 0x8D, /**< 10001101bin 141dec */
    SPEED_STEP_FORWARD_013 = 0x8E, /**< 10001110bin 142dec */
    SPEED_STEP_FORWARD_014 = 0x8F, /**< 10001111bin 143dec */
    SPEED_STEP_FORWARD_015 = 0x90, /**< 10010000bin 144dec */
    SPEED_STEP_FORWARD_016 = 0x91, /**< 10010001bin 145dec */
    SPEED_STEP_FORWARD_017 = 0x92, /**< 10010010bin 146dec */
    SPEED_STEP_FORWARD_018 = 0x93, /**< 10010011bin 147dec */
    SPEED_STEP_FORWARD_019 = 0x94, /**< 10010100bin 148dec */
    SPEED_STEP_FORWARD_020 = 0x95, /**< 10010101bin 149dec */
    SPEED_STEP_FORWARD_021 = 0x96, /**< 10010110bin 150dec */
    SPEED_STEP_FORWARD_022 = 0x97, /**< 10010111bin 151dec */
    SPEED_STEP_FORWARD_023 = 0x98, /**< 10011000bin 152dec */
    SPEED_STEP_FORWARD_024 = 0x99, /**< 10011001bin 153dec */
    SPEED_STEP_FORWARD_025 = 0x9A, /**< 10011010bin 154dec */
    SPEED_STEP_FORWARD_026 = 0x9B, /**< 10011011bin 155dec */
    SPEED_STEP_FORWARD_027 = 0x9C, /**< 10011100bin 156dec */
    SPEED_STEP_FORWARD_028 = 0x9D, /**< 10011101bin 157dec */
    SPEED_STEP_FORWARD_029 = 0x9E, /**< 10011110bin 158dec */
    SPEED_STEP_FORWARD_030 = 0x9F, /**< 10011111bin 159dec */
    SPEED_STEP_FORWARD_031 = 0xA0, /**< 10100000bin 160dec */
    SPEED_STEP_FORWARD_032 = 0xA1, /**< 10100001bin 161dec */
    SPEED_STEP_FORWARD_033 = 0xA2, /**< 10100010bin 162dec */
    SPEED_STEP_FORWARD_034 = 0xA3, /**< 10100011bin 163dec */
    SPEED_STEP_FORWARD_035 = 0xA4, /**< 10100100bin 164dec */
    SPEED_STEP_FORWARD_036 = 0xA5, /**< 10100101bin 165dec */
    SPEED_STEP_FORWARD_037 = 0xA6, /**< 10100110bin 166dec */
    SPEED_STEP_FORWARD_038 = 0xA7, /**< 10100111bin 167dec */
    SPEED_STEP_FORWARD_039 = 0xA8, /**< 10101000bin 168dec */
    SPEED_STEP_FORWARD_040 = 0xA9, /**< 10101001bin 169dec */
    SPEED_STEP_FORWARD_041 = 0xAA, /**< 10101010bin 170dec */
    SPEED_STEP_FORWARD_042 = 0xAB, /**< 10101011bin 171dec */
    SPEED_STEP_FORWARD_043 = 0xAC, /**< 10101100bin 172dec */
    SPEED_STEP_FORWARD_044 = 0xAD, /**< 10101101bin 173dec */
    SPEED_STEP_FORWARD_045 = 0xAE, /**< 10101110bin 174dec */
    SPEED_STEP_FORWARD_046 = 0xAF, /**< 10101111bin 175dec */
    SPEED_STEP_FORWARD_047 = 0xB0, /**< 10110000bin 176dec */
    SPEED_STEP_FORWARD_048 = 0xB1, /**< 10110001bin 177dec */
    SPEED_STEP_FORWARD_049 = 0xB2, /**< 10110010bin 178dec */
    SPEED_STEP_FORWARD_050 = 0xB3, /**< 10110011bin 179dec */
    SPEED_STEP_FORWARD_051 = 0xB4, /**< 10110100bin 180dec */
    SPEED_STEP_FORWARD_052 = 0xB5, /**< 10110101bin 181dec */
    SPEED_STEP_FORWARD_053 = 0xB6, /**< 10110110bin 182dec */
    SPEED_STEP_FORWARD_054 = 0xB7, /**< 10110111bin 183dec */
    SPEED_STEP_FORWARD_055 = 0xB8, /**< 10111000bin 184dec */
    SPEED_STEP_FORWARD_056 = 0xB9, /**< 10111001bin 185dec */
    SPEED_STEP_FORWARD_057 = 0xBA, /**< 10111010bin 186dec */
    SPEED_STEP_FORWARD_058 = 0xBB, /**< 10111011bin 187dec */
    SPEED_STEP_FORWARD_059 = 0xBC, /**< 10111100bin 188dec */
    SPEED_STEP_FORWARD_060 = 0xBD, /**< 10111101bin 189dec */
    SPEED_STEP_FORWARD_061 = 0xBE, /**< 10111110bin 190dec */
    SPEED_STEP_FORWARD_062 = 0xBF, /**< 10111111bin 191dec */
    SPEED_STEP_FORWARD_063 = 0xC0, /**< 11000000bin 192dec */
    SPEED_STEP_FORWARD_064 = 0xC1, /**< 11000001bin 193dec */
    SPEED_STEP_FORWARD_065 = 0xC2, /**< 11000010bin 194dec */
    SPEED_STEP_FORWARD_066 = 0xC3, /**< 11000011bin 195dec */
    SPEED_STEP_FORWARD_067 = 0xC4, /**< 11000100bin 196dec */
    SPEED_STEP_FORWARD_068 = 0xC5, /**< 11000101bin 197dec */
    SPEED_STEP_FORWARD_069 = 0xC6, /**< 11000110bin 198dec */
    SPEED_STEP_FORWARD_070 = 0xC7, /**< 11000111bin 199dec */
    SPEED_STEP_FORWARD_071 = 0xC8, /**< 11001000bin 200dec */
    SPEED_STEP_FORWARD_072 = 0xC9, /**< 11001001bin 201dec */
    SPEED_STEP_FORWARD_073 = 0xCA, /**< 11001010bin 202dec */
    SPEED_STEP_FORWARD_074 = 0xCB, /**< 11001011bin 203dec */
    SPEED_STEP_FORWARD_075 = 0xCC, /**< 11001100bin 204dec */
    SPEED_STEP_FORWARD_076 = 0xCD, /**< 11001101bin 205dec */
    SPEED_STEP_FORWARD_077 = 0xCE, /**< 11001110bin 206dec */
    SPEED_STEP_FORWARD_078 = 0xCF, /**< 11001111bin 207dec */
    SPEED_STEP_FORWARD_079 = 0xD0, /**< 11010000bin 208dec */
    SPEED_STEP_FORWARD_080 = 0xD1, /**< 11010001bin 209dec */
    SPEED_STEP_FORWARD_081 = 0xD2, /**< 11010010bin 210dec */
    SPEED_STEP_FORWARD_082 = 0xD3, /**< 11010011bin 211dec */
    SPEED_STEP_FORWARD_083 = 0xD4, /**< 11010100bin 212dec */
    SPEED_STEP_FORWARD_084 = 0xD5, /**< 11010101bin 213dec */
    SPEED_STEP_FORWARD_085 = 0xD6, /**< 11010110bin 214dec */
    SPEED_STEP_FORWARD_086 = 0xD7, /**< 11010111bin 215dec */
    SPEED_STEP_FORWARD_087 = 0xD8, /**< 11011000bin 216dec */
    SPEED_STEP_FORWARD_088 = 0xD9, /**< 11011001bin 217dec */
    SPEED_STEP_FORWARD_089 = 0xDA, /**< 11011010bin 218dec */
    SPEED_STEP_FORWARD_090 = 0xDB, /**< 11011011bin 219dec */
    SPEED_STEP_FORWARD_091 = 0xDC, /**< 11011100bin 220dec */
    SPEED_STEP_FORWARD_092 = 0xDD, /**< 11011101bin 221dec */
    SPEED_STEP_FORWARD_093 = 0xDE, /**< 11011110bin 222dec */
    SPEED_STEP_FORWARD_094 = 0xDF, /**< 11011111bin 223dec */
    SPEED_STEP_FORWARD_095 = 0xE0, /**< 11100000bin 224dec */
    SPEED_STEP_FORWARD_096 = 0xE1, /**< 11100001bin 225dec */
    SPEED_STEP_FORWARD_097 = 0xE2, /**< 11100010bin 226dec */
    SPEED_STEP_FORWARD_098 = 0xE3, /**< 11100011bin 227dec */
    SPEED_STEP_FORWARD_099 = 0xE4, /**< 11100100bin 228dec */
    SPEED_STEP_FORWARD_100 = 0xE5, /**< 11100101bin 229dec */
    SPEED_STEP_FORWARD_101 = 0xE6, /**< 11100110bin 230dec */
    SPEED_STEP_FORWARD_102 = 0xE7, /**< 11100111bin 231dec */
    SPEED_STEP_FORWARD_103 = 0xE8, /**< 11101000bin 232dec */
    SPEED_STEP_FORWARD_104 = 0xE9, /**< 11101001bin 233dec */
    SPEED_STEP_FORWARD_105 = 0xEA, /**< 11101010bin 234dec */
    SPEED_STEP_FORWARD_106 = 0xEB, /**< 11101011bin 235dec */
    SPEED_STEP_FORWARD_107 = 0xEC, /**< 11101100bin 236dec */
    SPEED_STEP_FORWARD_108 = 0xED, /**< 11101101bin 237dec */
    SPEED_STEP_FORWARD_109 = 0xEE, /**< 11101110bin 238dec */
    SPEED_STEP_FORWARD_110 = 0xEF, /**< 11101111bin 239dec */
    SPEED_STEP_FORWARD_111 = 0xF0, /**< 11110000bin 240dec */
    SPEED_STEP_FORWARD_112 = 0xF1, /**< 11110001bin 241dec */
    SPEED_STEP_FORWARD_113 = 0xF2, /**< 11110010bin 242dec */
    SPEED_STEP_FORWARD_114 = 0xF3, /**< 11110011bin 243dec */
    SPEED_STEP_FORWARD_115 = 0xF4, /**< 11110100bin 244dec */
    SPEED_STEP_FORWARD_116 = 0xF5, /**< 11110101bin 245dec */
    SPEED_STEP_FORWARD_117 = 0xF6, /**< 11110110bin 246dec */
    SPEED_STEP_FORWARD_118 = 0xF7, /**< 11110111bin 247dec */
    SPEED_STEP_FORWARD_119 = 0xF8, /**< 11111000bin 248dec */
    SPEED_STEP_FORWARD_120 = 0xF9, /**< 11111001bin 249dec */
    SPEED_STEP_FORWARD_121 = 0xFA, /**< 11111010bin 250dec */
    SPEED_STEP_FORWARD_122 = 0xFB, /**< 11111011bin 251dec */
    SPEED_STEP_FORWARD_123 = 0xFC, /**< 11111100bin 252dec */
    SPEED_STEP_FORWARD_124 = 0xFD, /**< 11111101bin 253dec */
    SPEED_STEP_FORWARD_125 = 0xFE, /**< 11111110bin 254dec */
    SPEED_STEP_FORWARD_126 = 0xFF, /**< 11111111bin 255dec */
} speed_step_t;


/**
 * @brief Pointer to the uint8_t array in flash which stores the CVs.
 */
extern const uint8_t *CV_ARRAY_FLASH;

/**
 * @brief Target speed step for the decoder.
 * 
 * This external variable represents the desired speed step that the decoder
 * should aim to achieve.
 */
extern speed_step_t speed_step_target;

/**
 * @brief Previous target speed step.
 *
 * This variable holds the previous target speed step value for the decoder.
 * It is used to keep track of the last speed step that was set.
 */
extern speed_step_t speed_step_target_prev;

/**
 * @brief Indicates whether the CV setup check has been completed.
 *
 * This flag is used to track the status of the CV setup check.
 * When the check is done, this variable should is set to true by core 0.
 */
extern bool cv_setup_check_done;


/**
 * @brief Indicates whether the core initialization for safe flash execution is complete.
 *
 * This flag is used to signal the completion of the flash safe execute core initialization.
 * When the initialization is done this variable is set to true by core 1. 
 */
extern bool flash_safe_execute_core_init_done;


/**
 * @brief Global variable to hold the current error state.
 *
 * This variable is used to track the error state.
 */
extern error_t error_state;


/**
 * @brief Entry point for the second core (core 1).
 *
 * This function is the entry point for the second core, basically the main() of core1.
 */
void core1_entry();

/**
 * @brief Retrieves a 32-bit Configuration Variable (CV) starting from the specified index.
 *
 * This function reads and returns a 32-bit CV value from the given starting index.
 *
 * @param CV_start_index The starting index of the 32-bit CV to be retrieved.
 * @return The 32-bit CV value.
 */
uint32_t get_32bit_CV (uint16_t CV_start_index);

/**
 * @brief Retrieves a 16-bit Configuration Variable (CV) value starting from the specified index.
 *
 * This function reads and returns a 16-bit CV value from the given starting index.
 *
 * @param CV_start_index The starting index of the 16-bit CV to be retrieved.
 * @return The 16-bit CV value starting from the specified index.
 */
uint16_t get_16bit_CV (uint16_t CV_start_index);

/*!
 * @brief Calculates the direction based on a speed step byte.
 *
 * This function calculates the direction of movement based on the value of 
 * the `speed_step` byte. It refers to the "128 Speed Control" standard as outlined 
 * in NMRA Standard 9.2.1 section 2.3.2.1 or RCN-212 section 2.2.2.
 *
 * The following speed steps are defined:
 *
 * | DEC | BIN       | DIRECTION | DEFINITION           |
 * |-----|-----------|-----------|----------------------|
 * | 000 | 0000 0000 | Reverse   | STOP                 |
 * | 001 | 0000 0001 | Reverse   | EMERGENCY STOP       |
 * | 002 | 0000 0010 | Reverse   | Speed step 001       |
 * | 003 | 0000 0011 | Reverse   | Speed step 002       |
 * | 004 | 0000 0100 | Reverse   | Speed step 003       |
 * | ... | .... .... | Reverse   | ...                  |
 * | 126 | 0111 1110 | Reverse   | Speed step 125       |
 * | 127 | 0111 1111 | Reverse   | Speed step 126       |
 * | 128 | 1000 0000 | Forward   | STOP                 |
 * | 129 | 1000 0001 | Forward   | EMERGENCY STOP       |
 * | 130 | 1000 0010 | Forward   | Speed step 001       |
 * | 131 | 1000 0011 | Forward   | Speed step 002       |
 * | 132 | 1000 0100 | Forward   | Speed step 003       |
 * | ... | .... .... | Forward   | ...                  |
 * | 254 | 1111 1110 | Forward   | Speed step 125       |
 * | 255 | 1111 1111 | Forward   | Speed step 126       |
 * 
 * also see @ref speed_step_t for available steps.
 *
 * @param speed_step The speed step byte.
 * @return `DIRECTION_FORWARD` for forward direction, `DIRECTION_REVERSE` for reverse direction.
 */
direction_t get_direction_of_speed_step(speed_step_t speed_step);

/**
 * @brief Sets a specific error.
 *
 * This function sets a specific error to the error_state variable.
 *
 * @param err The error code to set.
 */
void set_error(error_t err);

/**
 * @brief Clears the specified error.
 *
 * This function resets the error state for the given error code.
 *
 * @param err The error code to be cleared.
 */
void clear_error(error_t err);

/**
 * @brief Clears all error flags or statuses in the system.
 *
 * Clears error_state variable
 */
void clear_all_errors();

/**
 * @brief Retrieves the current error state.
 *
 * This function returns the current error state of the system.
 *
 * @return error_t The current error state.
 */
error_t get_error_state();
