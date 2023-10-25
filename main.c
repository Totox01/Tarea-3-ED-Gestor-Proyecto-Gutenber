#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <dirent.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <math.h>

#define MAX_FILES 100
#define MAX_WORD_LENGTH 100
#define TOP_WORDS 10

typedef struct Node {
    char filename[1000];
    struct Node* next;
} Node;

typedef struct Libro {
    char id[100];
    char titulo[1000];
    int palabras;
    int caracteres;
    struct Libro* izq;
    struct Libro* der;
} Libro;

typedef struct {
    char palabra[100];
    int frecuencia;
} Palabra;

typedef struct {
    char word[MAX_WORD_LENGTH];
    int count;
} WordCount;

typedef struct {
    char id[1000];
    char titulo[1000];
    int palabras;
    int vecesPalabra;
} LibroInfo;

Libro* root = NULL;
Node* loadedFiles = NULL;
int loadedFilesCount = 0;

void toLowerCase(char *str) {
    for(int i = 0; str[i]; i++){
        str[i] = tolower(str[i]);
    }
}

int isFileLoaded(char* filename) {
    Node* temp = loadedFiles;
    while(temp != NULL) {
        if(strcmp(temp->filename, filename) == 0) {
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

Libro* crearLibro(char* id, char* titulo, int palabras, int caracteres) {
    Libro* nuevoLibro = (Libro*)malloc(sizeof(Libro));
    strcpy(nuevoLibro->id, id);
    strcpy(nuevoLibro->titulo, titulo);
    nuevoLibro->palabras = palabras;
    nuevoLibro->caracteres = caracteres;
    nuevoLibro->izq = NULL;
    nuevoLibro->der = NULL;
    return nuevoLibro;
}

void insertarLibro(Libro** nodo, Libro* libro) {
    if(*nodo == NULL) {
        *nodo = libro;
        return;
    }
    if(strcmp((*nodo)->titulo, libro->titulo) > 0) {
        insertarLibro(&(*nodo)->izq, libro);
    } else {
        insertarLibro(&(*nodo)->der, libro);
    }
}

void buscarEnArbol(Libro* nodo, char* palabrasBusqueda) {
  if(nodo != NULL) {
      buscarEnArbol(nodo->izq, palabrasBusqueda);
      char *palabrasBusquedaCopia = strdup(palabrasBusqueda);
      char *palabra = strtok(palabrasBusquedaCopia, " ");
      bool todasLasPalabrasEstanEnElTitulo = true;
      while (palabra != NULL && todasLasPalabrasEstanEnElTitulo) {
          if (strstr(nodo->titulo, palabra) == NULL) {
              todasLasPalabrasEstanEnElTitulo = false;
          }
          palabra = strtok(NULL, " ");
      }
      free(palabrasBusquedaCopia);
      if (todasLasPalabrasEstanEnElTitulo) {
          printf("ID: %s\n", nodo->id);
          printf("Título: %s\n", nodo->titulo);
          printf("Palabras: %d\n", nodo->palabras);
          printf("Caracteres: %d\n\n", nodo->caracteres);
      }
      buscarEnArbol(nodo->der, palabrasBusqueda);
  }
}

bool estaEnListaBloqueo(char *palabra) {
    char *listaBloqueo[] = {"the","be","to","of","and","a","in","that","have","i","it","for","not","on","whit","he","as","you","do","at","this","but","his","by","from","they","we","say","her","she","or","an","will","my","one","all","would","there","their","what","so","up","out","if","about","who","get","which","go","me","when","make","can","like","time","no","just","him","know","take","people","into","year","your","good","some","could","them","see","other","than","then","now","look","only","come","its","over","think","also","back","after","use","two","how","our","work","first","well","way","even","new","want","because","any","these","give","day","most","us","is","with","are","was","more","had","has","very","much","were","been"};
    int numPalabrasBloqueo = sizeof(listaBloqueo) / sizeof(listaBloqueo[0]);
    for (int i = 0; i < numPalabrasBloqueo; i++) {
        if (strcmp(palabra, listaBloqueo[i]) == 0) {
            return true;
        }
    }
    return false;
}

void contarPalabrasCaracteres(char *nombreArchivo, int *palabras, int *caracteres) {
    FILE *archivo = fopen(nombreArchivo, "r");
    char c;
    *palabras = 0;
    *caracteres = 0;
    while ((c = fgetc(archivo)) != EOF) {
        (*caracteres)++;
        if (c == ' ' || c == '\n') {
            (*palabras)++;
        }
    }
    fclose(archivo);
}



void extraerTitulo(char *nombreArchivo, char *titulo) {
    FILE *archivo = fopen(nombreArchivo, "r");
    char linea[1024];
    fgets(linea, 1024, archivo);
    fclose(archivo);

    char *inicioTitulo = strstr(linea, "of ");
    char *finalTitulo = strstr(linea, ", ");
    if (inicioTitulo != NULL && finalTitulo != NULL) {
        inicioTitulo += 3; 
        strncpy(titulo, inicioTitulo, finalTitulo - inicioTitulo);
        titulo[finalTitulo - inicioTitulo] = '\0'; 
    } else {
        strcpy(titulo, "Título desconocido");
    }
}

Node* createNode(char* filename) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->filename, filename);
    newNode->next = NULL;
    return newNode;
}

void addFile(char* filename) {
    Node* newNode = createNode(filename);
    newNode->next = loadedFiles;
    loadedFiles = newNode;
    loadedFilesCount++;
}

void cargarDocumentos() {
    char nombre[1000];
    char opcion[10];
    struct stat info;
    char path[1024];

    while(1) {
        printf("¿Qué quieres leer? (carpeta o archivo): ");
        scanf("%s", opcion);

        if (strcmp(opcion, "carpeta") == 0 || strcmp(opcion, "archivo") == 0){
            printf("Ingrese el nombre del %s: ", opcion);
            scanf("%s", nombre);

            if(isFileLoaded(nombre)) {
                printf("El archivo %s ya ha sido cargado.\n", nombre);
                continue;
            }

            if(strcmp(opcion, "archivo") == 0){
                sprintf(path, "libro/%s", nombre);
            }else{
                sprintf(path, "%s", nombre);
            }

            if (stat(path, &info) == 0){
                if (S_ISREG(info.st_mode) && strcmp(opcion, "archivo") == 0){
                    printf("El archivo %s se abrió correctamente\n",nombre);
                    FILE *archivo = fopen(path, "r");
                    char titulo[1000];
                    int palabras;
                    int caracteres;
                    extraerTitulo(path, titulo); 
                    contarPalabrasCaracteres(path, &palabras, &caracteres); 
                    Libro* libro = crearLibro(nombre, titulo, palabras, caracteres);
                    insertarLibro(&root, libro);
                    fclose(archivo);
                    addFile(nombre);
                    break;
                }else if (S_ISDIR(info.st_mode) && strcmp(opcion, "carpeta") == 0){
                  printf("\nLa carpeta %s se abrió correctamente\n\n",nombre);
                  DIR* carpeta = opendir(path);
                      struct dirent* entrada;
                      while((entrada = readdir(carpeta)) != NULL) {
                          if(entrada->d_type == DT_REG) {
                              if(!isFileLoaded(entrada->d_name)) {
                                  char titulo[1000];
                                  int palabras;
                                  int caracteres;
                                  char filePath[1024];
                                  sprintf(filePath, "%s/%s", path, entrada->d_name);
                                  extraerTitulo(filePath, titulo); 
                                  contarPalabrasCaracteres(filePath, &palabras, &caracteres); 
                                  Libro* libro = crearLibro(entrada->d_name, titulo, palabras, caracteres);
                                  insertarLibro(&root, libro);
                                  addFile(entrada->d_name);
                              }
                          }
                      }
                      closedir(carpeta);
                      break;
                }else{
                    printf("El elemento %s no coincide con la opción %s\n",nombre, opcion);
                }
            }else{
                printf("El elemento %s no existe\n",nombre);
            }
        }else{
            printf("La opción %s no es válida\n", opcion);
        }
    }
}

void mostrarOrdenados(Libro* nodo) {
    if(nodo != NULL) {
        mostrarOrdenados(nodo->izq);
        printf("ID: %s\n", nodo->id);
        printf("Título: %s\n", nodo->titulo);
        printf("Palabras: %d\n", nodo->palabras);
        printf("Caracteres: %d\n\n", nodo->caracteres);
        mostrarOrdenados(nodo->der);
    }
}

void buscarTitulo(char *palabrasBusqueda) {
  char *palabrasBusquedaCopia = strdup(palabrasBusqueda);
  buscarEnArbol(root, palabrasBusquedaCopia);
  free(palabrasBusquedaCopia);
}

int compare(const void *a, const void *b) {
    WordCount *wordCountA = (WordCount *)a;
    WordCount *wordCountB = (WordCount *)b;
    return wordCountB->count - wordCountA->count;
}

void palabrasFrecuencia(char *idLibro) {
  if (!isFileLoaded(idLibro)) {
      printf("El archivo %s no está cargado\n", idLibro);
      return;
  }

  char nombreArchivo[1000];
  sprintf(nombreArchivo, "libro/%s", idLibro);
  FILE *archivo = fopen(nombreArchivo, "r");
  if (archivo == NULL) {
      printf("Error al abrir el archivo %s\n", nombreArchivo);
      return;
  }

  WordCount wordCounts[10000] = {0};
  int totalWords = 0;

  char palabra[MAX_WORD_LENGTH];
  while (fscanf(archivo, "%s", palabra) != EOF) {
      toLowerCase(palabra);
      if (!estaEnListaBloqueo(palabra)) {
          int found = 0;
          for (int i = 0; i < totalWords; i++) {
              if (strcmp(wordCounts[i].word, palabra) == 0) {
                  wordCounts[i].count++;
                  found = 1;
                  break;
              }
          }
          if (!found && totalWords < sizeof(wordCounts) / sizeof(WordCount)) {
              strcpy(wordCounts[totalWords].word, palabra);
              wordCounts[totalWords].count = 1;
              totalWords++;
          }
      }
  }

  fclose(archivo);

  qsort(wordCounts, totalWords, sizeof(WordCount), compare);

  for (int i = 0; i < TOP_WORDS && i < totalWords; i++) {
      printf("%s: %d\n", wordCounts[i].word, wordCounts[i].count);
  }
}

int contar_palabras(char *palabra, char *documento) {
    char nombreArchivo[1000];
    sprintf(nombreArchivo, "libro/%s", documento);
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo %s\n", nombreArchivo);
        return 0;
    }

    int count = 0;
    char palabraArchivo[MAX_WORD_LENGTH];
    while (fscanf(archivo, "%s", palabraArchivo) != EOF) {
        toLowerCase(palabraArchivo);
        if (strcmp(palabraArchivo, palabra) == 0) {
            count++;
        }
    }

    fclose(archivo);

    return count;
}

