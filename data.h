//
// Created by flour on 31/05/2023.
//
#ifndef PPPFINAL_DATA_H
#define PPPFINAL_DATA_H

typedef struct DataHora{
    int dia;
    int mes;
    int ano;
    int hora;
    int minuto;
} DataHora;

typedef struct Cliente{
    char nome[50];
    char contacto[20];
} Cliente;

typedef struct Reserva{
    int tipo; // 1 - Lavagem, 2 - Manutenção
    DataHora dataHora;
    Cliente cliente;

} Reserva;


typedef struct No{
    Reserva reserva;
    struct No* proximo;
} No;

typedef struct ListaEspera{
    No* inicio;
    No* fim;
} ListaEspera;
#define MAX_RESERVAS 100



Reserva* reservas;
ListaEspera listaEspera;
int count = 0;
int numReservas = 0;
char contatoBusca[20];






#endif //PPPFINAL_DATA_H
