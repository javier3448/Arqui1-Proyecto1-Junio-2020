;Variables De gameLoop: (podrian ser locales del proc pero esto es mas mantenible y facil para nosotros en este momento)
pauseInitialTime dd ?
pauseTotalTime dw ?

gameInitialTime dd ?

;Javier1a       N0        000       00:00
gameHeaderUsr db L_MAX_NAME + 1 dup(' '), "$"

gameHeaderLevel db "N";empieza en cursor 15, 0
gameHeaderLevelNumber db "0", "$"

gameHeaderPoints db "000", "$";empieza en cursor 25, 0

gameHeaderMinutes db "00:";empieza en cursor 35, 0
gameHeaderSeconds db "00", "$"

gameInitialXPosition equ 150 
gameInitialYPosition equ 150

gameBackgroundColor equ GRAY

gameAreaX0 equ 16
gameAreaY0 equ 16
gameAreaWidth equ 320 - 16 - 16
gameAreaHeight equ 200 - 8 - 16

;podrian solo ser varias constantes pero tenerlas en mem hace que sea mas faci llamar a los metodos de hitbox
gameArea HITBOX { gameAreaX0, gameAreaY0, gameAreaWidth, gameAreaHeight }
