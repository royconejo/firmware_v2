/* Copyright 2017, Alejandro Permingeat.
 * Copyright 2017, Rodrigo Furlani
 * Copyright 2017, Eric Pernia.
 * All rights reserved.
 *
 * This file is part sAPI library for microcontrollers.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*
 * Date: 2017-11-10
 */

/*==================[inclusions]=============================================*/

#include "sapi.h"         /* <= sAPI header */
#include <math.h>

CONSOLE_PRINT_ENABLE

/*==================[macros and definitions]=================================*/

#define BAUD_RATE 115200

#define ADDRESS_MAG  0x0D  //direccion magnetometro

/*==================[internal data declaration]==============================*/

static int16_t qmc5883l_x_raw;
static int16_t qmc5883l_y_raw;
static int16_t qmc5883l_z_raw;

/* Buffers */
static char uartBuff[10];

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

CONSOLE_PRINT_ENABLE

/*==================[internal functions definition]==========================*/
uint8_t registroAleer;

int16_t xValue;
int16_t yValue;
int16_t zValue;

uint8_t x_MSB, x_LSB;
uint8_t y_MSB, y_LSB;
uint8_t z_MSB, z_LSB;

uint8_t dataToReadBuffer;

/*==================[external functions definition]==========================*/



/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void){
    float angulo;
   /* ------------- INICIALIZACIONES ------------- */

   /* Inicializar la placa */
   boardConfig();

   /* Inicializar QMC5883L */
   QMC5883L_config_t qmc5883L_configValue;

   qmc5883lPrepareDefaultConfig( &qmc5883L_configValue );

   qmc5883L_configValue.mode = QMC5883L_continuous_measurement;

   qmc5883lConfig( qmc5883L_configValue );

   // Inicializar UART_USB como salida de consola
   consolePrintConfigUart( UART_USB, BAUD_RATE );


   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(1) {

      qmc5883lRead( &qmc5883l_x_raw, &qmc5883l_y_raw, &qmc5883l_z_raw );
      
      consolePrintString("QMC5883L eje X: ");
      consolePrintlnInt(qmc5883l_x_raw);
      
      consolePrintString("QMC5883L eje Y: ");
      consolePrintlnInt(qmc5883l_y_raw);

      consolePrintString("QMC5883L eje Z: ");
      consolePrintlnInt(qmc5883l_z_raw);
       
       
       /** Calculo de angulo en el plano X Y (con eje Z coincidiendo con el cenit)
           El angulo se calcula con respecto al eje Y es decir, 
           un valor de    0 grados indica que el norte coincide con el eje  Y, 
           un valor de   90 grados indica que el norte coincide con el eje  X,
           un valor de  180 grados indica que el norte coincide con el eje -Y, 
           un valor de  -90 grados indica que el norte coincide con el eje -X. 
       */
      angulo = atan2(qmc5883l_y_raw,qmc5883l_x_raw); /*valor en radianes*/
       
      angulo = angulo * 180 / 3.14; /*Convertir a grados*/

      consolePrintString("Angulo: ");
      consolePrintlnInt(angulo);
      consolePrintEnter();

      delay(1000);
   }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
   return 0 ;
}

/*==================[end of file]============================================*/
