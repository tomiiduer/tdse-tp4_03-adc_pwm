/*
 * Copyright (c) 2023 Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @file   : task_pwm.c
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/
/* Project includes. */
#include <stdlib.h>
#include "main.h"

/* Demo includes. */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes. */
#include "board.h"
#include "app.h"

/********************** macros and definitions *******************************/

/* ADC1 is 12 bit, right aligned: 0 .. 4095 */
#define ADC_MAX (4095)

/* PWM period (TIM3 ARR). With ARR = ADC_MAX the pulse width (CCR1) is the ADC
 * value itself, no scaling needed: duty = adc_value / (ARR + 1).
 * Timer clock is 8 MHz, so f_pwm = 8 MHz / 4096 ~= 1.95 kHz (period ~512 uS),
 * high enough for the eye not to see flicker. */
#define PWM_PERIOD (ADC_MAX)

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/
const char *p_task_pwm 		= "Task PWM";

/********************** external data declaration *****************************/
extern TIM_HandleTypeDef htim3;


/********************** external functions definition ************************/
void task_pwm_init(void *parameters)
{
	shared_data_type *shared_data = (shared_data_type *) parameters;

	shared_data->pwm_active = 0;

	/* Set the period once and start the PWM once. From now on only the pulse
	 * width (CCR1) is modified. */
	__HAL_TIM_SET_AUTORELOAD(&htim3, PWM_PERIOD);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, shared_data->pwm_active);
	/* CCR1 is loaded at the update event, so a new duty never cuts a cycle */
	__HAL_TIM_ENABLE_OCxPRELOAD(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);

	/* Print out: Task Initialized */
	LOGGER_LOG("  %s is running - %s\r\n", GET_NAME(task_pwm_init), p_task_pwm);
}

void task_pwm_update(void *parameters)
{
	shared_data_type *shared_data = (shared_data_type *) parameters;

	if ( shared_data->adc_end_of_conversion ) {
		shared_data->adc_end_of_conversion = false;

		/* Potentiometer position -> LED brightness */
		shared_data->pwm_active = shared_data->adc_value;
		if ( ADC_MAX < shared_data->pwm_active ) {
			shared_data->pwm_active = ADC_MAX;
		}
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, shared_data->pwm_active);
	}
}

/********************** end of file ******************************************/
