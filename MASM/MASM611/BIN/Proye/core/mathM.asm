mGenerateSeed macro
    push ax
    push cx
    push dx

    call generateSeed

    pop dx
    pop cx
    pop ax
endm

;returns ax: random word
mGetRandomWord macro
    push dx
    
    call getRandomWord

    pop dx
endm