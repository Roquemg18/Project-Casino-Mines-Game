#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#define FILAS 5
#define COLUMNAS 5

#define MAX_SCORES 100

typedef struct {
    int score;
    char name[50];
} Score;

void busca_minas();
void imprimir_tablero(int filas, int columnas, char **tablero);
bool busqueda_de_mina(char **tablero, int posicion1, int posicion2);
int score(int puntos);
void liberar_memoria(char **matriz, int filas);
char **crear_matriz(int filas, int columnas);

int main(int argc, char *argv[]) {
    busca_minas();
    return 0;
}

void imprimir_tablero(int filas, int columnas, char **tablero) {
    // Imprimir los números de las columnas
    printf("   "); // Espacio para la columna de letras
    for (int col = 1; col <= columnas; col++) {
        printf(" %d ", col);
    }
    printf("\n");

    for (int fila = 0; fila < filas; fila++) {
        // Imprimir la parte superior de las casillas
        printf("%c  ", 'A' + fila); // Imprimir la letra de la fila
        for (int col = 0; col < columnas; col++) {
            printf("+--");
        }
        printf("+\n");

        // Imprimir las paredes laterales de las casillas
        printf("   "); // Espacio debajo de la columna de letras
        for (int col = 0; col < columnas; col++) {
            printf("|%c ", tablero[fila][col]);
        }
        printf("|\n");
    }

    // Imprimir la parte inferior de la última fila de casillas
    printf("   "); // Espacio para la columna de letras
    for (int col = 0; col < columnas; col++) {
        printf("+--");
    }
    printf("+\n");
}

bool busqueda_de_mina(char **tablero, int posicion1, int posicion2) {
    return tablero[posicion1][posicion2] == '0';
}

int compare_scores(const void *a, const void *b) {
    Score *scoreA = (Score *)a;
    Score *scoreB = (Score *)b;
    return (scoreB->score - scoreA->score);
}

int score(int puntos) {
    char ruta[100] = {"./archivo/puntajes.txt"};
    char archivo[] = "./archivo/puntajes.txt";
    char nombre[50];

    printf("Ingrese su nombre para la puntuación:\n");
    scanf("%s", nombre);

    FILE *f = fopen(archivo, "a+");
    if (f == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1;
    }
    fprintf(f, " %d %s \n", puntos, nombre);
    fclose(f);

    FILE *file;
    Score scores[MAX_SCORES];
    int num_scores = 0;

    file = fopen(ruta, "r");
    if (file == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1;
    }

    while (fscanf(file, "%d %s", &scores[num_scores].score, scores[num_scores].name) != EOF) {
        num_scores++;
    }

    fclose(file);

    qsort(scores, num_scores, sizeof(Score), compare_scores);

    file = fopen(ruta, "w");
    if (file == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1;
    }

    for (int i = 0; i < num_scores; i++) {
        fprintf(file, "%d %s\n", scores[i].score, scores[i].name);
    }

    fclose(file);

    file = fopen(ruta, "r");
    if (file == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1;
    }

    printf("Las primeras cinco líneas del archivo actualizado son:\n");
    char line[100];
    for (int i = 0; i < 5; i++) {
        if (fgets(line, sizeof(line), file) != NULL) {
            printf("%s", line);
        }
    }

    fclose(file);
    return 0;
}

