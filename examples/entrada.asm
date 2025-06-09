# Arquivo de teste para pseudo-instruções
# Testa tanto as de 1-para-1 quanto as de 1-para-Muitos.

# --- Seção 1: Pseudo-Instruções Simples (1-para-1) ---

# mv deve ser traduzido para -> addi x10, x5, 0
mv x10, x5

# nop deve ser traduzido para -> addi x0, x0, 0
nop

# not deve ser traduzido para -> xori x11, x10, -1
not x11, x10


# --- Seção 2: Pseudo-Instruções Complexas (1-para-Muitos) ---

# Teste 1: 'li' com um valor pequeno que cabe em 12 bits.
# Deve ser traduzido para UMA instrução: addi x5, x0, 200
li x5, 200

# Teste 2: 'li' com um valor grande que exige duas instruções.
# Deve ser traduzido para DUAS instruções:
# 1. lui x6, 1
# 2. addi x6, x6, 0 
# (4096 = 0x1000. upper=1, lower=0)
li x6, 4096

# Teste 3: 'li' com um valor de 32 bits mais complexo.
# O valor é 80000 (0x13880).
# Deve ser traduzido para DUAS instruções:
# 1. lui x7, 20 (0x14, pois o bit 11 de 0x880 é 1)
# 2. addi x7, x7, -1920 (0x880 com sinal estendido)
li x7, 80000


# --- Seção 3: Instruções Reais ---
# Para garantir que o resto do montador continua funcionando.

add x1, x2, x3
sub x4, x5, x6
sw x7, 12(x2) # Instrução do seu 'entrada.asm' original