#include <p30f6014A.h>
#include <libpic30.h>
#include "collection.h"
#include "timer.h"
#include "adc.h"
#include <math.h>
#include "gpio.h"
#include "usart.h"


extern unsigned int cur_gmma_cnt;

unsigned char Collectflag;
static unsigned char Tempselcnt = 0;
unsigned char DataUpBuf[35];
/**************************************/
unsigned int CableheadVoltage;
unsigned int Tension;
unsigned char Drilling;
unsigned int LMotorVoltage;
unsigned char Heartlength;
unsigned int Oillevel;
unsigned int MCdownTemperature;
unsigned char steppingmotortemp;
unsigned char Ten7538temp;
unsigned char Powertemp;
unsigned char Lmotortemp;
unsigned char Pumpoiltemp;
unsigned int Pushingpressure;
unsigned int LPumppressure;
unsigned int SPumppressure;

void collection (void)
{
    if(Collectflag == 1)
    {
        DataUpBuf[3] = cur_gmma_cnt>>8;
        DataUpBuf[2] = cur_gmma_cnt;

        if(Tempselcnt == 0)
        {
            Adc_Channel_SW(6);                 
            Adc_Channel6_Channel_SW(1);       //stepping motor temp
            steppingmotortemp = GetTemperature(2);//channel 2 PT1000
            DataUpBuf[11] = steppingmotortemp;
            Tempselcnt++;
        }
        else if(Tempselcnt == 1)
        {
            Adc_Channel_SW(6);
            Adc_Channel6_Channel_SW(2);       //3981 temp
            Ten7538temp = GetTemperature(5);   //channel 5 PT500
            DataUpBuf[12] = Ten7538temp;
            Tempselcnt++;
        }
        else if(Tempselcnt == 2)
        {
            Adc_Channel_SW(6);
            Adc_Channel6_Channel_SW(3);       //Power temp
            Powertemp = GetTemperature(2);
            DataUpBuf[13] = Powertemp;
            Tempselcnt++;
        }
        else if(Tempselcnt == 3)
        {
            Adc_Channel_SW(6);
            Adc_Channel6_Channel_SW(4);       //LMotor temp
            Lmotortemp = GetTemperature(2);
            DataUpBuf[14] = Lmotortemp;
            Tempselcnt++;
        }
        else if(Tempselcnt == 4)
        {
            Adc_Channel_SW(6);
            Adc_Channel6_Channel_SW(5);       //Pump oil temp
            Pumpoiltemp = GetTemperature(2);
            DataUpBuf[15] = Pumpoiltemp;
            Tempselcnt = 0;
        }

        Adc_Channel_SW(1);                      //Cable head AC voltage
        CableheadVoltage = GetCableHeadVoltage();
        DataUpBuf[5] = CableheadVoltage>>8;
        DataUpBuf[4] = CableheadVoltage;

        Adc_Channel_SW(3);                      //Tension
        Tension = Get_Tension();
        DataUpBuf[7] = Tension>>8;
        DataUpBuf[6] = Tension;
       // TenOrMotorAC = 1;

        Adc_Channel_SW(5);                      //Cable head AC voltage
        LMotorVoltage = GetLMotorAcVoltage();
        DataUpBuf[10] = LMotorVoltage>>8;
        DataUpBuf[9] = LMotorVoltage;
        //TenOrMotorAC = 0;
        
        Adc_Channel_SW(6);
        Adc_Channel6_Channel_SW(6);           //Pushing pressure
        Pushingpressure = Get_PumpPressure();
        DataUpBuf[17] = Pushingpressure>>8;
        DataUpBuf[16] = Pushingpressure;

        Adc_Channel_SW(6);
        Adc_Channel6_Channel_SW(7);           //Large Pump pressure
        LPumppressure = Get_PumpPressure();
        DataUpBuf[19] = LPumppressure>>8;
        DataUpBuf[18] = LPumppressure;

        Adc_Channel_SW(6);
        Adc_Channel6_Channel_SW(8);           //Small Pump pressure
        SPumppressure = Get_PumpPressure();
        DataUpBuf[21] = SPumppressure>>8;
        DataUpBuf[20] = SPumppressure;

        
        //delay_us(1000);
        Adc_Channel_SW(4);                     //Drilling displacement
        Drilling = Get_Displacement(0);
        DataUpBuf[8] = Drilling;

        Adc_Channel_SW(7);                     //heart length displacement
        Heartlength = Get_Displacement(1);
        DataUpBuf[22] = Heartlength;

        Adc_Channel_SW(8);                     //oil level displacement
        Oillevel = Get_Displacement(2);
        //DataUpBuf[24] = 0xff;
        DataUpBuf[24] = Oillevel>>8;
        DataUpBuf[23] = Oillevel;
        
        MCdownTemperature = GetMCdownTemperature(); //Board temp
        DataUpBuf[1] = MCdownTemperature;
        
        Collectflag = 0;                            //Collect complete
    }
}
/*****************************************************Get temperature*****************************************************************/

