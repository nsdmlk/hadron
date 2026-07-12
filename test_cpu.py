from assembler import Assembler
from cpu import CPU

code = ""
print("Enter assembly code (type 'HALT' to finish):")
while True:
    line = input()
    if line == "HALT":
        code += line
        break
    code += line + "\n"

assembler = Assembler()
bytecode = assembler.assemble(code)

cpu = CPU()
cpu.memory[100] = 10
cpu.memory[101] = 18

for i, byte in enumerate(bytecode):
    cpu.memory[i] = byte

cpu.run()
print(cpu.regs)
print(cpu.memory[102]) 
print(cpu.memory[103])