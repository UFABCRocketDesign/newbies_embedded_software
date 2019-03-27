// Programa : Teste BMP085 - Barometro / temperatura
// Adaptacoes : Arduino e Cia

/*Based largely on code by  Jim Lindblom
 Get pressure, altitude, and temperature from the BMP085.
 Serial.print it out at 9600 baud to serial monitor.
*/

#include <Wire.h>
#include "bmp085.h"
#include "filtro.h"

// Creates a filter and initializes all variables as 0
// filter *averager = (filter*) malloc(sizeof(filter));
// filter *averager = new filter();
filter averager = filter();

// Medidas filtradas
int medidasFiltradas[NUMEROS_FILTRADOS];


void setup()
{
	Serial.begin(9600);
	Wire.begin();
	// Inicializa o BMP085
	bmp085Calibration();
}

void loop()
{
	// Chama a rotina que calcula a temperatura
	// Esta rotina DEVE ser executada primeiro
	float temperature = bmp085GetTemperature(bmp085ReadUT());
	// Chama a rotina que calcula a pressao
	float pressure = bmp085GetPressure(bmp085ReadUP());
	// Chama a rotina que calcula atmosfera
	float atm = pressure / 101325;
	// Chama a rotina que calcula a altitude
	float altitude = calcAltitude(pressure);

	Serial.print("Temperatura: ");
	// Mostra a temperatura com 2 casas decimais
	Serial.print(temperature, 2);
	Serial.println(" C");

	Serial.print("Pressao: ");
	Serial.print(pressure, 0);
	Serial.println(" Pa");

	Serial.print("Atmosfera padrao : ");
	// Mostra o valor com 4 casas decimais
	Serial.println(atm, 4); //display 4 decimal places

	Serial.print("Altitude: ");
	// Mostra o valor com 2 casas decimais
	Serial.print(altitude, 2);
	Serial.println(" M");

	Serial.println();

	//Aguarda 5 segundos e reinicia o processo
	//delay(5000);
}