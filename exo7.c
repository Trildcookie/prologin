
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


/// description d'une position dans le plan spatio-temporel
struct position {
    int x; ///< la coordonnée x dans le plan spatio-temporel
    int y; ///< la coordonnée y dans le plan spatio-temporel
};

typedef struct positiond{
    double x;
    double y;
}posd;

typedef struct position pos;




// ----------------------------------------------------------
void printpos(pos* tab, int n){
    for(int i = 0; i < n; i++){
        printf("( %d , %d )\n", tab[i].x, tab[i].y);
    }
    printf("\n");
}
// ----------------------------------------------------------

// ----------------------------------------------------------
posd intersection(pos a1, pos a2, pos a3, pos a4){ // L1 (a1,a2) and L2 (a3,a4)
    posd p;
    p.x = (double)( (a1.x * a2.y - a1.y * a2.x) * ( a3.x - a4.x ) - ( a1.x - a2.x ) * ( a3.x * a4.y - a3.y * a4.x)  ) / (double)(  ( a1.x - a2.x )*( a3.y - a4.y ) - ( a1.y - a2.y )*( a3.x - a4.x )  );
    p.y = (double)( (a1.x * a2.y - a1.y * a2.x) * ( a3.y - a4.y ) - ( a1.y - a2.y ) * ( a3.x * a4.y - a3.y * a4.x)  ) / (double)(  ( a1.x - a2.x )*( a3.y - a4.y ) - ( a1.y - a2.y )*( a3.x - a4.x )  );
    return p;
}
// ----------------------------------------------------------


// ----------------------------------------------------------
int compare_lexico(pos P, pos Q){

    if (P.x != Q.x){
        return P.x - Q.x;
    }
    return P.y - Q.y;
}
// ----------------------------------------------------------

// ----------------------------------------------------------
double slope(pos p, pos q ){
    if ( q.x - p.x != 0) {
        return (double)(q.y - p.y)/(double)(q.x - p.x);
    }
        return INT_MAX;
}
// ----------------------------------------------------------
double distance(pos p, pos q){
    return (p.x - q.x)*(p.x - q.x) + (p.y - q.y)*(p.y - q.y);
}

double distanced(posd p, posd q){
    return (p.x - q.x)*(p.x - q.x) + (p.y - q.y)*(p.y - q.y);
}

double distancepd(pos p, posd q){
    return ((double)p.x - q.x)*((double)p.x - q.x) + ((double)p.y - q.y)*((double)p.y - q.y);
}
// ----------------------------------------------------------
pos p0;
int compare_slope( const void* p, const void* q){
    pos P = *(const pos*)p;
    pos Q = *(const pos*)q;
    //pos start_p = *(pos*)start;
    if (slope(P , p0) == slope(Q, p0)){
        return (distance(P , p0) > distance(Q, p0)) - (distance(P , p0) < distance(Q, p0));
    }
    return (slope(P , p0) < slope(Q, p0)) - (slope(P , p0) > slope(Q, p0));
}
// ----------------------------------------------------------



// ----------------------------------------------------------
int orientation(pos p, pos q, pos r){ // Return 0 si collineaire, 1 c bon, -1 pas bon
   int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    return val;
}
// ----------------------------------------------------------

// ----------------------------------------------------------
pos* convex_hull(){

}
// ----------------------------------------------------------

// ----------------------------------------------------------
void swap (pos* tab, int i, int j) {
    pos temp = tab[i];

    tab[i] = tab[j];
    tab[j] = temp;
}
// ----------------------------------------------------------