float const RTD_TAB_PT1000[260]=
{
  1000.000, 1003.908, 1007.814, 1011.720, 1015.624, 1019.527, 1023.429, 1027.330, 1031.229, 1035.128, //0-9
  1039.025, 1042.921, 1046.816, 1050.710, 1054.603, 1058.495, 1062.385, 1066.274, 1070.162, 1074.049, //10-19
  1077.935, 1081.820, 1085.703, 1089.585, 1093.467, 1097.347, 1101.225, 1105.103, 1108.980, 1112.855, //20-29
  1116.729, 1120.602, 1124.474, 1128.345, 1132.215, 1136.083, 1139.950, 1143.817, 1147.681, 1151.545, //30-39
  1155.408, 1159.270, 1163.130, 1166.989, 1170.847, 1174.704, 1178.560, 1182.414, 1186.268, 1190.120, //40-49
  1193.971, 1197.821, 1201.670, 1205.518, 1209.364, 1213.210, 1217.054, 1220.897, 1224.739, 1228.579, //50-59
  1232.419, 1236.257, 1240.095, 1243.931, 1247.766, 1251.600, 1255.432, 1259.264, 1263.094, 1266.923, //60-69
  1270.751, 1274.578, 1278.404, 1282.228, 1286.052, 1289.874, 1293.695, 1297.515, 1301.334, 1305.152, //70-79
  1308.968, 1312.783, 1316.597, 1320.411, 1324.222, 1328.033, 1331.843, 1335.651, 1339.458, 1343.264, //80-89
  1347.069, 1350.873, 1354.676, 1358.477, 1362.277, 1366.077, 1369.875, 1373.671, 1377.467, 1381.262, //90-99
  1385.055, 1388.847, 1392.638, 1396.428, 1400.217, 1404.005, 1407.791, 1411.576, 1415.360, 1419.143, //100-109
  1422.925, 1426.706, 1430.485, 1434.264, 1438.041, 1441.817, 1445.592, 1449.366, 1453.138, 1456.910, //110-119
  1460.680, 1464.449, 1468.217, 1471.984, 1475.750, 1479.514, 1483.277, 1487.040, 1490.801, 1494.561, //120-129
  1498.319, 1502.077, 1505.833, 1509.589, 1513.343, 1517.096, 1520.847, 1524.598, 1528.381, 1532.139, //130-139
  1535.843, 1539.589, 1543.334, 1547.078, 1550.820, 1554.562, 1558.302, 1562.041, 1565.779, 1569.516, //140-149
  1573.251, 1576.986, 1580.719, 1584.451, 1588.182, 1591.912, 1595.641, 1599.368, 1603.094, 1606.820, //150-159
  1610.544, 1614.267, 1617.989, 1621.709, 1625.429, 1629.147, 1632.864, 1636.580, 1640.295, 1644.009, //160-169
  1647.721, 1651.433, 1655.143, 1658.852, 1662.560, 1666.267, 1669.972, 1673.677, 1677.380, 1681.082, //170-179
  1684.783, 1688.483, 1692.181, 1695.879, 1699.575, 1703.271, 1706.965, 1710.658, 1714.349, 1718.040, //180-189
  1721.729, 1725.418, 1729.105, 1732.791, 1736.475, 1740.159, 1743.842, 1747.523, 1751.203, 1754.882, //190-199
  1758.560, 1762.237, 1765.912, 1769.587, 1773.260, 1776.932, 1780.603, 1784.273, 1787.941, 1791.610, //200-209
  1795.275, 1798.940, 1802.604, 1806.267, 1809.929, 1813.590, 1817.249, 1820.907, 1824.564, 1828.220, //210-219
  1831.875, 1835.529, 1839.181, 1842.832, 1846.483, 1850.132, 1853.779, 1857.426, 1861.072, 1864.716, //220-229
  1868.359, 1872.001, 1875.642, 1879.282, 1882.921, 1886.558, 1890.194, 1893.830, 1897.463, 1901.096, //230-239
  1904.728, 1908.359, 1911.988, 1915.616, 1919.243, 1922.869, 1926.494, 1930.117, 1933.740, 1937.361, //240-249
  1940.981, 1944.600, 1948.218, 1951.835, 1955.450, 1959.065, 1962.678, 1966.290, 1969.901, 1973.510, //250-259
};
float const RTD_TAB_PT500[460] =
{
403.52, 405.50, 407.49, 409.47, 411.45, 413.43, 415.41, 417.39, 419.37, 421.35,//-40
423.33, 425.31, 427.29, 429.26, 431.24, 433.21, 435.19,	437.16,	439.14,	441.11,//-30
443.08,	445.05,	447.02,	448.99,	450.96,	452.93,	454.90,	456.87,	458.83,	460.80,//-20
462.77,	464.73,	466.69,	468.66,	470.62,	472.58,	474.55,	476.51,	478.47,	480.43,//-10
482.39,	484.35,	486.31,	488.26,	490.22,	492.18,	494.13,	496.09,	498.05,	500.00,//0
501.95,	503.91,	505.86,	507.81,	509.76,	511.71,	513.66,	515.61,	517.56,	519.51,//10
521.46,	523.41,	525.36,	527.30,	529.25,	531.19,	533.14,	535.08,	537.02,	538.97,//20
540.91,	542.85,	544.79,	546.73,	548.67,	550.61,	552.55,	554.49,	556.43,	558.36,//30
560.30,	562.24,	564.17,	566.11,	568.04,	569.98,	571.91,	573.84,	575.77,	577.70,//40
579.63,	581.56,	583.49,	585.42,	587.35,	589.28,	591.21,	593.13,	595.06,	596.99,//50
598.91,	600.84,	602.76,	604.68,	606.60,	608.53,	610.45,	612.37,	614.29,	616.21,//60
618.13,	620.05,	621.97,	623.88,	625.80,	627.72,	629.63,	631.55,	633.46,	635.38,//70
637.29,	639.20,	641.11,	643.03,	644.94,	646.85,	648.76,	650.67,	652.58,	654.48,//80
656.39,	658.30,	660.21,	662.11,	664.02,	665.92,	667.83,	669.73,	671.63,	673.53,//90
675.44,	677.34,	679.24,	681.14,	683.04,	684.94,	686.84,	688.73,	690.63,	692.53,//100
694.42,	696.32,	698.21,	700.11,	702.00,	703.90,	705.79,	707.68,	709.57,	711.46,//110
        
713.35,	715.24,	717.13,	719.02,	720.91,	722.80,	724.68,	726.57,	728.45,	730.34,//120
732.22,	734.11,	735.99,	737.87,	739.76,	741.64,	743.52,	745.40,	747.28,	749.16,//130
751.04,	752.92,	754.79,	756.67,	758.55,	760.42,	762.30,	764.17,	766.05,	767.92,//140
769.79,	771.67,	773.54,	775.41,	777.28,	779.15,	781.02,	782.89,	784.76,	786.63,//150
788.49,	790.36,	792.23,	794.09,	795.96,	797.82,	799.68,	801.55,	803.41,	805.27,//160
807.13,	808.99,	810.85,	812.71,	814.57,	816.43,	818.29,	820.15,	822.00,	823.86,//170
825.72,	827.57,	829.43,	831.28,	833.13,	834.99,	836.84,	838.69,	840.54,	842.39,//180
844.24,	846.09,	847.94,	849.79,	851.64,	853.48,	855.33,	857.17,	859.02,	860.86,//190
862.71,	864.55,	866.40,	868.24,	870.08,	871.92,	873.76,	875.60,	877.44,	879.28,//200
881.12,	882.96,	884.79,	886.63,	888.47,	890.30,	892.14,	893.97,	895.80,	897.64,//210
899.47,	901.30,	903.13,	904.96,	906.79,	908.62,	910.45,	912.28,	914.11,	915.94,//220
917.76,	919.59,	921.42,	923.24,	925.07,	926.89,	928.71,	930.54,	932.36,	934.18,//230
936.00,	937.82,	939.64,	941.46,	943.28,	945.10,	946.91,	948.73,	950.55,	952.36,//240
954.18,	955.99,	957.81,	959.62,	961.43,	963.25,	965.06,	966.87,	968.68,	970.49,//250
972.30,	974.11,	975.92,	977.73,	979.53,	981.34,	983.14,	984.95,	986.76,	988.56,//260
990.36,	992.17,	993.97,	995.77,	997.57,	999.37,	1001.17, 1002.97, 1004.77, 1006.57,//270
1008.37, 1010.17, 1011.96, 1013.76, 1015.55, 1017.35, 1019.14, 1020.94, 1022.73, 1024.52,// 280
1026.32, 1028.11, 1029.90, 1031.69, 1033.48, 1035.27, 1037.06, 1038.85, 1040.63, 1042.42, //290
1044.21, 1045.99, 1047.78, 1049.56, 1051.35, 1053.13, 1054.91, 1056.69, 1058.48, 1060.26, //300
1062.04, 1063.82, 1065.60, 1067.38, 1069.15, 1070.93, 1072.71, 1074.49, 1076.26, 1078.04, //310
1079.81, 1081.59, 1083.36, 1085.13, 1086.91, 1088.68, 1090.45, 1092.22, 1093.99, 1095.76, //320
1097.53, 1099.30, 1101.07, 1102.83, 1104.60, 1106.37, 1108.13, 1109.90, 1111.66, 1113.42, //330
1115.19, 1116.95, 1118.71, 1120.47, 1122.24, 1124.00, 1125.76, 1127.51, 1129.27, 1131.03, //340
1132.79, 1134.55, 1136.30, 1138.06, 1139.81, 1141.57, 1143.32, 1145.08, 1146.83, 1148.58, //350
1150.33, 1152.08, 1153.83, 1155.58, 1157.33, 1159.08, 1160.83, 1162.58, 1164.33, 1166.07, //360
1167.82, 1169.56, 1171.31, 1173.05, 1174.80, 1176.54, 1178.28, 1180.02, 1181.76, 1183.51, //370
1185.25, 1186.99, 1188.72, 1190.46, 1192.20, 1193.94, 1195.67, 1197.41, 1199.15, 1200.88, //380
1202.62, 1204.35, 1206.08, 1207.82, 1209.55, 1211.28, 1213.01, 1214.74, 1216.47, 1218.20, //390
1219.93, 1221.66, 1223.38, 1225.11, 1226.84, 1228.56, 1230.29, 1232.01, 1233.74, 1235.46, //400
1237.18, 1238.91, 1240.63, 1242.35, 1244.07, 1245.79, 1247.51, 1249.23, 1250.94, 1252.66 //410
};

