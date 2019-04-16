/********************************************************************************/
/************************************ Filter ************************************/
/********************************************************************************/

// Defines a filter type that will act as a circular list
// "filter" = circular list ?
typedef struct filter
{
	float values[NUMEROS_MEDIA];
	int front;
	float sum;
} filter;

float mediaMovel(filter &f, float newValue)
{
	// Decreases the sum value by the number being removed
	f.sum -= f.values[f.front];

	// Changes it's value
	f.values[f.front] = newValue;

	// Increases the sum value by the number inserted
	f.sum += newValue;

	// Walks one index and begin a new cicle if necessary
	f.front++;
	f.front %= NUMEROS_MEDIA;

	// Calculates de average and returns
	return f.sum / NUMEROS_MEDIA;
}


/********************************************************************************/
/***************************** Apogee detection e-e *****************************/
/********************************************************************************/
typedef struct medidas
{
	float values[NUMEROS_FILTRADOS];
	int front;
	float sum;
} medidas;
float last = 0;

// E se fossem int truncados? Talvez a "caindo em media"
int newVariation(float newValue)
{	
	/* [DEBUG] 
	Serial.print("{");
	Serial.print(last, 2);
	Serial.print(", ");
	Serial.print(newValue, 2);
	Serial.println("}");
	*/
	if(newValue < last)
	{
		last = newValue;
		return -1;
	}

	last = newValue;

	return 0;
}

void saveVariation(medidas &v, int newValue)
{
	// Decreases the sum value by the number being removed
	v.sum -= v.values[v.front];

	// Changes it's value
	v.values[v.front] = newValue;

	// Increases the sum value by the number inserted
	v.sum += newValue;

	// Walks one index and begin a new cicle if necessary
	v.front++;
	v.front %= NUMEROS_FILTRADOS;

	return;
}