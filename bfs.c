/*EL2208 Praktikum Pemecahan Masalah dengan C 2023/2024
*Hari dan Tanggal : Senin, 17 Mei 2024
*Nama (NIM)       : Osamah Waliyuddin Azhar (13222089)
*Asisten (NIM)    : Isnaini Azhar Ramadhan Wijaya (1832016)
*Nama File        : dfs.c
*Deskripsi        : Menyelesaikan TSP dengan BFS
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#define MAX_CITIES 15
#define M_PI 3.14
// Structure to represent a city
struct City {
    char name[50];
    double latitude;
    double longitude;
};
// Function to calculate distance between two cities using Haversine formula
double calculateDistance(struct City city1, struct City city2) {
    double lat1 = city1.latitude * M_PI / 180.0;
    double lon1 = city1.longitude * M_PI / 180.0;
    double lat2 = city2.latitude * M_PI / 180.0;
    double lon2 = city2.longitude * M_PI / 180.0;

    double dlon = lon2 - lon1;
    double dlat = lat2 - lat1;
    double a = pow(sin(dlat / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(dlon / 2), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    double distance = 6371 * c; // Radius of Earth in kilometers
    return distance;
}
// Function to find the shortest path using BFS
void findShortestPath(struct City cities[], int numCities, int startCity) {
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    int visited[numCities];
    memset(visited, 0, sizeof(visited));

    int queue[numCities];
    int front = 0, rear = 0;
    queue[rear++] = startCity;
    visited[startCity] = 1;

    double totalDistance = 0.0;

    printf("Shortest path: %s", cities[startCity].name);

    while (front < rear) {
        int currentCity = queue[front++];
        double minDistance = INFINITY;
        int nextCity = -1;

        for (int i = 0; i < numCities; i++) {
            if (!visited[i] && i != currentCity) {
                double distance = calculateDistance(cities[currentCity], cities[i]);
                if (distance < minDistance) {
                    minDistance = distance;
                    nextCity = i;
                }
            }
        }

        if (nextCity != -1) {
            totalDistance += minDistance;
            visited[nextCity] = 1;
            queue[rear++] = nextCity;
            printf(" -> %s", cities[nextCity].name);
        }
    }

    // Return to the starting city
    totalDistance += calculateDistance(cities[queue[rear - 1]], cities[startCity]);
    printf(" -> %s\n", cities[startCity].name);

    printf("Total distance: %.2f kilometers\n", totalDistance);

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time elapsed: %lf s\n", cpu_time_used);
}

int main() {
    char filename[100];
    char startCityName[50];

    // Input filename
    printf("Enter the filename (e.g., cities.csv): ");
    scanf("%s", filename);

    // Input starting city
    printf("Enter the starting city: ");
    scanf("%s", startCityName);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    // Read cities data from CSV file
    char cityName[50];
    double latitude, longitude;
    struct City cities[MAX_CITIES];
    int numCities = 0;
    while (fscanf(file, "%[^,],%lf,%lf\n", cityName, &latitude, &longitude) != EOF) {
        strcpy(cities[numCities].name, cityName);
        cities[numCities].latitude = latitude;
        cities[numCities].longitude = longitude;
        numCities++;
    }
    fclose(file);

    // Find the index of the starting city
    int startCityIndex = -1;
    for (int i = 0; i < numCities; i++) {
        if (strcmp(cities[i].name, startCityName) == 0) {
            startCityIndex = i;
            break;
        }
    }

    if (startCityIndex == -1) {
        printf("Starting city not found.\n");
        return 1;
    }

    // Find the shortest path
    findShortestPath(cities, numCities, startCityIndex);

    return 0;
}