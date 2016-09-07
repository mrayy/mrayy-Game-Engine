/*
 
 Copyright 2015-2016 Sendyne Corp., New York, USA
 http://www.sendyne.com
 
 C library for FTDI-based serial communication with Sendyne's SFP products.

 Authors:
    Damian Glinojecki (Sendyne Corp.)
    Nicolas Clauvelin (Sendyne Corp.)
 
 File:
    SFP10X_COM.h
 
 Abstract:
    The SFP10X_COM library defines a set of functions to facilitate serial
    communication with SFP products (for more details on SFP products refer to
    the relevant datasheets).
 
    Sendyne SFP product family:
    http://sendyne.com/Products/Sendyne%20Sensing%20Family.html
 
    Refer to the README.md file for usage instructions.
 
    The SFP10X_COM libary requires the FTDI FTD2XX driver library:
    http://www.ftdichip.com/Drivers/D2XX.htm
 
 GitHub project page:
    http://github.com/sendyne
 
 THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.
 
*/


#ifndef SFP10X_COM_LIB
#define SFP10X_COM_LIB


#include <windows.h>
// FTDI D2XX library header.
#include "FTD2XX.H"


// Byte type definition.
typedef unsigned char byte;


// Data structure for storing device number and communication handle to the
// device.
typedef struct SFPDevice_
{
    FT_HANDLE sfp_handle;   // Communication handle.
    int sfp_device_num;     // Device number.
} SFPDevice;


// Enumeration type for operation status.
enum Status
{
    RESERVED,			// 0x00 - Reserved for future use.
    SFP_OK,				// 0x01 - Success return code.
    PORT_FAIL,			// 0x02 - Port has failed to open or close.
    BAUD_FAIL,			// 0x03 - Changing baudrate failed. 
    DATA_CH_FAIL,		// 0x04 - Port charactarization - stop bits,
                            // partity and etc failed to be set.
    WRITE_FAIL,			// 0x05 - Write to the SFP module has failed.
    READ_FAIL,			// 0x06 - Reading the SFP module failed.
    CRC_ERROR,			// 0x07	- CRC did not pass.
    BYTES_INVALID,		// 0x08 - Returned number of bytes did not match 
                            // the expected number of bytes.
    RESPONSE_TIMEOUT,	// 0x09 - Response not recieved within the allowed
                            // time frame.
    DEVICE_BUSY,		// 0x0A - The requested device is taken or in an 
                            // unknown state.
    MEM_FAIL            // 0x0B - Memory allocation error.
};


// Enumeration type for data transaction size.
enum DataLength
{
    BYTES_1 = 0x00,		// Transaction with one byte.
    BYTES_2 = 0x01,		// Transaction with two bytes.
    BYTES_3 = 0x02,		// Transaction with three bytes.
    BYTES_6 = 0x03		// Transaction with six bytes.
};


// Enumeration type for baudrate selection.
enum Baudrate
{
    SFP_BAUD_9600 = 0x00,	// Baudrate 9600.
    SFP_BAUD_19200 = 0x01,	// Baudrate 19200.
    SFP_BAUD_115200 = 0x02	// Baudrate 115200.
};


/** Flag lookup function.
 *
 *	Accepts         a status flag.
 *
 *	flag            unsigned char in hex (see Status enum).
 *
 *	Returns         char array containing the description of the flag.
 */
char* FlagLookup(byte flag);


/** Initializes the communication with a specified device number.
 *
 *	Accepts         a device number and a SFPDevice pointer.
 *
 *	device_number   is the index number of the FTDI device as returned
 *                  by the FTDI library i.e. the GetFTDIDeviceCount() function.
 *
 *	sfp_dev         is an allocated SFPDevice structure pointer which is
 *                  initialized upon return.
 *
 *	Returns         status flag.
 */
byte Initialize(int device_num, SFPDevice * sfp_dev);


/** Changes the timeout time for write and read to and from the device. 
 *
 *	Accepts         SFPDevice pointer and new timeout value in milliseconds.
 *
 *	time_ms         is new timeout in milliseconds.
 *
 *	Returns         status flag.
 *
 *	The default timeout is 20ms as per FTDI specs. This can be increased
 *	or decreased. Be mindful that if the timeout is too low, the data
 *	might never be sent/read properly.
 */
