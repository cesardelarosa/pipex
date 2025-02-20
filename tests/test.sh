#!/bin/bash
# test_pipex.sh
# Script para ejecutar tests comparativos entre pipex y la ejecución de un pipeline en shell.
# Cada test se define en el array "tests" con el formato:
# infile|cmd1|cmd2|outfile

# Colores para los mensajes
GREEN="\e[32m"
RED="\e[31m"
RESET="\e[0m"

# Definir lista de tests
tests=(
  # Caso normal: infile existe, comandos válidos
  "infile.txt|cat|grep a|outfile.txt"
  # Infile no existe
  "nonexistent.txt|cat|grep a|outfile2.txt"
  # Infile existe, grep no encuentra coincidencias
  "infile.txt|cat|grep z|outfile3.txt"
  # Primer comando inexistente
  "infile.txt|nonexistentcommand|grep a|outfile4.txt"
  # Segundo comando inexistente
  "infile.txt|cat|nonexistentcommand|outfile5.txt"
  # Usando comando con ruta absoluta
  "infile.txt|/bin/cat|grep a|outfile6.txt"
  # Comandos con varios argumentos
  "infile.txt|echo hello world|grep hello|outfile7.txt"
  # Outfile de solo lectura: se creará con permisos restringidos
  "infile.txt|cat|grep a|readonly.txt"
  # Archivo de entrada vacío
  "empty.txt|cat|grep a|outfile_empty.txt"
  # Archivo de entrada sin permisos de lectura
  "noread.txt|cat|grep a|outfile_noread.txt"
  # Outfile es un directorio
  "infile.txt|cat|grep a|dir_out"
  # Archivo de entrada con espacios en el nombre
  "file with spaces.txt|cat|grep a|outfile_spaces.txt"
  # Comando que genera salida en stderr (opción inválida)
  "infile.txt|cat|grep --invalid-option|outfile_invalid.txt"
)

# Función para imprimir una separación visual
print_sep() {
  echo "-----------------------------------------"
}

# Recorrer cada test
for test in "${tests[@]}";
do
  # Separar los campos usando "|" como delimitador
  IFS='|' read -r infile cmd1 cmd2 outfile <<< "$test"

  echo "==========================="
  echo "Test:"
  echo "  Infile:  '$infile'"
  echo "  Cmd1:    '$cmd1'"
  echo "  Cmd2:    '$cmd2'"
  echo "  Outfile: '$outfile'"
  print_sep

  ##########################################
  # Preparación del entorno para el test
  ##########################################
  # Preparar el archivo de entrada (infile)
  if [ "$infile" = "infile.txt" ]; then
    # Si "infile.txt" no existe, se crea con contenido de ejemplo.
    if [ ! -f "$infile" ]; then
      echo "Creando '$infile' con contenido de ejemplo."
      echo -e "primera línea con a\nsegunda línea sin\notra línea con a" > "$infile"
    fi
  elif [ "$infile" = "empty.txt" ]; then
    # Crear archivo vacío.
    echo "Creando '$infile' como archivo vacío."
    > "$infile"
  elif [ "$infile" = "noread.txt" ]; then
    # Crear el archivo y quitarle permisos de lectura.
    echo "Creando '$infile' y quitando permisos de lectura."
    echo "contenido de ejemplo" > "$infile"
    chmod 000 "$infile"
  elif [ "$infile" = "file with spaces.txt" ]; then
    # Crear archivo con espacios en el nombre.
    echo "Creando '$infile' con contenido de ejemplo."
    echo "primera línea con a" > "$infile"
  fi

  # Preparar el archivo de salida (outfile)
  if [ "$outfile" = "readonly.txt" ]; then
    # Crear outfile con permisos de solo lectura.
    echo "Creando '$outfile' con permisos de solo lectura."
    echo "Contenido inicial" > "$outfile"
    chmod 444 "$outfile"
  elif [ "$outfile" = "dir_out" ]; then
    # Si outfile es un directorio, se crea el directorio si no existe.
    if [ ! -d "$outfile" ]; then
      echo "Creando directorio '$outfile'."
      mkdir "$outfile"
    fi
  else
    # En otros casos, se elimina el outfile previo.
    rm -f "$outfile"
  fi

  ##########################################
  # PRIMERA EJECUCIÓN: con ./pipex
  ##########################################
  echo "Ejecutando ./pipex ..."
  # Ejecutar pipex y capturar stdout y stderr (este último a archivo temporal)
  pipex_stdout=$(./pipex "$infile" "$cmd1" "$cmd2" "$outfile" 2>pipex_stderr.txt)
  pipex_status=$?
  # Guardar stdout en archivo temporal
  echo "$pipex_stdout" > pipex_stdout.tmp

  ##########################################
  # SEGUNDA EJECUCIÓN: pipeline equivalente en shell
  ##########################################
  # Si outfile no es readonly ni directorio, borrarlo para regenerarlo.
  if [ "$outfile" != "readonly.txt" ] && [ "$outfile" != "dir_out" ]; then
    rm -f "$outfile"
  fi
  # Construir el comando completo para el pipeline en shell.
  shell_cmd="< \"$infile\" $cmd1 | $cmd2 > \"$outfile\""
  echo "Ejecutando pipeline en shell:"
  echo "  $shell_cmd"
  # Ejecutar el pipeline y capturar stdout y stderr
  shell_stdout=$( { eval "$shell_cmd"; } 2>shell_stderr.txt )
  shell_status=$?
  # Guardar stdout en archivo temporal
  echo "$shell_stdout" > shell_stdout.tmp

  ##########################################
  # Comparación de resultados (exit status y stdout)
  ##########################################
  echo ""
  echo "Comparación de resultados:"
  # Comparar exit statuses
  if [ "$pipex_status" -eq "$shell_status" ]; then
    status_msg="${GREEN}OK${RESET}"
  else
    status_msg="${RED}FAIL${RESET}"
  fi
  printf "%-20s : PIPEX = %s | SHELL = %s   %b\n" "Exit Status" "$pipex_status" "$shell_status" "$status_msg"

  # Comparar stdout (se convierten los saltos de línea en espacios para facilitar la comparación)
  pipex_out=$(cat pipex_stdout.tmp | tr '\n' ' ')
  shell_out=$(cat shell_stdout.tmp | tr '\n' ' ')
  if [ "$pipex_out" = "$shell_out" ]; then
    stdout_msg="${GREEN}OK${RESET}"
  else
    stdout_msg="${RED}FAIL${RESET}"
  fi
  printf "%-20s : PIPEX = %s | SHELL = %s   %b\n" "Stdout" "$pipex_out" "$shell_out" "$stdout_msg"

  # Si hay diferencias en stdout, se muestra un diff lado a lado
  if [ "$stdout_msg" = "${RED}FAIL${RESET}" ]; then
    echo "Diferencias en stdout (lado a lado):"
    echo -e "PIPEX\t|\tSHELL"
    paste -d " | " pipex_stdout.tmp shell_stdout.tmp
  fi

  ##########################################
  # Mostrar stderr (sin comparación)
  ##########################################
  print_sep
  echo "pipex stderr:"
  cat pipex_stderr.txt
  echo "shell stderr:"
  cat shell_stderr.txt
  print_sep

  # Limpiar archivos temporales y outfile si corresponde
  rm -f pipex_stdout.tmp shell_stdout.tmp pipex_stderr.txt shell_stderr.txt
    
  rm -f "$outfile"

  chmod 644 "$infile"
  rm -f "$infile"

  echo "==========================="
  echo ""
done
rmdir dir_out
