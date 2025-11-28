#include "filters.h"


namespace Filters {

    
  // ======================================================
  // Filtro Pasa Bajos (IIR de primer orden)
  // ------------------------------------------------------
  // Fórmula matemática:
  //     y[n] = α * x[n] + (1 - α) * y[n-1]
  //
  // Donde:
  //   y[n]   → salida filtrada actual
  //   x[n]   → entrada actual (valor del sensor)
  //   y[n-1] → salida filtrada anterior
  //   α      → coeficiente de suavizado (0 < α ≤ 1)
  //
  // α grande → respuesta rápida, poco suavizado
  // α pequeño → respuesta lenta, más suavizado
  // ------------------------------------------------------
  // ======================================================
    float lowPassFilter(float input){
        static float prevOutput = 0.0f; // y[n-1]
        const float alpha = 0.1f;       // Coeficiente de suavizado

        float output = alpha * input + (1 - alpha) * prevOutput;
        prevOutput = output;
        return output;
    }

 // ======================================================
  // Filtro de Promedio Móvil (Moving Average Filter)
  // ------------------------------------------------------
  // Fórmula matemática:
  //          N-1
  // y[n] = (1/N) * Σ x[n - k]
  //          k=0
  //
  // Donde:
  //   y[n] → salida filtrada (promedio actual)
  //   N    → número de muestras en la ventana
  //   x[n-k] → últimas N lecturas del sensor
  //
  // Efecto:
  //   - Suaviza fluctuaciones aleatorias
  //   - Elimina ruido de alta frecuencia
  //   - Introduce un pequeño retardo de N/2 muestras
  // ======================================================

  float movingAverageFilter(float input) {
    const int N = 100; // Tamaño de la ventana
    static float buffer[N] = {0}; // Buffer circular para almacenar las últimas N muestras
    static int index = 0; // Índice actual en el buffer
    static float sum = 0; // Suma acumulada de las muestras en el buffer
    static bool filled = false; // Indica si el buffer está lleno

    // Restar el valor antiguo de la suma
    sum -= buffer[index];
    // Añadir el nuevo valor al buffer y a la suma  
    buffer[index] = input;
    sum += input;
    // Mover el índice circularmente
    index++;
    if (index >= N) {
         index = 0;
        filled = true; // El buffer ya está lleno
    }

    // Calcular el promedio
    int count = 0;

    if(filled){
        count = N;
    }
    else{
        count = index;
    }
    if(count == 0) count = 1; // Evitar división por cero
    return sum / count;
  }


    // ======================================================
  // Filtro de Mediana (Median Filter)
  // ------------------------------------------------------
  // Fórmula matemática:
  // y[n] = mediana( { x[n - N + 1], ..., x[n] } )
  //
  // Donde:
  //   y[n] → salida filtrada (valor central)
  //   N    → tamaño de la ventana (impar)
  //   x[n - N + 1 ... n] → últimas N lecturas ordenadas
  //
  // Efecto:
  //   - Elimina picos o valores atípicos ("outliers")
  //   - Preserva bordes y transiciones abruptas
  //   - No altera la forma general de la señal
  // ======================================================
float medianFilter(float input) {                      // Declara una función que recibe una muestra (input) y devuelve la mediana de una ventana móvil.
    const int N = 99;                                  // Tamaño de la ventana (impar para que haya un “centro” único cuando esté llena).
    static float buf[N] = {0};                         // Buffer circular que guarda las últimas N muestras (se conserva entre llamadas por ser 'static').
    static int idx = 0;                                // Índice de escritura dentro del buffer circular (posición actual donde caerá la nueva muestra).
    static bool full = false;                          // Bandera: indica si ya se llenó el buffer alguna vez (true ⇒ ya hay N muestras válidas).

    // insertar nueva muestra en buffer circular
    buf[idx] = input;                                  // Escribe la nueva muestra en la posición actual del buffer.
    idx = (idx + 1) % N;                               // Avanza el índice de forma circular (cuando llega a N, vuelve a 0).
    if (idx == 0) full = true;                         // Si regresó a 0, es porque dimos la vuelta completa ⇒ el buffer ya está “lleno”.

    // número de muestras válidas al inicio
    int count = full ? N : idx;                        // Si el buffer está lleno, hay N muestras válidas; si no, hay 'idx' (las que se han escrito).
    if (count <= 0) count = 1;                         // Seguro: evita count=0 (p.ej., en la primera llamada antes de incrementar idx).

    // copiar a arreglo temporal y ordenar (insertion sort)
    float tmp[N];                                      // Arreglo temporal donde se copiarán las muestras válidas para ordenarlas.
    for (int i = 0; i < count; ++i) tmp[i] = buf[i];   // Copia solo las 'count' muestras válidas desde el buffer.

    for (int i = 1; i < count; ++i) {                  // Inicia insertion sort desde la segunda posición hasta el final de 'tmp'.
      float key = tmp[i];                              // Toma el elemento a insertar en la parte ordenada.
      int j = i - 1;                                   // Empieza a comparar hacia la izquierda.
      while (j >= 0 && tmp[j] > key) {                 // Desplaza elementos mayores que 'key' una posición a la derecha.
        tmp[j + 1] = tmp[j];                           // Desplazamiento de elemento hacia la derecha.
        --j;                                           // Sigue avanzando hacia la izquierda.
      }
      tmp[j + 1] = key;                                // Inserta 'key' en la posición correcta dentro de la sublista ordenada.
    }

    // devolver mediana de las 'count' muestras
    int mid = count / 2;                               // Índice central (para tamaño impar, es el centro exacto; para par, es el “superior” de los dos centrales).
    if (count % 2 == 1) {                              // Si hay un número impar de muestras…
      return tmp[mid];                                 // …la mediana es el elemento del centro.
    } else {                                           // Si hay un número par de muestras (puede ocurrir al inicio, antes de llenar la ventana)…
                                                    // durante el arranque 'count' puede ser par: promedio de los dos centrales
      return 0.5f * (tmp[mid - 1] + tmp[mid]);         // …la mediana es el promedio de los dos valores centrales.
    }
}



}