static int CalculateTemperature(float fR,unsigned char sel)
{
        int fTem;
        unsigned int Lefsel, Rigsel;
        
    if(sel == 5)                                //pt500 sel
    { 
        Lefsel = 0; 
        Rigsel = 459;
        while(Rigsel - Lefsel >1)                 // Dichotomy
        {
            int mid = (Lefsel + Rigsel)>>1;
            
            if(fR == RTD_TAB_PT500[mid])
            {
                return mid - 49;
                break;
            }
            else if(fR < RTD_TAB_PT500[mid])
            {
                Rigsel = mid;
            }
            else if(fR > RTD_TAB_PT500[mid])
            {
                Lefsel = mid;
            }
        }
        fTem = Lefsel - 49;
        return fTem;
    }                                                                                                                
    else
    {
        Lefsel = 0; 
        Rigsel = 259;
        while(Rigsel - Lefsel > 1)                 // Dichotomy
        {
            int mid = (Lefsel + Rigsel)>>1;
            
            if(fR == RTD_TAB_PT1000[mid])
            {
                return mid;
                break;
            }
            else if(fR < RTD_TAB_PT1000[mid])
            {
                Rigsel = mid;
            }
            else if(fR > RTD_TAB_PT1000[mid])
            {
                Lefsel = mid;
            }
        }
     
        fTem= Lefsel; 
        return fTem;
    }
}


