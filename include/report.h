/*
 * report.h - Relatorio de desempenho (benchmark com clock()).
 *
 * [COMMIT 12 - RELATORIO DE DESEMPENHO]
 */
#ifndef REPORT_H
#define REPORT_H

#define REPORT_FILE "relatorio_desempenho.txt"

/* Mede, com dados sinteticos de varios tamanhos, Insertion x Selection Sort
 * (por codigo e por titulo) e busca sequencial x binaria. Mostra a tabela no
 * terminal e grava o mesmo texto em `path`. Devolve 0 ou -1 em erro. */
int report_run(const char *path);

#endif /* REPORT_H */
