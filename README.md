# Biblioteca Virtual

Projeto desenvolvido na disciplina de Programação Estruturada.

## Descrição
Este projeto simula uma biblioteca virtual, permitindo o cadastro e gerenciamento de livros, usuários e empréstimos de forma simples e interativa via terminal. O sistema armazena os dados em arquivos de texto, preservando as informações entre execuções.

## Funcionalidades
- Cadastro de livros
- Cadastro de usuários
- Registro de empréstimos e devoluções
- Listagem de registros
- Busca sequencial por identificadores e dados relevantes
- Persistência em arquivos TXT

## Estrutura do projeto
- `include/` - arquivos de cabeçalho
  - `book.h`
  - `user.h`
  - `loan.h`
  - `storage.h`
  - `menu.h`
- `src/` - implementação do sistema
  - `main.c`
  - `book.c`
  - `user.c`
  - `loan.c`
  - `storage.c`
- `data/` - arquivos de persistência
  - `books.txt`
  - `users.txt`
  - `loans.txt`

## Integrantes do grupo
- Luciano Nascimento
- João Guilherme

## Tecnologias
- Linguagem: C
- Paradigma: Programação Estruturada

## Como executar
1. Abra o terminal na pasta do projeto.
2. Compile com o comando:
   ```bash
   make
   ```
3. Execute o programa:
   ```bash
   ./biblioteca
   ```

## Observação
Este projeto foi desenvolvido como atividade acadêmica para a disciplina de Programação Estruturada, com foco em organização modular, manipulação de arquivos e lógica de controle em C.