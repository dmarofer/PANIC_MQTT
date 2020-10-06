#pragma once

#include <Arduino.h>
#include <NTPClient.h>					// Para la gestion de la hora por NTP
#include <WiFiUdp.h>					// Para la conexion UDP con los servidores de hora.

class Panic
{


private:
    
    bool HayQueSalvar;
	String mificheroconfig;

	unsigned long t_uptime;						// Para el tiempo que llevamos en marcha

    typedef void(*RespondeComandoCallback)(String comando, String respuesta);			// Definir como ha de ser la funcion de Callback (que le tengo que pasar y que devuelve)
	RespondeComandoCallback MiRespondeComandos = nullptr;								// Definir el objeto que va a contener la funcion que vendra de fuera AQUI en la clase.

	void Eventos_Boton (unsigned int l_Evento_Pulsador);					        	// Funcion mia de callback para eventos del pulsador


	// Para almacenar Alias (referencia) al objeto tipo NTPClient para poder usar en la clase el que viene del Main
    NTPClient &ClienteNTP;

	// Para el Bootime

	String BootTime;

	// 	#####################################################################################################################################
	//  ## PARA LA MELODIA
	// 	#####################################################################################################################################

	// NOTAS
	int c = 261.626*5; 
	int d = 293.665*5;
	int e = 329.628*5;
	int f = 349.228*5;
	int g = 391.995*5;
	int a = 440*5; 
	int Bb = 466.164*5;
	int b = 493.883*5;
	int highc = 523.251*5;

	// INTERVALOS
	int eighth = 500/2;
	int quarter = 1000/2;
	int half = 2000/2; 

	int NAvisos;

	void Cumple();
	    
public:

	enum TipoCategoriaAviso {

		AVISO_INICIO,
		AVISO_WARNING,
		AVISO_EMERGENCIA,
		AVISO_CUMPLE,
		AVISO_RECORDATORIO,

	}CategoriaAviso;


    Panic(String fich_config_Panic, NTPClient& ClienteNTP);
    ~Panic() {};

    //  Variables Publicas
	String HardwareInfo;											// Identificador del HardWare y Software
	
	// Funciones Publicas
	String MiEstadoJson(int categoria);								// Devuelve un JSON con los estados en un array de 100 chars (la libreria MQTT no puede con mas de 100)
	
    void SetRespondeComandoCallback(RespondeComandoCallback ref);	// Definir la funcion para pasarnos la funcion de callback del enviamensajes
	boolean LeeConfig();
	boolean SalvaConfig();

	void SetBootTime(String l_BootTime);

	void SetNAvisos(int l_NAvisos);

	void Avisar(TipoCategoriaAviso t_CategoriaAviso);													// Funcion para ejecutar en el Setup e inicializar el objeto
	void RunFast();													// Actualiza las propiedades de estado de este objeto en funcion del estado de motores y sensores
    
};
