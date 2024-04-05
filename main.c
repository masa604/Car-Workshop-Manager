#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "data.h"



void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

int compararReservas(const void *reserva1, const void *reserva2) {
    Reserva *r1 = (Reserva *)reserva1;
    Reserva *r2 = (Reserva *)reserva2;


    if (r1->dataHora.ano > r2->dataHora.ano) {
        return -1;
    } else if (r1->dataHora.ano < r2->dataHora.ano) {
        return 1;
    } else {
        if (r1->dataHora.mes > r2->dataHora.mes) {
            return -1;
        } else if (r1->dataHora.mes < r2->dataHora.mes) {
            return 1;
        } else {
            if (r1->dataHora.dia > r2->dataHora.dia) {
                return -1;
            } else if (r1->dataHora.dia < r2->dataHora.dia) {
                return 1;
            } else {
                if (r1->dataHora.hora > r2->dataHora.hora) {
                    return -1;
                } else if (r1->dataHora.hora < r2->dataHora.hora) {
                    return 1;
                } else {
                    if (r1->dataHora.minuto > r2->dataHora.minuto) {
                        return -1;
                    } else if (r1->dataHora.minuto < r2->dataHora.minuto) {
                        return 1;
                    } else {
                        return 0;
                    }
                }
            }
        }
    }
}



bool verificar_data(DataHora dataHora) {
    if (dataHora.ano >= 2023 && dataHora.mes >= 1 && dataHora.mes <= 12) {
        int last_dia_mes;
        if (dataHora.mes == 2) {
            if ((dataHora.ano % 4 == 0 && dataHora.ano % 100 != 0) || dataHora.ano % 400 == 0)
                last_dia_mes = 29;
            else
                last_dia_mes = 28;
        } else if (dataHora.mes == 4 || dataHora.mes == 6 || dataHora.mes == 9 || dataHora.mes == 11) {
            last_dia_mes = 30;
        } else {
            last_dia_mes = 31;
        }

        if (dataHora.dia >= 1 && dataHora.dia <= last_dia_mes)
            return true;
    }
    return false;
}

int verificarDisponibilidadeHorario(DataHora data) {
    for (int i = 0; i < numReservas; i++) {
        if (reservas[i].dataHora.dia == data.dia && reservas[i].dataHora.mes == data.mes && reservas[i].dataHora.ano == data.ano) {
            if ((reservas[i].tipo == 1 && data.hora == reservas[i].dataHora.hora && data.minuto == reservas[i].dataHora.minuto) ||
                (reservas[i].tipo == 2 && ((data.hora == reservas[i].dataHora.hora && data.minuto == reservas[i].dataHora.minuto) ||
                                           (data.hora == reservas[i].dataHora.hora + 1 && data.minuto == 0)))) {
                return 0; // Horário já ocupado
            }
        }
    }

    return 1; // Horário disponível
}

void exibirHorasDisponiveis(DataHora data) {
    printf("Horas disponiveis para o dia %02d/%02d/%04d:\n", data.dia, data.mes, data.ano);
    printf("-------------------------------------------\n");

    int hora = 8;
    int minuto = 0;

    while (hora < 18) {
        data.hora = hora;
        data.minuto = minuto;

        if (verificarDisponibilidadeHorario(data)) {
            if(minuto == 30 && hora == 17){
                printf("%02d:%02d (Para lavagem)\n", hora, minuto);
            }else{
                printf("%02d:%02d\n", hora, minuto);
            }

        }

        minuto += 30;
        if (minuto >= 60) {
            hora++;
            minuto = 0;
        }
    }

    printf("-------------------------------------------\n");
}


void inserirNaListaEspera(Reserva reserva) {
    No* novoNo = (No*)malloc(sizeof(No));
    novoNo->reserva = reserva;
    novoNo->proximo = NULL;

    if (listaEspera.inicio == NULL) {
        listaEspera.inicio = novoNo;
        listaEspera.fim = novoNo;
    } else {
        listaEspera.fim->proximo = novoNo;
        listaEspera.fim = novoNo;
    }
}

void removerDaListaEspera() {
    if (listaEspera.inicio == NULL) {
        return;
    }

    No* noRemovido = listaEspera.inicio;
    listaEspera.inicio = listaEspera.inicio->proximo;

    if (listaEspera.inicio == NULL) {
        listaEspera.fim = NULL;
    }

    free(noRemovido);
}

