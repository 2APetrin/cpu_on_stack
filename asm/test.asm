:3

push 2  
push 1

jb :1
:2

hlt

:1
push 123
push -1
mul
out

jmp :2
jmp :3