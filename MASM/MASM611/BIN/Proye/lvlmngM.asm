;null cuando _name es 0
LEVEL struct 2
    _name db 8 dup(" ")
    time dw 10
    time_enemy dw 3
    time_prize dw 2
    points_enemy dw -5
    points_prize dw 5
    color db RED
    ;1 byte padding
LEVEL ends

;LOS VALORES DEFAULT DE LEVEL ESTAN SETEADOS PARA DEBUGGING AHORITA.
;LOS VERDADEROS VALORES DEFAULT SERIAN:
    ;_name db 0
    ;;1 byte padding
    ;time dw 0
    ;time_enemy dw 0ffffh
    ;time_prize dw 0ffffh
    ;points_enemy dw 0
    ;points_prize dw 0
    ;color db RED