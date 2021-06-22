;Para copiar a headerBarValue dependiendo del valor que se va a hacer
headerPointsMsg db "POINTS                                  ", "$"
headerTimesMsg  db "TIEMPO                                  ", "$"

;Seria mejor hacer caracter por caracter supongo :/ bleh
frameLine db "----------------------------------------", "$"
topPointsMsg db 0ah, 0dh, "               Top  puntos", 0ah, 0dh, "$";empieza en columna 7
topTimesMsg db 0ah, 0dh, "               Top tiempos", 0ah, 0dh, "$";empieza en columna 7

L_TOP_USR_BUFFER equ lengthof topUsrBuffer + lengthof topUsrBufferName + lengthof topUsrBufferLevel + lengthof topUsrBufferValue
topUsrBuffer db "00.      "
topUsrBufferName db "                "
topUsrBufferLevel db "0           "
topUsrBufferValue db "000", 0ah, 0dh, "$"

delayVar dw 1;Por que es word? podria ser byte

delayMultiplier equ 2;TODO: Probar cual funciona mejor
y1Constant equ 183

realBuffer REAL4 ?

initialTime dd ?

topPointsTitle db "c:\Points.rep", 0
topTimesTitle db "c:\Time.rep", 0


