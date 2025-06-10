# Este código de exemplo demonstra o uso das sete instruções fornecidas.
# Ele executa operações de memória, aritmética e lógicas, e um desvio condicional.
#
# Registradores utilizados:
# x5, x6: para valores iniciais
# x7: para carregar valor da memória
# x8, x9: para resultados de operações
# x10 (a0): para o valor do shift e para a chamada de sistema (ecall)
# sp (x2): ponteiro da pilha (stack pointer) para operações de memória

_start:
    # Prepara alguns valores iniciais nos registradores x5 e x6
    li x5, 500      # x5 <- 500
    li x6, 120      # x6 <- 120

    # 1. Instrução 'sh' (Store Half)
    # Armazena os 16 bits inferiores de x6 (valor 120) na memória.
    # Usamos o ponteiro da pilha (sp) como endereço base.
    sh x6, -2(sp)

    # 2. Instrução 'lh' (Load Half)
    # Carrega um valor de 16 bits (half-word) da memória para o registrador x7.
    # Como acabamos de salvar 120 na posição sp-2, x7 receberá 120.
    lh x7, -2(sp)   # x7 <- 120 (lido da memória)

    # 3. Instrução 'sub' (Subtract)
    # Subtrai o valor de x7 (120) de x5 (500) e armazena em x8.
    # x8 = 500 - 120  =>  x8 <- 380
    sub x8, x5, x7

    # 4. Instrução 'or' (Bitwise OR)
    # Realiza um "OU" lógico bit a bit entre x6 e x7 e armazena em x9.
    # x6 = 120 (01111000)
    # x7 = 120 (01111000)
    # x9 = x6 | x7  =>  x9 <- 120
    or x9, x6, x7

    # 5. Instrução 'andi' (AND Immediate)
    # Realiza um "E" lógico bit a bit entre x8 (380) e o valor imediato 255.
    # x8 = 380 (...0001 0111 1100)
    # imm = 255 (...0000 1111 1111)
    # x8 = x8 & 255  =>  x8 <- 124 (...0000 0111 1100)
    andi x8, x8, 255

    # 6. Instrução 'srl' (Shift Right Logical)
    # Desloca os bits de x8 (agora 124) duas posições para a direita.
    # x8 = 124 (01111100)
    # x8 >> 2  =>  00011111 (31 em decimal)
    # Para isso, carregamos o valor 2 em um registrador (x10).
    li x10, 2
    srl x8, x8, x10  # x8 <- 31

    # 7. Instrução 'beq' (Branch if Equal)
    # Compara o registrador x9 (120) com o x7 (120).
    # Como eles são iguais, o programa deve pular para a etiqueta 'fim_do_programa'.
    beq x9, x7, fim_do_programa

    # O código abaixo só seria executado se o desvio 'beq' falhasse (se x9 != x7).
    # É um caminho alternativo que, neste caso, não será percorrido.
    li x5, 111 # Instrução de preenchimento

fim_do_programa:
    # Finaliza a execução do programa de forma limpa, como visto em 'entrada.asm'.
    li x10, 93      # Carrega o código de 'exit' (93) no registrador de argumento x10 (a0)
    ecall           # Faz a chamada ao sistema para encerrar o programa