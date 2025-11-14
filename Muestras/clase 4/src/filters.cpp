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
  float lowPassFilter(float input) {
    static float prevOutput = 0.0;  // último valor filtrado
    const float alpha = 0.1;        // factor de suavizado fijo

    float output = alpha * input + (1.0 - alpha) * prevOutput;
    prevOutput = output;            // actualizar memoria interna
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
  float movingAverage(float input) {
    const int N = 10;             // tamaño de la ventana
    static float buffer[N] = {0}; // almacenamiento circular
    static int index = 0;
    static bool filled = false;
    static float sum = 0.0;

    sum -= buffer[index];   // quitar el valor más antiguo
    buffer[index] = input;  // guardar el nuevo
    sum += input;           // agregar el nuevo valor

    index++;
    if (index >= N) {
      index = 0;
      filled = true;
    }

    int count = filled ? N : index;
    if (count == 0) count = 1; // evita división por cero
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
  float medianFilter(float input) {
    const int N = 7;                      // tamaño de ventana (impar)
    static float buf[N] = {0};            // buffer circular
    static int idx = 0;                   // índice de escritura
    static bool full = false;             // ya se llenó la ventana

    // insertar nueva muestra en buffer circular
    buf[idx] = input;
    idx = (idx + 1) % N;
    if (idx == 0) full = true;

    // número de muestras válidas al inicio
    int count = full ? N : idx;
    if (count <= 0) count = 1;

    // copiar a arreglo temporal y ordenar (insertion sort)
    float tmp[ N ];                       // tamaño máximo
    for (int i = 0; i < count; ++i) tmp[i] = buf[i];

    for (int i = 1; i < count; ++i) {
      float key = tmp[i];
      int j = i - 1;
      while (j >= 0 && tmp[j] > key) {
        tmp[j + 1] = tmp[j];
        --j;
      }
      tmp[j + 1] = key;
    }

    // devolver mediana de las 'count' muestras
    int mid = count / 2;                  // para impar: centro exacto
    if (count % 2 == 1) {
      return tmp[mid];
    } else {
      // durante el arranque 'count' puede ser par: promedio de los dos centrales
      return 0.5f * (tmp[mid - 1] + tmp[mid]);
    }
  }

} // namespace Filters