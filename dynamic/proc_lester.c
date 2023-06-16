#include "../rpc.h"
#include <stdlib.h>
#include <stdio.h>

/*
Autor: Lester Enrique Perez Carballedo
*/
typedef struct
{
    reportable_t parent;

    char str [100]; // cadena de caractéres de entrada que se analiza
    int resultado;  // valor donde se guarda si los signos de agrupamientos (paréntesis, corchetes o llaves) contenidos en la cadena están balanceados (1) o no (0)
} my_struct_t;

/*Función que carga los parámetros de entrada del buffer*/
void *parse_parameters(void *data)
{
    const char *buf = (const char *)(data);

    my_struct_t *d = (my_struct_t *)(malloc(sizeof(my_struct_t)));

    if (d)
    {   //Se lee la cadena del primer parámetro y se asigna a str
        sscanf(buf, "%s", d->str);
    }

    return (void *)d;
}

/*En el cuerpo de esta función lo que se hace es procesar la cadena str para determinar si 
los signos de agrupamientos (paréntesis, corchetes o llaves) contenidos en ella están balanceados, 
en caso de ser cierto se guarda en resultado = 1, caso contrario resultado = 0.*/
void *do_work(void *data)
{
    my_struct_t *d = (my_struct_t *)(data);
    int longitud = strlen(d->str);
    char pila[longitud]; // Crear una pila para almacenar los signos de agrupamiento abiertos
    int tope = 0; // Inicializar el tope de la pila a cero
    
    for (int i = 0; i < longitud; i++) {
        if (d->str[i] == '(' || d->str[i] == '[' || d->str[i] == '{') {
            pila[tope] = d->str[i];
            tope++;
        } else if (d->str[i] == ')' || d->str[i] == ']' || d->str[i] == '}') {
            if (tope == 0) {
                d->resultado = 0; // Si la pila está vacía, los signos de agrupamiento no están balanceados
                return data;
            }
            char ultimo = pila[tope-1];
            tope--;
            if ((d->str[i] == ')' && ultimo != '(') || \
                (d->str[i] == ']' && ultimo != '[') || \
                (d->str[i] == '}' && ultimo != '{')) {
                d->resultado = 0; // Si el último signo de agrupamiento abierto no coincide con el signo de agrupamiento cerrado, los signos de agrupamiento no están balanceados
                return data;
            }
        }
    }
    
    d->resultado = (tope == 0); // Si la pila está vacía al final, los signos de agrupamiento están balanceados
    return data;
}
    

/*En esta función se genera el resultado en consola a partir de los datos procesados anteriormente*/
reportable_t *report(void *data)
{
    my_struct_t *d = (my_struct_t *)(data);

    d->parent.data = (char *)(malloc(255 * sizeof(char)));

    //Contenido a imprimir, para decidir si los signos de agrupamientos (paréntesis, corchetes o llaves) contenidos en la cadena están balanceados se tiene en cuenta el valor de resultado
    snprintf(d->parent.data, 255, "La cadena: '%s' %s\n", d->str, 
    d->resultado?"los signos de agrupamientos contenidos en la cadena están balanceados":"los signos de agrupamientos contenidos en la cadena no están balanceados");
    d->parent.len = strlen(d->parent.data);

    return (reportable_t *)(data);
}

void clean_up(void *params, void *result, reportable_t *report)
{
    if (report && report->data)
    {
        free(report->data);
    }

    if (params)
    {
        free(params);
    }
}