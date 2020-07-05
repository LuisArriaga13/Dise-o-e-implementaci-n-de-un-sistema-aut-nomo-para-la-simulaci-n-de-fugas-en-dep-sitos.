#include "Arduino.h"

class Deposito_sim { 
  
  private:
  
  //Atributos estáticos//
  int valvula_llenado_tanque;                    //Señal que recoge el estado de la válvula de llenado//
  int valvula_vaciado_tanque;                    //Señal que recoge el estado de la válvula de vaciado//
  int sensor_vol;                                //Señal que representa el sensor de volumen del tanque//
  int activo_o_pasivo;                           //Si 0 -> el tanque es pasivo, si 1 -> el tanque es activo//
  bool es_fugas;                                 //Variable que nos indica si el tanque es el de fugas (1)//
  Deposito_sim *deposito_ant;                    //Puntero que señala al tanque anterior con motivo de saber si tiene agua o no//
  //Atributos de simulación// 
  float limite_tanque;                           //Volumen límite que puede almacenar el tanque
  float caudal_entrada_max;                      //Caudal de entrada máximo//
  float caudal_salida_max;                       //Caudal de salida mínimo//
  //Atributos de estado//
  float caudal;                                  //Variable que almacena el caudal que entra en el tanque de fugas//
  float incremento;                              //Variable que almacena cuanto se incrementará el volumen del tanque en ese ciclo//
  float decremento;                              //Variable que almacena cuanto se decrementará el volumen del tanque en ese ciclo//
  
  public:

  float volumen;                                 //Variable que almacena el valor de volumen del tanque//
  
  //Constructor de la clase Deposito_sim//

  Deposito_sim (int _valvula_llenado_tanque, int _valvula_vaciado_tanque, int _sensor_vol, float _caudal_entrada_max,
  float _caudal_salida_max, float vol_ini, float _limite_tanque, int _activo_o_pasivo, Deposito_sim *deposito_ant, bool _es_fugas);

  //Método que actualiza el valor de volumen del tanque//
  
  void actualizar_volumen (float t_ciclo);

  //Método que incrementa el valor de volumen del tanque//
  
  void incrementar_volumen (float cantidad_deseada);

  //Método que disminuye el valor de volumen del tanque//
  
  void disminuir_volumen (float cantidad_deseada);

  //Método que nos da el incremento de un tanque//

  float get_incremento();

  //Método que nos da el decremento de un tanque//

  float get_decremento();

  //Método que escribe el valor de volumen en el sensor//

  void sobrescribir_sensor ();

  //Método de conversión que transforma el volumen en una señal que se envía a tavés de los pines//

  float conversion_volumen ();

};
