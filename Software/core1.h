/*!
*
 * \file core1.h
 * Used exclusively by core1
 *
 */

#pragma once
#include "shared.h"


/**
 * @def BASE_PWM_ARR_LEN
 * @brief The length of the base PWM array.
 *
 * This macro defines the length of the base PWM ring buffer array.
 */
#define BASE_PWM_ARR_LEN 16

/**
 * @brief Enumeration for controller operating modes.
 *
 * Defines the two controller operating modes.
 *
 * @enum controller_mode_t
 */
typedef enum {
    STARTUP_MODE, /**< Controller startup mode state */
    PID_MODE, /**< Controller pid mode state */
} controller_mode_t;

/**
 * @brief Structure for startup controller parameters and variables.
 *
 * This structure defines the parameters and variables used in the startup 
 * controller, including the latest PWM level, base pwm level ring buffer array and the respective ring buffer index.
 *
 * @typedef startup_parameters_t
 * @struct startup_parameters_t
 */
typedef struct startup_parameters_t {
    uint16_t level; /**< Latest level */
    uint16_t base_pwm_arr[BASE_PWM_ARR_LEN]; /**< base pwm ring buffer array */
    uint16_t base_pwm_arr_i; /**< base pwm ring buffer array index */
    float k_ff; /**< Feed forward factor */
}startup_parameters_t;

/**
 * @brief Structure for PID controller parameters and variables.
 *
 * This structure defines the parameters and variables used in the PID 
 * controller, including gains, error tracking, integrator limits, ...
 *
 * @typedef pid_parameters_t
 * @struct pid_parameters_t
 */
typedef struct pid_parameters_t{
    // PID Controller variables
    float k_i;                      /**< Integral gain */
    float k_d;                      /**< Derivative gain */
    float t;                        /**< Sampling time */ 
    float tau;                      /**< Low-pass-filter time constant */ 
    float ci_0;                     /**< (k_i*t)/2 */ 
    float cd_0;                     /**< (k_d*2)/(2*tau+t) */ 
    float cd_1;                     /**< (2*tau-t)/(2*tau+t) */ 
    float int_lim_max;              /**< max limit for integrator */ 
    float int_lim_min;              /**< min limit for integrator */ 
    float max_output;               /**< max possible pwm output value */ 
    float k_p;                      /**< Proportional gain */ 
    float e;                        /**< Error */ 
    float e_prev;                   /**< Previous Error */ 
    float i_prev;                   /**< Previous Integral Value */ 
    float d_prev;                   /**< Previous Derivative Value */ 
    // Variable gain variables - See gain scheduling documentation for more details
    float k_p_x_1_shift;            /**< Effectively defines where x1 is shifted from 0% of max setpoint to 100% of max setpoint */ 
    float k_p_x_1;                  /**< x1 */ 
    float k_p_x_2;                  /**< x2 */ 
    float k_p_y_0;                  /**< y0 = KP @ x0 */ 
    float k_p_y_1;                  /**< y1 = KP @ x1 */ 
    float k_p_y_2;                  /**< y2 = KP @ x2 */ 
    float k_p_m_1;                  /**< slope from (x0, y0) to (x1, y1) */ 
    float k_p_m_2;                  /**< slope from (x1, y1) to (x2, y2) */ 
} pid_parameters_t;

/**
 * @brief Structure for various controller parameters.
 * 
 * This structure defines the highest level general controller parameters, including 
 * General settings (setpoint, mode), startup controller settings struct, PID controller struct, and measurement data.
 * 
 * @typedef controller_parameter_t
 * @struct controller_parameter_t
 */