int contar_documentos_con_palabra(char *palabra) {
  int count = 0;
  Node* temp = loadedFiles;

  while(temp != NULL) {
      if(contar_palabras(palabra, temp->filename) > 0) {
          count++;
      }
      temp = temp->next;
  }

  return count;
}

double palabrasRelevantes(char* palabra, char* documento, int num_documentos) {
  int veces_palabra = contar_palabras(palabra, documento);
  int num_palabras = contar_palabras(NULL, documento);
  int num_documentos_con_palabra = contar_documentos_con_palabra(palabra);

  if(num_documentos_con_palabra == 0) {
      printf("La palabra '%s' no se encuentra en ninguno de los documentos.\n", palabra);
      return 0.0;
  }

  double relevancia = veces_palabra / (double)num_palabras * log(num_documentos / (double)num_documentos_con_palabra);
  return relevancia;
}

void buscarPorPalabra(char *palabra) {
    LibroInfo libros[10000];
    int numLibros = 0;

    for(int i = 0; i < loadedFilesCount; i++) {
        char *idLibro = loadedFiles[i].filename;
        if (contar_palabras(palabra, idLibro) > 0) {
            strcpy(libros[numLibros].id, idLibro);
            extraerTitulo(idLibro, libros[numLibros].titulo);
            libros[numLibros].palabras = contar_palabras(NULL, idLibro);
            libros[numLibros].vecesPalabra = contar_palabras(palabra, idLibro);
            numLibros++;
        }
    }

    qsort(libros, numLibros, sizeof(LibroInfo), compare);

    for (int i = 0; i < numLibros; i++) {
        printf("ID: %s\n", libros[i].id);
        printf("Título: %s\n", libros[i].titulo);
        printf("Relevancia: %f\n\n", (double)libros[i].vecesPalabra / libros[i].palabras);
    }
}

