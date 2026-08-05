/* ==============================
   DISTANCE VECTOR ROUTING
   (Modified version - same functionality)
   ============================== */

#include <stdio.h>
#include <string.h>

#define MAX_NODES 10
#define INF 999

int nodes;
char router[MAX_NODES][20];
int distance[MAX_NODES][MAX_NODES];
int route[MAX_NODES][MAX_NODES];

/* ---------- Display one router table ---------- */
void showRouterTable(int src)
{
    printf("\nRouting Table of Router %s\n", router[src]);
    printf("%-12s%-10s%-10s\n", "Destination", "Cost", "NextHop");

    for (int dest = 0; dest < nodes; dest++)
    {
        if (src == dest)
            continue;

        printf("%-12s", router[dest]);

        if (distance[src][dest] >= INF)
            printf("%-10s%-10s\n", "INF", "-");
        else
            printf("%-10d%-10s\n", distance[src][dest], router[route[src][dest]]);
    }
}

/* ---------- Display all router tables ---------- */
void showAllTables()
{
    for (int i = 0; i < nodes; i++)
        showRouterTable(i);
}

/* ---------- Display matrix ---------- */
void printMatrix(const char *heading)
{
    printf("\n=========== %s ===========\n\n", heading);

    printf("%-12s", "Router");
    for (int j = 0; j < nodes; j++)
        printf("%-6s", router[j]);
    printf("\n");

    for (int i = 0; i < nodes; i++)
    {
        printf("%-12s", router[i]);

        for (int j = 0; j < nodes; j++)
        {
            if (distance[i][j] >= INF)
                printf("%-6s", "INF");
            else
                printf("%-6d", distance[i][j]);
        }
        printf("\n");
    }
}

/* ---------- Read input ---------- */
void getInput()
{
    printf("Enter number of nodes: ");
    scanf("%d", &nodes);

    printf("Enter router names:\n");
    for (int i = 0; i < nodes; i++)
        scanf("%s", router[i]);

    for (int i = 0; i < nodes; i++)
    {
        for (int j = 0; j < nodes; j++)
        {
            if (i == j)
                distance[i][j] = 0;
            else
                distance[i][j] = INF;
        }
    }

    for (int i = 0; i < nodes; i++)
    {
        for (int j = i + 1; j < nodes; j++)
        {
            int link, cost;

            printf("Is there a link between %s and %s? (1-Yes, 0-No): ",
                   router[i], router[j]);
            scanf("%d", &link);

            if (link)
            {
                printf("Enter cost: ");
                scanf("%d", &cost);

                distance[i][j] = cost;
                distance[j][i] = cost;
            }
        }
    }
}

/* ---------- Initialize next hop ---------- */
void initializeTables()
{
    for (int i = 0; i < nodes; i++)
    {
        for (int j = 0; j < nodes; j++)
        {
            if (i == j)
                route[i][j] = i;
            else if (distance[i][j] < INF)
                route[i][j] = j;
            else
                route[i][j] = -1;
        }
    }
}

/* ---------- Distance Vector Algorithm ---------- */
void distanceVector()
{
    int changed;

    do
    {
        changed = 0;

        for (int i = 0; i < nodes; i++)
        {
            for (int j = 0; j < nodes; j++)
            {
                if (i == j || distance[i][j] >= INF)
                    continue;

                for (int k = 0; k < nodes; k++)
                {
                    if (i == k)
                        continue;

                    int newCost = distance[i][j] + distance[j][k];

                    if (distance[j][k] < INF && newCost < distance[i][k])
                    {
                        distance[i][k] = newCost;
                        route[i][k] = j;
                        changed = 1;
                    }
                }
            }
        }

    } while (changed);
}

/* ---------- Search router by name ---------- */
int findRouter(char name[])
{
    for (int i = 0; i < nodes; i++)
    {
        if (strcmp(router[i], name) == 0)
            return i;
    }
    return -1;
}

