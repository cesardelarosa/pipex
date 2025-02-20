#!/bin/bash
# test_bonus_files.sh
# Script para ejecutar tests bonus de pipex comparando el contenido de los archivos de salida.
# Se prueban dos modos: here_doc y normal (múltiples pipes).

# Colores para los mensajes
GREEN="\e[32m"
RED="\e[31m"
RESET="\e[0m"

##########################################
# FUNCIONES DE UTILIDAD
##########################################
print_sep() {
  echo "-----------------------------------------"
}

cleanup() {
  rm -f pipex_stdout.tmp shell_stdout.tmp pipex_stderr.txt shell_stderr.txt
}

##########################################
# TESTS BONUS - HERE_DOC
##########################################

bonus_here_doc_tests=(
  # Formato: here_doc|LIMITER|cmd1|cmd2|[cmd_extra]|outfile
  "here_doc|LIMITAR|cat|grep a||bonus_hd_1.txt"
  "here_doc|FIN|cat|grep a|\"wc -l\"|bonus_hd_2.txt"
)


echo -e "\n${GREEN}===== TESTS BONUS - HERE_DOC (comparando archivos) =====${RESET}"
for test in "${bonus_here_doc_tests[@]}"; do
  IFS='|' read -r mode limiter cmd1 cmd2 extra outfile <<< "$test"
  
  # Para mostrar la cadena de comandos
  if [ -n "$extra" ]; then
    cmds="$cmd1 | $cmd2 | $extra"
  else
    cmds="$cmd1 | $cmd2"
  fi

  echo -e "===========================\nTest BONUS:"
  echo -e "  Modo:     '${GREEN}$mode${RESET}'"
  echo -e "  Limiter:  '${GREEN}$limiter${RESET}'"
  echo -e "  Comandos: '${GREEN}$cmds${RESET}'"
  echo -e "  Outfile:  '${GREEN}$outfile${RESET}'"
  print_sep

  # Eliminar outfile si existe
  rm -f "$outfile"

  ##########################################
  # EJECUCIÓN CON PIPEX (here_doc bonus)
  ##########################################
  ./pipex here_doc "$limiter" "$cmd1" "$cmd2" ${extra:+"$extra"} "$outfile" 2>pipex_stderr.txt <<EOF
Esta es la primera línea con a
Esta línea no tiene
Esta es la segunda línea con a
$limiter
EOF
  pipex_status=$?
  # Leer contenido del outfile generado por pipex
  pipex_file=$(cat "$outfile" 2>/dev/null)

  ##########################################
  # EJECUCIÓN CON PIPELINE EN SHELL (simulando here_doc)
  ##########################################
  rm -f "$outfile"
  # Usamos un heredoc directo sin eval
  cat <<EOF | $cmd1 | $cmd2 ${extra:+| $extra} > "$outfile" 2>shell_stderr.txt
Esta es la primera línea con a
Esta línea no tiene
Esta es la segunda línea con a
$limiter
EOF
  shell_status=$?
  # Leer contenido del outfile generado por el pipeline en shell
  shell_file=$(cat "$outfile" 2>/dev/null)

  echo ""
  echo -e "Comparación de resultados BONUS (here_doc):"
  if [ "$pipex_status" -eq "$shell_status" ]; then
    echo -e "Exit Status: PIPEX = ${GREEN}$pipex_status${RESET} | SHELL = ${GREEN}$shell_status${RESET}   OK"
  else
    echo -e "Exit Status: PIPEX = ${RED}$pipex_status${RESET} | SHELL = ${RED}$shell_status${RESET}   FAIL"
  fi

  if [ "$pipex_file" = "$shell_file" ] && [ -n "$pipex_file" ]; then
    echo -e "Contenido Outfile: OK"
    echo -e "Resultado: ${GREEN}$pipex_file${RESET}"
  else
    echo -e "Contenido Outfile: ${RED}FAIL${RESET}"
    echo -e "PIPEX: ${RED}$pipex_file${RESET}"
    echo -e "SHELL: ${RED}$shell_file${RESET}"
  fi

  echo -e "-----------------------------------------"
  echo "pipex stderr:"
  cat pipex_stderr.txt
  echo "shell stderr:"
  cat shell_stderr.txt
  print_sep

  rm -f pipex_stderr.txt shell_stderr.txt "$outfile"
  echo -e "===========================\n"
