in
in
in
    pop cx 
    pop bx 
    pop ax 

    push 8
    cast

    push 0
    pushr ax
        je :1

    pushr bx
    pushr bx
    mul

    push 4
    pushr ax
    pushr cx
    mul
    mul

    sub
    pop dx

    push 0
    pushr dx
        ja :3

    pushr bx
    push -1
    mul

    pushr dx
    sqrt
    add

    push 2
    pushr ax
    mul

    div

    pushr bx
    push -1
    mul

    pushr dx
    sqrt
    sub

    push 2
    pushr ax
    mul

    div

    pop ax
    pop bx

    pushr ax
    pushr bx
        je :2

    pushr ax
    pushr bx
    out
    out

    hlt


:1
    push 0
    pushr bx
        je :4
    
    push -1
    pushr cx
    mul
    pushr bx
    div

    out
    hlt


:2
    pushr ax
    out
    hlt


:3
    noroots
    hlt


:4
    push 0
    pushr cx
        jne :3
    
    allnum
    hlt

