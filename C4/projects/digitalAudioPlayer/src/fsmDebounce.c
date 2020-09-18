
/*=====[Inclusions of function dependencies]=================================*/
#include "fsmDebounce.h"
/*=====[Definition macros of private constants]==============================*/

/*=====[Definitions of extern global variables]==============================*/

/*=====[Definitions of public global variables]==============================*/

/*=====[Definitions of private global variables]=============================*/
// FSM state names
typedef enum{
   STATE_BUTTON_UP,
   STATE_BUTTON_DOWN,
   STATE_BUTTON_FALLING,
   STATE_BUTTON_RISING
} fsmButtonState_t;
static fsmButtonState_t fsmButtonState;

static void fsmButtonError( void );

/*=====[Main function, program entry point after power on or reset]==========*/

void fsmButtonError( void )
{
   fsmButtonState = STATE_BUTTON_UP;
}

void fsmButtonInit( void )
{
   fsmButtonState = STATE_BUTTON_UP;
}

bool_t fsmButtonUpdate( gpioMap_t tecla )
{
   bool_t retVal = FALSE;

   switch( fsmButtonState ){
      case STATE_BUTTON_UP:
         if( !gpioRead(tecla) ){
            fsmButtonState = STATE_BUTTON_FALLING;
         }
      break;
      case STATE_BUTTON_DOWN:
         if( gpioRead(tecla) ){
            fsmButtonState = STATE_BUTTON_RISING;
         }
      break;
      case STATE_BUTTON_FALLING:
         if( !gpioRead(tecla) ){
            fsmButtonState = STATE_BUTTON_DOWN;
            retVal=TRUE;
         } else{
            fsmButtonState = STATE_BUTTON_UP;
         }
      break;
      case STATE_BUTTON_RISING:
         if( gpioRead(tecla) ){
            fsmButtonState = STATE_BUTTON_UP;
         } else{
            fsmButtonState = STATE_BUTTON_DOWN;
         }
      break;
      default:
         fsmButtonError();
      break;
   }
   return retVal;
}
