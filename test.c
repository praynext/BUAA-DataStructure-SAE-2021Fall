#include<time.h>
#include<stdio.h>
#include<stdlib.h>

#include "AVLTree.h"
#include "BTree.h"
#include "RBTree.h"

int opcode[10][2] = {{0}};
FILE* result;

void search(int N, FILE*AVLData, FILE*BData, FILE*RBData){
    clock_t T1, T2;
    double T;

    T1 = clock();
    for(int i=0; i<N; i++){
        int n;
        struct avlnode *q;
        fscanf(AVLData, "%d", &n);
        q = avlSearch(n, avlroot);
    }
    T2 = clock();
    T = (double)(T2 - T1) / CLOCKS_PER_SEC;
    fprintf(result, "It takes %f seconds for the AVLTree to search %d nodes.\n", T, N);
    
    T1 = clock();
    for(int i=0; i<N; i++){
        int n;
        struct bnode *q;
        fscanf(BData, "%d", &n);
        q = bSearch(n, broot);
    }
    T2 = clock();
    T = (double)(T2 - T1) / CLOCKS_PER_SEC;
    fprintf(result, "It takes %f seconds for the BTree to search %d nodes.\n", T, N);

    T1 = clock();
    for(int i=0; i<N; i++){
        int n;
        struct rbnode *q;
        fscanf(RBData, "%d", &n);
        q = rbSearch(n, rbroot);
    }
    T2 = clock();
    T = (double)(T2 - T1) / CLOCKS_PER_SEC;
    fprintf(result, "It takes %f seconds for the AVLTree to search %d nodes.\n", T, N);

    fprintf(result, "-------------------------------------------------------------------------------------\n");
}
void insert(int N, FILE*AVLData, FILE*BData, FILE*RBData){
    clock_t T1, T2;
    double T;

    T1 = clock();
    for(int i=0; i<N; i++){
        int n;
        fscanf(AVLData, "%d", &n);
        avlroot = avlInsert(n, avlroot);
    }
    T2 = clock();
    T = (double)(T2 - T1) / CLOCKS_PER_SEC;
    fprintf(result, "It takes %f seconds for the AVLTree to insert %d nodes.\n", T, N);
    
    T1 = clock();
    for(int i=0; i<N; i++){
        int n;
        fscanf(BData, "%d", &n);
        bInsert(n);
    }
    T2 = clock();
    T = (double)(T2 - T1) / CLOCKS_PER_SEC;
    fprintf(result, "It takes %f seconds for the BTree to insert %d nodes.\n", T, N);

    T1 = clock();
    for(int i=0; i<N; i++){
        int n;
        fscanf(RBData, "%d", &n);
        rbInsert(n, rbroot);
    }
    T2 = clock();
    T = (double)(T2 - T1) / CLOCKS_PER_SEC;
    fprintf(result, "It takes %f seconds for the RBTree to insert %d nodes.\n", T, N);

    fprintf(result, "-------------------------------------------------------------------------------------\n");
}
void delete(int N,FILE*AVLData, FILE*BData, FILE*RBData){
    clock_t T1, T2;
    double T, t3;
    
    T1 = clock();
    for(int i=0; i<N; i++){
        int n;
        fscanf(AVLData, "%d", &n);
        avlroot = avlDelete(n, avlroot);
    }
    T2 = clock();
    T = (double)(T2 - T1) / CLOCKS_PER_SEC;
    fprintf(result, "It takes %f seconds for the AVLTree to delete %d nodes.\n", T, N);
    
    T1 = clock();
    for(int i=0; i<N; i++){
        int n;
        fscanf(BData, "%d", &n);
        bDelete(n);
    }
    T2 = clock();
    T = (double)(T2 - T1) / CLOCKS_PER_SEC;
    fprintf(result, "It takes %f seconds for the BTree to delete %d nodes.\n", T, N);

    T1 = clock();
    for(int i=0; i<N; i++){
        int n;
        fscanf(RBData, "%d", &n);
        rbDelete(n, rbroot);
    }
    T2 = clock();
    T = (double)(T2 - T1) / CLOCKS_PER_SEC;
    fprintf(result, "It takes %f seconds for the RBTree to delete %d nodes.\n", T, N);
}

int main()
{
    result = fopen("result.txt", "a");
    FILE *AllData,*AVLData, *BData, *RBData;
    int n, j = 0, i = 0;
    binit(3);
    NIL = (struct rbnode*)malloc(sizeof(struct rbnode));
    NIL->lchild = NIL->rchild = NIL->parent = NULL;
    NIL->color = BLACK;
    rbroot = NIL;

    AllData = fopen("AllData.txt", "r");
    AVLData = fopen("AVLData.txt", "r");
    BData = fopen("BData.txt", "r");
    RBData = fopen("RBData.txt", "r");
    fscanf(AllData, "%d", &n);
    while (i < n){
        fscanf(AllData, "%d", &opcode[j][0]);
        fscanf(AllData, "%d", &opcode[j][1]);
        i += opcode[j][0];
        j++;
    }
    int N = j-1;
    fprintf(result, "----------------------------------------begin----------------------------------------\n");
    while (j--) {
        switch (opcode[N-j][1]) {
            case 0:
                insert(opcode[N-j][0], AVLData, BData, RBData);
                break;
            case 1:
                search(opcode[N-j][0], AVLData, BData, RBData);
                break;
            case 2:
                delete(opcode[N-j][0], AVLData, BData, RBData);
                break;
        }
    }
    fprintf(result, "-----------------------------------------end-----------------------------------------\n");

    for(int i = 0;i < opcode[0][0];i++){
        int n;
        fscanf(AVLData, "%d", &n);
        avlroot = avlDelete(n, avlroot);
    }
    for(int i=0;i < opcode[0][0];i++){
        int n;
        fscanf(BData, "%d", &n);
        bDelete(n);
    }
    for(int i = 0;i < opcode[0][0]; i++){
        int n;
        fscanf(RBData, "%d", &n);
        rbDelete(n, rbroot);
    }
    free(NIL);
    fclose(AllData);
    fclose(AVLData);
    fclose(BData);
    fclose(RBData);
    fclose(result);
}