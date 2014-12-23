#include "Arduino.h"
#include "Serial_commander.h"

Serial_commander::Serial_commander( int enable_write, int package_life ){
  //General setup
  _enable_write = enable_write;
  _package_life = package_life;
  Serial.println("Serial Commander ready - waiting for command");
}

/*
  FIXME: hay que ver como se hace para pasar esto del lado del usuario
  COPY THIS FUNCTIONS IN YOUR PROG. AFTER THE LOOP
*/

void Serial_commander::sc_quick_command( char action ){

  //ADD here your own cases to excecute quick commands with a single char
  // but remember: 'R', 'A', 'H', 'L' and 'F' are taken!.. I reccomend use low case chars
  switch ( action ){
    case 't' : Serial.println("OK - Default Quick Command Excecuted - ERASE ME!"); break;
    //TEST: borrar esto
    case 'p' : digitalWrite(2, HIGH); digitalWrite(4, HIGH); break;
    case 'a' : digitalWrite(2, LOW); digitalWrite(4, LOW); break;

  }

}


// this function is excecuted in the loop
void Serial_commander::process_serial(){

  //listen to serial communication
  if (Serial.available() > 0) {

    char incoming_char =  Serial.read();

    if( !_param_1 ) { sc_open_command( incoming_char ); }
    else if( !_param_2 ) { sc_select_pin( incoming_char ); }
    else if( !_param_3 ) { sc_select_action( incoming_char ); }
    else { kill_command(0); }
  }

  //count life if a command started
  if ( _command_life_count > 0 ){ _command_life_count++; }
  if ( _command_life_count > _package_life ) { Serial.print("- Command Time Out -- "); kill_command(0);  }

  delay(80);
}

/*
  01 - OPEN COMMAND
    'R' read (digital), 'A' read (analog), 'H' write HIGH (digital), 'L' write LOW (digital), 'F' function
    Anything else will try a quick_command - sc_quick_command( char )
*/
void Serial_commander::sc_open_command( char caracter ){
  if( caracter ==  'R' || caracter ==  'A' || caracter ==  'H' || caracter == 'L' || caracter ==  'F' ){
    if( ( caracter == 'H' || caracter == 'L' ) && !_enable_write){
      Serial.println("ERR - Direct digitalWrite is disabled");
    }else{
      _param_1 = caracter;
      Serial.println("OK - Command Started --------------");
      //start counting
      _command_life_count = 1;
    }
  }else{
    sc_quick_command( caracter );
  }
}


/*
  02 - SELECT PIN NUMBER
    Should Get something between 100 and 170 (byte value)
    70 PINS considering Arduino Mega PIN numbers
*/
void Serial_commander::sc_select_pin( int pin_n ){

  pin_n = pin_n - 100;

  if( pin_n >= 0 && pin_n <= 70 ){

    switch( _param_1 ){

      case 'R' :
        Serial.print( "OK - Digital Read for PIN ");Serial.print( pin_n );Serial.print( ": ");Serial.println( digitalRead( pin_n ) );
        kill_command(1);
      break;
      case 'A' :
        Serial.print( "OK - Analog Read for PIN ");Serial.print( pin_n );Serial.print( ": ");Serial.println( analogRead( pin_n ) );
        kill_command(1);
      break;
      case 'H' :
        if(_enable_write){
          digitalWrite( pin_n, HIGH);
          Serial.println("OK - digital PIN set HIGH");
          kill_command(1);
        }
      break;
      case 'L' :
        if(_enable_write){
          digitalWrite( pin_n, LOW);
          Serial.println("OK - digital PIN set LOW");
          kill_command(1);
        }
      break;
      case 'F':
        _param_2 = pin_n;
        Serial.println("OK - PIN Selected");
      break;
      default : Serial.print("ERR - Wrong command combination"); kill_command(0); break;
    }

  }else{
    Serial.println("ERR - Wrong PIN number");
    kill_command(0);
  }

}

/*
  03 - SELECT ACTION
    Gets the first param (called action) for the sc_process_command() function so I should wait for a fourth command
*/
void Serial_commander::sc_select_action( char caracter ){

  if( _param_1 == 'F'){
    Serial.print("OK - Action selected");
  }//or nothing
  else{
    Serial.println("ERR - Wrong PIN action");
    kill_command(0);
  }
}


/*
  Kills current command
  If get success show an OK message
*/
void Serial_commander::kill_command( int success ){

  _command_life_count = 0;
  _param_1 = 0;
  _param_2 = 0;
  _param_3 = 0;
  _param_4 = 0;

  if( success ){
    Serial.println("OK - Current command Executed!\n");
  }else{
    Serial.println(" - Current command ended!\n");
  }

}