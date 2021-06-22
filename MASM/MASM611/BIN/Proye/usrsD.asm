;CONSTANTS
S_USR equ sizeof USR
L_PASSWORD equ lengthof USR.password ;Lo podemos definir de una porque simpre son 4 caracteres
L_MAX_NAME equ lengthof USR._name - 1

L_USRS equ 20

TYPE_ADMIN equ 1
TYPE_USR equ 0

loggedInUsr db lengthof USR._name dup(0)

usrsPath db "USRS.BIN", 0
usrs USR L_USRS dup({  })