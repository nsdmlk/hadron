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
            reg_dest = int(components[3][1]) - 1
            return [0x04, reg1, reg2, reg_dest]
        elif opcode == "SUB":
            reg1 = int(components[1].strip(',')[1]) - 1
            reg2 = int(components[2].strip(',')[1]) - 1
            reg_dest = int(components[3][1]) - 1
            return [0x05, reg1, reg2, reg_dest]
        elif opcode == "HALT":
            return [0xFF]

    def assemble(self, assembly_code: str):
        self.bytecode = []
        lines = assembly_code.strip().split("\n")
        for line in lines:
            line = line.strip()
            if line and not line.startswith("#"): # Ignore empty lines and comments
                instruction_bytes = self.parse_instruction(line)
                self.bytecode.extend(instruction_bytes)
        return self.bytecode