#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


int max(int a, int b){
    return a >= b ? a : b;
}

bool is_same(int* tab1, int* tab2, int n){
    for(int i = 0; i < n; i++){
        if (tab1[i] != tab2[i]) return false;
    }
    return true;
}

/// \param n Le nombre de points
/// \param m Le nombre de tuyaux
/// \param k Le nombre de degrés minimum de refroidissement
/// \param a Le point de départ
/// \param b Le point d'arrivée
/// \param tuyaux Les tuyaux orientés (point de départ, point d'arrivée, refroidissement)
void refroidissement(int n, int m, int k, int a, int b, int** tuyaux) {
    
    // remove duplicates de tuyaux + change m !!!!!!!!!!!!!!
    for(int i=0;i<m;i++){
        for(int j = i+1; j < m; j++){
            if( is_same(tuyaux[i],tuyaux[j],3) == true ){
                for(int k = j; k <m; k++){
                    tuyaux[k] = tuyaux[k+1];
                }

                j--;
                m--;
            }
        }
    }

    int* weight = (int*)malloc(sizeof(int)*n);
    int* fixed_old_weight = (int*)malloc(sizeof(int)*n);
    for(int k =0; k < n; k++){
        weight[k] = -1;
    }

    weight[a-1] = 0;
    int distance = 0;

    while(distance <= 2*max(k,n)){

        // If conditions met print result
        if (weight[ b-1 ] >= k){
            printf("%d\n", distance);
            return;
        }

        distance++;

        //Set previous weight
        for(int i = 0; i < n; i++){
            fixed_old_weight[i] = weight[i];
        }

        // Go throught all points
        for(int index = 0; index < n; index++){

            if (fixed_old_weight[index] != -1){

                // Go through all pipes
                for(int pipe_index = 0; pipe_index < m; pipe_index++ ){

                    int* pipe = tuyaux[pipe_index];

                    if ( pipe[0] == index + 1){

                        weight[ pipe[1]-1 ] = max(fixed_old_weight[ pipe[1] - 1], fixed_old_weight[ index ] + pipe[2]);

                    }

                }

            }
        }


    }
    printf("%d\n", -1);
    return;
}

int main() {
    int n;
    scanf("%d", &n);
    int m;
    scanf("%d", &m);
    int k;
    scanf("%d", &k);
    int a;
    scanf("%d", &a);
    int b;
    scanf("%d", &b);
    int** tuyaux = (int**)malloc(m * sizeof(int*));
    for (int i = 0; i < m; ++i) {
        tuyaux[i] = (int*)malloc(3 * sizeof(int));
        for (int j = 0; j < 3; ++j)
            scanf("%d", &tuyaux[i][j]);
    }
    refroidissement(n, m, k, a, b, tuyaux);

    return 0;
}
