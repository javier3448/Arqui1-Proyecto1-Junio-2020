direccion db 1
ASCENDENTE equ 1
DESCENDENTE equ 2
shouldQuickSortDraw db 1;Si esta en 0 no dibujamos

headerMin db "00:";empieza en la columna 24
headerSec db "00", "$"

headerSpeed db "VEL:";empieza 35
headerSpeedNumber db "0", "$"

;Para copiar a headerSortType dependiendo del sort que se va a hacer
headerBubbleMsg db "BUBBLE", "$";Empieza en 12
headerQuickMsg db  "QUICK ", "$"
headerShellMsg db  "SHELL ", "$"

tempUsr USR { }