byte ChangeTimeout(SFPDevice * device, int time_ms);


/** Reads data from a specific register on the SFP module.
 *
 *	Accepts         SFPDevice pointer, register address, number of bytes
 *                  requested, and a char array.
 *
 *	SFP_reg_address SFP register address to be passed-in. The register addresses
 *                  are to be taken from the datasheet for the module.
 *
 *	number_of_bytes the number of bytes to be requested from the SFP module, the
 *                  definitions can be found under the DataLength enum.
 *
 *	data            character array of length 8.
 *
 *	Returns         status flag.
 */
byte ReadRegister(SFPDevice * device,
                  byte SFP_reg_address,
                  byte number_of_bytes,
                  char * const data);


/** Reads data from a specific register on the SFP module with conversion.
*
*	Accepts         SFPDevice pointer, register address and number of bytes
*                   requested.
*
*	SFP_reg_address SFP register address to be passed-in. The register addresses
*                   are to be taken from the datasheet for the module.
*
*	number_of_bytes the number of bytes to be requested from the SFP module, the
*                   definitions can be found under the DataLength enum.
*
*	Returns         Signed data, in counts.
*/
byte ReadSignedRegister(SFPDevice * device,
                        byte SFP_reg_address,
                        byte number_of_bytes,
                        long long * signed_data);


/** Writes to a specific register on the SFP module.
 *
 *	Accepts         SFPDevice pointer, register address, number of bytes
 *                  requested, and a chara array with data to be written to the
 *                  specified register.
 *
 *	SFP_reg_address SFP register address to be passed-in. The register addresses
 *                  are to be taken from the datasheet for the module.
 *
 *	number_of_bytes the number of bytes to be requested from the SFP module, the
 *                  definitions can be found under the DataLength enum.
 *
 *	data            character array of length 8.
 *
 *	Returns         status flag.
 */
byte WriteRegister(SFPDevice * device,
                   byte SFP_reg_address,
                   byte number_of_bytes,
                   char * const data);


/** Changes the baudrate on the host and on the SFP module.
 *
 *	Accepts         SFPDevice pointer and new baudrate.
 *
 *	baud_rate       requested baudrate passed in as HEX. Definitions can
 *                  be found under the Baudrate enum.
 *
 *	Returns         status flag.
 */
byte ChangeBaudRate(SFPDevice * device, byte baud_rate);


/** Changes only the baud rate of the host.
 *
 *	Accepts         SFPDevice pointer and new baudrate.
 *
 *	baud_rate       requested baudrate passed in as HEX. Definitions can be
 *                  found under the Baudrate enum.
 *
 *	Returns         status flag.
 *
 *	This enables the user to change the baudrate on the host in case the
 *	SFP module is running on a non-default rate. This allows for the SFP
 *	module to be "recovered" without resetting it.
 */
byte ChangeOnlyHostBaudRate(SFPDevice * device, byte baud_rate);


/** Closes the open port.
 *
 *	Accepts         SFPDevice pointer.
 *
 *	Returns         status flag.
 *
 *  If a port is open, it closes it, else it does nothing and returns an error.
 */
byte ClosePort(SFPDevice * device);


/** Gets the number of FTDI devices available on the host.
 *
 *	Returns         number of devices or -1 if an error occured. 
 *
 *  Querries the FTDI drivers for the number of FTDI devices connected to the
 *  system.
 */
int GetFTDIDeviceCount();


/** Gets the device status/serial.
 *
 *	Accepts         device number and a char array.
 *
 *	m_device_num    the device number to be looked up. This number should not be
 *                  greater than the overall number of devices as returned by
 *                  the GetFTDIDeviceCount() function.
 *
 *	buffer          a character array of length 256 containing information about
 *                  the device.
 *                  If busy - returns busy else returns serial number of the
 *                  device as an array of characters.
 *
 *  Returns         status flag. 
 *
 */
byte GetFTDIDeviceInfo(int device_num, char *  buffer);


#endif  // SFP10X_COM_LIB
