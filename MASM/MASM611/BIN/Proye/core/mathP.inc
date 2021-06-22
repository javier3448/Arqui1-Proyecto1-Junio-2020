;Genera una semilla 'aleatoria' para la secuencia LCG, solo necesitamos 
;correrlo al inicio del programa
generateSeed proc
    mov ah, 00h   ; interrupt to get system timer in cx:dx 
    int 1ah
    mov mathRandomSeed, dx
    ret
generateSeed endp

;Random 16 bits:
;Returns: ax next random number
;clobbers:    ax, dx
;This implements a Linear Congruential Generator (LCG) with a power-of-2 modulus
;%65536 happens for free because the low 16 bits of the product + increment are in AX, and the higher bits aren't.
;https://stackoverflow.com/questions/40698309/assembly-random-numbers-generator
getRandomWord proc
    mov ax, 25173
    mul mathRandomSeed
    add ax, 13849
    mov mathRandomSeed, ax
    ret
getRandomWord endp