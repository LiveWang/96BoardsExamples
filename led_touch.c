/************************************************************/
/*                             */
/* Written with libsoc C library              */
/*                             */
/*                             */
/* You can do interrupt programming with this library    */
/*                             */
/************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "libsoc_gpio.h"
#include "libsoc_debug.h"
#include "libsoc_board.h"

unsigned int GPIO_LED;
unsigned int GPIO_BUTTON;

int last_touch;
int led_state = 0;
int running = 1;

/* This bit of code below makes this example work on all */ 
/* 96Boards, Though you could just call this in main */
__attribute__((constructor)) static void _init()
{
  board_config *config = libsoc_board_init();
  GPIO_BUTTON = libsoc_board_gpio_id(config, "GPIO-E");
  GPIO_LED = libsoc_board_gpio_id(config, "GPIO-G");
 libsoc_board_free(config);
}
/* End of 96Boards special code */

int main()
{
  gpio *gpio_led,*gpio_button;
  int touch;
 
  libsoc_set_debug(0);
  gpio_led = libsoc_gpio_request(GPIO_LED,LS_SHARED);
  gpio_button = libsoc_gpio_request(GPIO_BUTTON,LS_SHARED);
 
  if((gpio_led == NULL) || (gpio_button == NULL))
  {
    return(-1);
  }
  libsoc_gpio_set_direction(gpio_led,OUTPUT);
  libsoc_gpio_set_direction(gpio_button,INPUT);
 
  if((libsoc_gpio_get_direction(gpio_led) != OUTPUT)
  || (libsoc_gpio_get_direction(gpio_button) != INPUT)) 
  {
    return(-1);
  }
  while(running)
  {
    touch = libsoc_gpio_get_level(gpio_button);
    if(touch == 1 && last_touch == 0){
      led_state = led_state==1?0:1;
      libsoc_gpio_set_level(gpio_led,led_state);
      usleep(100000);
    }
    last_touch = touch;
    usleep(1);
  }  
  if(gpio_led || gpio_button)
  {
    printf("apply gpio resource fail!\n");
    libsoc_gpio_free(gpio_led);
    libsoc_gpio_free(gpio_button);
  }
  return EXIT_SUCCESS;
}
