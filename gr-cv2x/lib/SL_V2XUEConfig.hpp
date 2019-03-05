/*----------------------------------------------------------------------------
  Universidad de Málaga - Grupo de Ingeniería de Comunicaciones
  Proyecto: <proyecto>
  ----------------------------------------------------------------------------
  MODULO:
  SL_V2XCommunication
  ----------------------------------------------------------------------------
  Autor:
  Juan Carlos Ruiz Sicilia
  ----------------------------------------------------------------------------
  Control de ediciones
  25/02/19 Creacion de la clase
  ----------------------------------------------------------------------------
  Descripción
     Esta clase almacena los parametros propios del usuario.
     Se partirán de distintos parametros si el usuario recibe o transmite.
     Mediante la funcion PSxCH_Procedures se obtienen los restantes.
  ----------------------------------------------------------------------------
  Clases
     SL_V2XUEConfig      Almacena distintos parametros del usuario.
  ----------------------------------------------------------------------------
  Tipos de datos
  sduSize            Tamaño del paquete mac sin cabecera
  SFgap              Offset de tiempo entre transmission  y retransmision.
  Linit              Offset de frecuencia de la primera transmision.
  nsubCHstart        Offset de frecuencia de la segunda transmision respecto la
                     primera
  LsubCH             Numero de subcanales reservados para el usuario
  mcs_r14            Valor de modulacion y codificacion
  ----------------------------------------------------------------------------
  Métodos [rutinas] públicos
  bool isTx()           Devuelve si la instancia posee los parametros de Rx
                        o Tx
  int getSduSize()      Devuelve sduSize
  int getSFgap()        Devuelve SFgap
  int getLinit()        Devuelve Linit
  int getNsubCHstart()  Devuelve nsubCHstart
  int getLsubCH()       Devuelve LsubCH
  int getMsc_r14()      Devuelve mcs_r14
  --------------------------------------------------------------------------*/
#ifndef SL_V2XUECONFIG_HPP
#define SL_V2XUECONFIG_HPP
/* 1.- Ficheros include externos */
//    (de tipos de datos de atributos y de parámetros de los métodos)


/* 2.- Declaración de la clase 1 */
namespace LTEv{
class SL_V2XUEConfig
{

protected:


/* 2.1.- Declaración de atributos privados */
int sduSize;
int SFgap;
int Linit;
int nsubCHstart;
int LsubCH;
int mcs_r14;

/* 2.2.- Declaración de métodos privados */


public:

/* 2.3.- Declaración de atributos públicos */


/* 2.4.- Declaración de métodos públicos */
SL_V2XUEConfig(int sduSize, int SFgap, int Linit, int nsubCHstart);
SL_V2XUEConfig(int mcs_r14, int LsubCH, int SFgap, int Linit, int nsubCHstart);
bool isTx();
int getSduSize();
int getSFgap();
int getLinit();
int getNsubCHstart();
int getLsubCH();
int getMsc_r14();

};

/* 7.- Declaración de tipos de datos */


/* 8.- Declaración de rutinas públicas */
}
#endif
