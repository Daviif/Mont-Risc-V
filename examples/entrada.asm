# Exemplo de instruções para o Grupo 12 (lw, sw, sub, xor, addi, srl, beq)

# Exemplo com beq e rótulos

start_program:
addi x1, x0, 10      # end: 0x0
addi x2, x0, 5       # end: 0x4

loop_start:         # end: 0x8
sub x3, x1, x2      # end: 0xC
addi x1, x1, -1     # end: 0x10
beq x1, x0, loop_end # end: 0x14 - Este beq deve saltar para loop_end
xor x4, x3, x2      # end: 0x18
beq x0, x0, loop_start # end: 0x1C - Este beq deve saltar para loop_start

addi x5, x0, 100    # end: 0x20
loop_end:           # end: 0x24
srl x6, x5, x1      # end: 0x24
sw x6, 0(x0)        # end: 0x28

final_instruction:
sub x7, x6, x5      # end: 0x2C