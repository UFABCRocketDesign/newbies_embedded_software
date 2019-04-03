/********************************************************************************/
/************************************ Filter ************************************/
/***************************** Apogee detection e-e *****************************/
/********************************************************************************/

// Defines a filter type that will act as a circular list
// "filter" = circular list ?
typedef struct filter
{
	float values[NUMEROS_MEDIA];
	int front;
	float sum;
} filter;

typedef struct
{
	float values[NUMEROS_FILTRADOS];
	int front;
	int out;
	float varSum;
} medidas;

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

void novaMedida(medidas &m, float newValue)
{
	int back = m.front-1;
	if(back == -1)
		back = NUMEROS_FILTRADOS-1;

	m.varSum -= m.values[m.front] - m.values[m.out];

	m.out = m.values[m.front];
	m.values[m.front] = newValue;

	m.varSum += m.values[m.front] - m.values[back];

	m.front++;
	m.front %= NUMEROS_FILTRADOS;
}