#include "Planta.h"
#include "Deposito_sim.h"

//Asociación de valvulas de entradas y salidas//

void setup() {
}

//Tanques declarados a partir de la clase deposito_sim//

Deposito_sim tanque_retorno (valvula_vaciado_dispensador_1, valvula_llenado_recarga, sensor_vol_recarga, caudal_recarga_in, caudal_recarga_out, 500, 1000, 0,
&tanque_retorno, 0);

Deposito_sim tanque_recarga (valvula_llenado_recarga, valvula_vaciado_recarga, sensor_vol_recarga, caudal_recarga_in, caudal_recarga_out, vol_inicial_recarga, volumen_recarga_limite, 1,
&tanque_retorno, 0);

Deposito_sim tanque_almacenamiento (LOW, LOW, sensor_vol_almacenamiento, 0, 0, vol_inicial_almacenamiento, volumen_almacenamiento_limite, 0, &tanque_recarga, 0);

Deposito_sim dispensador_1 (valvula_llenado_dispensador_1, valvula_vaciado_dispensador_1, sensor_vol_dispensador_1, caudal_disp_1_in, caudal_disp_1_out, vol_inicial_disp_1, volumen_ventas_limite, 1,
&tanque_almacenamiento, 0);

Deposito_sim dispensador_2 (valvula_llenado_dispensador_2, valvula_vaciado_dispensador_2, sensor_vol_dispensador_2, caudal_disp_2_in, caudal_disp_2_out, vol_inicial_disp_2, volumen_ventas_limite, 1,
&tanque_almacenamiento, 0);

Deposito_sim tanque_fugas (HIGH, valvula_vaciado_tanque_fugas, sensor_vol_fugas, caudal_fugas_in, caudal_fugas_out, vol_inicial_fugas, volumen_fugas_limite, 1,
&tanque_almacenamiento, 1);

//Bucle que genera la lógica en función de las órdenes recibidas por la controladora//

void loop() {

  //Actualizamos el valor de volumen de los tanques//
  duracion_ciclo = millis() - duracion_ciclo;
  tanque_recarga.actualizar_volumen(duracion_ciclo);
  tanque_almacenamiento.incrementar_volumen(tanque_recarga.get_decremento());
  tanque_retorno.disminuir_volumen(tanque_recarga.get_incremento());
  dispensador_1.actualizar_volumen(duracion_ciclo);
  tanque_almacenamiento.disminuir_volumen(dispensador_1.get_incremento());
  tanque_retorno.incrementar_volumen(dispensador_2.get_decremento());
  dispensador_2.actualizar_volumen(duracion_ciclo);
  tanque_almacenamiento.disminuir_volumen(dispensador_2.get_incremento());
  tanque_retorno.incrementar_volumen(dispensador_2.get_decremento());
  tanque_fugas.actualizar_volumen(duracion_ciclo);
  tanque_almacenamiento.actualizar_volumen(duracion_ciclo); 
  tanque_retorno.actualizar_volumen(duracion_ciclo); 

  //Mandamos el valor actual de volumen de cada tanque a la controladora//

  tanque_recarga.sobrescribir_sensor();
  tanque_almacenamiento.sobrescribir_sensor();
  dispensador_1.sobrescribir_sensor();
  dispensador_2.sobrescribir_sensor();
  tanque_fugas.sobrescribir_sensor();
  
}
