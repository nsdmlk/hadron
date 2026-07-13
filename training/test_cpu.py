from training.assembler import Assembler
from training.cpu import CPU

code = """
LOADI R1, 5
LOADI R2, 1
LOADI R3, 0
CMP R1, R3
JE 20
SUB R1, R2, R1
JMP 9
HALT
"""
# print("Enter assembly code (type 'HALT' to finish):")
# while True:
#     line = input()
#     if line == "HALT":
#         code += line
#         break
#     code += line + "\n"

assembler = Assembler()
bytecode = assembler.assemble(code)

cpu = CPU()

for i, byte in enumerate(bytecode):
    cpu.memory[i] = byte

cpu.run()
print(cpu.regs)
print(cpu.flags)