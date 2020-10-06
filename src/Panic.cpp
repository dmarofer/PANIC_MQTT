
#include "Panic.h"
#include <Arduino.h>
#include <ArduinoJson.h>				// OJO: Tener instalada una version NO BETA (a dia de hoy la estable es la 5.13.4). Alguna pata han metido en la 6
#include <NTPClient.h>					// Para la gestion de la hora por NTP
#include <WiFiUdp.h>					// Para la conexion UDP con los servidores de hora.
#include <FS.h>							// Libreria Sistema de Ficheros
#include <Configuracion.h>
#include <IndicadorLed.h>
#include <Pulsador.h>

Pulsador Botonazo (PINPULSADOR, INPUT, DEBOUNCEBOTONAZO, HOLDTIMEBOTONAZO, false);
//Pulsador Botonazo (PINPULSADOR, INPUT_PULLUP, DEBOUNCEBOTONAZO, HOLDTIMEBOTONAZO, true);
IndicadorLed LedBotonazo(PINLED, false, PINBUZZER);

Panic::Panic(String fich_config_Panic, NTPClient& ClienteNTP) : ClienteNTP(ClienteNTP) {

    HardwareInfo = "Panic-1.0b";
	HayQueSalvar = false;
	mificheroconfig = fich_config_Panic;
	CategoriaAviso = Panic::AVISO_INICIO;
	BootTime = "NA";

}

void Panic::SetRespondeComandoCallback(RespondeComandoCallback ref) {

	MiRespondeComandos = (RespondeComandoCallback)ref;

}

// Metodo que devuelve un JSON con el estado
String Panic::MiEstadoJson(int categoria) {

	DynamicJsonBuffer jBuffer;
	JsonObject& jObj = jBuffer.createObject();

	struct rst_info *rtc_info = system_get_rst_info(); 

	// Dependiendo del numero de categoria en la llamada devolver unas cosas u otras
	switch (categoria)
	{

	case 1:

		// Esto llena de objetos de tipo "pareja propiedad valor"
		jObj.set("HI", HardwareInfo);									// Info del Hardware
		jObj.set("FH", ESP.getFreeHeap());								// Heap Libre
		jObj.set("TIME", ClienteNTP.getFormattedTime());				// HORA
		jObj.set("BOOT", BootTime);										// HORA de arranque del sistema
		jObj.set("UPT", t_uptime);										// Uptime en segundos
		//jObj.set("SWITCH", Botonazo.LeeEstado());						// Info de la conexion WIFI y MQTT
		jObj.set("RC", rtc_info->reason);								// Reset Cause (0=POWER ON, 1=WTD reset, 4=SOFTWARE RESET, 6=BOTON RESET, )
		jObj.set("RR", ESP.getResetReason());							// Reset Reason

		break;

	case 2:

		jObj.set("INFO2", "INFO2");							
		
		break;

	default:

		jObj.set("NOINFO", "NOINFO");						// MAL LLAMADO

		break;
	}


	// Crear un buffer (aray de 100 char) donde almacenar la cadena de texto del JSON
	char JSONmessageBuffer[200];

	// Tirar al buffer la cadena de objetos serializada en JSON con la propiedad printTo del objeto de arriba
	jObj.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));

	// devolver el char array con el JSON
	return JSONmessageBuffer;
	
}

boolean Panic::SalvaConfig(){
	

	File mificheroconfig_handler = SPIFFS.open(mificheroconfig, "w");

	if (!mificheroconfig_handler) {
		Serial.println("No se puede abrir el fichero de configuracion de mi proyecto");
		return false;
	}

	if (mificheroconfig_handler.print(MiEstadoJson(1))){

		return true;

	}

	else {

		return false;

	}

}

boolean Panic::LeeConfig(){

	Botonazo.SetCallbackEventos(std::bind(&Panic::Eventos_Boton, this, std::placeholders::_1));

	// Sacar del fichero de configuracion, si existe, las configuraciones permanentes
	if (SPIFFS.exists(mificheroconfig)) {

		File mificheroconfig_handler = SPIFFS.open(mificheroconfig, "r");
		if (mificheroconfig_handler) {
			size_t size = mificheroconfig_handler.size();
			// Declarar un buffer para almacenar el contenido del fichero
			std::unique_ptr<char[]> buf(new char[size]);
			// Leer el fichero al buffer
			mificheroconfig_handler.readBytes(buf.get(), size);
			DynamicJsonBuffer jsonBuffer;
			JsonObject& json = jsonBuffer.parseObject(buf.get());
			if (json.success()) {

				Serial.print("Configuracion de mi proyecto Leida: ");
				json.printTo(Serial);
				Serial.println("");
				return true;

			}

			return false;

		}

		return false;

	}

	return false;

}

