La señal PWM que sale por PA6 es una onda cuadrada que alterna entre dos estados:
- Estado alto: el LED recibe corriente y se enciende.
- Estado bajo: no circula corriente y el LED se apaga.

El período de la onda se mantiene fijo. Lo que cambia al girar el potenciómetro es el
duty cycle, o sea, la fracción del período en la que la señal está en estado alto.

Estados del Duty Cycle mostrados en las fotos de "imagenes_osciloscopio"
- Duty cycle bajo: los pulsos son angostos, la señal está casi siempre en estado bajo
  y el LED permanece poco tiempo encendido. El brillo es bajo.
- Duty cycle medio: la señal pasa tiempo similar en alto y en bajo. El brillo es medio.
- Duty cycle alto: la señal está casi siempre en estado alto y el LED permanece
  encendido casi todo el tiempo. El brillo es alto.
- Duty cycle nulo: la señal queda siempre en estado bajo y el LED permanece apagado.

Cuanto mayor es el duty cycle, mayor es el brillo del LED. Con el potenciómetro se
regula el duty cycle, por ende, el brillo, sin modificar el período de la señal.
