push 4
push 3
add
:3
push -3
:4

jbe :3
call :5
pop dx
pushr dx

hlt

:5
push 2
out
ret