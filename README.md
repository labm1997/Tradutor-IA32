# Tradutor-IA32

### Alunos

Matrícula | Nome
--- | ---
16/0030081 | Hélio Adson Bernardo
16/0013615 | Luiz Antônio Borges Martins

### Requisitos

- Para compilar é necessário make e g++
- Foi usado padrão c++11

### Compilação

Execute o comando:
`make`

### Execução

Execute o comando:
`./assembler path/to/file.asm`

A saída será `path/to/file.s`

### Biblioteca de IO

O arquivo `ia32/io.asm` é a implementação da biblioteca de IO em assembly IA-32 usando chamadas de sistema ao kernel do Linux.

### Pequeno teste

O arquivo `test/teste.asm` contém um pequeno teste usando assembly inventado para ser traduzido a assembly IA-32
