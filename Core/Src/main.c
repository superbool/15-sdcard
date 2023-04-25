/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "fatfs.h"
#include "rtc.h"
#include "sdio.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "log.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

//重定向printf函数
PUTCHAR_PROTOTYPE {
    HAL_UART_Transmit(&huart1, (uint8_t *) &ch, 1, 0xFFFF);//输出指向串口USART1
    return ch;
}

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

FATFS fs;                 // Work area (file system object) for logical drive
FIL fil;                  // file objects
uint32_t byteswritten;                /* File write counts */
uint32_t bytesread;                   /* File read counts */
uint8_t wtext[] = "This is STM32 working with FatFs\n"; /* File write buffer */
uint8_t rtext[200];                     /* File read buffers */
char filename[] = "2.txt";

RTC_DateTypeDef sdatestructure;
RTC_TimeTypeDef stimestructure;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void test_fs(void);

void print_time(void);

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void) {
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

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
    MX_DMA_Init();
    MX_USART1_UART_Init();
    MX_SDIO_SD_Init();
    MX_FATFS_Init();
    MX_RTC_Init();
    /* USER CODE BEGIN 2 */
    HAL_SD_Init(&hsd);
    HAL_SD_ConfigWideBusOperation(&hsd, SDIO_BUS_WIDE_4B);

    retSD = f_mount(&fs, "/", 1);
    log_init();




    /*
    log_set_level(0);
    log_set_quiet(0);

    FILE *fp1, *fp2;
    fp1 = fopen("log_info.txt", "ab");
    if (fp1 == NULL)
        return -1;

    fp2 = fopen("log_debug.txt", "ab");
    if (fp2 == NULL)
        return -1;

    log_add_fp(fp1, LOG_INFO);
    log_add_fp(fp2, LOG_DEBUG);

    log_debug("debug");
    log_info("info");
    log_warn("warn");

    fclose(fp2);
    fclose(fp1);
     */
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    uint32_t count = 0;

    while (1) {
        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
        //test_fs();
        //HAL_Delay(1000);
        log_info("count=%d", count);
        count++;
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
    */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 168;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */
//sd卡测试
void test_fs(void) {
    printf("\n ****** FatFs Example ******\n\n");
    print_time();
    /*##-1- Register the file system object to the FatFs module ##############*/
    BYTE work[1024];
    retSD = f_mount(&fs, "/", 1);
    /*----------------------- 格式化测试 ---------------------------*/
    /* 如果没有文件系统就格式化创建创建文件系统 */
    if (retSD == FR_NO_FILESYSTEM) {
        printf("f_mount error： FR_NO_FILESYSTEM\n");
        /* 格式化 */
        retSD = f_mkfs("/", 0, 0, work, sizeof(work));
        if (retSD == FR_OK) {
            printf("f_mkfs success\n");
            /* 格式化后，先取消挂载 */
            retSD = f_mount(NULL, "/", 1);
            /* 重新挂载 */
            retSD = f_mount(&fs, "/", 1);
            printf("f_mount success：%d\n", retSD);
        } else {
            printf("f_mkfs error: %d\n", retSD);
            //while (1);
        }
    } else if (retSD != FR_OK) {
        printf("f_mount error: %d\n", retSD);
        //while (1);
    } else {
        printf("f_mount success\n");
    }
    printf("\n");
    /*##-2- Create and Open new text file objects with write access ######*/
    retSD = f_open(&fil, filename, FA_OPEN_APPEND | FA_WRITE | FA_READ);
    if (retSD) {
        printf("w1 open file error : %d\n", retSD);
    } else {
        printf("w1 open file success\n");
    }

    /*##-3- Write data to the text files ###############################*/
    retSD = f_write(&fil, wtext, sizeof(wtext), (void *) &byteswritten);
    if (retSD) {
        printf("w2 write file error: %d\n", retSD);
    } else {
        printf("w2 write file success,data:%s\n", wtext);
    }

    /*##-4- Close the open text files ################################*/
    retSD = f_close(&fil);
    if (retSD) {
        printf("w3 close error: %d\n", retSD);
    } else {
        printf("w3 close success\n");
    }

    printf("\n");

    /*##-5- Open the text files object with read access ##############*/
    retSD = f_open(&fil, filename, FA_READ);
    if (retSD) {
        printf("r1 open file error: %d\n", retSD);
    } else {
        printf("r1 open file success\n");
    }

    /*##-6- Read data from the text files ##########################*/
    retSD = f_read(&fil, rtext, sizeof(rtext), (UINT *) &bytesread);
    if (retSD) {
        printf("r2 read error:%d\r\n", retSD);
    } else {
        printf("r2 read success,data:%s %c\n", rtext, rtext[33]);
    }

    /*##-7- Close the open text files ############################*/
    retSD = f_close(&fil);
    if (retSD) {
        printf("r3 close error:%d\n", retSD);
    } else {
        printf("r3 close success\n");
    }

    printf("\n");
    /*##-8- Compare read data with the expected data ############*/
    if (bytesread == byteswritten) {
        printf("FatFs is working well:%lu \n", bytesread);
    } else {
        printf("FatFs is working exp w:%lu r:%lu \n", byteswritten, bytesread);
    }

    /*
     * 9 unmount sdcard
     */
    retSD = f_mount(NULL, "/", 1);
    if (retSD) {
        printf("unmount error:%d\n", retSD);
    } else {
        printf("unmount success\n");
    }
    printf("\n");

}

void print_time() {
    //获取时间
    HAL_RTC_GetTime(&hrtc, &stimestructure, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &sdatestructure, RTC_FORMAT_BIN);
    /* Display date Format : yy/mm/dd */
    printf("%02d/%02d/%02d ", 2000 + sdatestructure.Year, sdatestructure.Month, sdatestructure.Date);
    /* Display time Format : hh:mm:ss */
    printf("%02d:%02d:%02d\n", stimestructure.Hours, stimestructure.Minutes, stimestructure.Seconds);
}

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    /* USER CODE BEGIN Callback 0 */

    /* USER CODE END Callback 0 */
    if (htim->Instance == TIM6) {
        HAL_IncTick();
    }
    /* USER CODE BEGIN Callback 1 */

    /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
