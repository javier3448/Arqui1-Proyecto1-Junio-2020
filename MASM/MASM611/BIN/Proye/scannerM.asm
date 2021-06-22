mScanNivelKeyword macro
    add si, 5 + 1;length of nivel + 1
endm

;TODO: ver que procedures de veras usan bx y cx para no hacer
;backups en el stack sin necesitarlos realmente

mScanLevel macro
    push bx
    push cx

    call scanLevel

    pop cx
    pop bx
endm

mScanNumber macro
    push bx
    push cx

    call scanNumber

    pop cx
    pop bx
endm

mScanColor macro
    push bx
    push cx

    call scanColor

    pop cx
    pop bx
endm