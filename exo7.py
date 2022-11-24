from dataclasses import dataclass
from typing import List
from functools import cmp_to_key

@dataclass
class Position:
    """description d'une position dans le plan spatio-temporel"""

    x: int  # la coordonnée x dans le plan spatio-temporel
    y: int  # la coordonnée y dans le plan spatio-temporel


def compare_lexico (P : Position, Q : Position) :
    
    if P.x != Q.x :
        return P.x - Q.x;
    return P.y - Q.y;

def swap(list, pos1, pos2):
    
    list[pos1], list[pos2] = list[pos2], list[pos1]
    
    return list

def slope(p: Position, q : Position ) :
    if q.x - p.x != 0 :
        return (q.y - p.y)/(q.x - p.x)
    else :
        return float('inf')

def distance(p :Position, q:Position) :
    return (p.x - q.x)*(p.x - q.x) + (p.y - q.y)*(p.y - q.y)

def compare_slope(P : Position, Q: Position) :

    if (slope(P , p0) == slope(Q, p0)) :
        return (distance(P , p0) > distance(Q, p0)) - (distance(P , p0) < distance(Q, p0))
    
    return (slope(P , p0) < slope(Q, p0)) - (slope(P , p0) > slope(Q, p0))

def ortientation(p, q,r) :
    val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y)
    return val

def intersection(a1, a2, a3, a4) :
    p = Position(0,0)
    try :
        p.x = ( (a1.x * a2.y - a1.y * a2.x) * ( a3.x - a4.x ) - ( a1.x - a2.x ) * ( a3.x * a4.y - a3.y * a4.x)  ) / (  ( a1.x - a2.x )*( a3.y - a4.y ) - ( a1.y - a2.y )*( a3.x - a4.x )  )
    except ZeroDivisionError :
        p.x = float('inf')
    try :
        p.y = ( (a1.x * a2.y - a1.y * a2.x) * ( a3.y - a4.y ) - ( a1.y - a2.y ) * ( a3.x * a4.y - a3.y * a4.x)  ) / (  ( a1.x - a2.x )*( a3.y - a4.y ) - ( a1.y - a2.y )*( a3.x - a4.x )  )
    except ZeroDivisionError :
        p.y = float('inf')
    return p

def modulo(a : int, b: int) :
  m = a % b
  if m < 0 :
    m = m - b if b < 0 else m + b
  return m



p0 = Position(0,0)
def aretes_minimales(d: int, n: int, points_de_controles: List[Position]) -> None:
    
    min_index = 0
    start = points_de_controles[0]
    
    for f in range(0, n+1) :
        if(compare_lexico(start, points_de_controles[f])>0) :
            start = points_de_controles[f]
            min_index = f
    
    p0 = start
    swap(points_de_controles, 0, min_index)
    
    #SORT BY SLOPE
    points_de_controles = sorted(points_de_controles, key=cmp_to_key(compare_slope))
    
    pile = [-1]*(n+1)
    pile[0] = points_de_controles[0]
    pile[1] = points_de_controles[1]
    hauteur = 1
    for i in range(2, n+1) :
        while hauteur >= 1 and ortientation(pile[hauteur-1], pile[hauteur], points_de_controles[i]) <= 0 :
            hauteur -= 1
        
        hauteur += 1
        pile[hauteur] = points_de_controles[i]
        
    if hauteur == 2 :
        print("3")
        return
    
    if hauteur == 1 :
        print("2")
        return
    
    min_sides = hauteur + 1
    
    for l in range(0, hauteur +1) :
        
        a1 = pile[l]
        b1 = pile[(l+1)%(hauteur+1)]
        
        for w in range(l+2, l + hauteur) :
            
            a2 = pile[w% (hauteur+1)]
            b2 = pile[(w+1)%(hauteur+1)]
            
            origin = Position(0,0)
            inter = intersection(a1,b1,a2,b2)
            
            if distance(inter, origin) <= d*d :
                
                longeur = min_sides
                
                if distance(a1, inter) < distance(b1, inter) :
                    
                    point = l
                    nb_arrete = modulo(l-(w+1),hauteur+1)
                    longeur = hauteur + 1 - nb_arrete
                
                else :
                    
                    point = (l+1)%(hauteur+1)
                    nb_arrete = modulo(w - (l+1), hauteur +1)
                    longueur = hauteur + 1 - nb_arrete
                
                if longueur < min_sides :
                    min_sides = longeur
    
    print(min_sides)
    return


if __name__ == "__main__":
    d = int(input())
    n = int(input())
    points_de_controles = [Position(*map(int, input().split())) for _ in range(n)]
    origin = Position(0,0)
    points_de_controles.append(origin)
    aretes_minimales(d, n, points_de_controles)
