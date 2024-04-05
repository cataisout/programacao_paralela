@echo off

rem Verifica se o número de argumentos é válido
if "%~1"=="" (
    echo Uso: %0 P
    exit /b 1
)

rem Arquivos de entrada fixos
set arquivo1=matriz1.txt
set arquivo2=matriz2.txt

rem Obtém o valor de P do argumento de linha de comando
set P=%1

rem Loop para executar o programa 10 vezes
for /l %%i in (1,1,10) do (
    paralelo_threads %arquivo1% %arquivo2% %P%
)