void reservarServico() {
    if (numReservas >= MAX_RESERVAS) {
        printf("Nao e possivel fazer mais reservas.\n");
        return;
    }

    Reserva novaReserva;

    printf("Tipo de servico (1 - Lavagem, 2 - Manutencao): ");
    if (scanf("%d", &novaReserva.tipo) != 1 || (novaReserva.tipo != 1 && novaReserva.tipo != 2)) {
        printf("Tipo de servico invalido.\n");
        return;
    }
    limparBufferEntrada();

    printf("Data (DD/MM/AAAA): ");
    if (scanf("%d/%d/%d", &novaReserva.dataHora.dia, &novaReserva.dataHora.mes, &novaReserva.dataHora.ano) != 3 || !verificar_data(novaReserva.dataHora)) {
        printf("Data invalida.\n");
        return;
    }
    limparBufferEntrada();

    printf("Hora (HH:MM): ");
    if (scanf("%d:%d", &novaReserva.dataHora.hora, &novaReserva.dataHora.minuto) != 2 ||
        (novaReserva.dataHora.minuto != 0 && novaReserva.dataHora.minuto != 30) ||
        ( novaReserva.dataHora.hora < 8 || novaReserva.dataHora.hora >= 18) ||
        (novaReserva.tipo == 2 && novaReserva.dataHora.minuto == 30 && novaReserva.dataHora.hora == 17)) {
        printf("Hora inválida.\n");
        exibirHorasDisponiveis(novaReserva.dataHora);
        return;
    }
    limparBufferEntrada();

    if (!verificarDisponibilidadeHorario(novaReserva.dataHora)) {
        printf("Desculpe, o horario escolhido ja esta ocupado.\n");
        printf("Horas disponiveis para o dia selecionado:\n");
        exibirHorasDisponiveis(novaReserva.dataHora);
        return;
    }

    printf("Nome do cliente: ");
    if (fgets(novaReserva.cliente.nome, sizeof(novaReserva.cliente.nome), stdin) == NULL) {
        printf("Erro ao ler o nome do cliente.\n");
        return;
    }
    novaReserva.cliente.nome[strcspn(novaReserva.cliente.nome, "\n")] = '\0';

    if (novaReserva.cliente.nome[0] == '\0') {
        printf("Nome do cliente invalido.\n");
        return;
    }

    printf("Contato do cliente: ");
    if (fgets(novaReserva.cliente.contacto, sizeof(novaReserva.cliente.contacto), stdin) == NULL) {
        printf("Erro ao ler o contato do cliente.\n");
        return;
    }
    novaReserva.cliente.contacto[strcspn(novaReserva.cliente.contacto, "\n")] = '\0';

    if (novaReserva.cliente.contacto[0] == '\0') {
        printf("Contato do cliente invalido.\n");
        return;
    }

    reservas[numReservas] = novaReserva;
    numReservas++;

    printf("Reserva realizada com sucesso.\n");

    printf("------------------\n");
    //printf("guarde o seu numero de reserva: %d\n", numReservas);
    printf("inicio do servico: %02d:%02d\n", novaReserva.dataHora.hora, novaReserva.dataHora.minuto);
    if (novaReserva.tipo == 1) {
        DataHora hora_fim = novaReserva.dataHora;
        hora_fim.minuto += 30;
        if (hora_fim.minuto >= 60) {
            hora_fim.minuto -= 60;
            hora_fim.hora += 1;
        }
        printf("fim do servico: %02d:%02d\n", hora_fim.hora, hora_fim.minuto);
    } else if (novaReserva.tipo == 2) {
        DataHora hora_fim = novaReserva.dataHora;
        hora_fim.hora += 1;
        printf("fim do servico: %02d:%02d\n", hora_fim.hora, novaReserva.dataHora.minuto);
    }
}

