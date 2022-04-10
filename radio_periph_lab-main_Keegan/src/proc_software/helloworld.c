#define RADIO_PERIPH_PRESENT

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xiic_l.h"
#include "xuartps.h"
#include "radio_tuner.h"

#define IIC_BASE_ADDRESS	XPAR_IIC_0_BASEADDR
#define CODEC_ADDRESS 26
#define UARTLITE_ADDR	   XPAR_PS7_UART_1_BASEADDR




#ifdef RADIO_PERIPH_PRESENT


void play_tune(u32 BaseAddress, float base_frequency)
{
	int i;
	float freqs[16] = {1760.0,1567.98,1396.91, 1318.51, 1174.66, 1318.51, 1396.91, 1567.98, 1760.0, 0, 1760.0, 0, 1760.0, 1975.53, 2093.0,0};
	float durations[16] = {1,1,1,1,1,1,1,1,.5,0.0001,.5,0.0001,1,1,2,0.0001};
	for (i=0;i<16;i++)
	{
		radioTuner_setAdcFreq(BaseAddress,freqs[i]+base_frequency);
		usleep((int)(durations[i]*500000));
	}
}
#endif


void write_codec_register(unsigned int codec_reg_num, unsigned int data)
{
	   unsigned char databuf[2];
	   databuf[0]=(codec_reg_num << 1) | ((data>>8)&0x01); // this puts the register address in the proper place and adds top bit of 9 bit data
	   databuf[1]=data&0xff;
	   XIic_Send(IIC_BASE_ADDRESS,	CODEC_ADDRESS,	databuf,2,XIIC_STOP);
}

void configure_codec()
{
	write_codec_register(15,0x00);  //Reset
    usleep(1000);
    write_codec_register(6,0x30);  // power on everything except OUT and OSC
    write_codec_register(0,0x17);
    write_codec_register(1,0x17);
    write_codec_register(2,0x79);
    write_codec_register(3,0x79);
    write_codec_register(4,0x10);
    write_codec_register(5,0x00);
    write_codec_register(7,0x02);
    write_codec_register(8,0x00);
    usleep(75000);
    write_codec_register(9,0x01);  // set active bit
    write_codec_register(6,0x00);  // set "out"
}


int main()
{
	u32 start_time,stop_time;
	int i;

    init_platform();
    print("\r\n\r\n\r\nLab 7 Brian Keegan - Custom Peripheral Demonstration\n\r");
    print("Type r to reset ADC_DDS, p to play tune,  and x to exit\r\n");
    print("Configuring Codec Now\r\n");
    configure_codec();
    radioTuner_controlReset(XPAR_RADIO_TUNER_0_S00_AXI_BASEADDR, 1);

while(XUartPs_RecvByte(UARTLITE_ADDR)!='x'){
    if( XUartPs_RecvByte(UARTLITE_ADDR)=='p' ) {
#ifdef RADIO_PERIPH_PRESENT
    print("Tuning Radio to 30MHz\n\r");
    radioTuner_tuneRadio(XPAR_RADIO_TUNER_0_S00_AXI_BASEADDR,30e6);
    print("Playing Tune at near 30MHz\r\n");
    play_tune(XPAR_RADIO_TUNER_0_S00_AXI_BASEADDR,30e6);

    // the below code does a little benchmark
    start_time = RADIO_TUNER_mReadReg(XPAR_RADIO_TUNER_0_S00_AXI_BASEADDR, RADIO_TUNER_TIMER_REG_OFFSET);
    for (i=0;i<2048;i++)
        stop_time = RADIO_TUNER_mReadReg(XPAR_RADIO_TUNER_0_S00_AXI_BASEADDR, RADIO_TUNER_TIMER_REG_OFFSET);
    printf("Elapsed time in clocks = %u\n",stop_time-start_time);
    float throughput=0;
    // please insert your code here for calculate the actual throughput in Mbytes/second
    // how much data was transferred? How long did it take?
    throughput = 2048*4/((stop_time-start_time)*8e-9*1e6);  // 2048 * u32bytes /( delta_time * 8ns * 1Mbyte)
  
    printf("Estimated Transfer throughput = %f Mbytes/sec\r\n",throughput);
#endif
    } else if (XUartPs_RecvByte(UARTLITE_ADDR)=='r'){
    	radioTuner_controlReset(XPAR_RADIO_TUNER_0_S00_AXI_BASEADDR, 1);
    }
}
    cleanup_platform();
    return 0;
}