void mostrarPalabraEnContexto(char *tituloLibro, char *palabra) {
    char nombreArchivo[1000];
    sprintf(nombreArchivo, "libro/%s", tituloLibro);
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo %s\n", nombreArchivo);
        return;
    }

    char linea[1024];
    while (fgets(linea, 1024, archivo) != NULL) {
        char *posicion = strstr(linea, palabra);
        if (posicion != NULL) {
            printf("%s\n", linea);
        }
    }

    fclose(archivo);
}

int main(void) {
  int opcion;
  char tituloBuscado[100];
  char idLibro[10];
   char* documento;
  printf("                        Menú Principal                   \n");
  printf("-=============================================================-\n");
  printf("  Bienvenido al programa de gestión de biblioteca.\n");
  do {
    printf("-=============================================================-\n");
    printf("1. [Cargar documentos]\n");
    printf("2. [Mostrar documentos ordenados]\n");
    printf("3. [Buscar un libro por título]\n");
    printf("4. [Palabras con mayor frecuencia]\n");
    printf("5. [Palabras más relevantes]\n");
    printf("6. [Buscar por palabra]\n");
    printf("7. [Mostrar palabra en su contexto dentro del libro]\n");
    printf("8. [Salir]\n\n");
    printf("Seleccione una opcion: ");

    scanf("%i",&opcion);
    switch (opcion) {
      case 1:
        cargarDocumentos();
        break;
      case 2:
        mostrarOrdenados(root);
        break;
      case 3:
        printf("Ingrese las palabras a buscar: \n");
        scanf(" %[^\n]", tituloBuscado);
        buscarTitulo(tituloBuscado);
        break;
      case 4:
        printf("Ingrese el ID del libro: ");
        scanf(" %[^\n]", idLibro);
        palabrasFrecuencia(idLibro);
      break;
      case 5:
        printf("Ingrese el nombre del libro: ");
        scanf(" %[^\n]", tituloBuscado);
        int num_documentos = 10;

        char palabra[MAX_WORD_LENGTH];
        printf("Ingrese la palabra a buscar: ");
        scanf("%s", palabra);

        double relevancia = palabrasRelevantes(palabra, tituloBuscado, num_documentos);
        printf("La relevancia de la palabra '%s' en el documento '%s' es: %f\n", palabra, tituloBuscado, relevancia);
      break;
      case 6:
        printf("Ingrese la palabra a buscar: ");
        scanf(" %[^\n]", palabra);
        buscarPorPalabra(palabra);
      break;

      case 7:
      printf("Ingrese el nombre del libro: ");
      scanf(" %[^\n]", tituloBuscado);
      printf("Ingrese la palabra a buscar: ");  
      scanf(" %[^\n]", palabra);
      printf("Palabra en su contexto:\n");
      mostrarPalabraEnContexto(tituloBuscado, palabra);

      default:
        if (opcion<1 && opcion>8){
          printf("Opción inválida. Por favor, ingrese una opción válida.\n");
        }
    }
  } while (opcion != 8);
  printf("*** Saliendo del programa ***");

  return 0;
}