void preReservarServico() {
    if (numReservas >= MAX_RESERVAS) {
        printf("Nao é possivel fazer mais reservas.\n");
        return;
    }

    Reserva novaPreReserva;

    printf("Tipo de servico (1 - Lavagem, 2 - Manutencao): ");
    if (scanf("%d", &novaPreReserva.tipo) != 1 || (novaPreReserva.tipo != 1 && novaPreReserva.tipo != 2)) {
        printf("Tipo de servico invalido.\n");
        return;
    }
    limparBufferEntrada();

    printf("Data (DD/MM/AAAA): ");
    if (scanf("%d/%d/%d", &novaPreReserva.dataHora.dia, &novaPreReserva.dataHora.mes, &novaPreReserva.dataHora.ano) != 3 || !verificar_data(novaPreReserva.dataHora)) {
        printf("Data invalida.\n");
        return;
    }
    limparBufferEntrada();

    printf("Hora (HH:MM): ");
    if (scanf("%d:%d", &novaPreReserva.dataHora.hora, &novaPreReserva.dataHora.minuto) != 2 ||
        (novaPreReserva.dataHora.minuto != 0 && novaPreReserva.dataHora.minuto != 30) ||
        ( novaPreReserva.dataHora.hora < 8 || novaPreReserva.dataHora.hora >= 18) ||
        (novaPreReserva.tipo == 2 && novaPreReserva.dataHora.minuto == 30 && novaPreReserva.dataHora.hora == 17)) {
        printf("Hora inválida.\n");
        exibirHorasDisponiveis(novaPreReserva.dataHora);
        return;
    }
    limparBufferEntrada();

    printf("Nome do cliente: ");
    if (fgets(novaPreReserva.cliente.nome, sizeof(novaPreReserva.cliente.nome), stdin) == NULL) {
        printf("Erro ao ler o nome do cliente.\n");
        return;
    }
    novaPreReserva.cliente.nome[strcspn(novaPreReserva.cliente.nome, "\n")] = '\0';

    if (novaPreReserva.cliente.nome[0] == '\0') {
        printf("Nome do cliente invalido.\n");
        return;
    }

    printf("Contato do cliente: ");
    if (fgets(novaPreReserva.cliente.contacto, sizeof(novaPreReserva.cliente.contacto), stdin) == NULL) {
        printf("Erro ao ler o contato do cliente.\n");
        return;
    }
    novaPreReserva.cliente.contacto[strcspn(novaPreReserva.cliente.contacto, "\n")] = '\0';

    inserirNaListaEspera(novaPreReserva);

    printf("Pre-reserva realizada com sucesso.\n");
}



void cancelarReserva() {
    int indice;

    printf("Indice da reserva a ser cancelada: ");
    if (scanf("%d", &indice) != 1) {
        printf("Entrada invalida. Certifique-se de digitar um numero.\n");
        limparBufferEntrada();
        return;
    }
    limparBufferEntrada();

    if (indice < 0 || indice >= numReservas) {
        printf("Indice invalido.\n");
        return;
    }
    Reserva reservaCancelada = reservas[indice];
    if (listaEspera.inicio != NULL && verificarDisponibilidadeHorario(reservas[indice].dataHora)) {


        reservas[indice] = listaEspera.inicio->reserva;
        removerDaListaEspera();

        printf("Reserva cancelada com sucesso.\n");

        printf("Nova reserva (de pre-reserva para reserva):\n");
        printf("Tipo de servico: %s\n", (reservas[indice].tipo == 1) ? "Lavagem" : "Manutencao");
        printf("Data: %02d/%02d/%04d\n", reservas[indice].dataHora.dia, reservas[indice].dataHora.mes, reservas[indice].dataHora.ano);
        printf("Hora: %02d:%02d\n", reservas[indice].dataHora.hora, reservas[indice].dataHora.minuto);

        printf("Cliente: %s\n", reservas[indice].cliente.nome);
        printf("Contato: %s\n", reservas[indice].cliente.contacto);


        printf("Reserva cancelada:\n");
        printf("Tipo de servico: %s\n", (reservaCancelada.tipo == 1) ? "Lavagem" : "Manutencao");
        printf("Data: %02d/%02d/%04d\n", reservaCancelada.dataHora.dia, reservaCancelada.dataHora.mes, reservaCancelada.dataHora.ano);
        printf("Hora: %02d:%02d\n", reservaCancelada.dataHora.hora, reservaCancelada.dataHora.minuto);
        printf("Cliente: %s\n", reservaCancelada.cliente.nome);
        printf("Contacto: %s\n", reservaCancelada.cliente.contacto);
    } else {
        for (int i = indice; i < numReservas - 1; i++) {
            reservas[i] = reservas[i + 1];
        }

        numReservas--;

        printf("Reserva cancelada com sucesso.\n");
    }
}



