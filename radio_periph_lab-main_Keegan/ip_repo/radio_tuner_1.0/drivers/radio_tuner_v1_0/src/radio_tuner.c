

/***************************** Include Files *******************************/
#include "radio_tuner.h"

/************************** Function Definitions ***************************/

void radioTuner_tuneRadio(u32 BaseAddress, float freq){

	long double intermediate=0;
	int pinc;

	//Fix this if statement
	if (freq==0){
		pinc = 0;
		printf("Setting TUNER frequency to %.2f / pinc = %d\n\r",freq,pinc);
		RADIO_TUNER_mWriteReg(BaseAddress, RADIO_TUNER_TUNER_REG_OFFSET, pinc );
		return;
	}

	intermediate = (long double)(1<<DDS_OUTPUT_WIDTH) / (long double)(CLK_FRQ) ;
	pinc = (int)((int)freq*intermediate);

	printf("Setting TUNER frequency to %.2f / pinc = %d\n\r",freq,pinc);
	RADIO_TUNER_mWriteReg(BaseAddress, RADIO_TUNER_TUNER_REG_OFFSET, pinc );

}

void radioTuner_setAdcFreq(u32 BaseAddress, float freq){

	long double intermediate=0;
	int pinc;

	//Fix this if statement
	if (freq==0){
		pinc = 0;
		printf("Setting ADC frequency to %.2f / pinc = %d\n\r",freq,pinc);
		RADIO_TUNER_mWriteReg(BaseAddress, RADIO_TUNER_ADC_REG_OFFSET, pinc );
		return;
	}

	intermediate = (long double)(1<<DDS_OUTPUT_WIDTH) / (long double)(CLK_FRQ) ;
	pinc = (int)(freq*intermediate);

	printf("Setting ADC frequency to %.2f / pinc = %d\n\r",freq,pinc);
	RADIO_TUNER_mWriteReg(BaseAddress, RADIO_TUNER_ADC_REG_OFFSET, pinc );

}

void radioTuner_controlReset(u32 BaseAddress, u8 resetval){
	 RADIO_TUNER_mWriteReg(XPAR_RADIO_TUNER_0_S00_AXI_BASEADDR, RADIO_TUNER_ADC_RST_REG_OFFSET, 0 ); //Set reset low
	 RADIO_TUNER_mWriteReg(XPAR_RADIO_TUNER_0_S00_AXI_BASEADDR, RADIO_TUNER_ADC_RST_REG_OFFSET, 1 ); //Set reset high
}