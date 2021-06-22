;Aqui
mDrawFramePoints macro
    pusha

    call writePointsHeader
    call drawFrame

    popa
endm

mDrawFrameTimes macro
    pusha

    call writeTimesHeader
    call drawFrame

    popa
endm

mGraphUsrsPoints macro
    pusha

    call graphUsrsPoints

    popa
endm

mGraphUsrsTimes macro
    pusha

    call graphUsrsTimes

    popa
endm

mClearGraphingArea macro
    push si
    push di
    push cx

    call clearGraphingArea

    pop cx
    pop di
    pop si
endm

;time: numero de 0 a 9
mDelay macro time
    push dx
    push si
    push di
    push ax

    mov ax, time
    call delay

    pop ax
    pop di
    pop si
    pop dx
endm


mNoteOn macro freq
    push ax
    push si

    mov si, freq
    call noteOn

    pop si
    pop ax
endm

mNoteOff macro
    push ax

    call noteOff

    pop ax
endm

;Lo mismo que mDrawRectangle pero agrega 2 pixeles de padding en x0 y x1
mDrawBar macro x0, y0, x1, y1, color
    pusha

    mov cl, color
    mov ax, y1
    push ax
    mov ax, x1
    sub ax, 2 ;x1 = x1 - 2
    push ax
    mov ax, y0
    push ax
    mov ax, x0
    add ax, 2 ;x0 = x0 - 2
    push ax
    call drawRectangle

    popa
endm

mUpdateTime macro
    pusha 

    call updateTime

    popa
endm

;destructive macro
dmInitTime macro
    call getMonthSecs
    mov initialTime, eax
endm


mPrintTopPoints macro
    pusha

    call printTopPoints

    popa
endm

mPrintTopTimes macro
    pusha

    call printTopTimes

    popa
endm