void listarReservas() {
    printf("Lista de reservas:\n");
    printf("------------------\n");

    qsort(reservas, numReservas, sizeof(Reserva), compararReservas);

    for (int i = 0; i < numReservas; i++) {
        printf("Indice: %d\n", i);
        printf("Tipo de servico: %s\n", (reservas[i].tipo == 1) ? "Lavagem" : "Manutencao");
        printf("Data: %02d/%02d/%04d\n", reservas[i].dataHora.dia, reservas[i].dataHora.mes, reservas[i].dataHora.ano);
        printf("Hora: %02d:%02d\n", reservas[i].dataHora.hora, reservas[i].dataHora.minuto);
        printf("Cliente: %s\n", reservas[i].cliente.nome);
        printf("Contacto: %s\n", reservas[i].cliente.contacto);
        printf("------------------\n");
    }
}

int compararDataHora(DataHora dh1, DataHora dh2) {
    if (dh1.ano < dh2.ano)
        return -1;
    else if (dh1.ano > dh2.ano)
        return 1;
    else {
        if (dh1.mes < dh2.mes)
            return -1;
        else if (dh1.mes > dh2.mes)
            return 1;
        else {
            if (dh1.dia < dh2.dia)
                return -1;
            else if (dh1.dia > dh2.dia)
                return 1;
            else {
                if (dh1.hora < dh2.hora)
                    return -1;
                else if (dh1.hora > dh2.hora)
                    return 1;
                else {
                    if (dh1.minuto < dh2.minuto)
                        return -1;
                    else if (dh1.minuto > dh2.minuto)
                        return 1;
                    else
                        return 0; // As datas e horas são iguais
                }
            }
        }
    }
}

// Função para ordenar a lista de pré-reservas por ordem cronológica
void ordenarLista() {
    No* noAtual = listaEspera.inicio;
    count = 0;

    while (noAtual != NULL) {
        No* proximo = noAtual->proximo;
        int swapped = 0;

        while (proximo != NULL) {
            if (compararDataHora(noAtual->reserva.dataHora, proximo->reserva.dataHora) == 1) {
                // Trocar os nós
                Reserva temp = noAtual->reserva;
                noAtual->reserva = proximo->reserva;
                proximo->reserva = temp;
                swapped = 1;
            }
            proximo = proximo->proximo;
        }

        if (swapped == 0)
            break;

        noAtual = noAtual->proximo;
    }
}

// Função para listar as pré-reservas por ordem cronológica
void listarPreReservas() {
    printf("Lista de pre-reservas:\n");
    printf("-----------------------\n");

    ordenarLista(); // Chamar a função para ordenar a lista

    No* noAtual = listaEspera.inicio;


    while (noAtual != NULL) {
        printf("Indice: %d\n", count);
        printf("Tipo de serviço: %s\n", (noAtual->reserva.tipo == 1) ? "Lavagem" : "Manutencao");
        printf("Data: %02d/%02d/%04d\n", noAtual->reserva.dataHora.dia, noAtual->reserva.dataHora.mes, noAtual->reserva.dataHora.ano);
        printf("Hora: %02d:%02d\n", noAtual->reserva.dataHora.hora, noAtual->reserva.dataHora.minuto);
        printf("Cliente: %s\n", noAtual->reserva.cliente.nome);
        printf("Contato: %s\n", noAtual->reserva.cliente.contacto);
        printf("-----------------------\n");

        noAtual = noAtual->proximo;
        count++;
    }
}

void realizarServico() {
    int indice;

    printf("Indice da reserva a ser concluida: ");
    if (scanf("%d", &indice) != 1) {
        printf("Entrada invalida. Certifique-se de digitar um numero.\n");
        limparBufferEntrada();
        return;
    }
    limparBufferEntrada();

    if (indice < 0 || indice >= numReservas) {
        printf("Indice invalido.\n");
        return;
    }

    for (int i = indice; i < numReservas - 1; i++) {
        reservas[i] = reservas[i + 1];
    }

    numReservas--;

    printf("Servico concluido com sucesso.\n");
}
void buscarReservasPorContatoA() {

    printf("Digite o numero de contato: ");
    if (fgets(contatoBusca, sizeof(contatoBusca), stdin) == NULL) {
        printf("Erro ao ler o número de contato.\n");
        return;
    }
    contatoBusca[strcspn(contatoBusca, "\n")] = '\0';

    if (contatoBusca[0] == '\0') {
        printf("Numero de contato inválido.\n");
        return;
    }

    int encontradas = 0;

    printf("Reservas encontradas:\n");
    printf("------------------\n");

    for (int i = 0; i < numReservas; i++) {
        if (strcmp(reservas[i].cliente.contacto, contatoBusca) == 0) {
            printf("\n");
            printf("Tipo de servico: %s\n", (reservas[i].tipo == 1) ? "Lavagem" : "Manutencao");
            printf("Data: %02d/%02d/%04d\n", reservas[i].dataHora.dia, reservas[i].dataHora.mes, reservas[i].dataHora.ano);
            printf("Hora: %02d:%02d\n", reservas[i].dataHora.hora, reservas[i].dataHora.minuto);
            printf("Cliente: %s\n", reservas[i].cliente.nome);
            printf("Contacto: %s\n", reservas[i].cliente.contacto);
            printf("------------------\n");
            encontradas++;
        }
    }

    if (encontradas == 0) {
        printf("Nenhuma reserva encontrada com o numero de contato fornecido.\n");
    }
}

