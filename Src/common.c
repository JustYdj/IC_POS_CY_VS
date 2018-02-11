/**
  ******************************************************************************
  * @file    IAP_Main/Src/common.c 
  * @author  MCD Application Team
  * @version V1.6.0
  * @date    12-May-2017
  * @brief   This file provides all the common functions.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 

/** @addtogroup STM32F1xx_IAP_Main
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "common.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Convert an Integer to a string
  * @param  p_str: The string output pointer
  * @param  intnum: The integer to be converted
  * @retval None
  */
void Int2Str(uint8_t *p_str, uint32_t intnum)
{
  uint32_t i, divider = 1000000000, pos = 0, status = 0;

  for (i = 0; i < 10; i++)
  {
    p_str[pos++] = (intnum / divider) + 48;

    intnum = intnum % divider;
    divider /= 10;
    if ((p_str[pos-1] == '0') & (status == 0))
    {
      pos = 0;
    }
    else
    {
      status++;
    }
  }
}

/**
  * @brief  Convert a string to an integer
  * @param  p_inputstr: The string to be converted
  * @param  p_intnum: The integer value
  * @retval 1: Correct
  *         0: Error
  */
uint32_t Str2Int(uint8_t *p_inputstr, uint32_t *p_intnum)
{
  uint32_t i = 0, res = 0;
  uint32_t val = 0;

  if ((p_inputstr[0] == '0') && ((p_inputstr[1] == 'x') || (p_inputstr[1] == 'X')))
  {
    i = 2;
    while ( ( i < 11 ) && ( p_inputstr[i] != '\0' ) )
    {
      if (ISVALIDHEX(p_inputstr[i]))
      {
        val = (val << 4) + CONVERTHEX(p_inputstr[i]);
      }
      else
      {
        /* Return 0, Invalid input */
        res = 0;
        break;
      }
      i++;
    }

    /* valid result */
    if (p_inputstr[i] == '\0')
    {
      *p_intnum = val;
      res = 1;
    }
  }
  else /* max 10-digit decimal input */
  {
    while ( ( i < 11 ) && ( res != 1 ) )
    {
      if (p_inputstr[i] == '\0')
      {
        *p_intnum = val;
        /* return 1 */
        res = 1;
      }
      else if (((p_inputstr[i] == 'k') || (p_inputstr[i] == 'K')) && (i > 0))
      {
        val = val << 10;
        *p_intnum = val;
        res = 1;
      }
      else if (((p_inputstr[i] == 'm') || (p_inputstr[i] == 'M')) && (i > 0))
      {
        val = val << 20;
        *p_intnum = val;
        res = 1;
      }
      else if (ISVALIDDEC(p_inputstr[i]))
      {
        val = val * 10 + CONVERTDEC(p_inputstr[i]);
      }
      else
      {
        /* return 0, Invalid input */
        res = 0;
        break;
      }
      i++;
    }
  }

  return res;
}
/**
  * @brief 二进制转成ASCII
  * 只针对0-F的ASCII转换，A，B，C，D，E，F 必须大写
  * @param  hex 范围0x00-0x0F
  * @retval ASCII值
  */
//uint8_t AsciiToHex(uint8_t ascii)
//{
//	uint8_t hex;
//	
//	hex = (ascii > 0x40) ? (ascii-0x41 +10) : (ascii-0x30);
//	
//	return hex;
//}
/**
  * @brief 二进制转成ASCII,并带判断
  * 只针对0-F的ASCII转换，A，B，C，D，E，F 必须大写
  * @param  hex 范围0x00-0x0F
  * @retval ASCII值
  */
