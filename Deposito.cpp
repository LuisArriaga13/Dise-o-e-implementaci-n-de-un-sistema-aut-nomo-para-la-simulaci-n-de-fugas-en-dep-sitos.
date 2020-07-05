#include "Arduino.h"
#include "Deposito.h"

#define TANQUE_VACIO 0
#define LLENADO 1
#define TANQUE_LLENO 2
#define DESCARGANDO 3
#define EMERGENCIA 4
#define REINICIO 5
#define bit_PIN 1023
#define bit_PIN_OUT 255
#define t_ciclo 0.003
#define r_max 51
#define v_r_max 512

Deposito::Deposito (int _PIN_llenado_tanque, int _PIN_vaciado_tanque, int _puerto_sensor_vol, float _capacidad_maxima, float _vol_max_seguridad, float _volumen_descarga_max,
float _volumen_descarga_min, float _t_prox_descarga_max, float _t_prox_descarga_min, bool _recarga_o_descarga, Deposito *deposito_sig, bool _es_fugas) {   

  estado_actual = TANQUE_VACIO;    
  PIN_llenado_tanque = _PIN_llenado_tanque;
  PIN_vaciado_tanque = _PIN_vaciado_tanque;
  puerto_sensor_vol = _puerto_sensor_vol;
  capacidad_maxima = _capacidad_maxima;
  vol_max_seguridad = _vol_max_seguridad;
  volumen_descarga_max = _volumen_descarga_max;
  volumen_descarga_min = _volumen_descarga_min;
  t_prox_descarga_max = _t_prox_descarga_max;
  t_prox_descarga_min = _t_prox_descarga_min;  
  vol_prox_mov = 0;
  t_prox_mov = 0;
  nuevo_vol_descarga();
  nuevo_t_prox_mov();    
  recarga_o_descarga = _recarga_o_descarga; 
  es_fugas = _es_fugas;
  this->deposito_sig = deposito_sig;

  //Asignación de los pines a las válvulas correspondientes//

  pinMode(puerto_sensor_vol, INPUT);      
  pinMode(PIN_llenado_tanque, OUTPUT);
  pinMode(PIN_vaciado_tanque, OUTPUT); 
}
  

//Método que comprueba el estado del deposito//
  
bool Deposito::actualizar_estado (int tSimulacion, int alarma, int volver_a_empezar) {

  if ((es_fugas == 1) && (tSimulacion > 7200)) {
    genera_caudal();
  }

//El parámetro volver_a_empezar indíca que el conjunto de todos los depositos se han vaciado y podemos salir del estado de reinicio//
  
  if (alarma == HIGH) {                                                                                     //Condición de emergencia en función del reset//
    estado_actual = EMERGENCIA;
  }

  if ((volumen >= vol_max_seguridad) && (estado_actual != REINICIO)) {                                      //Condición de emergencia//
    estado_actual = EMERGENCIA;
  }
    
  if (estado_actual == TANQUE_VACIO) {                                                                      //Estado que define las condiciones de cuando el tanque espera una descarga//
    cerrar_valvula_llenado();                                                                               //Cerramos la válvula de llenado del tanque//  
    cerrar_valvula_vaciado();                                                                               //Cerramos la válvula de vaciado del tanque//        
    if ((tSimulacion >= t_prox_mov) || (recarga_o_descarga == true)) {                                      //Condición de cambio de estado, tanque vacio a llenado//       
      estado_actual = LLENADO;        
    }
  }     
  if (estado_actual == LLENADO) {                                                                           //Estado que define el llenado del tanque//
    abrir_valvula_llenado();                                                                                //Abrimos la válvula de llenado del tanque//    
    cerrar_valvula_vaciado();                                                                               //Cerramos la válvula de vaciado del tanque//                
    if ((volumen >= vol_prox_mov) && (es_fugas == 0)) {                                                     //Condición de cambio de estado, llenado a tanque lleno//
      estado_actual = TANQUE_LLENO;
    }   
  }
  if (estado_actual == TANQUE_LLENO) {                                                                      //Estado que define las condiciones de cuando el tanque ha recibido una descarga//
    cerrar_valvula_llenado();                                                                               //Cerramos la válvula de llenado del tanque//    
    cerrar_valvula_vaciado();                                                                               //Cerramos la válvula de vaciado del tanque//                
    if ((deposito_sig->ready_to_receive(vol_prox_mov) == 1) && ((tSimulacion >= t_prox_mov) ||              //Condición de cambio de estado, tanque lleno a descargando//
    (recarga_o_descarga == false))) {
      estado_actual = DESCARGANDO;
    }    
  }
  if (estado_actual == DESCARGANDO) {                                                                       //Estado que define cuando el tanque está descargando//
    cerrar_valvula_llenado();                                                                               //Cerramos la válvula de llenado del tanque//    
    abrir_valvula_vaciado();                                                                                //Abrimos la válvula de vaciado del tanque//                
    if (volumen <= 0) {                                                                                     //Condición de cambio de estado, descargando a tanque vacio//
      estado_actual = TANQUE_VACIO;
      nuevo_vol_descarga();                                                                                 //Asignamos nuevamente valores aleatorios a las variables de simulación//
      nuevo_t_prox_mov();      
    }    
  }  
  if (estado_actual == EMERGENCIA) {                                                                        //Estado que define la entrada a los límites de seguridad del tanque//
    cerrar_valvula_llenado();                                                                               //Cerramos la válvula de llenado del tanque//    
    cerrar_valvula_vaciado();                                                                               //Cerramos la válvula de vaciado del tanque//      
    if (alarma == LOW) {                                                                                    //Condición de cambio de estado, emergencia a reinicio//
      estado_actual = REINICIO;
    }
    return true;
  }
  if (estado_actual == REINICIO) {                                                                          //Estado que define el reinicio de la planta//
    abrir_valvula_llenado();                                                                                //Abrimos la válvula de llenado del tanque//    
    abrir_valvula_vaciado();                                                                                //Abrimos la válvula de vaciado del tanque//      
    if (volver_a_empezar == 1) {                                                                            //Condición de cambio de estado, de reinicio a tanque vacio//
      estado_actual = TANQUE_VACIO;
      nuevo_vol_descarga();                                                                                 //Asignamos nuevamente valores aleatorios a las variables de simulación//
      nuevo_t_prox_mov();       
    }
  }
  return false;
}

