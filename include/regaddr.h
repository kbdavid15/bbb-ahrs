/* HMC5883L - 3-Axis Digital Compass IC
 * Connected via I2C
 * Register Definitions
 */
#define HMC_DEVICE_ADDRESS	0x1E
#define HMC_READ_ADDRESS 	0x3D
#define HMC_WRITE_ADDRESS 	0x3C
#define HMC_CONFIG_REG_A	0x00
#define HMC_CONFIG_REG_B	0x01
#define HMC_MODE_REG		0x02
#define HMC_DATA_2B_START_X	0x03
#define HMC_DATA_2B_START_Y	0x05
#define HMC_DATA_2B_START_Z	0x07
#define HMC_STATUS_REG		0x09
#define HMC_ID_REG_A		0x0A
#define HMC_ID_REG_B		0x0B
#define HMC_ID_REG_C		0x0C



/* ADXL345 - Digital Accelerometer
 * Connected to SPI0_0
 * Register Definitions
 */
#define ADXL_THRESH_TAP		0x1D
#define ADXL_OFFSET_X		0x1E
#define ADXL_OFFSET_Y		0x1F
#define ADXL_OFFSET_Z		0x20
#define ADXL_TAP_DURATION	0x21
#define ADXL_TAP_LATENCY	0x22
#define ADXL_TAP_WINDOW		0x23
#define ADXL_THRESH_ACT		0x24
#define ADXL_THRESH_INACT	0x25
#define ADXL_TIME_INACT		0x26
#define ADXL_ACT_INACT_CTL	0x27
#define ADXL_THRESH_FF		0x28
#define ADXL_TIME_FF		0x29
#define ADXL_TAP_AXES		0x2A
#define ADXL_ACT_TAP_STATUS	0x2B
#define ADXL_BW_RATE		0x2C
#define ADXL_POWER_CTL		0x2D
#define ADXL_INT_ENABLE		0x2E
#define ADXL_INT_MAP		0x2F
#define ADXL_INT_SOURCE		0x30
#define ADXL_DATA_FORMAT	0x31
#define ADXL_DATAX0			0x32
#define ADXL_DATAX1			0x33
#define ADXL_DATAY0			0x34
#define ADXL_DATAY1			0x35
#define ADXL_DATAZ0			0x36
#define ADXL_DATAZ1			0x37
#define ADXL_FIFO_CTL		0x38
#define ADXL_FIFO_STATUS	0x39

/* L3G4200D - 3-Axis Digital Gyroscope
 * Connected to SPI1_0
 * Register Definitions
 */

