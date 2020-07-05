#include "Arduino.h"
#include "Deposito_sim.h"
#define bit_PIN 255
float bit_ANALOG = 1023;

Deposito_sim::Deposito_sim (int _valvula_llenado_tanque, int _valvula_vaciado_tanque, int _sensor_vol, float _caudal_entrada_max,
float _caudal_salida_max, float vol_ini, float _limite_tanque, int _activo_o_pasivo, Deposito_sim *deposito_ant, bool _es_fugas) {   
    
  valvula_llenado_tanque = _valvula_llenado_tanque;
  valvula_vaciado_tanque = _valvula_vaciado_tanque;
  caudal_entrada_max = _caudal_entrada_max;
  caudal_salida_max = _caudal_salida_max;
  volumen = vol_ini;
  sensor_vol = _sensor_vol;
  limite_tanque = _limite_tanque;
  activo_o_pasivo = _activo_o_pasivo;
  this->deposito_ant = deposito_ant;
  es_fugas = _es_fugas;
  incremento = 0.000000000001;
  caudal = 0.000000000001;
  decremento = 0;

  //Asignación de los pines a las válvulas correspondientes//
     
  pinMode(valvula_llenado_tanque, INPUT);
  pinMode(valvula_vaciado_tanque, INPUT); 
  pinMode(sensor_vol, OUTPUT);   
}

bool valvula_llenado;
bool valvula_vaciado;

//Método que comprueba el estado del deposito//
  
void Deposito_sim::actualizar_volumen (float t_ciclo) {
  valvula_llenado = digitalRead(valvula_llenado_tanque);
  valvula_vaciado = digitalRead(valvula_vaciado_tanque);
  if (activo_o_pasivo == 1) {
    if ((valvula_llenado == HIGH) || (es_fugas == 1)) {
      if (es_fugas == 0) {
        incremento = (caudal_entrada_max * t_ciclo);
      }
      if (es_fugas == 1) {
        caudal = analogRead(caudal_entrada_max) / bit_ANALOG;
        incremento = (caudal * t_ciclo/bit_PIN);
      }
      if ((deposito_ant->volumen) < incremento) {
        incremento = deposito_ant->volumen;
      } 
      if ((volumen + incremento) >= limite_tanque) {
        volumen = limite_tanque;
      }
      else {
        volumen = volumen + incremento;
      }
    }   
    if (valvula_vaciado == HIGH) {
      decremento = (caudal_salida_max * t_ciclo);
      if ((volumen - decremento) >= 0) {
        volumen = volumen - decremento;
      }
      if ((volumen - decremento) <= 0) {
        volumen = 0;
      }
      if (volumen == 0) {
        decremento = 0;
      }
    }
    if ((valvula_llenado == LOW) && (es_fugas == 0)) {
      incremento = 0.000000000001;
    }
    if (valvula_vaciado == LOW) {
      decremento = 0;
    }
  }
  if (activo_o_pasivo == 0) {
    if (volumen >= limite_tanque) {
      volumen = limite_tanque;
    }
  }
}

//Método que incrementa el valor de volumen del tanque//
  
void Deposito_sim::incrementar_volumen (float cantidad_deseada) {
  volumen = volumen + cantidad_deseada;
}

//Método que disminuye el valor de volumen del tanque//
  
void Deposito_sim::disminuir_volumen (float cantidad_deseada) {
  volumen = volumen - cantidad_deseada;
}

//Método que nos da el incremento de un tanque//

float Deposito_sim::get_incremento() {
  return incremento;
}

//Método que nos da el decremento de un tanque//

float Deposito_sim::get_decremento() {
  return decremento;
}

//Método que escribe el valor de volumen en el sensor (en forma de voltaje)//

void Deposito_sim::sobrescribir_sensor () {
  analogWrite(sensor_vol, conversion_volumen());
}

//Método de conversión que transforma voltaje (recibido por las entradas analógicas) en volumen//

float Deposito_sim::conversion_volumen () {
  return (volumen/limite_tanque)*bit_PIN;
}
 
