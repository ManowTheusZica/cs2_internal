@echo off
chcp 65001 >nul
title PRISM CS2 Internal - Build

echo ==========================================
echo   PRISM CS2 Internal - Build Script
echo ==========================================
echo.

:: Check for Visual Studio
where msbuild >nul 2>&1
if %errorlevel% neq 0 (
    echo [!] MSBuild nao encontrado!
    echo [!] Abra o Visual Studio Developer Command Prompt
    echo [!] Ou instale o Visual Studio 2022 com C++ Desktop workload
    echo.
    pause
    exit /b 1
)

echo [1/3] Compilando cs2_cheat.dll (Release x64)...
msbuild cs2_internal.vcxproj /p:Configuration=Release /p:Platform=x64 /t:Build /verbosity:minimal
if %errorlevel% neq 0 (
    echo [!] ERRO ao compilar cs2_cheat.dll!
    pause
    exit /b 1
)
echo [+] cs2_cheat.dll compilado!

echo.
echo [2/3] Compilando injector.exe...
cl /EHsc /std:c++20 /O2 /Fe:injector.exe injector.cpp /link /SUBSYSTEM:CONSOLE
if %errorlevel% neq 0 (
    echo [!] ERRO ao compilar injector.exe!
    pause
    exit /b 1
)
echo [+] injector.exe compilado!

echo.
echo [3/3] Copiando DLL para diretorio do injector...
copy /Y "x64\Release\cs2_cheat.dll" "cs2_cheat.dll" >nul
echo [+] Arquivos prontos!

echo.
echo ==========================================
echo   BUILD COMPLETO!
echo ==========================================
echo.
echo Para usar:
echo   1. Abra CS2 com -insecure
echo   2. Rode: injector.exe cs2_cheat.dll
echo   3. Pressione ] no jogo para menu
echo.
echo Arquivos gerados:
echo   - cs2_cheat.dll (o cheat)
echo   - injector.exe (injetor)
echo.
pause
