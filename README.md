# SGB — Sistema de Gerenciamento de Biblioteca Universitária

Projeto da disciplina **Programação Estruturada** (Prof. Monique Soares), em **C puro**
(somente a biblioteca padrão: `stdio.h`, `stdlib.h`, `string.h`, `ctype.h`, `time.h`...).
Nenhuma função pronta de busca ou ordenação (`qsort`, `bsearch`) é usada.

## Funcionalidades

**Base (Parte 1)**
- Cadastro de livros (código, título, autor, ano, quantidade disponível) e de usuários (matrícula, nome, curso).
- Empréstimo e devolução, com controle de exemplares disponíveis.
- Listagens e busca sequencial por trecho do título/autor e por matrícula.
- Menu interativo no terminal com validação de entrada.

**Parte 2**
| Requisito | Onde está |
|---|---|
| Alocação dinâmica (`malloc`/`realloc`/`free`) sem vazamentos | `src/storage.c` — `struct Library` com 3 vetores que dobram de capacidade; `lib_free` libera tudo |
| Ordenação manual (2 algoritmos) | `src/sort.c` — **Insertion Sort** e **Selection Sort**, por código ou título |
| Pesquisa binária manual | `src/search.c` — por código e por título; o menu confere a ordem antes (`sort_is_sorted`) |
| Arquivos binários | `src/persist.c` — `biblioteca.bin`; migra automaticamente os `.txt` da Parte 1 |
| Relatório de desempenho com `clock()` | `src/report.c` — ordenações e buscas em N = 1000 a 10000 |
| Extra 1: **Histórico por usuário** | menu *Usuários → Histórico*: todos os empréstimos, abertos e devolvidos, com totais |
| Extra 2: **Controle de atrasos e multa** | `src/fine.c` — prazo de 7 dias, R$ 1,00 por dia de atraso, listagem de atrasados |

## Estrutura

```
include/            src/
  book.h              book.c       modelo Book e comparadores (código, título)
  storage.h           storage.c    Library: vetores dinâmicos, cadastros, empréstimos
  search.h            search.c     busca sequencial e binária
  sort.h              sort.c       Insertion Sort e Selection Sort
  persist.h           persist.c    arquivos texto (.txt) e binário (.bin)
  fine.h              fine.c       prazo, atraso e multa
  report.h            report.c     relatório de desempenho
  util.h              util.c       textos, números e datas
  cli.h                 clic.c       menu interativo
                      main.c       carrega, roda o menu, salva e libera a memória
tests/test_books.c    testes automáticos com assert
docs/                 histórico de commits e exemplo de relatório
```

## Compilar e executar

Com `make` (Linux, macOS ou Windows com MSYS2/MinGW):

```sh
make          # gera ./biblioteca
make run      # compila e executa
make test     # roda os testes automáticos
make clean
```

Sem `make`:

```sh
gcc -Wall -Wextra -std=c99 -Iinclude src/book.c src/clic.c src/fine.c src/main.c src/persist.c src/report.c src/search.c src/sort.c src/storage.c src/util.c -o biblioteca
./biblioteca
```

## Arquivos de dados

- `biblioteca.bin`: formato principal. Carregado ao iniciar e salvo ao sair (ou pela opção *Salvar dados*).
  Cabeçalho `SGB2`, as três quantidades e os vetores gravados com `fwrite`. A gravação usa um arquivo
  `.tmp` e só substitui o anterior no fim, e um arquivo corrompido é recusado sem apagar nada.
- `livros.txt`, `usuarios.txt`, `emprestimos.txt`: exportação em texto (campos separados por `|`,
  datas como `aaaammdd`). Se o `.bin` não existir, o programa lê esses arquivos, inclusive o
  `livros.txt` da Parte 1 (`id|ano|titulo|autor`), e passa a gravar em binário.

O `.bin` guarda as structs como estão na memória: vale para a mesma plataforma e compilador que o gerou.
Para levar os dados para outra máquina, use a exportação em texto.

## Algoritmos

| Algoritmo | Melhor caso | Pior caso | Observação |
|---|---|---|---|
| Insertion Sort | O(n) | O(n²) | estável; desloca structs inteiras |
| Selection Sort | O(n²) | O(n²) | no máximo n−1 trocas |
| Busca sequencial | O(1) | O(n) | não exige ordenação |
| Busca binária | O(1) | O(log n) | exige o vetor ordenado pelo mesmo campo |

Antes da busca binária, o menu verifica se o acervo está ordenado pelo campo escolhido. Se não estiver,
ordena com Insertion Sort e avisa.

## Relatório de desempenho

Menu *Dados e relatórios → Relatório de desempenho*. Ele gera livros sintéticos (semente fixa) e mede com
`clock()`:
1. Insertion × Selection Sort, por código e por título (cada um ordena uma cópia dos mesmos dados);
2. busca sequencial × binária por código, em microssegundos por busca. O lote de 1000 buscas é repetido até
   somar 100 ms, porque uma busca binária leva menos que a resolução do `clock()`.

A tabela aparece no terminal e é gravada em `relatorio_desempenho.txt`, junto com uma análise calculada a
partir das medições. Exemplo de execução (Windows), em [`docs/relatorio_desempenho.txt`](docs/relatorio_desempenho.txt):

| N | Insertion/cód | Selection/cód | Insertion/tít | Selection/tít | Sequencial | Binária |
|---|---|---|---|---|---|---|
| 1000 | 4 ms | 1 ms | 5 ms | 5 ms | 0,478 µs | 0,024 µs |
| 10000 | 388 ms | 167 ms | 514 ms | 494 ms | 6,800 µs | 0,079 µs |

- Dobrar N multiplica o tempo das ordenações por cerca de 4: crescimento O(n²).
- Por código, o Selection Sort ganha: comparar inteiros é barato, e o Insertion paga por mover structs de
  268 bytes. Por título, as comparações de texto pesam mais e os dois empatam.
- A busca sequencial cresce junto com N; a binária quase não muda (86× mais rápida com N = 10000), mas
  depende de o acervo estar ordenado.

Os números variam com a máquina; gere o seu pelo menu.

## Memória

Todo vetor nasce em `NULL` com capacidade 0 e cresce com `realloc`, dobrando a capacidade. Se o `realloc`
falhar, o vetor antigo continua válido e a operação devolve erro, sem perder dados. `lib_free` libera os três
vetores ao sair, e o relatório libera seus vetores temporários ao fim de cada medição. Para conferir no Linux:

```sh
valgrind --leak-check=full ./biblioteca
```

Projeto da disciplina Programação Estruturada
Alunos: Luciano Nascimento e João Guilherme
