/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l1xx_hal.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "max30102.h"
#include "algorithm.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
uint32_t aun_ir_buffer[500]; //IR LED sensor data
uint16_t n_ir_buffer_length;    //data length
uint32_t aun_red_buffer[500];    //Red LED sensor data
int16_t n_sp02; //SPO2 value
int8_t ch_spo2_valid;   //indicator to show if the SP02 calculation is valid
int16_t n_heart_rate;   //heart rate value
int8_t  ch_hr_valid;    //indicator to show if the heart rate calculation is valid

#define MAX_BRIGHTNESS 255

/* USER CODE END 0 */

int main(void)
{

    /* USER CODE BEGIN 1 */
    uint32_t un_min, un_max;
    int i;
    uint8_t temp[6];
    uint8_t dis_hr=0,dis_spo2=0;
    /* USER CODE END 1 */

    /* MCU Configuration----------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_USART1_UART_Init();

    /* USER CODE BEGIN 2 */
    printf("System Running\r\n");
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    MAX_INT_H;
    max30102_init();

    printf("\r\n MAX30102  init  \r\n");
    un_min=0x3FFFF;
    un_max=0;

    n_ir_buffer_length=500; //buffer length of 100 stores 5 seconds of samples running at 100sps
    //read the first 500 samples, and determine the signal range
    for(i=0;i<n_ir_buffer_length;i++)
    {
        while(MAX_INT_read==1);   //wait until the interrupt pin asserts

        max30102_FIFO_ReadBytes(REG_FIFO_DATA,temp);
        aun_ir_buffer[i] =  (long)((long)((long)temp[0]&0x03)<<16) | (long)temp[1]<<8 | (long)temp[2];    // Combine values to get the actual number
        aun_red_buffer[i] = (long)((long)((long)temp[3] & 0x03)<<16) |(long)temp[4]<<8 | (long)temp[5];   // Combine values to get the actual number
        //      aun_ir_buffer[i]
        //maxim_max30102_read_fifo((aun_red_buffer+i), (aun_ir_buffer+i));  //read from MAX30102 FIFO
        if(un_min>aun_red_buffer[i])
            un_min=aun_red_buffer[i];    //update signal min
            if(un_max<aun_red_buffer[i])
                un_max=aun_red_buffer[i];    //update signal max
                printf("red=");
            printf("%i", aun_red_buffer[i]);
        printf(", ir=");
        printf("%i\n\r", aun_ir_buffer[i]);
    }
    //	un_prev_data=aun_red_buffer[i];
    //calculate heart rate and SpO2 after first 500 samples (first 5 seconds of samples)
    maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);

    while(1)
    {
        i=0;
        un_min=0x3FFFF;
        un_max=0;

        //dumping the first 100 sets of samples in the memory and shift the last 400 sets of samples to the top
        for(i=100;i<500;i++)
        {
            aun_red_buffer[i-100]=aun_red_buffer[i];
            aun_ir_buffer[i-100]=aun_ir_buffer[i];

            //update the signal min and max
            if(un_min>aun_red_buffer[i])
                un_min=aun_red_buffer[i];
            if(un_max<aun_red_buffer[i])
                un_max=aun_red_buffer[i];
        }
        //take 100 sets of samples before calculating the heart rate.
        for(i=400;i<500;i++)
        {
            while(MAX_INT_read==1);
            max30102_FIFO_ReadBytes(REG_FIFO_DATA,temp);
            aun_ir_buffer[i] =  (long)((long)((long)temp[0]&0x03)<<16) | (long)temp[1]<<8 | (long)temp[2];    // Combine values to get the actual number
            aun_red_buffer[i] = (long)((long)((long)temp[3] & 0x03)<<16) |(long)temp[4]<<8 | (long)temp[5];   // Combine values to get the actual number
            //send samples and calculation result to terminal program through UART
            if(ch_hr_valid == 1 && ch_spo2_valid ==1 && (n_heart_rate-100)<120 && n_sp02<101 && n_heart_rate>140)//**/ ch_hr_valid == 1 && ch_spo2_valid ==1 && (n_heart_rate-100)<120 && n_sp02<101
            {
                dis_hr = n_heart_rate-100;   //这个-100我加的
                dis_spo2 = n_sp02;
                printf("HR=%i, ", dis_hr);
                printf("HRvalid=%i, ", ch_hr_valid);
                printf("SpO2=%i, ", dis_spo2);
                printf("SPO2Valid=%i\r\n", ch_spo2_valid);
            }
            else
            {
                dis_hr = 0;
                dis_spo2 = 0;
            }

        }
        printf("HR=%i, ", dis_hr);
        printf("HRvalid=%i, ", ch_hr_valid);
        printf("SpO2=%i, ", dis_spo2);
        printf("SPO2Valid=%i\r\n", ch_spo2_valid);
        maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);


    }

    /* USER CODE END WHILE */


}
