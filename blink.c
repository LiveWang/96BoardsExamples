#include <stdio.h>
#include <stdlib.h>

#include "libsoc_gpio.h"
#include "libsoc_board.h"


/* This bit of code below makes this example work on all 96Boards */
unsigned int GPIO_LED;

__attribute__((constructor)) static void _init()
{
	board_config *config = libsoc_board_init();
	GPIO_LED = libsoc_board_gpio_id(config, "GPIO-G");
	libsoc_board_free(config);
}
/* End of 96Boards special code */


int main()
{
	gpio *gpio_led;

	gpio_led = libsoc_gpio_request(GPIO_LED, LS_SHARED);
    
	if ((gpio_led == NULL)) {
		fprintf(stderr, "Unable to request GPIOs\n");
        	goto fail;
	}

	libsoc_gpio_set_direction(gpio_led, OUTPUT);

	if ((libsoc_gpio_get_direction(gpio_led) != OUTPUT)) {
		fprintf(stderr, "Unable to set led/btn directions\n");
		goto fail;
	}
	
	while (1) {
		libsoc_gpio_set_level(gpio_led, 1);
		usleep(500000);
		libsoc_gpio_set_level(gpio_led, 0);
		usleep(500000);
    }

fail:
	if (gpio_led)
		libsoc_gpio_free(gpio_led);

    return EXIT_SUCCESS;
}