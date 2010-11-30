/*
 * Code file containing the HTTP Daemon module.
 * It supports GET (not completely implemented) and POST Request methods
 * 
 * Author: Simon Kueppers
 * Email: simon.kueppers@web.de
 * Homepage: http://klinkerstein.m-faq.de
 * 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 Copyright 2008 Simon Kueppers
 * */

#include <avr/pgmspace.h>
#include <string.h>
#include "HttpD.h"
#include "../uip/uip.h"
#include "../../Hardware/Gpio.h"
#include "../../Hardware/temperature.h"

#define STATE_CONNECTED		0
#define STATE_GOTGET		1
#define STATE_GOTPOST		2
#define STATE_HEADERSENT	3

static const prog_char
		g_HtmlPageDefault[] =
				"<html><head>"
                    "<title>uWebSrv</title>"
					"<style type='text/css'>"
					".s0 { background-color: red; }"
					".s1 { background-color: green; }"
					".a { background-color: orange; }"
					"td { text-align: center; }"
					"</style>"
					"</head><body>"
					"<h1>Toms uWebSrv</h1>"
					"<form method='POST' action='/'>"
					"<table border='1px'><colgroup><col width='100px'><col width='40px'><col width='40px'><col width='40px'></colgroup>"
					"<tr><td>-</td><td>PIN</td><td>PORT</td><td>DDR</td></tr>"
					"<tr><td>ADC 00</td><td class='a'>%a00</td></tr>"
					"<tr><td>GPIO 01</td><td class='s%i01'>%i01</td><td class='s%o01'><input type='text' name='o01' value='%o01' maxlength='1' size='1'></td><td class='s%d01'><input type='text' name='d01' value='%d01' maxlength='1' size='1'></td></tr>"
/*
					"<tr><td>GPIO 02</td><td class='s%i02'>%i02</td><td class='s%o02'><input type='text' name='o02' value='%o02' maxlength='1' size='1'></td><td class='s%d02'><input type='text' name='d02' value='%d02' maxlength='1' size='1'></td></tr>"
					"<tr><td>GPIO 03</td><td class='s%i03'>%i03</td><td class='s%o03'><input type='text' name='o03' value='%o03' maxlength='1' size='1'></td><td class='s%d03'><input type='text' name='d03' value='%d03' maxlength='1' size='1'></td></tr>"
					"<tr><td>GPIO 04</td><td class='s%i04'>%i04</td><td class='s%o04'><input type='text' name='o04' value='%o04' maxlength='1' size='1'></td><td class='s%d04'><input type='text' name='d04' value='%d04' maxlength='1' size='1'></td></tr>"
					"<tr><td>GPIO 05</td><td class='s%i05'>%i05</td><td class='s%o05'><input type='text' name='o05' value='%o05' maxlength='1' size='1'></td><td class='s%d05'><input type='text' name='d05' value='%d05' maxlength='1' size='1'></td></tr>"
					"<tr><td>GPIO 06</td><td class='s%i06'>%i06</td><td class='s%o06'><input type='text' name='o06' value='%o06' maxlength='1' size='1'></td><td class='s%d06'><input type='text' name='d06' value='%d06' maxlength='1' size='1'></td></tr>"
					"<tr><td>GPIO 07</td><td class='s%i07'>%i07</td><td class='s%o07'><input type='text' name='o07' value='%o07' maxlength='1' size='1'></td><td class='s%d07'><input type='text' name='d07' value='%d07' maxlength='1' size='1'></td></tr>"
					"<tr><td>GPIO 08</td><td class='s%i08'>%i08</td><td class='s%o08'><input type='text' name='o08' value='%o08' maxlength='1' size='1'></td><td class='s%d08'><input type='text' name='d08' value='%d08' maxlength='1' size='1'></td></tr>"
					"<tr><td>GPIO 09</td><td class='s%i09'>%i09</td><td class='s%o09'><input type='text' name='o09' value='%o09' maxlength='1' size='1'></td><td class='s%d09'><input type='text' name='d09' value='%d09' maxlength='1' size='1'></td></tr>"
					"<tr><td>GPIO 10</td><td class='s%i10'>%i10</td><td class='s%o10'><input type='text' name='o10' value='%o10' maxlength='1' size='1'></td><td class='s%d10'><input type='text' name='d10' value='%d10' maxlength='1' size='1'></td></tr>"
					"<tr><td>GPIO 11</td><td class='s%i11'>%i11</td><td class='s%o11'><input type='text' name='o11' value='%o11' maxlength='1' size='1'></td><td class='s%d11'><input type='text' name='d11' value='%d11' maxlength='1' size='1'></td></tr>"
					"<tr><td>GPIO 12</td><td class='s%i12'>%i12</td><td class='s%o12'><input type='text' name='o12' value='%o12' maxlength='1' size='1'></td><td class='s%d12'><input type='text' name='d12' value='%d12' maxlength='1' size='1'></td></tr>"
					"<tr><td>GPIO 13</td><td class='s%i13'>%i13</td><td class='s%o13'><input type='text' name='o13' value='%o13' maxlength='1' size='1'></td><td class='s%d13'><input type='text' name='d13' value='%d13' maxlength='1' size='1'></td></tr>"
*/
					"</table>"
					"<input type='submit' value='Modify'>"
					"</form>"
					"</body></html>";

