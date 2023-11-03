/*
SIMULADOR DE MEMORIA CACHE
TURMA M1: HEITOR SILVA AVILA; GABRIEL GONZAGA SEABRA;
COMPILAR COM: gcc cache_simulator.c -o cache_simulator -lm -Wall
DISPONÍVEL EM: https://github.com/heitorzxc/aoc2/
*/
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>

// REPRESENTAÇÃO DE UMA MEMÓRIA CACHE
typedef struct cachetable
{
    int tag_cache;
    int validity_cache;
    int hits_cache;
} cachetable;

int main(int argc, char const *argv[])
{
    if (argc != 7)
    {
        printf("Numero de argumentos incorreto. Utilize:\n");
        printf("./cache_simulator <nsets> <bsize> <assoc> <substituição> <flag_saida> <arquivo_de_entrada.bin>\n");
        exit(EXIT_FAILURE);
    }
    srand(time(NULL));
    int nsets = atoi(argv[1]), bsize = atoi(argv[2]), associativity = atoi(argv[3]), flag = atoi(argv[5]), loop_nsets = 0, loop_assoc = 0, n_bits_index = 0, n_bits_offset = 0, index_read = 0, tag_read = 0, hit_occurred = 0, enable_replacement_alg = 0, total_addresses = 0, hit = 0, miss_compulsory = 0, miss_capacity = 0, miss_conflict = 0, loop = 0, loop_for_rand = 0, empty_block_found = 0;
    unsigned int address;
    char replacement[3];
    strcpy(replacement, argv[4]);
    FILE *input_file = fopen(argv[6], "rb");
    if (input_file == NULL)
    {
        printf("Erro ao abrir o arquivo de entrada. Verifique o caminho e tente novamente.\n");
        exit(EXIT_FAILURE);
    }
    n_bits_index = log2(nsets);
    n_bits_offset = log2(bsize);

    // ALOCAÇÃO DINÂMICA DE MEMÓRIA PARA CACHEMEMORY[NSETS][ASSOC]
    cachetable **cachememory = (cachetable **)malloc(nsets * sizeof(cachetable *));
    for (loop_nsets = 0; loop_nsets < nsets; loop_nsets++)
    {
        cachememory[loop_nsets] = (cachetable *)malloc(associativity * sizeof(cachetable));
    }
    // INICIALIZAÇÃO DA MEMÓRIA CACHE
    for (loop_nsets = 0; loop_nsets < nsets; loop_nsets++)
    {
        for (loop_assoc = 0; loop_assoc < associativity; loop_assoc++)
        {
            cachememory[loop_nsets][loop_assoc].hits_cache = 0;
            cachememory[loop_nsets][loop_assoc].tag_cache = 0;
            cachememory[loop_nsets][loop_assoc].validity_cache = 0;
        }
    }
    // MAPEAMENTO DIRETO
    if (associativity == 1)
    {
        while (fread(&address, 4, 1, input_file) == 1)
        {
            /* FUNÇÃO INTRÍNSECA DO C CUJA ABREVIAÇÃO É "BYTE SWAP 32" USADA PARA CONVERTER BIG ENDIAN PARA LITTLE ENDIAN
            https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html */
            address = __builtin_bswap32(address);
            index_read = (address >> n_bits_offset) & ((int)pow(2, n_bits_index) - 1);
            tag_read = address >> (n_bits_offset + n_bits_index);
            total_addresses++;
            // CONDIÇÃO PARA HIT
            if (cachememory[index_read][0].tag_cache == tag_read && cachememory[index_read][0].validity_cache == 1)
            {
                hit++;
            }
            // DEFININDO OS TIPOS DE MISSES
            else if (cachememory[index_read][0].validity_cache == 0)
            {
                miss_compulsory++;
                cachememory[index_read][0].tag_cache = tag_read;
                cachememory[index_read][0].validity_cache = 1;
            }
            else
            {
                miss_conflict++;
                cachememory[index_read][0].tag_cache = tag_read;
                cachememory[index_read][0].validity_cache = 1;
            }
        }
    }
    // TOTALMENTE ASSOCIATIVO
    else if (nsets == 1)
    {
        while (fread(&address, 4, 1, input_file) == 1)
        {
            address = __builtin_bswap32(address);
            index_read = (address >> n_bits_offset) & ((int)pow(2, n_bits_index) - 1);
            tag_read = address >> (n_bits_offset + n_bits_index);
            total_addresses++;
            // VARREDURA EM BUSCA DO HIT
            for (loop = 0; loop < associativity; loop++)
            {
                // BUSCAR HIT NAS VIAS
                if (cachememory[0][loop].tag_cache == tag_read && cachememory[0][loop].validity_cache == 1)
                {
                    hit++;
                    cachememory[0][loop].hits_cache++;
                    hit_occurred = 1;
                    enable_replacement_alg = 0;
                    break;
                }
            }
            // MISS CAPACIDADE OU COMPULSÓRIO? QUANTAS VIAS LIVRES?
            for (loop = 0; loop < associativity; loop++)
            {
                if (cachememory[0][loop].validity_cache == 0)
                {
                    // PELO MENOS UMA VIA LIVRE INTERROMPE O LAÇO E DESLIGA O ALGORITMO DE SUBSTITUIÇÃO
                    enable_replacement_alg = 0;
                    cachememory[0][loop].hits_cache = 1;
                    cachememory[0][loop].tag_cache = tag_read;
                    cachememory[0][loop].validity_cache = 1;
                    miss_compulsory++;
                    break;
                }
                else
                {
                    enable_replacement_alg = 1;
                }
            }
            if (enable_replacement_alg == 1)
            {
                // RANDOM
                if (strcmp(replacement, "R") == 0 || strcmp(replacement, "r") == 0)
                {
                    loop_for_rand = rand() % associativity;
                    cachememory[0][loop_for_rand].tag_cache = tag_read;
                    cachememory[0][loop_for_rand].validity_cache = 1;
                    cachememory[0][loop_for_rand].hits_cache = 1;
                    miss_capacity++;
                }
            }
        }
    }
    // CONJUNTO ASSOCIATIVO
    else
    {
        while (fread(&address, 4, 1, input_file) == 1)
        {
            address = __builtin_bswap32(address);
            index_read = (address >> n_bits_offset) & ((int)pow(2, n_bits_index) - 1);
            tag_read = address >> (n_bits_offset + n_bits_index);
            total_addresses++;
            // VARREDURA EM BUSCA DO HIT
            for (loop = 0; loop < associativity; loop++)
            {
                if (cachememory[index_read][loop].tag_cache == tag_read && cachememory[index_read][loop].validity_cache == 1)
                {
                    hit++;
                    cachememory[index_read][loop].hits_cache++;
                    hit_occurred = 1;
                    enable_replacement_alg = 0;
                    break;
                }
                else
                {
                    hit_occurred = 0;
                }
            }
            // MISS
            if (hit_occurred == 0)
            {
                empty_block_found = 0;
                for (loop = 0; loop < associativity; loop++)
                {
                    if (cachememory[index_read][loop].validity_cache == 0)
                    {
                        miss_compulsory++;
                        cachememory[index_read][loop].tag_cache = tag_read;
                        cachememory[index_read][loop].validity_cache = 1;
                        cachememory[index_read][loop].hits_cache = 1;
                        empty_block_found = 1;
                        break;
                    }
                }
                if (!empty_block_found)
                {
                    miss_conflict++;
                    enable_replacement_alg = 1;
                }
                else
                {
                    enable_replacement_alg = 0;
                }
            }

            // ALGORITMO DE SUBSTITUIÇÃO
            if (enable_replacement_alg == 1)
            {
                // RANDOM
                if (strcmp(replacement, "R") == 0 || strcmp(replacement, "r") == 0)
                {
                    loop_for_rand = rand() % associativity;
                    cachememory[index_read][loop_for_rand].tag_cache = tag_read;
                    cachememory[index_read][loop_for_rand].validity_cache = 1;
                    cachememory[index_read][loop_for_rand].hits_cache = 1;
                    miss_capacity++;
                }
            }
        }
    }
    float miss_total = miss_capacity + miss_conflict + miss_compulsory;
    float hit_rate = (float)hit / total_addresses;
    float miss_rate = 1.0f - hit_rate;
    float rate_miss_capacity = (float)miss_capacity / miss_total;
    float rate_miss_compulsory = (float)miss_compulsory / miss_total;
    float rate_miss_conflict = (float)miss_conflict / miss_total;
    if (flag == 0)
    {
        printf("Acessos: %d\nTaxa hits: %.2f\nTaxa miss: %.2f\nTaxa compulsorio: %.2f\nTaxa capacidade: %.2f\nTaxa conflito: %.2f\n", total_addresses, hit_rate, miss_rate, rate_miss_compulsory, rate_miss_capacity, rate_miss_conflict);
    }
    else
    {
        printf("%d, %.2f, %.2f, %.2f, %.2f, %.2f", total_addresses, hit_rate, miss_rate, rate_miss_compulsory, rate_miss_capacity, rate_miss_conflict);
    }
    for (loop_nsets = 0; loop_nsets < nsets; loop_nsets++)
    {
        free(cachememory[loop_nsets]);
    }
    fclose(input_file);
    return 0;
}