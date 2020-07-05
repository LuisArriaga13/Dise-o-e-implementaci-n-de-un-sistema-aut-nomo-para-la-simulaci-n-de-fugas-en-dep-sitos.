#include "Controladora.h"
#include "Deposito.h"

//Asociación de pines de entradas y salidas//

void setup () {
  pinMode (alarma_rebosamiento, OUTPUT);
  pinMode (PIN_START, INPUT);
  pinMode (PIN_STOP, INPUT);
  pinMode (PIN_EMERGENCIA, INPUT);    
  pinMode (PIN_DIAL, INPUT); 
}


//Tanques declarados a partir de la clase deposito//

Deposito tanque_retorno (PIN_vaciado_dispensador_1, PIN_llenado_recarga, 10000, 10000, 10000, 0, 0, 0, 0, 0, &tanque_retorno, 0);

Deposito dispensador_1 (PIN_llenado_dispensador_1, PIN_vaciado_dispensador_1, sensor_vol_dispensador_1, volumen_ventas_limite,
volumen_ventas_seguridad, volumen_ventas_max, volumen_ventas_min, t_prox_venta_max, t_prox_venta_min, 0, &tanque_retorno, 0);

Deposito dispensador_2 (PIN_llenado_dispensador_2, PIN_vaciado_dispensador_2, sensor_vol_dispensador_2, volumen_ventas_limite,
volumen_ventas_seguridad, volumen_ventas_max, volumen_ventas_min, t_prox_venta_max, t_prox_venta_min, 0, &tanque_retorno, 0);

Deposito tanque_almacenamiento (222, 222, sensor_vol_almacenamiento, volumen_almacenamiento_limite, volumen_almacenamiento_seguridad, 0, 0, 0, 0, 1, &dispensador_1, 0);

Deposito tanque_recarga (PIN_llenado_recarga, PIN_vaciado_recarga, sensor_vol_recarga, volumen_recarga_limite, volumen_recarga_seguridad,
volumen_recarga_max, volumen_recarga_min, t_prox_recarga_max, t_prox_recarga_min, 1, &tanque_almacenamiento, 0);

Deposito tanque_fugas (caudal_fuga, PIN_vaciado_tanque_fugas, sensor_vol_fugas, volumen_fugas_limite, volumen_fugas_seguridad, 0, 0, 0, 0, 1, &tanque_almacenamiento, 1);


//Bucle principal donde se irán recorriendo los valores de los sensores y se ejecutaran las llamadas a las funciones pertinentes//

void loop () {

  start = digitalRead (PIN_START);
  stop_ = digitalRead (PIN_STOP);
  seta_emergencia = digitalRead (PIN_EMERGENCIA);
  dial = digitalRead (PIN_DIAL);

  tanque_recarga.get_volumen();
  tanque_almacenamiento.get_volumen();
  dispensador_1.get_volumen();
  dispensador_2.get_volumen();
  tanque_retorno.get_volumen();
  tanque_fugas.get_volumen();

  if (seta_emergencia == HIGH) {
    estado_actual = EMERGENCIA;
  }
  
  if (estado_actual == PAUSA) {
    tanque_recarga.cerrar_valvula_llenado();
    tanque_recarga.cerrar_valvula_vaciado();
    dispensador_1.cerrar_valvula_llenado();
    dispensador_1.cerrar_valvula_vaciado();
    dispensador_2.cerrar_valvula_llenado();
    dispensador_2.cerrar_valvula_vaciado();              
    if ((stop_ == LOW) && (seta_emergencia == LOW) && (start == HIGH)) {
      estado_actual = FUNC_NORMAL;
    }
  }

  if (estado_actual == FUNC_NORMAL) {
    if (stop_ == HIGH) {
      estado_actual = PAUSA;
    }
    estado_recarga = tanque_recarga.actualizar_estado(tSimulacion, seta_emergencia, 0);
    estado_disp_1 = dispensador_1.actualizar_estado(tSimulacion, seta_emergencia, 0);
    estado_disp_2 = dispensador_2.actualizar_estado(tSimulacion, seta_emergencia, 0);
    estado_fugas = tanque_fugas.actualizar_estado(tSimulacion, seta_emergencia, 0);
    if ((estado_recarga == true) || (estado_disp_1 == true) || (estado_disp_2 == true) || (estado_fugas == true)) {
      estado_actual = EMERGENCIA;
    }
  }

  if (estado_actual == EMERGENCIA) {
    digitalWrite (alarma_rebosamiento, HIGH);
    tanque_recarga.actualizar_estado(tSimulacion, HIGH, 0);
    dispensador_1.actualizar_estado(tSimulacion, HIGH, 0);
    dispensador_2.actualizar_estado(tSimulacion, HIGH, 0);   
    tanque_fugas.actualizar_estado(tSimulacion, HIGH, 0); 
    if ((seta_emergencia == LOW) && (start == HIGH)) {
      estado_actual = LLEVAR_COND_INIC;
    }
  }
  
  if (estado_actual == LLEVAR_COND_INIC) {
    digitalWrite (alarma_rebosamiento, LOW);
    tanque_recarga.actualizar_estado(tSimulacion, seta_emergencia, 0);
    tanque_recarga.cerrar_valvula_llenado();
    dispensador_1.actualizar_estado(tSimulacion, seta_emergencia, 0);
    dispensador_2.actualizar_estado(tSimulacion, seta_emergencia, 0); 
    tanque_fugas.actualizar_estado(tSimulacion, seta_emergencia, 0);     
    if (((tanque_recarga.get_volumen() <= 0) && (tanque_almacenamiento.get_volumen() <= 0) && (dispensador_1.get_volumen() <= 0) &&
    (dispensador_2.get_volumen() <= 0) && (dial == 0)) || ((dial == 1) && (start == HIGH))) {
       tanque_recarga.actualizar_estado(tSimulacion, seta_emergencia, 1);
      dispensador_1.actualizar_estado(tSimulacion, seta_emergencia, 1);
      dispensador_2.actualizar_estado(tSimulacion, seta_emergencia, 1);  
      tanque_fugas.actualizar_estado(tSimulacion, seta_emergencia, 1);
      estado_actual = FUNC_NORMAL;         
    }
  }

  //Incrementamos tiempos de simulación//

  tSimulacion = millis();
  tSimulacion = tSimulacion/1000;
}
