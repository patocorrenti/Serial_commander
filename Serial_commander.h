/*
  Serial_commander.h - Library for execute functions throug serial port bytes.
  Created by Pato Correnti, December, 2014.
  Released into the public domain.
*/
#ifndef Serial_commander_h
#define Serial_commander_h

#include "Arduino.h"

class Serial_commander
{
  public:
    Serial_commander( int enable_write, int package_life );
    void process_serial();
  private:
    int _enable_write;
    int _package_life;
    char _param_1;
    int  _param_2;
    char _param_3;
    int  _param_4;
    int  _command_life_count;
    void sc_quick_command( char action );
    void sc_open_command( char caracter );
    void sc_select_pin( int pin_n );
    void sc_select_action( char caracter );
    void kill_command( int success );
};

#endif