static uint16_t CopyStringP(uint8_t** ppBuffer,
							const prog_char* pString)
{
	uint16_t nBytes = 0;
	char Character;

	while ((Character = pgm_read_byte(pString)) != '\0')
	{
		**ppBuffer = Character;
		*ppBuffer = *ppBuffer + 1;
		pString = pString + 1;
		nBytes++;
	}

	return nBytes;
}

static uint16_t CopyValue(	uint8_t** ppBuffer,
							uint32_t nValue)
{
	static PROGMEM int32_t nDecTab[] =
	{
		1000000000,
		100000000,
		10000000,
		1000000,
		100000,
		10000,
		1000,
		100,
		10,
		1,
		0};

	int32_t nDec;
	uint8_t nCurTabIndex = 0;
	char cNumber;
	uint8_t nBytes = 0;
    char pre = 1;

	while ((nDec = pgm_read_dword(&nDecTab[nCurTabIndex])) != 0)
	{
		for (cNumber='0'; cNumber<'9'; cNumber++)
		{
			int32_t nTmp = nValue - nDec;

			if (nTmp < 0)
				break;

			nValue = nTmp;
		}
		nCurTabIndex++;

        if (pre && cNumber == '0' 
                && nCurTabIndex < 10)   // Sonst fehlt '0'
            continue;                   // Führende '0' weg

        pre=0;
		**ppBuffer = cNumber;
		*ppBuffer = *ppBuffer + 1;
		nBytes++;
	}

	return nBytes;
}

static uint16_t CopyHttpHeader(	uint8_t* pBuffer,
								uint32_t nDataLen)
{
	uint16_t nBytes = 0;

	nBytes += CopyStringP(&pBuffer, PSTR("HTTP/1.1 200 OK"));
	nBytes += CopyStringP(&pBuffer, PSTR("\r\n"));

	nBytes += CopyStringP(&pBuffer, PSTR("Content-Length:"));
	nBytes += CopyValue(&pBuffer, nDataLen);
	nBytes += CopyStringP(&pBuffer, PSTR("\r\n"));

	nBytes += CopyStringP(&pBuffer, PSTR("Content-Type:text/html\r\n"));
	nBytes += CopyStringP(&pBuffer, PSTR("Connection:close\r\n"));
	nBytes += CopyStringP(&pBuffer, PSTR("\r\n"));

	return nBytes;
}

static uint16_t CopyHttpData(	uint8_t* pBuffer,
								const prog_void** ppData,
								uint32_t* pDataLeft,
								uint16_t nMaxBytes)
{
	uint16_t nBytes = 0;
	uint8_t nByte;
	uint8_t nParsedNum;
	uint8_t nParsedMode;

	while (nMaxBytes--)
	{
		if (*pDataLeft > 0)
		{
			nByte = pgm_read_byte(*ppData);

			if (nByte == '%')
			{
				*ppData = *ppData + 1;
				*pDataLeft = *pDataLeft - 1;
				nParsedMode = pgm_read_byte(*ppData);
				*ppData = *ppData + 1;
				*pDataLeft = *pDataLeft - 1;
				nParsedNum = (pgm_read_byte(*ppData) - '0') * 10;
				*ppData = *ppData + 1;
				*pDataLeft = *pDataLeft - 1;
				nParsedNum += (pgm_read_byte(*ppData) - '0') * 1;

				if (nParsedMode == 'i')
					*pBuffer = GpioGetPin(nParsedNum) + '0';
				else if (nParsedMode == 'o')
					*pBuffer = GpioGetPort(nParsedNum) + '0';
				else if (nParsedMode == 'd')
					*pBuffer = GpioGetDdr(nParsedNum) + '0';
				else if (nParsedMode == 'a') {
                    uint16_t adcValue = ReadADCChannel(nParsedNum);
	                nBytes += CopyValue(&pBuffer, adcValue)-1;
			        pBuffer -= 1;
                }
			}
			else
				*pBuffer = nByte;

			*ppData = *ppData + 1;
			*pDataLeft = *pDataLeft - 1;
			pBuffer++;
			nBytes++;

		}
		else
			break;
	}

	return nBytes;
}