uint8_t AsciiToHex(uint8_t ascii,uint8_t *hex)
{
	uint8_t ret = 1;
	
	/* 0-9 */
	if(ascii >=0x30 && ascii <= 0x39)
	{
		*hex = ascii - 0x30;
		ret = 0;
	}
	/* A-F */
	else if(ascii >=0x41 && ascii <= 0x46)
	{
		*hex = ascii - 0x41 + 0x0A;
		ret = 0;
	}
	/* a-f */
	else if(ascii >=0x61 && ascii <= 0x66)
	{
		*hex = ascii - 0x61 + 0x0A;
		ret = 0;
	}
	else
	{
		ret = 1;
	}
	
	return ret;
}
/**
  * @brief 二进制转成ASCII
  * @param  hex 范围 0x00-0x0F
  * @retval ASCII值
  */
uint8_t HexToASCII(uint8_t hex)
{
	uint8_t ascii;
	
	ascii = hex & 0x0F;
	ascii = (ascii <= 9) ? (ascii + 0x30) : (ascii - 9 + 0x40);
	
	return ascii;
}
/**
  * @brief  MODBUS CRC
  * @param  
  * @retval 
  */
uint16_t CRC_Modbus(uint8_t *buf,uint16_t index_header,uint16_t index_end)			
{
  uint8_t  bits;
	uint16_t loop;
  uint16_t crc16reg = 0xffff;

	for(loop = index_header;loop<=index_end;loop++)
	{
	   /* 低位异或 */
		crc16reg ^= buf[loop];
	   
	  for(bits = 0;bits < 8;bits++)
	  {    	
		  if(crc16reg & 0x01 )
		  {
				crc16reg = crc16reg >> 1;
				crc16reg &= 0x7fff;
				/* 多项式 异或 */
				crc16reg ^= 0xA001;
		  }
		  else
		  {
				/* 右移一位，高位补0 */
				crc16reg = crc16reg >> 1;
				crc16reg &= 0x7fff;
		  }
	  }	   
	}

	return crc16reg	;
}

uint8_t CRC_Byte(uint8_t *buf, uint8_t length)
{ 
	uint8_t i; 
	uint8_t crc=0;
	//长度不包括1位CRC长度
	while(length--!=0) 
	{ 
		crc=crc^*buf;
		for(i=0x80; i!=0; i/=2) 
		{ 
			if((crc&0x80)==0x80) 
			{
				crc *= 2; 
				crc ^= 0xE5;		//多项式位0xE5
			}    
			else 
				crc *= 2; 
		} 
		buf++;
	} 
	return crc;
}
/**
  * @brief  Compares two buffers.
  * @param  pBuffer, pBuffer1: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 0: pBuffer identical to pBuffer1
  *         1: pBuffer differs from pBuffer1
  */
uint8_t Buffercmp(const uint8_t* pBuffer, uint8_t* pBuffer1, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer != *pBuffer1)
    {
      return 1;
    }

    pBuffer++;
    pBuffer1++;
  }

  return 0;
}
/**
  * @brief  找到指定字符
  * @param  
  * @retval 
  */
uint16_t FindCharater(uint8_t* buf,uint16_t length, char charater)
{
	uint16_t loop;
	uint16_t ret;
	
	ret = 0xFFFF;
	for(loop=0;loop<length;loop++)
	{
		if(buf[loop] == charater)
		{
			ret = loop;
			loop = length;
		}
	}
	
	return ret;
}
/**
  * @brief  找到指定字符
  * @param  
  * @retval 
  */
void ClearBuffer(uint8_t* buf,uint16_t length)
{
	uint16_t loop;
	
	for(loop=0;loop<length;loop++)
		buf[loop] = 0;
}
/**
  * @brief  找到指定字符,longStr中的'0'不是结束标识
  * @param  
  * @retval 
  */
char *mystrstr(uint8_t *longStr,uint16_t longStrLen,char *shortStr)
{
	uint16_t loop,lp;
	
	if(*shortStr != '\0')
	{
		for(loop=0;loop<longStrLen;loop++)
		{
			for(lp=0; *((&longStr[loop])+lp) == *(shortStr+lp); lp++)
			{
				if(*(shortStr+lp+1) == '\0')
					return (char *)(&longStr[loop]);
			}
		}
		return NULL;
	}
	
	return NULL;
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