done

##########################################
# TESTS BONUS - NORMAL (múltiples pipes)
##########################################
bonus_normal_tests=(
  # Formato: normal|infile|cmd1|cmd2|cmd3|[cmd_extra]|outfile
  # Test con tres comandos: campo extra vacío si no se usa.
  "normal|infile_bonus.txt|cat|grep a|wc -l||bonus_norm_1.txt"
  # Test con cuatro comandos
  "normal|infile_bonus.txt|grep a|cat|wc -l|uniq|bonus_norm_2.txt"
)

echo -e "\n${GREEN}===== TESTS BONUS - NORMAL (múltiples pipes, comparando archivos) =====${RESET}"
# Preparar un infile de ejemplo para los tests normales bonus
if [ ! -f "infile_bonus.txt" ]; then
  echo -e "primera línea con a\nsegunda línea sin\notra línea con a" > infile_bonus.txt
fi

for test in "${bonus_normal_tests[@]}"; do
  IFS='|' read -r mode infile cmd1 cmd2 cmd3 extra outfile <<< "$test"
  
  cmds="$cmd1 | $cmd2 | $cmd3"
  if [ -n "$extra" ]; then
    cmds="$cmds | $extra"
  fi

  echo -e "===========================\nTest BONUS:"
  echo -e "  Modo:     '${GREEN}$mode${RESET}'"
  echo -e "  Infile:   '${GREEN}$infile${RESET}'"
  echo -e "  Comandos: '${GREEN}$cmds${RESET}'"
  echo -e "  Outfile:  '${GREEN}$outfile${RESET}'"
  print_sep

  # Preparar infile y outfile
  if [ ! -f "$infile" ]; then
    echo -e "Creando '$infile' con contenido de ejemplo."
    echo -e "primera línea con a\nsegunda línea sin\notra línea con a" > "$infile"
  fi
  rm -f "$outfile"

  ##########################################
  # EJECUCIÓN CON PIPEX (modo normal bonus)
  ##########################################
  if [ -n "$extra" ]; then
    ./pipex "$infile" "$cmd1" "$cmd2" "$cmd3" "$extra" "$outfile" 2>pipex_stderr.txt
  else
    ./pipex "$infile" "$cmd1" "$cmd2" "$cmd3" "$outfile" 2>pipex_stderr.txt
  fi
  pipex_status=$?
  pipex_file=$(cat "$outfile" 2>/dev/null)

  ##########################################
  # EJECUCIÓN CON PIPELINE EN SHELL (modo normal)
  ##########################################
  rm -f "$outfile"
  if [ -n "$extra" ]; then
    eval "< \"$infile\" $cmd1 | $cmd2 | $cmd3 | $extra > \"$outfile\"" 2>shell_stderr.txt
  else
    eval "< \"$infile\" $cmd1 | $cmd2 | $cmd3 > \"$outfile\"" 2>shell_stderr.txt
  fi
  shell_status=$?
  shell_file=$(cat "$outfile" 2>/dev/null)

  echo ""
  echo -e "Comparación de resultados BONUS (normal):"
  if [ "$pipex_status" -eq "$shell_status" ]; then
    echo -e "Exit Status: PIPEX = ${GREEN}$pipex_status${RESET} | SHELL = ${GREEN}$shell_status${RESET}   OK"
  else
    echo -e "Exit Status: PIPEX = ${RED}$pipex_status${RESET} | SHELL = ${RED}$shell_status${RESET}   FAIL"
  fi

  if [ "$pipex_file" = "$shell_file" ] && [ -n "$pipex_file" ]; then
    echo -e "Contenido Outfile: OK"
    echo -e "Resultado: ${GREEN}$pipex_file${RESET}"
  else
    echo -e "Contenido Outfile: ${RED}FAIL${RESET}"
    echo -e "PIPEX: ${RED}$pipex_file${RESET}"
    echo -e "SHELL: ${RED}$shell_file${RESET}"
  fi

  echo -e "-----------------------------------------"
  echo "pipex stderr:"
  cat pipex_stderr.txt
  echo "shell stderr:"
  cat shell_stderr.txt
  print_sep

  rm -f pipex_stderr.txt shell_stderr.txt "$outfile"
  echo -e "===========================\n"
done

# Limpieza final
rm -f infile_bonus.txt
rmdir dir_out