void HttpDInit()
{
	//Start listening on our port
	uip_listen(HTONS(PORT_HTTPD));
}

void HttpDCall(	uint8_t* pBuffer,
				uint16_t nBytes,
				struct tHttpD* pSocket)
{
	uint16_t nBufSize;

	if (uip_connected())
	{
		//Initialize this connection
		pSocket->pData = g_HtmlPageDefault;
		pSocket->nDataLeft = sizeof(g_HtmlPageDefault)-1;
		pSocket->nNewlines = 0;
		pSocket->nState = STATE_CONNECTED;
	}
	else if (uip_newdata() || uip_acked())
	{
		if (pSocket->nState == STATE_CONNECTED)
		{
			if (nBytes > 6)
			{
				if (pBuffer[0] == 'G' && pBuffer[1] == 'E' && pBuffer[2] == 'T')
				{
					pSocket->nState = STATE_GOTGET;
				}
				else if (pBuffer[0] == 'P' && pBuffer[1] == 'O' && pBuffer[2]
						== 'S' && pBuffer[3] == 'T')
				{
					pSocket->nState = STATE_GOTPOST;
				}
			}
		}

		if (pSocket->nState == STATE_GOTGET || pSocket->nState == STATE_GOTPOST)
		{
			while (nBytes--)
			{
				if (*pBuffer == '\n')
				{
					pSocket->nNewlines++;
				}
				else if (*pBuffer == '\r')
				{
				}
				else
					pSocket->nNewlines = 0;

				pBuffer++;

				if (pSocket->nNewlines == 2)
				{
					//We found two \r\n or two \n

					if (pSocket->nState == STATE_GOTPOST)
					{
						uint8_t* pParam = pBuffer;
						uint8_t nBytes = 167;
						uint8_t nParsedMode;
						uint8_t nParsedNum;
						uint8_t nParsedState;

						while (nBytes--)
						{
							if (*pBuffer == '&' || nBytes == 1)
							{
								nParsedMode = *pParam;
								pParam++;
								nParsedNum = (*pParam - '0' ) * 10;
								pParam++;
								nParsedNum += (*pParam - '0' ) * 1;
								pParam++;
								pParam++;

								if ((*pParam - '0') != 0)
									nParsedState = 1;
								else
									nParsedState = 0;

								pParam = pBuffer + 1;

								if (nParsedMode == 'o')
									GpioSetPort(nParsedNum, nParsedState);
								else if (nParsedMode == 'd')
									GpioSetDdr(nParsedNum, nParsedState);

							}
							pBuffer++;
						}
					}

					//Put HTML Response Header
					nBufSize = CopyHttpHeader(pBuffer, pSocket->nDataLeft);
					uip_send(pBuffer, nBufSize);

					pSocket->nState = STATE_HEADERSENT;
				}
			}

		}
		else if (pSocket->nState == STATE_HEADERSENT)
		{
			//We have sent the HTML Header or HTML Data previously.
			//Now we send (further) Data depending on the Socket's pData pointer
			//If all data has been sent, we close the connection
			nBufSize
					= CopyHttpData(pBuffer, &pSocket->pData, &pSocket->nDataLeft, uip_mss());

			if (nBufSize == 0)
			{
				//No Data has been copied. Close connection
				uip_close();
			}
			else
			{
				//Else send copied data
				uip_send(pBuffer, nBufSize);
			}
		}
		else
		{
			//huh? the best way to handle this is to close...
			uip_close();
		}

	}
	else if (uip_rexmit())
	{
		//We will implement this later
		uip_abort();
	}

}