void Panic::SetBootTime(String l_BootTime){

	if (BootTime == "NA"){BootTime = l_BootTime;}

}

void Panic::Avisar(TipoCategoriaAviso t_CategoriaAviso){

	CategoriaAviso=t_CategoriaAviso;

	switch (CategoriaAviso){

		case AVISO_INICIO:

			LedBotonazo.Pulsos(50,50,2,1000);
									
		break;
		
		case AVISO_WARNING:

			LedBotonazo.Pulsos(500,500,3,300);

		break;
		
		case AVISO_EMERGENCIA:

			LedBotonazo.Pulsos(200,200,15,300);

		break;

		case AVISO_CUMPLE:

			LedBotonazo.Encender();
			this->Cumple();
			LedBotonazo.Apagar();
			LedBotonazo.Ciclo(200,200,8000,NAvisos);

		break;

		case AVISO_RECORDATORIO:

			LedBotonazo.Pulsos(150,800,6,800);
			//LedBotonazo.Pulsos(1000,800,1,800);
			
		break;
		
		
	}


}

void Panic::Cumple(){

	tone(PINBUZZER,c,150*2); 
	delay(200*2); 
	tone(PINBUZZER,c,50*2); 
	delay(50*2); 
	tone(PINBUZZER,d,quarter); 
	delay(quarter); 
	tone(PINBUZZER,c,quarter); 
	delay(quarter); 
	tone(PINBUZZER,f,quarter); 
	delay(quarter); 
	tone(PINBUZZER,e,half); 
	delay(half); 
	tone(PINBUZZER,c,150*2); 
	delay(200*2); 
	tone(PINBUZZER,c,50*2); 
	delay(50*2); 
	tone(PINBUZZER,d,quarter); 
	delay(quarter); 
	tone(PINBUZZER,c,quarter); 
	delay(quarter); 
	tone(PINBUZZER,g,quarter); 
	delay(quarter); 
	tone(PINBUZZER,f,half); 
	delay(half); 
	tone(PINBUZZER,c,150*2); 
	delay(200*2); 
	tone(PINBUZZER,c,50*2); 
	delay(50*2); 
	tone(PINBUZZER,highc,quarter); 
	delay(quarter); 
	tone(PINBUZZER,a,quarter); 
	delay(quarter); 
	tone(PINBUZZER,f,quarter); 
	delay(quarter); 
	tone(PINBUZZER,e,quarter); 
	delay(quarter); 
	tone(PINBUZZER,d,half); 
	delay(half); 
	tone(PINBUZZER,Bb,150*2); 
	delay(200*2); 
	tone(PINBUZZER,Bb,50*2); 
	delay(50*2); 
	tone(PINBUZZER,a,quarter); 
	delay(quarter); 
	tone(PINBUZZER,f,quarter); 
	delay(quarter); 
	tone(PINBUZZER,g,quarter); 
	delay(quarter); 
	tone(PINBUZZER,f,half); 
	delay(half);

}

void Panic::SetNAvisos(int l_NAvisos){

	NAvisos = l_NAvisos;
	LedBotonazo.Apagar();
	LedBotonazo.Ciclo(200,200,8000,NAvisos);

}

void Panic::Eventos_Boton (unsigned int l_Evento_Pulsador){

	switch (l_Evento_Pulsador){

		case Pulsador::PUL_PULSACION_SIMPLE:
			
			this->Avisar(Panic::AVISO_EMERGENCIA);
			this->MiRespondeComandos("SWITCH","PULSADO");
						
		break;

		case Pulsador::PUL_HOLD:
			
			this->Avisar(Panic::AVISO_CUMPLE);
			this->MiRespondeComandos("SWITCH","HOLD");
						
		break;
				
		default:
		break;
	
	}


}

// Esta funcion se lanza desde una Task y hace las "cosas periodicas de la clase". No debe atrancarse nunca tampoco por supuesto (ni esta ni ninguna)
void Panic::RunFast() {
	
	LedBotonazo.RunFast();
	Botonazo.Run();

	
	
	if (HayQueSalvar){

		SalvaConfig();
		HayQueSalvar = false;

	}

}