typedef struct controller_parameter_t{
    // General controller parameters
    controller_mode_t mode;         /**< Current controller mode */
    float feed_fwd;                 /**< Current feed forward value set by startup controller */
    uint32_t setpoint;              /**< Current setpoint */
    uint16_t speed_table[127];      /**< Array with setpoint values corresponding to every speed step */
    // Startup controller parameters
    startup_parameters_t startup;   /**< Struct for startup controller specific variables */
    // PID controller parameters
    pid_parameters_t pid;            /**< Struct for PID controller specific variables */
    // Measurement variables
    float measurement;              /**< Latest measurement value */ 
    float measurement_prev;         /**< Previous measurement value */ 
    float measurement_corrected;    /**< Corrected measurement value (measurement - adc_offset = measurement_corrected) */ 
    float adc_offset;               /**< ADC offset value */ 
    uint8_t msr_delay_in_us;        /**< Delay before V_EMF is measured */ 
    uint8_t msr_total_iterations;   /**< Amount of samples */ 
    uint8_t l_side_arr_cutoff;      /**< Discarded outlier samples (left side) */ 
    uint8_t r_side_arr_cutoff;      /**< Discarded outlier samples (right side) */ 
} controller_parameter_t;


/**
 * @brief Measures Back-EMF voltage (proportional to the rotational speed of the motor) on GPIO 28 and GPIO 29 respectively (depending on direction).
 *
 * This function performs multiple ADC measurements, sorts the results using insertion sort, and calculates the average of the middle values,
 * discarding a specified number of the lowest and highest values.
 *
 * @param total_iterations The total number of ADC measurements to perform.
 * @param measurement_delay_us The delay in microseconds between the switch-off of the motor PWM and the measurement.
 * @param l_side_arr_cutoff The number of lowest ADC values to discard.
 * @param r_side_arr_cutoff The number of highest ADC values to discard.
 * @param direction The direction of the motor.
 * @return The average of the middle ADC values after discarding the specified number of lowest and highest values.
 */
float measure(uint8_t total_iterations,
              uint8_t measurement_delay_us,
              uint8_t l_side_arr_cutoff,
              uint8_t r_side_arr_cutoff,
              direction_t direction);


/**
 * @brief Get the speed step table index based on the speed step value.
 *
 * @param speed_step The speed step value.
 * @return The index in the speed step table.
 */
uint8_t get_speed_step_table_index_of_speed_step(uint8_t speed_step);

/**
 * @brief Repeating timer callback function called every x milliseconds to implement a time delay in acceleration or deceleration.
 * 
 * x is CV_175
 *
 * @param ctrl_par Pointer to the controller parameter structure.
 */
void speed_helper(controller_parameter_t * ctrl_par);

/**
 * @brief Function to adjust the PWM level/duty cycle.
 *
 * @param level The new PWM level.
 */
void adjust_pwm_level(uint16_t level);

/**
 * @brief Function to calculate and return the proportional gain based on the speed step.
 *
 * This is done in order to make kp dependent on the speed step (gain scheduling).
 * 
 * @param ctrl_par Pointer to the controller parameter structure.
 * @return The proportional gain.
 */
float get_kp(const controller_parameter_t *ctrl_par);

/**
 * @brief Calculate and return the initial PWM level.
 * 
 * Calculation is done by taking the last 16 initial levels (when available) and calculating the average.
 * The average is the multiplied by a factor of 2/3.
 *
 * @param ctrl_par Pointer to the controller parameter structure.
 * @return The initial PWM level.
 */
uint16_t get_initial_level(controller_parameter_t * ctrl_par);

/**
 * @brief Controller function for startup mode.
 * 
 * This is called inside the controller_general() function when the controller mode is set to STARTUP_MODE.
 *
 * @param ctrl_par Pointer to the controller parameter structure.
 */
void controller_startup_mode(controller_parameter_t * ctrl_par);

/**
 * @brief Controller function for PID mode.
 * 
 * This is called inside the controller_general() function when the controller mode is set to PID_MODE.
 *
 * @param ctrl_par Pointer to the controller parameter structure.
 */
void controller_pid_mode(controller_parameter_t *const ctrl_par);

/**
 * @brief General controller function called every x milliseconds.
 *
 * @param ctrl_par Pointer to the controller parameter structure.
 */
void controller_general(controller_parameter_t * ctrl_par);

/**
 * @brief Initialize controller variables, measurement parameters, and speed table.
 *
 * @param ctrl_par Pointer to the controller parameter structure.
 */
void init_controller(controller_parameter_t * ctrl_par);