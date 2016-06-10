#include <stdio.h>
#include <stdlib.h>

#include "libsoc_gpio.h"
#include "libsoc_board.h"


/* This bit of code below makes this example work on all 96Boards */
unsigned int GPIO_LED;
unsigned int GPIO_BUTTON;

__attribute__((constructor)) static void _init()
{
        board_config *config = libsoc_board_init();
        GPIO_BUTTON = libsoc_board_gpio_id(config, "GPIO-E");
        GPIO_LED = libsoc_board_gpio_id(config, "GPIO-G");
        libsoc_board_free(config);
}
/* End of 96Boards special code */

gpio *gpio_led, *gpio_button;
static int interrupt_count = 0;

int callback_test(void* arg){
        int* tmp_count = (int*) arg;
        int level;

        *tmp_count = *tmp_count + 1;
        level = libsoc_gpio_get_level(gpio_button);
        libsoc_gpio_set_level(gpio_led, level);
        printf("Got it (%d), button = %d\n", *tmp_count, level);

        return EXIT_SUCCESS;
}

int main(void)
{
        //gpio *gpio_led, *gpio_button;
        //gpio_level level;
        gpio_led = libsoc_gpio_request(GPIO_LED, LS_SHARED);
        gpio_button = libsoc_gpio_request(GPIO_BUTTON, LS_SHARED);

        if ((gpio_led == NULL) || (gpio_button == NULL)) {
                fprintf(stderr, "Unable to request GPIOs\n");
                goto fail;
		}

        libsoc_gpio_set_direction(gpio_led, OUTPUT);
        libsoc_gpio_set_direction(gpio_button, INPUT);

        if ((libsoc_gpio_get_direction(gpio_led) != OUTPUT)
             || (libsoc_gpio_get_direction(gpio_button) != INPUT)) {
                fprintf(stderr, "Unable to set led/btn directions\n");
                goto fail;
        }
       
        if (libsoc_gpio_set_edge(gpio_button, BOTH) == EXIT_FAILURE) {
                fprintf(stderr, "Unable to set btn edge\n");
                goto fail;
        }

        libsoc_gpio_callback_interrupt(gpio_button, &callback_test, (void*)&interrupt_count);


		printf("push the button...\n");

        libsoc_set_debug(0);

        sleep(10);

        libsoc_set_debug(1);

        libsoc_gpio_callback_interrupt_cancel(gpio_button);

fail:
        if (gpio_led)
                libsoc_gpio_free(gpio_led);

        if (gpio_button)
                libsoc_gpio_free(gpio_button);
			
		return EXIT_SUCCESS;
}