/* ---------- Main ---------- */
int main()
{
    int option;

    getInput();
    initializeTables();

    printMatrix("INITIAL ROUTING TABLE");

    distanceVector();

    printMatrix("FINAL ROUTING TABLE");

    while (1)
    {
        printf("\n----- MENU -----\n");
        printf("1. Display one router table\n");
        printf("2. Display all router tables\n");
        printf("3. Display final matrix\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &option);

        if (option == 1)
        {
            char r[20];
            printf("Enter router name: ");
            scanf("%s", r);

            int index = findRouter(r);

            if (index == -1)
                printf("Invalid router name!\n");
            else
                showRouterTable(index);
        }
        else if (option == 2)
        {
            showAllTables();
        }
        else if (option == 3)
        {
            printMatrix("FINAL ROUTING TABLE");
        }
        else if (option == 4)
        {
            break;
        }
        else
        {
            printf("Invalid choice!\n");
        }
    }

    printf("\nProgram terminated successfully.\n");
    return 0;
}









/* ==========================================
   LINK STATE ROUTING ALGORITHM
   (Modified version - same functionality)
   ========================================== */

#include <stdio.h>
#include <string.h>

#define MAX_NODES 10
#define INF 999

int nodes;
char router[MAX_NODES][20];
int graph[MAX_NODES][MAX_NODES];
int shortest[MAX_NODES][MAX_NODES];
int nextRouter[MAX_NODES][MAX_NODES];

/* ---------- Display Link State Packets ---------- */
void showLSP()
{
    printf("\n=========== LINK STATE PACKETS ===========\n");

    for (int i = 0; i < nodes; i++)
    {
        printf("\nLSP of Router %s\n", router[i]);
        printf("%-12s%-10s\n", "Neighbor", "Cost");

        for (int j = 0; j < nodes; j++)
        {
            if (i != j && graph[i][j] < INF)
                printf("%-12s%-10d\n", router[j], graph[i][j]);
        }
    }
}

/* ---------- Display one routing table ---------- */
void showRoutingTable(int src)
{
    printf("\nRouting Table of Router %s\n", router[src]);
    printf("%-12s%-10s%-10s\n", "Destination", "Cost", "NextHop");

    for (int dest = 0; dest < nodes; dest++)
    {
        if (src == dest)
            continue;

        printf("%-12s", router[dest]);

        if (shortest[src][dest] >= INF)
            printf("%-10s%-10s\n", "INF", "-");
        else
            printf("%-10d%-10s\n", shortest[src][dest], router[nextRouter[src][dest]]);
    }
}

/* ---------- Display all routing tables ---------- */
void showAllRoutingTables()
{
    for (int i = 0; i < nodes; i++)
        showRoutingTable(i);
}

/* ---------- Display matrix ---------- */
void printMatrix(const char *heading, int matrix[MAX_NODES][MAX_NODES])
{
    printf("\n=========== %s ===========\n\n", heading);

    printf("%-12s", "Router");
    for (int j = 0; j < nodes; j++)
        printf("%-6s", router[j]);
    printf("\n");

    for (int i = 0; i < nodes; i++)
    {
        printf("%-12s", router[i]);

        for (int j = 0; j < nodes; j++)
        {
            if (matrix[i][j] >= INF)
                printf("%-6s", "INF");
            else
                printf("%-6d", matrix[i][j]);
        }
        printf("\n");
    }
}

/* ---------- Read input ---------- */
void getInput()
{
    printf("Enter number of nodes: ");
    scanf("%d", &nodes);

    printf("Enter router names:\n");
    for (int i = 0; i < nodes; i++)
        scanf("%s", router[i]);

    for (int i = 0; i < nodes; i++)
    {
        for (int j = 0; j < nodes; j++)
        {
            if (i == j)
                graph[i][j] = 0;
            else
                graph[i][j] = INF;
        }
    }

    for (int i = 0; i < nodes; i++)
    {
        for (int j = i + 1; j < nodes; j++)
        {
            int link, cost;

            printf("Is there a link between %s and %s? (1-Yes, 0-No): ",
                   router[i], router[j]);
            scanf("%d", &link);

            if (link)
            {
                printf("Enter cost: ");
                scanf("%d", &cost);

                graph[i][j] = cost;
                graph[j][i] = cost;
            }
        }
    }
}

/* ---------- Dijkstra Algorithm ---------- */
void dijkstra(int source)
{
    int visited[MAX_NODES] = {0};

    for (int i = 0; i < nodes; i++)
    {
        shortest[source][i] = graph[source][i];

        if (i != source && graph[source][i] < INF)
            nextRouter[source][i] = i;
        else
            nextRouter[source][i] = -1;
    }

    shortest[source][source] = 0;
    nextRouter[source][source] = source;
    visited[source] = 1;

    for (int count = 1; count < nodes; count++)
    {
        int u = -1;
        int min = INF;

        for (int i = 0; i < nodes; i++)
        {
            if (!visited[i] && shortest[source][i] < min)
            {
                min = shortest[source][i];
                u = i;
            }
        }

        if (u == -1)
            break;

        visited[u] = 1;

        for (int v = 0; v < nodes; v++)
        {
            if (!visited[v] && graph[u][v] < INF)
            {
                int newCost = shortest[source][u] + graph[u][v];

                if (newCost < shortest[source][v])
                {
                    shortest[source][v] = newCost;
                    nextRouter[source][v] =
                        (u == source) ? v : nextRouter[source][u];
                }
            }
        }
    }
}

/* ---------- Run Dijkstra for all routers ---------- */
void runLinkState()
{
    for (int i = 0; i < nodes; i++)
        dijkstra(i);
}

/* ---------- Find router index ---------- */
int findRouter(char r[])
{
    for (int i = 0; i < nodes; i++)
    {
        if (strcmp(router[i], r) == 0)
            return i;
    }
    return -1;
}

/* ---------- Main ---------- */
int main()
{
    int choice;

    getInput();

    showLSP();

    printMatrix("INITIAL ROUTING TABLE", graph);

    runLinkState();

    printMatrix("FINAL ROUTING TABLE", shortest);

    do
    {
        printf("\n----- MENU -----\n");
        printf("1. Display one router table\n");
        printf("2. Display all router tables\n");
        printf("3. Display final matrix\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        {
            char r[20];
            printf("Enter router name: ");
            scanf("%s", r);

            int index = findRouter(r);

            if (index == -1)
                printf("Invalid router name!\n");
            else
                showRoutingTable(index);

            break;
        }

        case 2:
            showAllRoutingTables();
            break;

        case 3:
            printMatrix("FINAL ROUTING TABLE", shortest);
            break;

        case 4:
            printf("\nProgram terminated successfully.\n");
            break;

        default:
            printf("Invalid choice!\n");
        }

    } while (choice != 4);

    return 0;
}