#define ERROR_TEMPERATURE      255
#define A_plus_10_mV           10000
unsigned char GetTemperature(unsigned char sel)
{
    long sum=0;
    unsigned char temperature;
    unsigned int temperature_with_point;
    unsigned long int negative_in_x_1000,mV_mul,half_mV,negative_in_x_500;
    unsigned int m,i,j,t,val_buffer[32];
    unsigned long int output_x_1000; // mV
    float total_R;
   
    if(sel > 5 || sel < 1) {return ERROR_TEMPERATURE;}
    
    for(m = 0;m < 32;m ++)
	{
		val_buffer[m] = ads8320();
	}
    
	for(i=0;i<32;i++)
	{
		for(j=0;j<32-i-1;j++)
		{
			if(val_buffer[j]>val_buffer[j+1])
			{
				t = val_buffer[j];
				val_buffer[j] = val_buffer[j+1];
				val_buffer[j+1] = t;
			}
		}
	}
	sum = 0;
	for(i=8;i<24;i++)
	{
		sum += val_buffer[i] ; 
	}
	sum = sum >> 4;
    
    if(sel == 5)                                //PT500 sel
    {
        negative_in_x_500 = 927;

        output_x_1000 = sum;
        output_x_1000 *= 5000;
        output_x_1000 = output_x_1000>>16 ;     //reality voltage
        
        mV_mul = output_x_1000/6.86;            //ad8227 gain
        mV_mul += negative_in_x_500;

        if(mV_mul > A_plus_10_mV) {return ERROR_TEMPERATURE;}
        half_mV =  A_plus_10_mV - mV_mul;
            
        total_R = (float)half_mV/(float)mV_mul;
        total_R = 4990/total_R - 10;
        
////////        unsigned long test;
////////        test = total_R * 1000;
////////        
////////        ComSendChar(1,test>>16);
////////        ComSendChar(1,test>>8);
////////        ComSendChar(1,test);
        
        temperature_with_point = CalculateTemperature(total_R,sel);
        temperature = temperature_with_point;
    }
    else
    {
        negative_in_x_1000 = 1389;

        output_x_1000 = sum;
        output_x_1000 *= 5000;
        output_x_1000 = output_x_1000 >> 16;

        mV_mul = output_x_1000/5;
        mV_mul += negative_in_x_1000;

        if(mV_mul > A_plus_10_mV) {return ERROR_TEMPERATURE;}
        half_mV =  A_plus_10_mV - mV_mul;
        
////////        unsigned long test;
////////        test = total_R * 1000;
////////        
////////        ComSendChar(1,test>>16);
////////        ComSendChar(1,test>>8);
////////        ComSendChar(1,test);
       
        total_R = (float)half_mV/(float)mV_mul;
        total_R = 6200/total_R;
        
        temperature_with_point = CalculateTemperature(total_R,sel);
        temperature = temperature_with_point;
    }

    return temperature;
}

