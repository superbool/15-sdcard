/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
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
#include "fatfs.h"

uint8_t retSD;    /* Return value for SD */
char SDPath[4];   /* SD logical drive path */
FATFS SDFatFS;    /* File system object for SD logical drive */
FIL SDFile;       /* File object for SD */

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the SD driver ###########################*/
  retSD = FATFS_LinkDriver(&SD_Driver, SDPath);

  /* USER CODE BEGIN Init */
  /* additional user code for init */
  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
  // 通过RTC获取准确时间
    RTC_TimeTypeDef rTime;
    RTC_DateTypeDef rDate;
    DWORD time;
    HAL_RTC_GetTime(&hrtc, &rTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &rDate, RTC_FORMAT_BIN);

    time = (((DWORD)rDate.Year - 1980) << 25)
             | ((DWORD)rDate.Month << 21)
             | ((DWORD)rDate.Date << 16)
             | (WORD)(rTime.Hours << 11)
             | (WORD)(rTime.Minutes << 5)
             | (WORD)(rTime.Seconds >> 1);
    return time;
  /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */

/* USER CODE END Application */
