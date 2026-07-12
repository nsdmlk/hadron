"""
A simple CPU emulator
"""
class CPU:
    
    def __init__(self):
        self.regs = [0, 0, 0, 0]  # R1, R2, R3, R4
        self.ip = 0
        self.memory = [0] * 256  # Simple memory model with 256 bytes
        
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
        elif instruction == 0xFF:  # HALT
            return False
        return True
    
    def run(self):
        while True:
            instruction = self.fetch()
            result = self.decode_and_execute(instruction)
            if not result:
                break