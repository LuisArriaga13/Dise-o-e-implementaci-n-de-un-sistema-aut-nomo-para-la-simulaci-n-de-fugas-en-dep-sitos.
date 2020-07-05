//Declaración de variables//

//Variables asociadas a las entradas y salidas analógicas del arduino, encargadas de las lectura de los sensores//

#define sensor_vol_recarga A0                                                                                      //Sensor de volumen (báscula) asociado al tanque de recarga//
#define sensor_vol_almacenamiento A1                                                                               //Sensor de volumen (báscula) asociado al tanque de descarga//
#define sensor_vol_dispensador_1 A2                                                                                //Sensor de volumen (báscula) asociado al dispensador 1//
#define sensor_vol_dispensador_2 A3                                                                                //Sensor de volumen (báscula) asociado al dispensador 2//
#define sensor_vol_fugas A4                                                                                        //Sensor de volumen (báscula) asociado al tanque de fugas//

//Variables asociadas a las salidas digitales del arduino y las PINes de apertura y cierre de válvulas, así como las señales de emergencia//

#define PIN_llenado_recarga 1                                                                                      //Señal que ordena el llenado del tanque de recarga//
#define PIN_vaciado_recarga 2                                                                                      //Señal que ordena el vaciado del tanque de recarga en el tanque principal//                              
#define PIN_llenado_dispensador_1 3                                                                                //Señal que ordena el llenado del dispensador 1// 
#define PIN_vaciado_dispensador_1 4                                                                                //Señal que ordena el vaciado del dispensador 1// 
#define PIN_llenado_dispensador_2 5                                                                                //Señal que ordena el llenado del dispensador 2// 
#define PIN_vaciado_dispensador_2 6                                                                                //Señal que ordena el vaciado del dispensador 2//
#define caudal_fuga 9                                                                                              //Salida analógica que del caudal que se está fugando// 
#define PIN_vaciado_tanque_fugas 7                                                                                 //Señal que ordena el vaciado del tanque de fugas//                                                                                 

//Variables que definen los límites de los depositos//

float volumen_recarga_limite = 20;                                                                                 //Valor máximo de volumen del tanque de recarga//
float volumen_recarga_seguridad = 18;                                                                              //Valor de seguridad de volumen del tanque de recarga//
float volumen_almacenamiento_limite = 150;                                                                         //Valor máximo de volumen del tanque de almacenamiento//
float volumen_almacenamiento_seguridad = 100;                                                                      //Valor de seguridad de volumen del tanque de almacenamiento//
float volumen_ventas_limite = 1.5;                                                                                 //Valor máximo de volumen de los dispensadores//
float volumen_ventas_seguridad = 1.2;                                                                              //Valor de seguridad de volumen de los dispensadores//
float volumen_fugas_limite = 1.5;                                                                                  //Valor máximo de volumen del tanque de fugas//
float volumen_fugas_seguridad = 1.2;                                                                               //Valor de seguridad de volumen del tanque de fugas//

//Variables que definen los límites de cada una de las distribuciones aleatorias que se usarán a lo largo del código//

#define volumen_recarga_min 100                                                                                    //Valor mínimo del volumen de descarga para su distribución aleatoria//
#define volumen_recarga_max 1600                                                                                   //Valor máximo del volumen de descarga para su distribución aleatoria//
#define volumen_ventas_min 10                                                                                      //Valor mínimo del volumen de ventas para su distribución aleatoria//
#define volumen_ventas_max 100                                                                                     //Valor máximo del volumen de ventas para su distribución aleatoria//
#define t_prox_recarga_min 25000                                                                                   //Valor mínimo del tiempo de la próxima descarga para su distribución aleatoria//
#define t_prox_recarga_max 50000                                                                                   //Valor máximo del tiempo de la próxima descarga para su distribución aleatoria//
#define t_prox_venta_min 13000                                                                                     //Valor mínimo del tiempo de la próxima ventas para su distribución aleatoria//
#define t_prox_venta_max 20000                                                                                     //Valor máximo del tiempo de la próxima ventas para su distribución aleatoria//

//Variables temporales que influyen en la simulación de los eventos aleatorios//

bool estado_recarga;
bool estado_disp_1;
bool estado_disp_2;
bool estado_fugas;
long tSimulacion = 0;                                                                                               //Tiempo de la simulación//

//Variables de la máquina de estados de la controladora y del panel de control//

#define FUNC_NORMAL 0
#define PAUSA 1
#define EMERGENCIA 2
#define LLEVAR_COND_INIC 3
int estado_actual = FUNC_NORMAL;

#define alarma_rebosamiento 8                                                                                      //Señal de alarma en caso de rebosamiento//
#define PIN_START 0                                                                                                //Botón START//
#define PIN_STOP 10                                                                                                //Botón STOP/ 
#define PIN_EMERGENCIA 11                                                                                          //Reset de la planta//                             
#define PIN_DIAL 12                                                                                                //Posición del dial (0->automático, 1->manual)//  
int start;                                                                                                         //Señal de START//
int stop_;                                                                                                         //Señal de STOP/ 
int seta_emergencia;                                                                                               //Señal de reset//                             
int dial;                                                                                                          //Señal que recoge la posición del dial (0->automático, 1->manual)// 
