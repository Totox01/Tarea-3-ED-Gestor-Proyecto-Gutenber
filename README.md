# Gestor de libros del proyecto Gutenberg

Esta aplicacion permite la carga y analisis de libros en formato txt del proyecto Gutenberg.

## COMO COMPILAR Y EJECUTAR EL PROGRAMA

1. Clonar el repositorio:
   *git clone https://github.com/Totox01/Tarea-3-ED-Gestor-Proyecto-Gutenber*
2. Navegar hasta el directorio del protecto:
   *cd Tarea-3-ED-Gestor-Proyecto-Gutenber*
4. Compilar el codigo:
   *gcc main.c -o GestorGutenberg*
5. Ejecutar la aplicacion
   *./GestorGutenberg*

## FUNCIONALIDADES

1. Cargar archivos txt (manualmente o una carpeta entera).
2. Mostrar los documentos ordenados por titulo.
3. Buscar libros por una palabra o frase en su titulo.
4. Ver las 10 palabras que mas se repiten en un libro.
5. Ver la relevancia de alguna palabra en un libro.
6. Mostrar libros que contengan una palabra dada.
7. Mostrar una palabra de un libro en sus distintos contextos. 


### PROBLEMAS CONOCIDOS CON LAS FUNCIONALIDADES

- La funcion que calcula la relevancia de una palabra genera errores de calculo y retorna "nan" aveces dependiendo de si la palabra dada tiene mayuscula o no.
- La funcion que muestra los libros que contengan una palabra dada genera errores que terminan la ejecucion del programa al ser llamada.

### FUNCIONALIDADES A MEJORAR

- La funcion de relevancia de una palabra deberia demostrar las 10 mas relevantes en cambio solo sirve para calcular la relevancia de una palabra por lo que deberiamos de cambiar eso para que muestre las 10 mas relevantes
- Mejorar los manejos de memoria en general y el rendimiento general de las funciones.


## CONTRIBUCIONES

Javier Montoya: -Diseño inicial de las funciones del codigo.
                -Creación de la interfaz de usuario.
                -Autoevaluacion: (3)Aporte excelente.
                
Ricardo Santana:-Revision y correccion del codigo.
                -Diseño y redaccion del README.
                -Autoevaluacion: (3)Aporte excelente.
                