
#ifndef RADIO_TUNER_H
#define RADIO_TUNER_H


/****************** Include Files ********************/
#include "xil_types.h"
#include "xstatus.h"
#include "xil_io.h"

#define RADIO_TUNER_S00_AXI_SLV_REG0_OFFSET 0
#define RADIO_TUNER_S00_AXI_SLV_REG1_OFFSET 4
#define RADIO_TUNER_S00_AXI_SLV_REG2_OFFSET 8
#define RADIO_TUNER_S00_AXI_SLV_REG3_OFFSET 12

#define RADIO_TUNER_ADC_REG_OFFSET 		RADIO_TUNER_S00_AXI_SLV_REG0_OFFSET
#define RADIO_TUNER_TUNER_REG_OFFSET 	RADIO_TUNER_S00_AXI_SLV_REG1_OFFSET
#define RADIO_TUNER_ADC_RST_REG_OFFSET 	RADIO_TUNER_S00_AXI_SLV_REG2_OFFSET
#define RADIO_TUNER_TIMER_REG_OFFSET 	RADIO_TUNER_S00_AXI_SLV_REG3_OFFSET

#define CLK_FRQ 125000000
#define DDS_OUTPUT_WIDTH   0x0000001b //27

/**************************** Type Definitions *****************************/

void radioTuner_tuneRadio(u32 BaseAddress, float freq);
void radioTuner_setAdcFreq(u32 BaseAddress, float freq);
void radioTuner_controlReset(u32 BaseAddress, u8 resetval);

/**
 *
 * Write a value to a RADIO_TUNER register. A 32 bit write is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is written.
 *
 * @param   BaseAddress is the base address of the RADIO_TUNERdevice.
 * @param   RegOffset is the register offset from the base to write to.
 * @param   Data is the data written to the register.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void RADIO_TUNER_mWriteReg(u32 BaseAddress, unsigned RegOffset, u32 Data)
 *
 */
#define RADIO_TUNER_mWriteReg(BaseAddress, RegOffset, Data) \
  	Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))

/**
 *
 * Read a value from a RADIO_TUNER register. A 32 bit read is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is read from the register. The most significant data
 * will be read as 0.
 *
 * @param   BaseAddress is the base address of the RADIO_TUNER device.
 * @param   RegOffset is the register offset from the base to write to.
 *
 * @return  Data is the data from the register.
 *
 * @note
 * C-style signature:
 * 	u32 RADIO_TUNER_mReadReg(u32 BaseAddress, unsigned RegOffset)
 *
 */
#define RADIO_TUNER_mReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))

/************************** Function Prototypes ****************************/
/**
 *
 * Run a self-test on the driver/device. Note this may be a destructive test if
 * resets of the device are performed.
 *
 * If the hardware system is not built correctly, this function may never
 * return to the caller.
 *
 * @param   baseaddr_p is the base address of the RADIO_TUNER instance to be worked on.
 *
 * @return
 *
 *    - XST_SUCCESS   if all self-test code passed
 *    - XST_FAILURE   if any self-test code failed
 *
 * @note    Caching must be turned off for this function to work.
 * @note    Self test may fail if data memory and device are not on the same bus.
 *
 */
XStatus RADIO_TUNER_Reg_SelfTest(void * baseaddr_p);

#endif // RADIO_TUNER_H
