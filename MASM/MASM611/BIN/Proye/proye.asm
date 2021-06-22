include core/macros.asm
include core/filesM.asm
include core/stringM.asm
include core/videoM.asm
include core/memM.asm
include core/timeM.asm
include core/mathM.asm
include usrsM.asm
include graphM.asm
include sortM.asm
include gameM.asm
include lvlmngM.asm
include playerM.asm
include blockM.asm
include spwnrM.asm
include scannerM.asm

.model small, stdcall
.486P
.stack 400h

.data
    include core/stringD.asm
    include core/videoD.asm
    include core/colrsD.asm
    include core/timeD.asm
    include core/mathD.asm
    include usrsD.asm
    include graphD.asm
    include sortD.asm
    include gameD.asm
    include lvlmngD.asm
    include playerD.asm
    include blockD.asm
    include spwnrD.asm
    include scannerD.asm

    newLine db 0ah, 0dh, "$"
    
    mainMenuMsg db 0ah, 0dh,
               "1. Ingresar", 0ah, 0dh,
               "2. Registrar", 0ah, 0dh,
               "3. Salir", 0ah, 0dh, "$"

    headerMsg db 0ah, 0dh,
                "UNIVERSIDAD DE SAN CARLOS DE GUATEMALA", 0ah, 0dh,
                "FACULTAD DE INGENIERIA", 0ah, 0dh,
                "CIENCIAS Y SISTEMAS", 0ah, 0dh,
                "ARQUITECTURA DE COMPUTADORES Y ENSAMBLADORES 1", 0ah, 0dh,
                "NOMBRE: HERBERTH JOSUE ARGUETA", 0ah, 0dh,
                "CARNET: 201612383", 0ah, 0dh,
                "SECCION: A", 0ah, 0dh, '$'

    gameMenuMsg db 0ah, 0dh,
               "1. Iniciar Juego", 0ah, 0dh,
               "2. Cargar Juego", 0ah, 0dh,
               "3. Salir", 0ah, 0dh, "$"

    adminMenuMsg db 0ah, 0dh,
               "1. Top puntos", 0ah, 0dh,
               "2. Top tiempo", 0ah, 0dh,
               "3. Salir", 0ah, 0dh, "$"

    sortMenuMsg db 0ah, 0dh,
               "1. BubbleSort", 0ah, 0dh,
               "2. QuickSort", 0ah, 0dh,
               "3. ShellSort", 0ah, 0dh, "$"

    velocidadMsg db 0ah, 0dh,
                "Velocidad: ", "$"
    
    direccionMsg db 0ah, 0dh,
               "1. ASCENDENTE", 0ah, 0dh,
               "2. DESCENDENTE", 0ah, 0dh, "$"

    requestUsrMsg db "Usuario:", 0ah, 0dh, "$"
    requestPasswordMsg db "Contrasenna:", 0ah, 0dh, "$"
    cargarMsg db "Direccion:", "$"
    errorCargarLvlsMsg db "No se pudo leer el archivo exitosamente", "$"

    usrInput1 db 32 dup('$')
    usrInput2 db 32 dup('$')

    coefDe db "Coeficiente de $"

    noValid db "No valido, intente de nuevo: $"

    adminName db "admin", 0
    adminPassword db "1234"
    debChar db "a"
.code 
    include core/cmdP.inc
    include core/stringP.inc
    include core/videoP.inc
    include core/timeP.inc
    include core/colrsP.inc
    include core/mathP.inc
    include usrsP.inc
    include graphP.inc
    include sortP.inc
    include gameP.inc
    include lvlmngP.inc
    include playerP.inc
    include blockP.inc
    include spwnrP.inc
    include scannerP.inc

