#pragma once

#include "Arduino.h"

class IndicadorLed{

private:

    int pin;

    unsigned int TiempoONPulso;
    unsigned int TiempoOFFPulso;
    unsigned int NumeroPulsos;

    unsigned int TiempoONCiclo;
    unsigned int TiempoOFFCiclo;
    unsigned long TiempoCiclo;
    unsigned int NumeroPulsosCiclo;

    unsigned long CuentaMillisPulso;
    unsigned long CuentaMillisCiclo;
    unsigned int CuentaPulsos;
    
    bool Invertir;

    int PinBuzzer;
    bool BuzzerPresent;

    int FrecuenciaTono;

public:

    enum TipoEstadoLed {

        LED_APAGADO,
        LED_ENCENDIDO,
        LED_PULSO_ON,
        LED_PULSO_OFF,
        LED_CICLO,

    }EstadoLed, EstadoLedCiclos;

    IndicadorLed(int l_pin, bool l_Invertir);
    IndicadorLed(int l_pin, bool l_Invertir, int l_pinBuzzer);
    ~IndicadorLed(){};

    void Pulsos(unsigned int l_TiempoONPulso, unsigned int l_TiempoOFFPulso, unsigned int l_NumeroPulsos);
    void Ciclo(unsigned int l_TiempoONPulso, unsigned int l_TiempoOFFPulso, unsigned int l_TiempoCiclo, unsigned int l_NumeroPulsos);
    void SetFrecuencia(int l_frecuenciaTono);
    void Encender();
    void Apagar();

    void RunFast();

};



