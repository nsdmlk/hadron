class Assembler:
    
    def parse_instruction(self, instruction: str):
        components = instruction.split()
        opcode = components[0].upper()
        if opcode == "NOP":
            return [0x00]
        elif opcode == "MOV":
            reg1 = int(components[1].strip(',')[1]) - 1
            reg2 = int(components[2].strip(',')[1]) - 1
            return [0x01, reg1, reg2]
        elif opcode == "LOAD":
            reg = int(components[1].strip(',')[1]) - 1
            address = int(components[2])
            return [0x02, reg, address]
        elif opcode == "STORE":
            reg = int(components[1].strip(',')[1]) - 1
            address = int(components[2])
            return [0x03, reg, address]
        elif opcode == "ADD":
            reg1 = int(components[1].strip(',')[1]) - 1
            reg2 = int(components[2].strip(',')[1]) - 1
            reg_dest = int(components[3].strip(',')[1]) - 1
            return [0x04, reg1, reg2, reg_dest]
        elif opcode == "SUB":
            reg1 = int(components[1].strip(',')[1]) - 1
            reg2 = int(components[2].strip(',')[1]) - 1
            reg_dest = int(components[3].strip(',')[1]) - 1
            return [0x05, reg1, reg2, reg_dest]
        elif opcode == "CMP":
            reg1 = int(components[1].strip(',')[1]) - 1
            reg2 = int(components[2].strip(',')[1]) - 1
            return [0x06, reg1, reg2]
        elif opcode == "JMP":
            address = int(components[1])
            return [0x07, address]
        elif opcode == "JE":
            address = int(components[1])
            return [0x08, address]
        elif opcode == "JNE":
            address = int(components[1])
            return [0x09, address]
        elif opcode == "HALT":
            return [0xFF]
        elif opcode == "PUSH":
            reg = int(components[1].strip(',')[1]) - 1
            return [0x0A, reg]
        elif opcode == "POP":
            reg = int(components[1].strip(',')[1]) - 1
            return [0x0B, reg]
        elif opcode == "CALL":
            addr = int(components[1])
            return [0x0C, addr]
        elif opcode == "RET":
            return [0x0D]
        elif opcode == "LOADI":
            reg = int(components[1].strip(',')[1]) - 1
            value = int(components[2])
            return [0x0E, reg, value]
        elif opcode == "ADDI":
            reg = int(components[1].strip(',')[1]) - 1
            value = int(components[2])
            return [0x0F, reg, value]
        elif opcode == "SUBI":
            reg = int(components[1].strip(',')[1]) - 1
            value = int(components[2])
            return [0x10, reg, value]
        elif opcode == "AND":
            reg1 = int(components[1].strip(',')[1]) - 1
            reg2 = int(components[2].strip(',')[1]) - 1
            dest = int(components[3].strip(',')[1]) - 1
            return [0x11, reg1, reg2, dest]
        elif opcode == "OR":
            reg1 = int(components[1].strip(',')[1]) - 1
            reg2 = int(components[2].strip(',')[1]) - 1
            dest = int(components[3].strip(',')[1]) - 1
            return [0x12, reg1, reg2, dest]
        elif opcode == "XOR":
            reg1 = int(components[1].strip(',')[1]) - 1
            reg2 = int(components[2].strip(',')[1]) - 1
            dest = int(components[3].strip(',')[1]) - 1
            return [0x13, reg1, reg2, dest]
        else:
            print(f"Unknown instruction: {opcode}")
            return []

    def assemble(self, assembly_code: str):
        self.bytecode = []
        lines = assembly_code.strip().split("\n")
        for line in lines:
            line = line.strip()
            if line and not line.startswith("#"): # Ignore empty lines and comments
                instruction_bytes = self.parse_instruction(line)
                self.bytecode.extend(instruction_bytes)
        return self.bytecode