//Método que abre la válvula de llenado//

void Deposito::abrir_valvula_llenado () {
  if (es_fugas == 0) {
    digitalWrite(PIN_llenado_tanque, HIGH);   
  }
  else {
    analogWrite(PIN_llenado_tanque, caudal);   
  }
}

//Método que abre la válvula de vaciado//

void Deposito::abrir_valvula_vaciado () {
  digitalWrite(PIN_vaciado_tanque, HIGH); 
}

//Método que cierra la válvula de llenado//

void Deposito::cerrar_valvula_llenado () {
  if (es_fugas == 0) {
    digitalWrite(PIN_llenado_tanque, LOW);   
  }
  else {
    analogWrite(PIN_llenado_tanque, 0);   
  }
}

//Método que cierra la válvula de vaciado//

void Deposito::cerrar_valvula_vaciado () {
  digitalWrite(PIN_vaciado_tanque, LOW); 
}

//Método que generará valores aleatorios de volumen a las variables de la simulación//

void Deposito::nuevo_vol_descarga () {
  vol_prox_mov = random (volumen_descarga_min, volumen_descarga_max);
  vol_prox_mov = vol_prox_mov/100;
}

//Método que generará valores aleatorios de tiempo a las variables de la simulación//

void Deposito::nuevo_t_prox_mov () {
  t_prox_mov = t_prox_mov + random (t_prox_descarga_min, t_prox_descarga_max)/1000;
}

//Método que nos permite ver si el tanque está listo para que se realice una descarga en él//

int Deposito::ready_to_receive (int vol_prox_mov) {
  if(vol_max_seguridad > (volumen + vol_prox_mov)) {
    return 1;
  }
  else{
    return 0;
  }
}

//Método que nos permite saber el volumen del tanque en todo momento//

float Deposito::get_volumen () {
  volumen = conversion_volumen();
  return volumen;
}

//Método de conversión de voltaje a volumen//

float Deposito::conversion_volumen () {
  float resultado;
  resultado = analogRead(puerto_sensor_vol);
  resultado = resultado -10 - (r_max - (((v_r_max - resultado)/bit_PIN)*(v_r_max - resultado)/bit_PIN)*bit_PIN/5);
  resultado = (resultado*capacidad_maxima)/bit_PIN;
  return resultado;
}

//Método genera un caudal para las fugas//

void Deposito::genera_caudal () {
  if (deposito_sig->get_volumen() > 100) {
    caudal = bit_PIN_OUT;
  }
  if ((deposito_sig->get_volumen() < 100) && (deposito_sig->get_volumen() > 30))  {
    caudal = (deposito_sig->get_volumen() / 150)*bit_PIN_OUT;
  }
  if (deposito_sig->get_volumen() < 30) {
    caudal = 0;
  }
}
