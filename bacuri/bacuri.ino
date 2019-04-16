// Programa : Teste BMP085 - Barometro / temperatura
// Adaptacoes : Arduino e Cia

/*Based largely on code by  Jim Lindblom
 Get pressure, altitude, and temperature from the BMP085.
 Serial.print it out at 9600 baud to serial monitor.
*/

#include <Wire.h>
#include <SD.h>
#include "configurations.h"
#include "bmp085.h"
#include "filtro.h"

// Define de SD object (?)
File dataFile = SD.open("test.txt", FILE_WRITE);
int sdCounter = 0;

// Creates a filter and initializes all variables as 0
// filter *averager = (filter*) malloc(sizeof(filter));
// filter *averager = new filter();
filter averager = filter();

// Medidas filtradas
medidas medidasFiltradas = medidas();

float initialAltitude;

unsigned long apogeeDetected = 0;

void setup()
{
	Serial.begin(250000);
	Wire.begin();
	// Inicializa o BMP085
	bmp085Calibration();

	pinMode(4, OUTPUT);

	if(!SD.begin(53))
	{
		Serial.println("Faiô");
	}

	dataFile = SD.open("test.txt");

	for(int i=0; i<VALORES_INICIAIS; i++)
	{
		bmp085GetTemperature(bmp085ReadUT());
		initialAltitude += calcAltitude(bmp085GetPressure(bmp085ReadUP()));
	}
	initialAltitude /= VALORES_INICIAIS;

	Serial.print("Altitude inicial: ");
	Serial.print(initialAltitude, 2);
	Serial.println(" m");

	if(dataFile)
	{
		dataFile.print("Altitude inicial: ");
		dataFile.print(initialAltitude, 2);
		dataFile.println(" m");
	}
}

void loop()
{
	// Chama a rotina que calcula a temperatura (C)
	// Esta rotina DEVE ser executada primeiro
	float temperature = bmp085GetTemperature(bmp085ReadUT());
	// Chama a rotina que calcula a pressao (Pa)
	float pressure = bmp085GetPressure(bmp085ReadUP());
	// Chama a rotina que calcula atmosfera (atm)
	float atm = pressure / 101325;
	// Chama a rotina que calcula a altitude (m)
	float altitude = calcAltitude(pressure);
	// Calcula a altura (m)
	float altura = altitude - initialAltitude;

	Serial.print(altura, 2);
	Serial.print(" ");
	float filtrado = mediaMovel(averager,altura);
	Serial.print(filtrado, 2);
	Serial.print(" ");
	saveVariation(medidasFiltradas, newVariation(filtrado));
	Serial.print(medidasFiltradas.sum, 0);

	

/* [DEBUG]
	float filtrado = mediaMovel(averager, altura);
	saveVariation(medidasFiltradas, newVariation(filtrado));

	Serial.println("Filtro ");
	printVec(averager.values, NUMEROS_MEDIA);
	Serial.println("Medidas: ");
	printVec(medidasFiltradas.values, NUMEROS_FILTRADOS);

	Serial.print("Soma = ");
	Serial.println(medidasFiltradas.sum, 2);
*/
	if((!apogeeDetected)&&(medidasFiltradas.sum < LIMITE_APOGEU))
	{
		digitalWrite(4, HIGH);
		apogeeDetected = micros();
		Serial.println("AAAAAAAApogeu!");
		dataFile.println("AAAAAAAApogeu!");
	}

	if(micros() - apogeeDetected == 3000)
	{
		digitalWrite(4, LOW);
	}

	Serial.println();

	if(dataFile)
	{
		dataFile.print(altura, 2);
		dataFile.print(" ");
		dataFile.print(filtrado, 2);
		dataFile.print(" ");
		dataFile.print(medidasFiltradas.sum, 0);
		dataFile.println();
	}

	sdCounter++;
	if(sdCounter > 100){
		sdCounter = 0;
		dataFile.close();
		dataFile = SD.open("test.txt", FILE_WRITE);
	}

	//Aguarda 5 segundos e reinicia o processo
	//delay(5000);
}

void printVec(float v[], int size){
	int i;
	Serial.print("{ ");
	for(i=0; i<size-1; i++)
	{
		Serial.print(v[i], 2);
		Serial.print(", ");
	}
	Serial.print(v[i], 2);
	Serial.println("}");
}
