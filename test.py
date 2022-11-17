from typing import List

        
def refroidissement(n, m, k, a, b, tuyau) :

    #sort les tuyaux

    tuyaux = []
    [tuyaux.append(x) for x in tuyau if x not in tuyaux]


    #print("Tuyaux sort : ", tuyaux)
    #Indice stock comment sont organise les tuyaux
    #indice = []
    #prev = 0
    #for i in range(len(tuyaux)) :
    #    if tuyaux[i][0] != prev :
    #        indice.append(i)
    #        prev = tuyaux[i][0]
    #indice.append(len(tuyaux) + 1)
    #---#


    debut = a 
    weight = [ -1 for i in range(n) ]
    weight[debut - 1] = 0
    distance = 0

    while distance <= 2* max(k,n) :
        #print(" Current distance : ", distance)
        #print(" ============ ")
        #print(" Weight : ", weight)
        #print(" Prev weight : ", prev_weight)
        #print(" ============ ")

        if weight[ b - 1 ] >= k :
            return distance
        
        distance += 1
        
        #print("ON POSE FIXED OLD WEIGHT = ", weight)
        fixed_old_weight = weight.copy()
        for index in range(n) :

            #print(" => From the point ", index + 1)
            #print(" The weight of this index is : ", fixed_old_weight[index], " in the list ", fixed_old_weight)
            if fixed_old_weight[index] != -1 : 

                for pipe in tuyaux : # A changer pour ne pas passer sur tous les tuyaux
                    #print(" On test le tuyau : ", pipe)
                    if pipe[0] == index + 1 :#TODO pas necessaire
                        #print(" On the good starting point ! ", pipe[0], " = ", index + 1)
                        weight[ pipe[1] - 1 ] = max( fixed_old_weight[ pipe[1] - 1] , fixed_old_weight[ index ] + pipe[2] )
                        #print("====> Weight after modification : ", weight)
        
    return -1

                

if __name__ == "__main__":
    n = int(input())
    m = int(input())
    k = int(input())
    a = int(input())
    b = int(input())
    tuyaux = [list(map(int, input().split())) for _ in range(m)]
    #print("Fini le parse !")
    print(refroidissement(n,m,k,a,b,tuyaux))