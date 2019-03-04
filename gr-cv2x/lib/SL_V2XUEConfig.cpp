/*----------------------------------------------------------------------------
  Universidad de Málaga - Grupo de Ingeniería de Comunicaciones
  Proyecto: <proyecto>
  ----------------------------------------------------------------------------
  MODULO:      <nombre>
  ----------------------------------------------------------------------------
  Autor:       <autor>
  ----------------------------------------------------------------------------
  Control de ediciones
     <fecha>   <descripción>
  ----------------------------------------------------------------------------
  Descripción
     xxxxxxxxxxxxxxxxxxxxxxxxx
  ----------------------------------------------------------------------------
  Métodos [rutinas] públicos
     <nombre>          <descripción>
     <nombre>          <descripción>
  ----------------------------------------------------------------------------*/
  //Métodos [rutinas] internos

/*----------------------------------------------------------------------------*/


/* 1.- Variables de compilación condicional */


/* 2.- Ficheros include del módulo */
#include "SL_V2XUEConfig.hpp"

/* 3.- Ficheros include externos */


/* 4.- Ficheros include del proyecto */


/* 5.- Definición de literales */


/* 6.- Declaración de rutinas internas */


/* 7.- Implementación de las clases */

/* 7.1. – Clase SL_V2XUEConfig */
namespace LTEv{
/*------------------------------------------------------------------------------
                                MÉTODOS PÚBLICOS
/*----------------------------------------------------------------------------*/
SL_V2XUEConfig::SL_V2XUEConfig(int sduSize, int SFgap, int Linit, int nsubCHstart){
   this->sduSize = sduSize;
   this->SFgap = SFgap;
   this->Linit = Linit;
   this->nsubCHstart = nsubCHstart;
   LsubCH = -1;
   mcs_r14 = -1;
}
SL_V2XUEConfig::SL_V2XUEConfig(int mcs_r14, int Linit, int SFgap, int nsubCHstart, int LsubCH){
   sduSize = -1;
   this->SFgap = SFgap;
   this->Linit = Linit;
   this->nsubCHstart = nsubCHstart;
   this->LsubCH = LsubCH;
   this->mcs_r14 = mcs_r14;
}
bool SL_V2XUEConfig::isTx(){
   return (sduSize >= 0);
}
int SL_V2XUEConfig::getSduSize(){
   return sduSize;
}

int SL_V2XUEConfig::getSFgap(){
   return SFgap;
}
int SL_V2XUEConfig::getLinit(){
   return Linit;
}

int SL_V2XUEConfig::getNsubCHstart(){
   return nsubCHstart;
}

int SL_V2XUEConfig::getLsubCH(){
   return LsubCH;
}

int SL_V2XUEConfig::getMsc_r14(){
   return mcs_r14;
}

/*------------------------------------------------------------------------------
                                MÉTODOS PRIVADOS
/*----------------------------------------------------------------------------*/




/* 8.- Implementación de las rutinas */

/*------------------------------------------------------------------------------
                                RUTINAS PÚBLICAS
/*----------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
                                RUTINAS INTERNAS
/*----------------------------------------------------------------------------*/

}
