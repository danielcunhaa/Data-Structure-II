#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <queue>
#include <climits>      // INT_MAX
#include <set>
#include <algorithm>    // swap

#define INF INT_MAX // Constante infita para o método Dijkstra
#define COUNT 4

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::list;
using std::vector;
using std::map;
using std::priority_queue;
using std::set;
using std::swap;

struct Edge;
struct Vertex
{
    int index;      // Índice do vértice na lista de vértices
    string label;   // Rótulo do vértice
    list<Edge> adj;  // Lista de adjacência

    Vertex(int index, string label) // Constructor
    {
        this->index = index;
        this->label = label;
    }    
};

struct Edge
{
    string label;   // Rótulo
    int w;          // Peso
    int o;          // Origem  -> índice do vértice origem
    int d;          // Destino -> índice do vértice destino na lista de vértices

    Edge(string label, int origin, int dest, int weight)
    {
        this->label = label;
        this->o     = origin;
        this->d     = dest;
        this->w     = weight;
    }

    bool operator<(const Edge& tmp) const
    {
        return w > tmp.w || (w == tmp.w && label > tmp.label);
    }
};

class Graph
{
    int V;  // Número de vértices

protected:
    struct dijkstraUtil;
    Edge getNewEdge(string label, int origin, int destiny, int weight); // Função auxiliar para adicionar uma aresta ao grafo
    int  shorterDist(dijkstraUtil table[]); // Função auxiliar para encontrar o índice não finalizado com menor custo da tabela no algoritmo de dijkstra
    void printMST(int root, list<int> table[], int space);  // Imprime a MST
    void printMSTTable(int root, list<int> table[]);        // Imprime a tabela de arestas
    set<int> getNewSet(int i, int j);   // Retorna um novo Set contendo os inteiros i e j

public:
    vector<Vertex> vertexList;   // Lista de vértices
    vector<Edge>   edgeList;     // Lista de arestas
    map<string, int> vertexUtil; // Map auxiliar para armazenar o índice do vértice na lista de vértices
    map<string, int> edgeUtil;   // Map auxiliar para armazenar o índice da aresta na lista de arestas

    Graph(int V);    // Constructor
    void addVertex(int index, string label);    // Adiciona um vértice ao grafo
    void addEdge(int index, string label, string orig, string dest, int weight); // Adiciona uma aresta ao grafo
    void dijkstra(string source, string dest);  // Método de Dijkstra
    void prim(string source);   // Método de Prim
    void kruskal();             // Método de Kruskal
};

Graph::Graph(int V)
{
    this->V = V;
}

void Graph::addVertex(int index, string label)
{
    Vertex tmp(index, label);
    vertexList.push_back(tmp);
    vertexUtil[label] = index;
}

Edge Graph::getNewEdge(string label, int origin, int destiny, int weight)
{
    Edge tmp(label, origin, destiny, weight);
    return tmp;
}

void Graph::addEdge(int index, string label, string orig, string dest, int weight)
{
    // Índices dos vértices
    int i = vertexUtil[orig], j = vertexUtil[dest];

    vertexList[i].adj.push_back(getNewEdge(label, i, j, weight));
    vertexList[j].adj.push_back(getNewEdge(label, j ,i, weight));

    edgeList.push_back(getNewEdge(label, i, j, weight));
    edgeUtil[label] = index;
}

// Estrutura auxiliar para a execução do algoritmo de Dijkstra
struct Graph::dijkstraUtil
{
    int dist, previous;
    bool visited;

    dijkstraUtil()       // Para cada nodo do grafo
    {
        dist = INF;      // Declara o custo dele como infinito
        previous = -1;   // O índice do seu anterior como inválido
        visited = false; // E como não visitado
    }
};

void Graph::dijkstra(string source, string dest)
{
    // Distância (custo) da origem, índice do vértice que o antecede no caminho e uma flag para caso tenho sido finalizado
    dijkstraUtil table[V];

    int v = vertexUtil[source];  // Acha o índice da origem
    table[v].dist = 0; // Zera o custo da origem

    int rest = V; // Variável de controle para executar o método enquanto houve nodo a ser visitado
    int aux, n;   // Variável auxiliar, neighbor

    while(rest--)
    {
        v = shorterDist(table); // Pega o índice do vértice com menor custo
        table[v].visited = true;

        // Caso índice com o menor custo seja o destino, ele sai do laço
        if(vertexList[v].label == dest) break;

        for(auto it = vertexList[v].adj.begin(); it != vertexList[v].adj.end(); ++it)
        {
            n = it->d; // Índice do vértice destino
            if(!table[n].visited)
            {
                aux = table[v].dist + it->w; // Custo de v + peso da aresta

                if(aux < table[n].dist)
                {
                    table[n].dist = aux;
                    table[n].previous = v;
                }
            }
        }
    }

    // Imprimir a tabela com os dados
    cout << "\nDijkstra: " << source << " to " << dest << endl;
    cout << "Rótulo\tVisitado\tCusto\tCaminho\n";
    for(int i = 0; i < V; ++i)
    {
        cout << vertexList[i].label << "\t";
        (table[i].visited) ? cout << "TRUE\t\t" : cout << "FALSE\t\t";
        (table[i].dist != INF) ? cout << table[i].dist << "\t" : cout << "INF\t";
        
        if(table[i].previous == -1) // Caso seja a origem ou não foi visitado
            (vertexList[i].label == source) ? cout << "ORIGEM\n" : cout << "NULL\n";
        else
        {
            aux = table[i].previous;

            while(aux != -1)
            {
                cout << vertexList[aux].label;
                aux = table[aux].previous;
                (aux == -1) ? cout << "\n" : cout << " ";
            }
        }
    }
}

