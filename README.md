Projeto esqueleto para "biblioteca" — estrutura de arquivos

Objetivo: fornecer a estrutura mínima (headers e fontes) pedida na imagem:
- Cadastros: livros, usuários, empréstimos
- Operações: listagem, busca sequencial, persistência em texto
- Menu interativo via terminal

Estrutura criada:
- include/
  - book.h
  - user.h
  - loan.h
  - storage.h
  - menu.h
- src/
  - book.c (existente, esqueleto)
  - user.c (esqueleto)
  - loan.c (esqueleto)
  - storage.c (esqueleto)
  - main.c (menu e inicialização)
- data/
  - books.txt
  - users.txt
  - loans.txt

Próximos passos sugeridos:
- Implementar funções CRUD em `src/book.c` e `src/user.c`.
- Implementar buscas manuais (sequenciais) no `storage.c`.
- Implementar gravação/leitura CSV simples em `storage.c`.
# bibliotecaCprogEstruturada