// ----------------------------------------------------------
void reverse(pos* arr, int n)
{
    pos aux[n];
 
    for (int i = 0; i < n; i++) {
        aux[n - 1 - i] = arr[i];
    }
 
    for (int i = 0; i < n; i++) {
        arr[i] = aux[i];
    }
}
// ----------------------------------------------------------
int modulo(int a, int b) {
  int m = a % b;
  if (m < 0) {
    m = (b < 0) ? m - b : m + b;
  }
  return m;
}
// ----------------------------------------------------------
void aretes_minimales(int d, int n, pos* points_de_controles) {

    int min_sides;
    // STARTING POINT

    //qsort(points_de_controles, n+1, sizeof(pos), compare_lexico);
    
    int min_index = 0;
    pos start = points_de_controles[0];

    for( int f = 0; f < n+1; f++){
        if(compare_lexico(start, points_de_controles[f]) > 0){
            start = points_de_controles[f];
            min_index = f;
        }
    }
    printf(" start : (%d,%d)\n", start.x, start.y);
    

     // TODO simplifier en O(n)


    p0 = start; //global variable
    swap(points_de_controles, 0, min_index); // on met le point start au debut et on va trier le tableau en ignorant le premier element

    printpos(points_de_controles, n+1);

    // SORT BY SLOPE

    qsort(points_de_controles+1, n, sizeof(pos), compare_slope); // tri par pente

    printpos(points_de_controles, n+1); 

   

    //PASS THROUGH EVERY POINT KEEP ONLY THE GOOD ORIENTATIONS AND GET HULL
    pos* pile = (pos*)malloc(sizeof(pos)*(n+1));

    pile[0] = points_de_controles[0];
    pile[1] = points_de_controles[1]; // Cas ou il y a que deux points a traiter TODO
    int hauteur = 1;

    for(int i = 2; i <= n;i++){
        while(hauteur >= 1 && orientation(pile[hauteur-1], pile[hauteur], points_de_controles[i]) <= 0){ // Edge case a verif
            hauteur--;
        }
        hauteur++;
        pile[hauteur] = points_de_controles[i]; // possible index portal oob skip
    }

    if(hauteur == 2){
        printf("%d\n", 3);
        return;
    }
    if(hauteur == 1){
        printf("%d\n",2);
        return;
    }

    pos* hull = (pos*)malloc(sizeof(pos)*(hauteur+1)); // TODO pas necessaire suffit de travailler sur pile
    for(int k = 0; k <= hauteur; k++){
        hull[k] = pile[k];
    }
    printpos(hull, hauteur+1);


    //CALCULATE INTERSECTIONS DEDUCE NEW SIDE COUNT OF POLYGON

    min_sides = hauteur+1;

    pos a1; // Line 1
    pos b1;

    pos a2; // Line 2
    pos b2;

    int point;
    int nb_arrete;
    int longueur;

    for(int l = 0; l < hauteur+1; l++){
        
        a1 = hull[l];                   // On parcourt toutes les lignes
        b1 = hull[(l+1) % (hauteur+1)]; // module pour les fins de liste
        printf("\nFor line (%d,%d)-(%d,%d)\n", a1.x, a1.y, b1.x, b1.y);

        for(int w = l + 2; w < l + hauteur; w++ ){ // On parcout les lignes qui vont se couper non trivialement i.e pas sur le polygon mais en dehors
            
            a2 = hull[w % (hauteur+1)];
            b2 = hull[(w+1) % (hauteur+1)];  // Ligne 2 
            printf(" -test line (%d,%d)-(%d,%d)\n", a2.x, a2.y, b2.x, b2.y);

            posd origin = { .x = 0, .y=0};
            posd inter = intersection(a1, b1, a2, b2);
            printf("   Intersection : (%f,%f)\n",inter.x, inter.y);

            if (distanced(inter, origin) <= d*d) { // + petit ou egal no sabe
                printf("    -> is in circle\n");
                // closes point in line 1 to intersection
                if (distancepd(a1,inter) < distancepd(b1,inter)) {
                    printf(" a1(%d,%d) le plus proche", a1.x, a1.y);
                    point = l;
                    nb_arrete = modulo(l-(w+1), hauteur+1); // a1 - b2 mod hauteur+1 donne le nb d'arrete entre a1 et b2  // TODO verif si on enleve les modules
                    longueur = hauteur + 1 - nb_arrete;
                } else {
                    printf(" b1(%d,%d) le plus proche", b1.x, b1.y);
                    point = (l+1)%(hauteur+1);
                    nb_arrete = modulo(w - (l+1), hauteur +1); // a2 - b1 mod hauteur+1
                    longueur = hauteur + 1 - nb_arrete;
                }
                printf("     ==> nb arrete : %d   | longeur : %d\n", nb_arrete, longueur);
                if(longueur < min_sides) min_sides = longueur;
            }


        }
    }
    printf("%d\n", min_sides);
    return;
}
// ----------------------------------------------------------

// ----------------------------------------------------------
int main() {
    int d;
    scanf("%d", &d);
    int n;
    scanf("%d", &n);
    pos* points_de_controles = (pos*)malloc((n+1) * sizeof(pos));

    for (int i = 0; i < n; ++i) {
        scanf("%d %d", &points_de_controles[i].x, &points_de_controles[i].y);
    }

    points_de_controles[n].x = 0;
    points_de_controles[n].y = 0;

    //printpos(points_de_controles, n+1);

    aretes_minimales(d, n, points_de_controles);
    return 0;
}
// ----------------------------------------------------------