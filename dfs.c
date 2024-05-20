/*EL2208 Praktikum Pemecahan Masalah dengan C 2023/2024
*Hari dan Tanggal : Senin, 13 Mei 2024
*Nama (NIM)       : Bianca Chiquita Irene Tawang (13222092)
*Asisten (NIM)    : Isnaini Azhar Ramadhan Wijaya (1832016)
*Nama File        : dfs.c
*Deskripsi        : Menyelesaikan TSP dengan DFS
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef struct {
    char name[50];
    double latitude;
    double longitude;
} City;

double calculateDistance(City city1, City city2) {
    double dLat, dLong, j, distance;
    dLat = (city2.latitude) - (city1.latitude);
    dLong = (city2.longitude) - (city1.longitude);
    j = pow((sin(dLat / 2)), 2)+ (cos(city1.latitude) * cos(city2.latitude) *pow((sin(dLong / 2)), 2));
    distance = 2*6371*asin(sqrt(j));
    return distance;
}

void dfsTSP(City cities[], int numCities, int startLocation, int visited[], int currentCity, int depth, double *bestdis, int path[], int bestpath[]) {

    if (depth == numCities) {
        double distance = 0;
        for (int i = 0; i < numCities; i++) {
            int currentCityIndex = path[i];
            int nextCityIndex = path[(i + 1) % numCities];
            distance = distance + calculateDistance(cities[currentCityIndex], cities[nextCityIndex]);
        }

        if (distance < *bestdis) {
            *bestdis = distance;
            memcpy(bestpath, path, numCities * sizeof(int));
        }
        return;
    }
    for (int nextCity = 0; nextCity < numCities; nextCity++) {
        if (!visited[nextCity]) {
            visited[nextCity] = 1;
            path[depth] = nextCity;
            dfsTSP(cities, numCities, startLocation, visited, nextCity, depth + 1, bestdis, path, bestpath);
            visited[nextCity] = 0;
        }
    }
}

void findShortestPath(City cities[], int numCities, int startLocation, int visited[], double bestdis, int path[], int bestpath[]) {
    visited[startLocation] = 1;
    clock_t start = clock();
    dfsTSP(cities, numCities, startLocation, visited, startLocation, 1, &bestdis, path, bestpath);
    clock_t end = clock();
    double timeElapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Best route found: ");
    int startIndex = -1;
    for (int i = 0; i < numCities; i++) {
        if (bestpath[i] == startLocation) {
            startIndex = i;
            break;
        }
    }
    for (int i = startIndex; i < numCities; i++) {
        printf("%s", cities[bestpath[i]].name);
        if (i < numCities - 1) {
            printf(" -> ");
        }
    }
    printf(" -> %s\n", cities[startLocation].name);

    printf("Best route distance: %f km\n", bestdis);
    printf("Time elapsed: %f s\n", timeElapsed);
}

int main() {
    FILE *file;
    char filename [60];
    char tampung [15][60];
    char tampung2 [15][60];
    char tampung3 [15][60];
    char start[10];
    City cities[15];
    char *pisah[60];
    double d,j;
    double bestdis = INFINITY;
    int x = 0;
    

    printf("Enter list of cities file name: ");
    scanf("%s", filename);
    file = fopen(filename, "r");

    if (file == NULL){
        printf("Error: Cities file name not found");
        return 0;
    }
    else{
        while(fgets(tampung[x], sizeof(tampung[x]), file)!=NULL){
            x++;
        }
    }

    char *lintang[x];
    char *bujur[x];
    int visited[x];
    int path[x];
    int bestpath[x];
   
    for(int y = 0; y<x;y++){
        strcpy(tampung2[y], tampung[y]);
        strcpy(tampung3[y], tampung[y]);
        pisah[y] = strtok(tampung[y], "\n");
        strcpy(cities[y].name, strtok(pisah[y], ","));
        lintang[y] = strtok(tampung2[y], ",");
        lintang[y] = strtok(NULL, ",");
        bujur[y] = strtok(tampung3[y], ",");
        bujur[y] = strtok(NULL, ",");
        bujur[y] = strtok(NULL, ",");
    }

    for(int z = 0;z<x;z++){
        cities[z].latitude = (atof(lintang[z]))*(3.14159265358979323846/180);
        cities[z].longitude = (atof(bujur[z]))*(3.14159265358979323846/180);
    }

    int strpnt = -1;
    printf("Enter starting point: ");
    scanf("%s", start);
    for (int o = 0; o < x; o++) {
        visited[o] = 0;
        path[o]= -1;
        bestpath[o]=-1;
        if (strcmp(cities[o].name, start) == 0) {
            strpnt = o;
            path[0] = o;
        }
    }
    if (strpnt == -1) {
        printf("Error: Starting city not found in the list.\n");
        return 1;
    }
    findShortestPath(cities, x, strpnt, visited, bestdis, path, bestpath);
    fclose(file);
    return 0;
}