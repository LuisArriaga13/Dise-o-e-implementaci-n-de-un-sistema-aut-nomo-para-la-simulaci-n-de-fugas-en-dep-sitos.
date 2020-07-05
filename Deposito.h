#include "Arduino.h"

class Deposito { 
  
  private:
  
  //Atributos estáticos//
  bool recarga_o_descarga;                  //Variable que nos indica si, a la hora de simular, nos interesa si el deposito se recarga o descarga agua en el siguiente//  
  bool es_fugas ;                           //Parámetro que nos indicará si el tanque es el de fugas (1)//
  int PIN_llenado_tanque;                   //Señal que recoge la válvula de llenado//
  int PIN_vaciado_tanque;                   //Señal que recoge la válvula de llenado//
  int puerto_sensor_vol;                    //Señal que da el valor del sensor de volumen del propio deposito//
  Deposito *deposito_sig;                   //Puntero que apunta al deposito siguiente con objetivo de obtener el permiso para realizar una descarga//
  //Atributos de simulación//
  float capacidad_maxima;                   //Límite máximo que puede alcanzar el deposito//
  float vol_max_seguridad;                  //Límite hasta el cual se podrá llenar el deposito sin llegar al umbral máximo//
  float volumen_descarga_max;               //Límites de la simulación//
  float volumen_descarga_min;               //Límites de la simulación//
  float t_prox_descarga_max;                //Límites de la simulación//
  float t_prox_descarga_min;                //Límites de la simulación//
  //Atributos de estado//
  int estado_actual;                        //Recoge el estado en el que se encuentra el deposito//
  float volumen;                            //Señal que almacena el valor de volumen del deposito// 
  float caudal;                             //En caso de que el tanque sea el de fugas, esta variable guardará cuanto fugará//   
  float vol_prox_mov;                       //Cantidad de agua que descargará el deposito cuando la simulación lo diga//
  float t_prox_mov;                         //Tiempo en el que se realizará el próximo movimiento del tanque//
  
  public:

  //Constructor de la clase deposito//

  Deposito (int _PIN_llenado_tanque, int _PIN_vaciado_tanque, int _puerto_sensor_vol, float _capacidad_maxima, float _vol_max_seguridad, float _volumen_descarga_max,
  float _volumen_descarga_min, float _t_prox_descarga_max, float _t_prox_descarga_min, bool _recarga_o_descarga, Deposito *deposito_sig, bool _es_fugas);

  //Método que comprueba el estado del deposito//
  
  bool actualizar_estado (int tSimulacion, int alarma, int volver_a_empezar);

  //Método que abre la válvula de llenado//

  void abrir_valvula_llenado ();

  //Método que abre la válvula de vaciado//

  void abrir_valvula_vaciado ();

  //Método que cierra la válvula de llenado//

  void cerrar_valvula_llenado ();

  //Método que cierra la válvula de vaciado//

  void cerrar_valvula_vaciado ();

  //Método que generará valores aleatorios de volumen a las variables de la simulación//

  void nuevo_vol_descarga ();
  
  //Método que generará valores aleatorios de tiempo a las variables de la simulación//

  void nuevo_t_prox_mov ();

  //Método que nos permite ver si el tanque está listo para que se realice una descarga en él//

  int ready_to_receive (int vol_prox_mov);

  //Método que nos permite saber el volumen del tanque en todo momento//

  float get_volumen ();

  //Método que convierte la señal recibida por los sensores a volumen//

  float conversion_volumen ();

  //Método genera un caudal para las fugas//

  void genera_caudal ();
  
};
