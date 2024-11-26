#ifndef __GPIO_H
#define __GPIO_H

#ifdef __cplusplus
	extern "C"{
#endif

        
typedef enum
{
    GPIO_a = 0,
    GPIO_b,
    GPIO_c,
    GPIO_d,
    GPIO_f,
    GPIO_g,
}e_IO_port;

typedef enum
{
    GPIO_input = 0,
    GPIO_output,
}e_IO_mode;

#define PIN_HIGH           1 
#define PIN_LOW            0

/**************************************************************************************************/
#define G_CONCAT_2_(__A, __B)     __A##__B
#define G_CONCAT_2(__A, __B)      G_CONCAT_2_(__A, __B)

#define o_GPIO_a       _LATA
#define i_GPIO_a       _RA

#define o_GPIO_b       _LATB
#define i_GPIO_b       _RB

#define o_GPIO_c       _LATC
#define i_GPIO_c       _RC

#define o_GPIO_d       _LATD
#define i_GPIO_d       _RD

#define o_GPIO_f       _LATF
#define i_GPIO_f       _RF

#define o_GPIO_g       _LATG
#define i_GPIO_g       _RG

#define GPIO_PIN_SET(_port,_pin,_value)       G_CONCAT_2(G_CONCAT_2(o_,_port),_pin) = _value
#define GPIO_PIN_READ(_port,_pin)             G_CONCAT_2(G_CONCAT_2(i_,_port),_pin)


	#define ADS8320_CLK_PORT					GPIO_c
	#define ADS8320_CLK_PIN						4

    #define ADS8320_CS_PORT                     GPIO_g
	#define ADS8320_CS_PIN						7

    #define ADS8320_IN_PORT                     GPIO_g
	#define ADS8320_IN_PIN						6

        
    #define ADC_CHANNEL_SW_A0_PORT              GPIO_b
    #define ADC_CHANNEL_SW_A0_PIN               15

    #define ADC_CHANNEL_SW_A1_PORT              GPIO_d
    #define ADC_CHANNEL_SW_A1_PIN               14

    #define ADC_CHANNEL_SW_A2_PORT              GPIO_d
    #define ADC_CHANNEL_SW_A2_PIN               15

    #define ADC_CHANNEL6_CHANNEL_SW_A0_PORT     GPIO_d
    #define ADC_CHANNEL6_CHANNEL_SW_A0_PIN      1

    #define ADC_CHANNEL6_CHANNEL_SW_A1_PORT     GPIO_d
    #define ADC_CHANNEL6_CHANNEL_SW_A1_PIN      3

    #define ADC_CHANNEL6_CHANNEL_SW_A2_PORT     GPIO_d
    #define ADC_CHANNEL6_CHANNEL_SW_A2_PIN      2



       
    #define TEST1_PORT						    GPIO_g
	#define TEST1_PIN						    15

    #define TEST2_PORT						    GPIO_g
	#define TEST2_PIN						    13

    #define TEST3_PORT						    GPIO_g
	#define TEST3_PIN						    12   



    #define TEMPERATURE_PORT					GPIO_b
	#define TEMPERATURE_PIN						3   


        

    #define MOTOR_EN_PORT                      GPIO_b
    #define MOTOR_EN_PIN                       12

    #define MOTOR_PLUSE_PORT                   GPIO_b
    #define MOTOR_PLUSE_PIN                    13

    #define MOTOR_DIRECTION_PORT               GPIO_b
    #define MOTOR_DIRECTION_PIN                14
        


#define  MODE_SW_PORT
#define  MODE_SW_PIN
        
#define  PUSH_PORT             GPIO_c
#define  PUSH_PIN              13

#define  MOTOR_RUN_STOP_PORT   GPIO_d
#define  MOTOR_RUN_STOP_PIN    9
        
#define  SPI1SS_PORT           GPIO_b
#define  SPI1SS_PIN            2

#define  SPI1MISO_PORT         GPIO_f
#define  SPI1MISO_PIN          8

#define  SPI1MOSI_PORT         GPIO_f
#define  SPI1MOSI_PIN          7

#define  SPI1SCK_PORT          GPIO_f
#define  SPI1SCK_PIN           6
#define  DRILL_PORT            GPIO_d
#define  DRILL_PIN             11

#define  GMMA_OPEN_CLOSE_PORT
#define  GMMA_OPEN_CLOSE_PIN
        
#define  GMMA_ONOFF_PORT        GPIO_d
#define  GMMA_ONOFF_PIN         12
#define  GMMA_OUT_PORT          GPIO_c
#define  GMMA_OUT_PIN           14


#define         TEST2_1                                         GPIO_PIN_SET(TEST2_PORT,TEST1_PIN,1)
#define         TEST2_0                                         GPIO_PIN_SET(TEST2_PORT,TEST1_PIN,0)

/**************************************************************************************************/

void GpioInit(void);
void GpioPinConfig(e_IO_port port,unsigned char pin,e_IO_mode mode);

#ifdef __cplusplus
		}
#endif
#endif
