/********************************************************************************/
/***************************** Apogee detection e-e *****************************/
/********************************************************************************/
#define NUMEROS_MEDIA 5
#define NUMEROS_FILTRADOS 20

// Defines a filter type that will act as a circular list
typedef struct filter
{
	int values[NUMEROS_MEDIA];
	int front;
	int sum;
} filter;

float mediaMovel(filter &f, int newValue)
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