int Graph::shorterDist(dijkstraUtil table[])
{
    int min = INF, i; // Menor custo encontrado, índice do menor custo
    for(int j = 0; j < V; ++j)
        if(!table[j].visited && table[j].dist < min)
        {
            min = table[j].dist;
            i = j;
        }

    return i;
}

void Graph::prim(string source)
{
    int i = vertexUtil[source], j;  // Índice do vértice de origem
    int E = edgeList.size();        // Número de arestas
    bool table[E];                  // Tabela de arestas que já foram visitadas
    for(j = 0; j < E; ++j) table[j] = false;

    priority_queue<Edge> edgeQueue; // Fila de prioridade para as arestas
    set<int> notInTree;             // Conjunto contendo os vértices que ainda não foram inseridos

    // Adiciona ao set os índices do vértices, menos do vértice de origem
    for(j = 0; j < V; ++j)
        if(j != i)
            notInTree.insert(j);

    // Adiciona as arestas do vértice de origem na fila
    // E os marca como visitadas na tabela
    for(auto it = vertexList[i].adj.begin(); it != vertexList[i].adj.end(); ++it)
    {
        table[ edgeUtil[it->label] ] = true;
        edgeQueue.push(*it);
    }

    // Declara o custo da MST como 0 e a raiz da árvore como i
    int weight = 0;
    int root   = i;

    // Cria uma lista de inteiros para armazenar a MST
    list<int> MST[V];

    // Cria uma lista de inteiros para armazenar os índices das arestas presentes na MST
    list<int> edgesMST;

    // Enquanto houver vértice fora da árvore (considerando que o grafo seja um grafo conexo)
    while(!notInTree.empty())
    {
        // Pega a aresta do topo da lista
        auto edge = edgeQueue.top();
        edgeQueue.pop();

        // Logicamente, todo vértice origem da aresta já estará na MST
        // Então basta apenas validar o vértice destino
        // Pega os índices do vértice Origem e vértice Destino
        i = edge.o; j = edge.d;

        // Caso o vértice j (destino) não estejá na MST. Ele é adicionado como filho de i,
        // ele é removido do conjunto notInTree e todas as suas arestas são adicionadas
        // à fila de prioridade.
        if(notInTree.count(j))
        {
            MST[i].push_back(j);
            notInTree.erase(j);
            weight += edge.w;

            edgesMST.push_back( edgeUtil[ edge.label ] );
            // Adiciona as arestas do vértice de origem na fila
            // Apenas irá adicionar as arestas que ainda não foram adicionadas (pelo seu destino)
            for(auto it = vertexList[j].adj.begin(); it != vertexList[j].adj.end(); ++it)
                if(!table[ edgeUtil[it->label] ])
                {
                    table[ edgeUtil[it->label] ] = true;
                    edgeQueue.push(*it);
                }

        }
    }

    // Imprime a lista de arestas
    cout << "\n\tMétodo de Prim\n\n";
    cout << "Custo da MST:  " << weight << endl;
    cout << "Vértice fonte: " << source << endl;
    cout << "\nLista de arestas da MST:\n";
    cout << "Rótulo\tVértices\tPeso\n";

    for(auto it = edgesMST.begin(); it != edgesMST.end(); ++it)
    {
        auto edge = edgeList[*it];
        cout << edge.label << "\t(\"" << edge.o << "\", \"" << edge.d << "\")\t " << edge.w << "\n";
    }

    // Impressão da árvore
    cout << "\nImpressão da árvore\n";
    printMST(root, MST, 0);
}

void Graph::printMST(int root, list<int> table[], int space)
{
    space += COUNT;

    for(int i = COUNT; i < space; i++) cout << " ";
    cout << vertexList[root].label << endl;

    for(auto it = table[root].begin(); it != table[root].end(); ++it)
    {
        printMST(*it, table, space);
    }
}

set<int> Graph::getNewSet(int i, int j)
{
    set<int> tmp;
    tmp.insert(i); tmp.insert(j);
    return tmp;
}