#define ERROR_MCTEMPERATURE      0
#define BridgeVlo               10000
int GetMCdownTemperature(void)
{
    unsigned char temperature;
    unsigned int temperature_with_point;
    unsigned long int negative_in_x_1000,mV_mul,half_mV;
    unsigned long int output_x_1000; // mV
    float total_R;
    
    unsigned int temp;  
    temp = ADC();
    
    negative_in_x_1000 = 1389;
    
    output_x_1000 = temp;
    output_x_1000 *= 5000;
    output_x_1000 = output_x_1000 >> 12;
    
    mV_mul = output_x_1000/5;
    mV_mul += negative_in_x_1000;
    
    if(mV_mul > BridgeVlo) {return ERROR_MCTEMPERATURE;}
    half_mV =  BridgeVlo - mV_mul;
    
    total_R = (float)half_mV/(float)mV_mul;
    total_R = 6200/total_R;
    
//        unsigned long test;
//        test = total_R * 1000;
//        
//        ComSendChar(1,test>>16);
//        ComSendChar(1,test>>8);
//        ComSendChar(1,test);
    
    temperature_with_point = CalculateTemperature(total_R,2);
    temperature = temperature_with_point;
    return temperature;
}
/******************************************************Get Cable head Ac Voltage**********************************************************/
unsigned long Sum_2;
int GetCableHeadVoltage(void)
{
    unsigned char  x=0,y;
    long cur_adc0[500];
    long sqr0;
    unsigned long int temp_value ;
    //GPIO_PIN_SET(TEST1_PORT,TEST1_PIN,1);
    temp_value = 0;
    for(x = 0;x < 100;x ++)
	{
		cur_adc0[x] = ads8320();
        delay_us(74);                      //sampling rate 200us
	}
    for(y=0;y<100;y++)
    {
		cur_adc0[y] = cur_adc0[y] * 5000;
		cur_adc0[y] = cur_adc0[y] >> 16;    //reality voltage
        
        sqr0 = cur_adc0[y] * cur_adc0[y];
        Sum_2 += sqr0;
    }
    Sum_2 = Sum_2 / 100;
    Sum_2 = sqrt(Sum_2);                   //root-mean-square
    Sum_2 = Sum_2 * 0.98333;
    temp_value = Sum_2;
    return (unsigned int)temp_value;
}