void buscarPorContatoB() {

    printf("Marcacoes para o contato %s:\n", contatoBusca);
    printf("-----------------------\n");

    No* noAtual = listaEspera.inicio;

    int encontrou = 0;

    while (noAtual != NULL) {
        if (strcmp(noAtual->reserva.cliente.contacto, contatoBusca) == 0) {
            printf("\n");
            printf("Pre-reserva/as:\n");
            printf("Tipo de serviço: %s\n", (noAtual->reserva.tipo == 1) ? "Lavagem" : "Manutencao");
            printf("Data: %02d/%02d/%04d\n", noAtual->reserva.dataHora.dia, noAtual->reserva.dataHora.mes, noAtual->reserva.dataHora.ano);
            printf("Hora: %02d:%02d\n", noAtual->reserva.dataHora.hora, noAtual->reserva.dataHora.minuto);
            printf("Cliente: %s\n", noAtual->reserva.cliente.nome);
            printf("Contato: %s\n", noAtual->reserva.cliente.contacto);
            printf("-----------------------\n");

            encontrou = 1;
            count++;
        }

        noAtual = noAtual->proximo;
    }

    if (!encontrou) {
        printf("Nenhuma pre-reserva encontrada para o contato %s.\n", contatoBusca);
    }
}






void removerPreReserva() {

    int indice;
    printf("Digite o indice da reserva a ser removida: ");

    if (scanf("%d", &indice) != 1) {
        printf("Indice invalido.\n");
        return;
    }
    if (indice < 0 || indice >= numReservas) {
        printf("Indice invalido.\n");
        return;
    }

    No* noAtual = listaEspera.inicio;
    No* noAnterior = NULL;
    count = 0;

    // Percorre a lista até encontrar o nó correspondente ao índice
    while (noAtual != NULL && count < indice) {
        noAnterior = noAtual;
        noAtual = noAtual->proximo;
        count++;
    }

    if (noAtual == NULL) {
        printf("Reserva nao encontrada.\n");
        return;
    }

    // Remove o nó da lista
    if (noAnterior == NULL) {
        // Se o nó a ser removido é o primeiro da lista
        listaEspera.inicio = noAtual->proximo;
    } else {
        noAnterior->proximo = noAtual->proximo;
    }

    free(noAtual);  // Libera a memória ocupada pelo nó

    printf("Reserva removida com sucesso.\n");
}