void Graph::kruskal()
{
    priority_queue<Edge> edgeQueue; // Fila de prioridade para as arestas
    vector< set<int> > Trees;       // Vetor de conjuntos de inteiros onde serão armazenados os índices dos vértices
    list<int> edgesKruskal;         // Cria uma lista de inteiros para armazenar os índices das arestas presentes na MST
    set<int> notInTree;             // Conjunto de inteiros contendo os índices dos vértices que ainda não fazem parte de nenhuma árvore
                                    // Utilizado apenas para controle

    // Adiciona todas as arestas do grafo na fila de prioridade
    for(auto it = edgeList.begin(); it != edgeList.end(); ++it)
        edgeQueue.push(*it);

    // Adiciona todos os índices dos vértices do grafo no conjunto notInTree
    for(int i = 0; i < V; ++i)
        notInTree.insert(i);

    int i, j; // Variáveis auxiliares
    int weight = 0; // Declara o peso da MST como 0

    // Enquanto não existir apenas uma árvore e ainda existirem vértices fora da árvore
    while(Trees.size() != 1 || !notInTree.empty())
    {
        auto edge = edgeQueue.top(); // Pega a aresta do topo
        edgeQueue.pop();             // E a retira da fila

        i = edge.o; // Vértice origem
        j = edge.d; // Vértice destino

        // Variáveis de controle para testar se I e J estão presentes no conjunto notInTree
        bool hasI = notInTree.count(i),
             hasJ = notInTree.count(j);

        // Caso os dois vértices estejam contidos no conjunto notInTree, eles são retirados, é criado um novo conjunto
        // e adicionado ao vector, e sua aresta é adicionada na lista de arestas da MST
        if(hasI && hasJ)
        {
            Trees.push_back(getNewSet(i, j));
            edgesKruskal.push_back(edgeUtil[ edge.label ]);
            weight += edge.w;
            notInTree.erase(i); notInTree.erase(j);
        }
        // Caso apenas um esteja no conjunto, este é retirado, adicionado ao conjunto do outro presente no vector
        // e a aresta é adicionada na lista de arestas da MST
        else if(hasI || hasJ)
        {
            // Se i for o vértice contido em notInTree, realiza um swap(i, j)
            if(hasI) swap(i, j);

            notInTree.erase(j);
            edgesKruskal.push_back(edgeUtil[ edge.label ]);
            weight += edge.w;

            // Busca o conjunto de i no vector. Quando encontrar, adiciona j no conjunto e interrompe o laço
            for(auto it = Trees.begin(); it != Trees.end(); ++it)
                if(it->count(i))
                {
                    it->insert(j);
                    break;
                }
        }
        /*
            Caso nenhum dos vértices estejam no conjunto notInTree, então os dois já fazem parte de alguma árvore
            Nesse caso existem duas possibilidades:
                1. Os vértices fazem parte da mesma árvore e deve-se ignorar essa aresta
                2. Os vértices fazem parte de árvores diferentes. Nesse caso deve realizar um merge
                   e adicionar a aresta na lista de arestas da MST.
        */
        else
        {
            // Procuramos o índice das árvores de cada um
            int indexI, indexJ;
            indexI = indexJ = -1; // Inicia os índices com -1

            for(int k = 0; k < (int)Trees.size(); ++k)
            {
                if(Trees[k].count(i)) indexI = k;
                if(Trees[k].count(j)) indexJ = k;
                // Caso já tenha encontrado os dois índices, interrompe o laço
                if(indexI != -1 && indexJ != -1)
                    break;
            }

            // Se os índices forem iguais, deve-se ignorar essa aresta e partir para a próxima
            // Caso contrário, realiza um merge e adiciona a aresta na lista de arestas da MST
            if(indexI != indexJ)
            {
                // Adiciona os valores do conjunto do vértice J ao conjunto do vértice I
                for(auto it = Trees[indexJ].begin(); it != Trees[indexJ].end(); ++it)
                    Trees[indexI].insert(*it);

                // Remove do vector o conjunto do vértice J
                Trees.erase(Trees.begin() + indexJ);

                // Adiciona a aresta na lista de arestas da MST
                edgesKruskal.push_back(edgeUtil[ edge.label ]);
                weight += edge.w;
            }
        }
    }

    // Imprime a lista de arestas da MST
    cout << "\n\tMétodo de Kruskal\n\n";
    cout << "Custo da MST: " << weight;
    cout << "\nLista de arestas da MST:\n";
    cout << "Rótulo\tVértices\tPeso\n";

    for(auto it = edgesKruskal.begin(); it != edgesKruskal.end(); ++it)
    {
        auto edge = edgeList[*it];
        cout << edge.label << "\t(\"" << edge.o << "\", \"" << edge.d << "\")\t " << edge.w << "\n";
    }
}