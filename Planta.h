//Declaración de variables//

//Variables asociadas a las válvulas del sistema u órdenes que manda la controladora (Entradas de la planta simulada)//

#define valvula_llenado_recarga 13                                                             //Válvula de llenado del tanque de recarga//     
#define valvula_vaciado_recarga 2                                                             //Válvula de vaciado del tanque de recarga//                              
#define valvula_llenado_dispensador_1 12                                                      //Válvula de llenado del dispensador 1// 
#define valvula_vaciado_dispensador_1 4                                                       //Válvula de vaciado del dispensador 1// 
#define valvula_llenado_dispensador_2 5                                                       //Válvula de llenado del dispensador 2// 
#define valvula_vaciado_dispensador_2 8                                                       //Válvula de vaciado del dispensador 2// 
#define valvula_vaciado_tanque_fugas 7                                                        //Válvula de vaciado del tanque de fugas// 

//Variables asociadas al valor que tomaran los sensores en la simulación de la planta (Salidas de la planta simulada)//

#define sensor_vol_recarga 6                                                                  //Variable que almacena el volumen asociado al tanque de recarga//
#define sensor_vol_almacenamiento 9                                                           //Variable que almacena el volumen asociado al tanque de almacenamiento//
#define sensor_vol_dispensador_1 10                                                           //Variable que almacena el volumen asociado al dispensador 1//
#define sensor_vol_dispensador_2 11                                                           //Variable que almacena el volumen asociado al dispensador 2//
#define sensor_vol_fugas 3                                                                    //Variable que almacena el volumen asociado al tanque de fugas//
#define caudal_fugas_in A0                                                                    //Variable que da valor al caudal de salida del tanque de fugas//

//Variables de caudal que serán la entrada y salida de los depositos//

#define caudal_recarga_in 10                                                                  //Variable que da valor al caudal de entrada del tanque de recarga//
#define caudal_recarga_out 10                                                                 //Variable que da valor al caudal de salida del tanque de recarga//
#define caudal_disp_1_in 10                                                                   //Variable que da valor al caudal de entrada del dispensador 1//
#define caudal_disp_1_out 10                                                                  //Variable que da valor al caudal de salida del dispensador 1//
#define caudal_disp_2_in 10                                                                   //Variable que da valor al caudal de entrada del dispensador 2//
#define caudal_disp_2_out 10                                                                  //Variable que da valor al caudal de salida del dispensador 2//
#define caudal_fugas_out 10                                                                   //Variable que da valor al caudal de salida del tanque de fugas//

//Variables que definen el volumen inicial de los depositos//

#define vol_inicial_recarga 0                                                                 //Valor inicial de volumen del tanque de recarga//
#define vol_inicial_almacenamiento 80                                                         //Valor inicial de volumen del tanque de almacenamiento//
#define vol_inicial_disp_1 0                                                                  //Valor inicial de volumen del dispensador 1//
#define vol_inicial_disp_2 0                                                                  //Valor inicial de volumen del dispensador 2//
#define vol_inicial_fugas 0                                                                   //Valor inicial de volumen del tanque de fugas//

//Variables que definen los límites de los depositos//

#define volumen_recarga_limite 20                                                             //Valor máximo de volumen del tanque de recarga//
#define volumen_almacenamiento_limite 150                                                     //Valor máximo de volumen del tanque de almacenamiento//
#define volumen_ventas_limite 1.5                                                             //Valor máximo de volumen de los dispensadores//
#define volumen_fugas_limite 1.5                                                              //Valor máximo de volumen del tanque de fugas//
float duracion_ciclo = 0;                                                                     //Tiempo que dura un ciclo de simulación//