void gravarPreReservasEmArquivo() {
    FILE *arquivo;
    arquivo = fopen("pre_reservas.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    No* noAtual = listaEspera.inicio;

    while (noAtual != NULL) {
        fprintf(arquivo, "%d\n", noAtual->reserva.tipo);
        fprintf(arquivo, "%02d/%02d/%04d\n", noAtual->reserva.dataHora.dia, noAtual->reserva.dataHora.mes, noAtual->reserva.dataHora.ano);
        fprintf(arquivo, "%02d:%02d\n", noAtual->reserva.dataHora.hora, noAtual->reserva.dataHora.minuto);
        fprintf(arquivo, "%s\n", noAtual->reserva.cliente.nome);
        fprintf(arquivo, "%s\n", noAtual->reserva.cliente.contacto);

        noAtual = noAtual->proximo;
    }

    fclose(arquivo);
    printf("Dados gravados com sucesso.\n");
}

void carregarPreReservasDoArquivo() {
    FILE *arquivo;
    arquivo = fopen("pre_reservas.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }



    Reserva novaPreReserva;
    char linha[100];

    while (fgets(linha, sizeof(linha), arquivo)) {
        novaPreReserva.tipo = atoi(linha);

        fgets(linha, sizeof(linha), arquivo);
        sscanf(linha, "%d/%d/%d", &novaPreReserva.dataHora.dia, &novaPreReserva.dataHora.mes, &novaPreReserva.dataHora.ano);

        fgets(linha, sizeof(linha), arquivo);
        sscanf(linha, "%d:%d", &novaPreReserva.dataHora.hora, &novaPreReserva.dataHora.minuto);

        fgets(linha, sizeof(linha), arquivo);
        sscanf(linha, "%[^\n]", novaPreReserva.cliente.nome);

        fgets(linha, sizeof(linha), arquivo);
        sscanf(linha, "%[^\n]", novaPreReserva.cliente.contacto);

        inserirNaListaEspera(novaPreReserva);
    }

    fclose(arquivo);
    printf("Dados carregados com sucesso.\n");
}

void gravarReservasEmArquivo(const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para gravacao.\n");
        return;
    }

    for (int i = 0; i < numReservas; i++) {
        fprintf(arquivo, "%d\n", reservas[i].tipo);
        fprintf(arquivo, "%02d/%02d/%04d\n", reservas[i].dataHora.dia, reservas[i].dataHora.mes, reservas[i].dataHora.ano);
        fprintf(arquivo, "%02d:%02d\n", reservas[i].dataHora.hora, reservas[i].dataHora.minuto);
        fprintf(arquivo, "%s\n", reservas[i].cliente.nome);
        fprintf(arquivo, "%s\n", reservas[i].cliente.contacto);
    }

    fclose(arquivo);
    printf("Dados gravados com sucesso no arquivo.\n");
}

void carregarReservasDeArquivo(const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para leitura.\n");
        return;
    }

    Reserva reserva;

    while (fscanf(arquivo, "%d", &reserva.tipo) == 1) {
        fscanf(arquivo, "%d/%d/%d", &reserva.dataHora.dia, &reserva.dataHora.mes, &reserva.dataHora.ano);
        fscanf(arquivo, "%d:%d", &reserva.dataHora.hora, &reserva.dataHora.minuto);
        fscanf(arquivo, " %[^\n]", reserva.cliente.nome);
        fscanf(arquivo, " %[^\n]", reserva.cliente.contacto);

        reservas[numReservas] = reserva;
        numReservas++;

        if (numReservas >= MAX_RESERVAS) {
            printf("O numero maximo de reservas foi alcancado. Algumas reservas nao foram carregadas.\n");
            break;
        }
    }

    fclose(arquivo);
    printf("Dados carregados com sucesso do arquivo.\n");
}



int main() {
    reservas = (Reserva*)malloc(MAX_RESERVAS * sizeof(Reserva));

    numReservas = 0;
    listaEspera.inicio = NULL;
    listaEspera.fim = NULL;

    int opcao;

    do {
        printf("\n");
        printf("1 - Reservar lavagem ou manutencao\n");
        printf("2 - Pre-reservar lavagem ou manutencao\n");
        printf("3 - Cancelar uma reserva\n");
        printf("4 - Cancelar uma pre-reserva\n");
        printf("5 - Listar reservas e pre-reservas ordenadas por data\n");
        printf("6 - Listar reservas e pre-reservas de um cliente.\n");
        printf("7 - Realizar uma lavagem ou manutencao\n");
        printf("8 - Carregar reservas\n");
        printf("9 - Gravar reservas\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        while (scanf("%d", &opcao) != 1 || opcao < 0 || opcao > 9) {
            printf("Escolha uma opcao valida.\n");
            scanf("%*[^\n]");  // discard the rest of the input line
        }
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                reservarServico();
                break;
            case 2:
                preReservarServico();
                break;
            case 3:
                listarReservas();
                cancelarReserva();
                break;
            case 4:
                listarPreReservas();
                removerPreReserva();
                break;
            case 5:
                listarReservas();
                listarPreReservas();
                break;
            case 6:
                buscarReservasPorContatoA();
                buscarPorContatoB();
                break;
            case 7:
                listarReservas();
                realizarServico();
                break;
            case 8:
                carregarPreReservasDoArquivo();
                carregarReservasDeArquivo("reservas.txt");
                break;
            case 9:
                gravarPreReservasEmArquivo();
                gravarReservasEmArquivo("reservas.txt");
                break;
            case 0:
                printf("Encerrando a aplicacao.\n");
                break;
            default:
                printf("Opcao invalida.\n");
                break;
        }

    } while (opcao != 0);

    free(reservas);

    return 0;
}