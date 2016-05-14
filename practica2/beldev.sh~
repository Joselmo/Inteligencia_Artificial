#!/bin/bash
#Script para lanzar Belkan guardando el log de reglas en un 
# fichero "logBelkan_fecha"

_fecha=$(date +"%H_%M_%e_%b")
_file="logs/bel$_fecha.log"
make clean
make
./BelKan_Client > "${_file}"
