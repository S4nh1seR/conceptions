data intro Hello, I will help you to find fibonacci number!
data suggest Please, enter the index of number!

jlabel base
pop res
push reg2
return

def fibonacci
pop reg1
pop reg2
push reg1
eq reg2 0
jlnz res base
eq reg2 1
jlnz res base
push reg2
sub reg2 1
push reg2
push_ra
call fibonacci
pop reg1
pop reg2
sub reg2 2
push reg1
push reg2
push_ra
call fibonacci
pop reg1
pop reg2
add reg1 reg2
pop res
push reg1
return

begin
prints #intro
prints #suggest
read reg1
push reg1
push_ra
call fibonacci
pop reg1
prints Your number is:
print reg1
end