/******************************************************Get LMotor Ac Voltage**********************************************************/

unsigned int GetLMotorAcVoltage(void)
{
    unsigned int  x,y,z,a,b;
    long result;
    long cur_adc[250];
    long sum = 0;
    long sqr;
    unsigned int fcnt = 0;
    unsigned int pointcnt;
    unsigned char cnt = 0;
    unsigned int temp;
    long min = 0xffff;
    long max = 0;
    long mid;
    unsigned int acqpoint;
    
        delay_us(1000);
    for(x = 0;x < 250;x ++)     //80ms 15Hz-55Hz
	{
		cur_adc[x] = ads8320();
        delay_us(195);      //sampling rate 320us
    } 
    for(y = 0;y < 250;y ++)
    {
        if(min > cur_adc[y])
        {
            min = cur_adc[y];
        }
        if(max < cur_adc[y])
        {
            max = cur_adc[y];
        }
    }
    mid = (min + max)>>1;   //Center level
    
    for(z = 0;z < 250;z ++)
    {
        if(mid>=cur_adc[z-1] && mid<cur_adc[z])
        {
            cnt ++ ;
            pointcnt = z - fcnt;
            fcnt = z;	
            
        }
        else if(mid<=cur_adc[z-1] && mid>cur_adc[z])
        {
            cnt++;
            pointcnt = z - fcnt;
            fcnt = z;
        }
    }
    if(pointcnt!=0 && pointcnt<=125)
    {
        acqpoint = pointcnt*2;
        temp = 250/acqpoint;
        acqpoint = acqpoint*temp;
      if(pointcnt > 250)      //less than 12Hz
      {
          acqpoint = 250;
          for(b=0;b<acqpoint;b++)
          {
              cur_adc[b] = cur_adc[b] - mid;
              cur_adc[b] = cur_adc[b]*5000;
              cur_adc[b] = cur_adc[b]>>16;
              sqr = cur_adc[b] * cur_adc[b];
              sum += sqr;      
          }
      }
      else 
      {
          for(a=0;a<acqpoint;a++)
          {   
              cur_adc[a] = cur_adc[a] - mid;
              cur_adc[a] = cur_adc[a]*5000;
              cur_adc[a] = cur_adc[a]>>16;
              sqr = cur_adc[a] * cur_adc[a];
              sum += sqr;      
          }
      }
    }
    
    sum = sum/acqpoint;
    sum = sqrt(sum);        //root-mean-square
    sum = sum * 2.4178;
    if(sum < 100)sum=0;       
    result = sum;
    return result;
}

