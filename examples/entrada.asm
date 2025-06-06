or x2, x2, x1
andi x2, x1, 0x10   # 0x10 em hexadecimal é 16 em decimal
addi x2, x3, -8     # Números negativos continuam funcionando
beq x5, x6, 20
sh x5, 0xC(x6)      # 0xC em hexadecimal é 12 em decimal