;Sorts para arreglo usrs

mBubbleSortPoints macro
    pusha

    call bubbleSortPoints

    popa
endm

mBubbleSortTimes macro
    pusha

    call bubbleSortTimes

    popa
endm

;Sirve como entry inicial a la recursion tambien
mQuickSortPoints macro
LOCAL SORT
    pusha

    cmp shouldQuickSortDraw, 0
    je SORT
    call writeQuickHeader

    SORT:
    call usrsUpdateRam
    call usrsGetEnd ;di = endUsrs
    sub di, S_USR;di-- porque end no es inclusivo
    push di
    mov si, offset usrs
    push si
    call quickSortPoints
    popa
endm

;Sirve como entry inicial a la recursion tambien
mQuickSortTimes macro
LOCAL SORT
    pusha

    cmp shouldQuickSortDraw, 0
    je SORT
    call writeQuickHeader

    SORT:
    call usrsUpdateRam
    call usrsGetEnd ;di = endUsrs
    sub di, S_USR;di-- porque end no es inclusivo
    push di
    mov si, offset usrs
    push si
    call quickSortTimes
    popa
endm

mPartitionPoints macro _low, _high
    pushaButAx

    mov ax, _high
    push ax
    mov ax, _low
    push ax 
    call partitionPoints

    popaButAx
endm

mPartitionTimes macro _low, _high
    pushaButAx

    mov ax, _high
    push ax
    mov ax, _low
    push ax 
    call partitionTimes

    popaButAx
endm

mShellSortPoints macro
    pusha

    call shellSortPoints

    popa
endm

mShellSortTimes macro
    pusha

    call shellSortTimes

    popa
endm