/******************************************************Get Tension**********************************************************/
/*long Tension_sample(void)
{
  	int i,m;
    long accum;
	int ntaps;
	const  double h[21] = {
		      181,    319,    558,    868,   1237,   1641,   2049,   2421,   2720,
		     2915,   2982,   2915,   2720,   2421,   2049,   1641,   1237,    868,
		      558,    319,    181
				};
	double z[21] = {0};

	ntaps = 21;

	for(i = 0;i < ntaps;i ++)
	{
		for(m = 0;m < 1050;m ++);
		z[i] =  ads8320();	
	}
    accum = 0;
    for (i = 0; i < ntaps; i++) 
    {
        accum += h[i] * z[i];
    }

    return accum;
}*/

int Get_Tension(void)
{
    unsigned int m,n;
    unsigned long asum = 0;
    long sum=0;
    int zl = 0;
    
    for(m = 0;m < 10;m ++);
	sum = 0;
    for(unsigned char i=0;i<64;i++)
    {
        n = ads8320();
        sum+=n;
    }
    asum = sum>>6;
    asum = asum *5000;
	zl = asum >> 16;
    
	return (zl);
}

/******************************************************Get PumpPressure**********************************************************/
int Get_PumpPressure(void)//,unsigned char sel
{
    long data;
    unsigned int m,n;
    long sum=0;
    sum = 0;
	//for(m = 0;m < 100;m ++);
	for(m = 0;m < 64;m ++)
	{
		n =  ads8320();
		sum +=n;		
	}
    data = sum >> 6;
////    data = data - 270;
////    data = data /46.8;
////    if(data > 200)
////    {
////        data = 0;
////    }
    return data;	
}

/******************************************************Get Displacement**********************************************************/

int Get_Displacement(unsigned char sel)
{
    int data;
    unsigned int m,n;
    long sum=0;
    
    
    sum = 0;
	for(m = 0;m < 100;m ++);
	for(m = 0;m < 64;m ++)
	{
		n =  ads8320();
		sum +=n;		
	}
	sum = sum >> 6;
    if(sel == 1) // core length 
    {
        data = (sum >> 8);
    }
    else if(sel == 0) // drill bit
    {
        data = (sum >> 8);
    }
    else if(sel == 2) // oil position 
    {
        data = (sum >> 7);
    }
    else
    {
        data = 12345;
    }
	
    return data;	
}

int Checksum (unsigned char *p,unsigned char len)
{
    unsigned int sum = 0;
    for(unsigned i=0;i<len;i++)
    {
        sum+=p[i];
        //ComSendChar(2,p[i]);
    }
    return sum;
}