void liberar_memoria(char **matriz, int filas) {
    for (int i = 0; i < filas; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

char **crear_matriz(int filas, int columnas) {
    char **matriz = (char **)malloc(filas * sizeof(char *));
    if (matriz == NULL) {
        fprintf(stderr, "Error en la asignación de memoria.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < filas; i++) {
        matriz[i] = (char *)malloc(columnas * sizeof(char));
        if (matriz[i] == NULL) {
            fprintf(stderr, "Error en la asignación de memoria.\n");
            liberar_memoria(matriz, i);
            exit(EXIT_FAILURE);
        }
    }

    return matriz;
}

void busca_minas() {
    int filas = FILAS;
    int columnas = COLUMNAS;
    int cantidad_minas = 0;
    bool perdio = true;
    int intento = 0;
    int max_intento = 0;
    int posicion1 = 0;
    int posicion2 = 0;
    int puntos = 0;
    float apuesta = 0.0;
    float multiplicador = 0.0;
    float ganancias = 0.0;
    bool validacion_de_diemenciones = true;

    // Multiplicadores iniciales según la cantidad de minas
    float multiplicadores_inicio[] = {0.0, 0.03, 0.08, 0.13, 0.18, 0.24, 0.30, 0.38, 0.46, 0.55, 0.65, 0.77, 0.90, 1.06, 1.25, 1.48, 1.75, 2.09, 2.54, 3.14, 3.95, 5.19, 6.82, 9.20, 11.01};

    // Obtener la apuesta del usuario
    printf("Ingrese la cantidad de dinero a apostar:\n");
    scanf("%f", &apuesta);

    // Crear tableros y llenar tableros
    while (validacion_de_diemenciones) {
        printf("Ingrese la cantidad de minas:\n");
        scanf("%d", &cantidad_minas);

        if (cantidad_minas < 1 || cantidad_minas > 24) {
            printf("Ingrese una cantidad válida de minas (1-24).\n");
        } else {
            validacion_de_diemenciones = false;
            max_intento = (columnas * filas) - cantidad_minas;
            multiplicador = multiplicadores_inicio[cantidad_minas];
        }
    }

    char **tablero_oculto = crear_matriz(filas, columnas);
    char **tablero_lleno = crear_matriz(filas, columnas);

    // Llenar tablero con espacios vacíos para mostrar al usuario
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            tablero_oculto[i][j] = ' ';
        }
    }

    // Llenar tablero con minas aleatoriamente
    srand(time(NULL));
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            tablero_lleno[i][j] = '0';
        }
    }

    for (int i = 0; i < cantidad_minas; i++) {
        int mina_y = rand() % columnas;
        int mina_x = rand() % filas;
        tablero_lleno[mina_x][mina_y] = '1';
    }

    // Mostrar tablero
    imprimir_tablero(filas, columnas, tablero_oculto);

    while (perdio) {
        // Pedir al usuario una posición
        printf("Posición en x (1 a 5):\n");
        scanf("%d", &posicion1);
        printf("Posición en y (A a E):\n");
        char letra_y;
        scanf(" %c", &letra_y);
        posicion2 = letra_y - 'A';

        // Ajustar la posición x al índice correcto
        posicion1--;

        // Verificar si la posición es válida
        if (posicion1 < 0 || posicion1 >= filas || posicion2 < 0 || posicion2 >= columnas) {
            printf("Posición no válida\n");
        } else {
            perdio = busqueda_de_mina(tablero_lleno, posicion1, posicion2);
            if (perdio == false) {
                imprimir_tablero(filas, columnas, tablero_lleno);
                printf("Tu puntuación fue de: %d\n", puntos);
                printf("Perdiste tu apuesta de: %.2f\n", apuesta);
                printf("Ganancia actual: %.2f\n", ganancias);
                break;
            } else {
                tablero_oculto[posicion1][posicion2] = '0';
                imprimir_tablero(filas, columnas, tablero_oculto);
                puntos += 100;
                intento++;
                multiplicador += 0.1;  // Incremento del multiplicador
                ganancias = apuesta * multiplicador;
                printf("¡Bien hecho! Sigue buscando.\n");
                printf("Puntuación actual: %d\n", puntos);
                printf("Ganancia actual: %.2f\n", ganancias);

                if (intento == max_intento) {
                    imprimir_tablero(filas, columnas, tablero_lleno);
                    printf("¡Felicidades! Has ganado con una puntuación de: %d\n", puntos);
                    printf("Ganaste un total de: %.2f\n", ganancias);
                    break;
                }
            }
        }
    }

    // Guardar la puntuación
    score(puntos);

    // Liberar memoria
    liberar_memoria(tablero_oculto, filas);
    liberar_memoria(tablero_lleno, filas);
}
