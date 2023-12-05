//g++ -o connect connect.cpp
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <algorithm>  // for sorting
#include <cstring>
using namespace std;
//test
typedef struct Queue
{
    int capacity;
    int size;
    int front;
    int rear;
    int *elements;
} Queue;

// Modified Stack structure
typedef struct Stack
{
    int capacity;
    int size;
    int front;
    int rear;
    int *elements;
} Stack;

// Function to create a Stack
Stack *createStack(int maxElements)
{
    Stack *S;
    S = (Stack *)malloc(sizeof(Stack));
    S->elements = (int *)malloc(sizeof(int) * maxElements);
    S->size = 0;
    S->capacity = maxElements;
    return S;
}


// Function to push an element onto the Stack
void Push(Stack *S, int element)
{
    if (S->size == S->capacity)
    {
        printf("Stack is Full\n");
    }
    else
    {
        S->size++;
        S->elements[S->size - 1] = element;
    }
    return;
}

// Function to pop an element from the Stack
int Pop(Stack *S)
{
    int element = -1;
    if (S->size == 0)
    {
        return element;
    }
    else
    {
        element = S->elements[S->size - 1];
        S->size--;
    }
    return element;
}

// Function to create a Queue
Queue *createQueue(int maxElements)
{
    Queue *Q;
    Q = (Queue *)malloc(sizeof(Queue));
    Q->elements = (int *)malloc(sizeof(int) * maxElements);
    Q->size = 0;
    Q->capacity = maxElements;
    Q->front = 0;
    Q->rear = -1;
    return Q;
}

// Function to enqueue an element into the Queue
void Enqueue(Queue *Q, int element)
{
    if (Q->size == Q->capacity)
    {
        printf("Queue is Full\n");
    }
    else
    {
        Q->size++;
        Q->rear = Q->rear + 1;
        if (Q->rear == Q->capacity)
        {
            Q->rear = 0;
        }
        Q->elements[Q->rear] = element;
    }
    return;
}

// Function to dequeue an element from the Queue
int Dequeue(Queue *Q)
{
    int element = -1;
    if (Q->size == 0)
    {
        return element;
    }
    else
    {
        element = Q->elements[Q->front];
        Q->size--;
        Q->front++;
        if (Q->front == Q->capacity)
        {
            Q->front = 0;
        }
    }
    return element;
}

// Function to print the graph in Adjacency List format (for debugging)
void PrintAdjacencyListFormat(int *FirstVertex, int V, int *EdgeList, int E)
{
    int v;
    int e;
    cout << "V = " << V << "  E = " << E << endl;
    for (v = 0; v < V; v++)
    {
        cout << endl;
        cout << v << ": ";

        for (e = FirstVertex[v]; e < FirstVertex[v + 1]; e++)
            cout << EdgeList[e] << "-> ";

        cout << "nil";
    }

    cout << endl;
    cout << v << " : ";
    cout << EdgeList[e] << " That's Edge List  nil  Folks " << endl;
}

// Function to perform Breadth First Search (BFS)
void BFS(Queue *Q, int *Found, int *FirstVertex, int V, int *EdgeList)
{
    while (Q->size > 0)
    {
        int current = Dequeue(Q);

        for (int e = FirstVertex[current]; e < FirstVertex[current + 1]; e++)
        {
            int neighbor = EdgeList[e];
            if (Found[neighbor] == -1)
            {
                Found[neighbor] = 1;
                Enqueue(Q, neighbor);
            }
        }
    }
}

// Function to find connected components using BFS
int find_connected_components_BFS(int *FirstVertex, int V, int *EdgeList, int E)
{
    int NumberCC_BFS = 0;
    int *Found = new int[V];
    fill_n(Found, V, -1);
    Queue *Q = createQueue(V);

    for (int i = 0; i < V; ++i)
    {
        if (Found[i] == -1)
        {
            Found[i] = 1;
            Enqueue(Q, i);
            BFS(Q, Found, FirstVertex, V, EdgeList);
            ++NumberCC_BFS;
        }
    }

    delete[] Found;
    delete Q;
    return NumberCC_BFS;
}

// Function to perform Depth First Search (DFS) using a stack
void DFS_with_stack(int start, int *Found, int *FirstVertex, int *EdgeList,int V)
{
    Stack *S = createStack(V);
    Push(S, start);

    while (S->size > 0)
    {
        int current = Pop(S);
        Found[current] = 1;

        for (int e = FirstVertex[current]; e < FirstVertex[current + 1]; e++)
        {
            int neighbor = EdgeList[e];
            if (Found[neighbor] == -1)
            {
                Push(S, neighbor);
            }
        }
    }

    free(S->elements);
    free(S);
}

// Function to find connected components using DFS with a stack
int find_connected_components_DFS(int *FirstVertex, int V, int *EdgeList, int E)
{
    int NumberCC_DFS = 0;
    int *Found = new int[V];
    fill_n(Found, V, -1);

    for (int i = 0; i < V; ++i)
    {
        if (Found[i] == -1)
        {
            DFS_with_stack(i, Found, FirstVertex, EdgeList,V);
            ++NumberCC_DFS;
        }
    }

    delete[] Found;
    return NumberCC_DFS;
}

int main(int argc, char *argv[])
{
    chrono::time_point<chrono::steady_clock> start, stop;
    chrono::duration<double> difference_in_time;
    double difference_in_seconds_BFS, difference_in_seconds_DFS;
    int NumberCC_BFS, NumberCC_DFS;

    ifstream infile1;
    int V, E;

    infile1.open(argv[1]);
    if (!infile1)
    {
        cout << "Error opening file " << endl;
        return -1;
    }

    infile1 >> V;
    infile1 >> E;

    int *FirstVertex = new int[V + 1];
    for (int i = 0; i < V + 1; i++)
        infile1 >> FirstVertex[i];

    int *EdgeList = new int[E + 1];
    for (int i = 0; i < E + 1; i++)
        infile1 >> EdgeList[i];

    infile1.close();

    #if 1 // set to 1 to debug
        cout << endl
             << "A print in Adjacency  List form to help with Debugging! " << endl;
        PrintAdjacencyListFormat(FirstVertex, V, EdgeList, E);
    #endif

    // Find NumberCC_BFS
    start = chrono::steady_clock::now();
    NumberCC_BFS = find_connected_components_BFS(FirstVertex, V, EdgeList, E);
    stop = chrono::steady_clock::now();
    difference_in_time = stop - start;
    difference_in_seconds_BFS = double(difference_in_time.count());

    // Find NumberCC_DFS
    start = chrono::steady_clock::now();
    NumberCC_DFS = find_connected_components_DFS(FirstVertex, V, EdgeList, E);
    stop = chrono::steady_clock::now();
    difference_in_time = stop - start;
    difference_in_seconds_DFS = double(difference_in_time.count());

    // Output results
    ofstream outfile(strcat(argv[1], "_out"));
    outfile << difference_in_seconds_BFS << endl;
    outfile << NumberCC_BFS << endl;
    outfile << difference_in_seconds_DFS << endl;
    outfile << NumberCC_DFS << endl;

    return 0;
}
