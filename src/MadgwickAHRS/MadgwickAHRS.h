//=====================================================================================================
// MadgwickAHRS.h
//=====================================================================================================
//
// Implementation of Madgwick's IMU and AHRS algorithms.
// See: http://www.x-io.co.uk/node/8#open_source_ahrs_and_imu_algorithms
//
// Date			Author          Notes
// 29/09/2011	SOH Madgwick    Initial release
// 02/10/2011	SOH Madgwick	Optimised for reduced CPU load
//
//=====================================================================================================
#ifndef MadgwickAHRS_h
#define MadgwickAHRS_h

//----------------------------------------------------------------------------------------------------
// Variable declaration

extern volatile float beta;				// algorithm gain
extern volatile float q0, q1, q2, q3;	// quaternion of sensor frame relative to auxiliary frame

//---------------------------------------------------------------------------------------------------
// Function declarations

/// @brief Algorithm AHRS update method using gyroscope, accelerometer, and magnetometer
/// @param gx Gyroscope x axis measurement in radians/s.
/// @param gy Gyroscope y axis measurement in radians/s.
/// @param gz Gyroscope z axis measurement in radians/s.
/// @param ax Accelerometer x axis measurement in any calibrated units.
/// @param ay Accelerometer y axis measurement in any calibrated units.
/// @param az Accelerometer z axis measurement in any calibrated units.
/// @param mx Magnetometer x axis measurement in any calibrated units.
/// @param my Magnetometer y axis measurement in any calibrated units.
/// @param mz Magnetometer z axis measurement in any calibrated units.
void MadgwickAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
void MadgwickAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az);

#endif
//=====================================================================================================
// End of file
//=====================================================================================================