main proc far
    mov ax,@DATA 
    mov ds,ax

    ;fake login admin SOLO PARA DEBUGGING
    ; mCopyCString offset adminName, offset loggedInUsr
    ; mUsrsUpdateRam

    call generateSeed

    MAIN_MENU:
        print mainMenuMsg
        cmdGetChar

        cmp al, "1"
        je INGRESAR
        cmp al, "2"
        je REGISTRAR
        cmp al, "3"
        je RETURN
        cmp al, "4"
        je DEBUG
        cmp al, "q";SOLO PARA DEBUGGING
        je RETURN
        cmp al, "Q";SOLO PARA DEBUGGING
        je RETURN
        cmp al, "g";SOLO PARA DEBUGGING
        je DEBUG
        cmp al, "G";SOLO PARA DEBUGGING
        je DEBUG
        println noValid
        jmp MAIN_MENU

        INGRESAR:
        print newLine
        print requestUsrMsg
        cmdGetString usrInput1, 0
        print requestPasswordMsg
        cmdGetString usrInput2, 0

        mUsrsTryLogin offset usrInput1, offset usrInput2
        jz LOGIN_EXITO
        jmp LOGIN_FALLO

        jmp MAIN_MENU

        LOGIN_FALLO:
        debPrintln<"Usuario y contrasenna no validos">
        cmdGetChar
        jmp MAIN_MENU

        LOGIN_EXITO:
        debPrintln<"Se ha logeado exitosamente">
        cmdGetChar
        mUsrIsAdmin offset loggedInUsr
        jz ADMIN_MENU
        jmp GAME_MENU

        REGISTRAR:
        print newLine
        INGRESAR_USUARIO:
            print requestUsrMsg
            cmdGetString usrInput1, 0
            mCStringLength offset usrInput1
            cmp ax, 0
            je USR_NO_VALIDO
            cmp ax, L_MAX_NAME
            ja USR_NO_VALIDO
            mUsrsContains offset usrInput1
            jz USR_YA_EXISTE
        INGRESAR_CONTRASENNA:
            debPrintln<"Contrasenna:">
            cmdGetString usrInput2, 0
            mCStringLength offset usrInput2
            cmp ax, L_PASSWORD
            jne CONTRASENNA_NO_VALIDA
            mStringIsDigits offset usrInput2, L_PASSWORD
            jnz CONTRASENNA_NO_VALIDA
        mUsrsAddUsr offset usrInput1, offset usrInput2
        debPrintln<"Usuario registrado exitosamente">
        cmdGetChar
        jmp MAIN_MENU

        USR_NO_VALIDO:
        debPrintln<"Usuario debe tener[0, 7] caracteres">
        jmp INGRESAR_USUARIO

        USR_YA_EXISTE:
        debPrintln<"Ya existe un usuario con ese nombre">
        jmp INGRESAR_USUARIO

        CONTRASENNA_NO_VALIDA:
        debPrintln<"Contrasenna debe ser 4 numeros">
        jmp INGRESAR_CONTRASENNA

    GAME_MENU:
        print headerMsg
        print gameMenuMsg
        cmdGetChar

        cmp al, "1"
        je INICIAR_JUEGO
        cmp al, "2"
        je CARGAR_JUEGO
        cmp al, "3"
        je RETURN
        cmp al, "4"
        je DEBUG
        cmp al, "q";SOLO PARA DEBUGGING
        je RETURN
        cmp al, "Q";SOLO PARA DEBUGGING
        je RETURN
        cmp al, "g";SOLO PARA DEBUGGING
        je DEBUG
        cmp al, "G";SOLO PARA DEBUGGING
        je DEBUG
        println noValid
        jmp GAME_MENU
    
        INICIAR_JUEGO:
        mStartVideoMode
        call play
        mExitVideoMode
        jmp GAME_MENU

        CARGAR_JUEGO:
        println cargarMsg
        cmdGetString usrInput1, 0
        push offset usrInput1
        call scan
        jz GAME_MENU

        println errorCargarLvlsMsg
        jmp CARGAR_JUEGO

    ADMIN_MENU:
        print headerMsg
        print adminMenuMsg
        cmdGetChar

        cmp al, "1"
        je POINTS
        cmp al, "2"
        je TIME
        cmp al, "3"
        je RETURN
        cmp al, "4"
        je DEBUG
        cmp al, "q";SOLO PARA DEBUGGING
        je RETURN
        cmp al, "Q";SOLO PARA DEBUGGING
        je RETURN
        cmp al, "g";SOLO PARA DEBUGGING
        je DEBUG
        cmp al, "G";SOLO PARA DEBUGGING
        je DEBUG
        println noValid
        jmp ADMIN_MENU

        POINTS:
        mUsrsUpdateRam;Hacemos muchos updateRam no necesarios

        ;beg hacer un quickSort sin dibujar nada
        mov shouldQuickSortDraw, 0
        mov direccion, DESCENDENTE
        mQuickSortPoints
        mov shouldQuickSortDraw, 1
        ;end

        mStartVideoMode
        mPrintTopPoints
        cmdGetChar
        mUsrsUpdateRam;Metemos en ram nuevamente los valores desordenados
        mStartVideoMode;Clear screen
        mov delayVar, 0
        mGraphUsrsPoints
        mDrawFramePoints
        cmdGetChar
        mExitVideoMode

        ;falta imprimir el top 10 de usuarios y otro monton de cosas.
        ;ver pag 9 del segundo cuaderno
        
            SORT_POINTS:
            print sortMenuMsg
            cmdGetChar
            cmp al, "1"
            je BUBBLE_POINTS
            cmp al, "2"
            je QUICK_POINTS
            cmp al, "3"
            je SHELL_POINTS
            jmp SORT_POINTS

            ;escribimos codigo de mas en cada opcion de menu, lo inicial es igual para codigo del menu

            BUBBLE_POINTS:
            print direccionMsg
            cmdGetChar
            sub al, '0'
            mov byte ptr[direccion], al
            cmp al, DESCENDENTE
            ja BUBBLE_POINTS;chapuz, si cambiamos las ASCENDENTE y DESCENDENTE vamos a causar bugs
            print velocidadMsg
            cmdGetChar
            sub al, '0'
            mov byte ptr[delayVar], al
            mStartVideoMode;Clear screen
            mGraphUsrsPoints
            mDrawFramePoints
            cmdGetChar
            mBubbleSortPoints
            cmdGetChar
            mExitVideoMode
            jmp ADMIN_MENU

            QUICK_POINTS:
            print direccionMsg
            cmdGetChar
            sub al, '0'
            mov byte ptr[direccion], al
            cmp al, DESCENDENTE
            ja QUICK_POINTS;chapuz, si cambiamos las ASCENDENTE y DESCENDENTE vamos a causar bugs
            print velocidadMsg
            cmdGetChar
            sub al, '0'
            mov byte ptr[delayVar], al
            mStartVideoMode;Clear screen
            mGraphUsrsPoints
            mDrawFramePoints
            cmdGetChar
            mQuickSortPoints
            cmdGetChar
            mExitVideoMode
            jmp ADMIN_MENU
            cmdGetChar
            jmp ADMIN_MENU

            SHELL_POINTS:
            print direccionMsg
            cmdGetChar
            sub al, '0'
            mov byte ptr[direccion], al
            cmp al, DESCENDENTE
            ja SHELL_POINTS;chapuz, si cambiamos las ASCENDENTE y DESCENDENTE vamos a causar bugs
            print velocidadMsg
            cmdGetChar
            sub al, '0'
            mov byte ptr[delayVar], al
            mStartVideoMode;Clear screen
            mGraphUsrsPoints
            mDrawFramePoints
            cmdGetChar
            mShellSortPoints
            cmdGetChar
            mExitVideoMode
            jmp ADMIN_MENU

        TIME:
        mUsrsUpdateRam;Hacemos muchos updateRam no necesarios

        ;beg hacer un quickSort sin dibujar nada
        mov shouldQuickSortDraw, 0
        mov direccion, DESCENDENTE
        mQuickSortTimes
        mov shouldQuickSortDraw, 1
        ;end

        mStartVideoMode
        mPrintTopTimes
        cmdGetChar
        mUsrsUpdateRam;Metemos en ram nuevamente los valores desordenados
        mStartVideoMode;Clear screen
        mov delayVar, 0
        mGraphUsrsTimes
        mDrawFrameTimes
        cmdGetChar
        mExitVideoMode

        ;falta imprimir el top 10 de usuarios y otro monton de cosas.
        ;ver pag 9 del segundo cuaderno
        
            SORT_TIMES:
            print sortMenuMsg
            cmdGetChar
            cmp al, "1"
            je BUBBLE_TIMES
            cmp al, "2"
            je QUICK_TIMES
            cmp al, "3"
            je SHELL_TIMES
            jmp SORT_TIMES

            ;escribimos codigo de mas en cada opcion de menu, lo inicial es igual para codigo del menu

            BUBBLE_TIMES:
            print direccionMsg
            cmdGetChar
            sub al, '0'
            mov byte ptr[direccion], al
            cmp al, DESCENDENTE
            ja BUBBLE_TIMES;chapuz, si cambiamos las ASCENDENTE y DESCENDENTE vamos a causar bugs
            print velocidadMsg
            cmdGetChar
            sub al, '0'
            mov byte ptr[delayVar], al
            mStartVideoMode;Clear screen
            mGraphUsrsTimes
            mDrawFrameTimes
            cmdGetChar
            mBubbleSortTimes
            cmdGetChar
            mExitVideoMode
            jmp ADMIN_MENU

            QUICK_TIMES:
            print direccionMsg
            cmdGetChar
            sub al, '0'
            mov byte ptr[direccion], al
            cmp al, DESCENDENTE
            ja QUICK_TIMES;chapuz, si cambiamos las ASCENDENTE y DESCENDENTE vamos a causar bugs
            print velocidadMsg
            cmdGetChar
            sub al, '0'
            mov byte ptr[delayVar], al
            mStartVideoMode;Clear screen
            mGraphUsrsTimes
            mDrawFrameTimes
            cmdGetChar
            mQuickSortTimes
            cmdGetChar
            mExitVideoMode
            jmp ADMIN_MENU
            cmdGetChar
            jmp ADMIN_MENU

            SHELL_TIMES:
            print direccionMsg
            cmdGetChar
            sub al, '0'
            mov byte ptr[direccion], al
            cmp al, DESCENDENTE
            ja SHELL_TIMES;chapuz, si cambiamos las ASCENDENTE y DESCENDENTE vamos a causar bugs
            print velocidadMsg
            cmdGetChar
            sub al, '0'
            mov byte ptr[delayVar], al
            mStartVideoMode;Clear screen
            mGraphUsrsTimes
            mDrawFrameTimes
            cmdGetChar
            mShellSortTimes
            cmdGetChar
            mExitVideoMode
            jmp ADMIN_MENU


    DEBUG:
        int 3

        jmp MAIN_MENU

RETURN:
    mov ah,4ch
    int 21h

main endp 
end main 