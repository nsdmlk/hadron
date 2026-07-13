"""
A simple CPU emulator
"""
class CPU:
    
    def __init__(self):
        self.regs = [0, 0, 0, 0]  # R1, R2, R3, R4
        self.ip = 0
        self.memory = [0] * 256  # Simple memory model with 256 bytes
        self.flags = {"ZF": False, "SF": False, "CF": False, "OF": False} # basic flags
        self.stack = []
        self.sp = 0
        
    def fetch(self):
        instruction = self.memory[self.ip]
        self.ip += 1
        return instruction
    
    def decode_and_execute(self, instruction):
        if instruction == 0x00:  # NOP
            return True
        elif instruction == 0x01:  # MOV reg, reg
            source = self.fetch()
            destination = self.fetch()
            if 0 <= destination < len(self.regs): self.regs[destination] = self.regs[source]
            return True
        elif instruction == 0x02: # LOAD reg, address
            reg = self.fetch()
            address = self.fetch()
            if 0 <= reg < len(self.regs): self.regs[reg] = self.memory[address]
            return True
        elif instruction == 0x03: # STORE reg, address
            reg = self.fetch()
            addr = self.fetch()
            if 0 <= reg < len(self.regs): self.memory[addr] = self.regs[reg]
            return True
        elif instruction == 0x04: # ADD reg1, reg2, reg_dest
            reg1 = self.fetch()
            reg2 = self.fetch()
            reg_dest = self.fetch()
            if 0 <= reg1 < len(self.regs) and 0 <= reg2 < len(self.regs) and 0 <= reg_dest < len(self.regs): self.regs[reg_dest] = self.regs[reg1] + self.regs[reg2]
            return True
        elif instruction == 0x05: # SUB reg1, reg2, reg_dest
            reg1 = self.fetch()
            reg2 = self.fetch()
            reg_dest = self.fetch()
            if 0 <= reg1 < len(self.regs) and 0 <= reg2 < len(self.regs) and 0 <= reg_dest < len(self.regs): self.regs[reg_dest] = self.regs[reg1] - self.regs[reg2]
            return True
        elif instruction == 0x06: #CMP
            reg1 = self.fetch()
            reg2 = self.fetch()
            if 0 <= reg1 < len(self.regs) and 0 <= reg2 < len(self.regs):
                if self.regs[reg1] - self.regs[reg2] == 0: self.flags["ZF"] = True
                else: self.flags["ZF"] = False
                if self.regs[reg1] - self.regs[reg2] < 0: self.flags["SF"] = True
                else: self.flags["SF"] = False
            return True
        elif instruction == 0x07: #JMP
            address = self.fetch()
            self.ip = address
            return True
        elif instruction == 0x08: #JE
            address = self.fetch()
            if self.flags["ZF"]: self.ip = address
            return True
        elif instruction == 0x09: #JNE
            address = self.fetch()
            if not self.flags["ZF"]: self.ip = address
            return True
        elif instruction == 0x0A: #PUSH
            reg = self.fetch()
            if 0 <= reg < len(self.regs):
                self.stack.append(self.regs[reg])
                self.sp += 1
            return True
        elif instruction == 0x0B: #POP
            reg = self.fetch()
            if 0 <= reg < len(self.regs) and self.sp > 0:
                self.sp -= 1
                self.regs[reg] = self.stack.pop()
            return True
        elif instruction == 0x0C: #CALL
            address = self.fetch()
            self.stack.append(self.ip)
            self.sp += 1
            self.ip = address
            return True
        elif instruction == 0x0D: #RET
            if self.sp > 0:
                self.sp -= 1
                self.ip = self.stack.pop()
            return True
        elif instruction == 0x0E: #LOADI
            reg = self.fetch()
            value = self.fetch()
            if 0 <= reg < len(self.regs): self.regs[reg] = value
            return True
        elif instruction == 0x0F: #ADDI
            reg = self.fetch()
            value = self.fetch()
            if 0 <= reg < len(self.regs): self.regs[reg] += value
            return True
        elif instruction == 0x10: #SUBI
            reg = self.fetch()
            value = self.fetch()
            if 0 <= reg < len(self.regs): self.regs[reg] -= value
            return True
        elif instruction == 0x11: #AND
            reg1 = self.fetch()
            reg2 = self.fetch()
            reg_dest = self.fetch()
            if 0 <= reg1 < len(self.regs) and 0 <= reg2 < len(self.regs) and 0 <= reg_dest < len(self.regs): self.regs[reg_dest] = self.regs[reg1] & self.regs[reg2]
            return True
        elif instruction == 0x12: #OR
            reg1 = self.fetch()
            reg2 = self.fetch()
            reg_dest = self.fetch()
            if 0 <= reg1 < len(self.regs) and 0 <= reg2 < len(self.regs) and 0 <= reg_dest < len(self.regs): self.regs[reg_dest] = self.regs[reg1] | self.regs[reg2]
            return True
        elif instruction == 0x13: #XOR
            reg1 = self.fetch()
            reg2 = self.fetch()
            reg_dest = self.fetch()
            if 0 <= reg1 < len(self.regs) and 0 <= reg2 < len(self.regs) and 0 <= reg_dest < len(self.regs): self.regs[reg_dest] = self.regs[reg1] ^ self.regs[reg2]
            return True
        elif instruction == 0xFF:  # HALT
            return False
        return True
    
    def run(self):
        while True:
            instruction = self.fetch()
            result = self.decode_and_execute(instruction)
            if not result:
                break