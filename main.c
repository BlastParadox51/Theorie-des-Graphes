#include <stdio.h>
#include <stdlib.h>

/// creation de la structure Node pour faire une matrice d'ajacences et info_list.
typedef struct Node{
    int Node; // id if the node
    int Node2;
    int Link; // if 2 nodes are link it's it has a 1 if not 0
    int prev_node; // so that every node are link (for BFS and DFS)
    int next_node;
    int cheak;
    int weight;
    int Analyse_Done;
    struct Node* nextNode;
    struct Node* previous_node;
}T_Node;

///faire une pile pour l'openlist
typedef struct DChaines
{
    struct Node* firstnode;
} T_DChaines;

/// ajoute un maillon au début de la pile
T_DChaines pushlist(T_DChaines nodes,T_Node* node)
{
    node->nextNode = NULL;
    if(nodes.firstnode == NULL)
    {
        nodes.firstnode = node;
    }
    else
    {
        node->nextNode = nodes.firstnode;
        nodes.firstnode = node;
    }
    return nodes;
}

/// supprime un maillon au début de la pile
T_DChaines delet_first_element(T_DChaines first)
{
    if(first.firstnode == NULL)
    {
        return first;
    }
    else if(first.firstnode->nextNode == NULL)
    {
        free(first.firstnode);
        first.firstnode = NULL;
        return first;
    }
    else
    {
        T_Node* holder = first.firstnode;
        first.firstnode = first.firstnode->nextNode;
        free(holder);
        return first;
    }
}

/// range la pile
void Sorter(T_DChaines nodes)
{
    if(nodes.firstnode == NULL)
    {

    }
    else
    {
        T_Node* Cursor = nodes.firstnode;
        T_Node holder;
        while(Cursor->nextNode != NULL)
        {
            if(Cursor->weight <= Cursor->nextNode->weight)
            {
                Cursor = Cursor->nextNode;
            }
            else
            {
                puts("test");
                holder.weight = Cursor->weight;
                holder.Node = Cursor->Node;
                holder.Node2 = Cursor->Node2;
                Cursor->weight = Cursor->nextNode->weight;
                Cursor->Node = Cursor->nextNode->Node;
                Cursor->Node2 = Cursor->nextNode->Node2;
                Cursor->nextNode->weight = holder.weight;
                Cursor->nextNode->Node = holder.Node;
                Cursor->nextNode->Node2 = holder.Node2;

                Cursor = nodes.firstnode;
            }
        }
    }
}

/// initialise la pile
T_DChaines iniChaine(void)
{
    T_DChaines n;
    n.firstnode = NULL;
    return n;
}

/// self explained
void free_Graph(T_Node ** Graph,int N_nodes)
{
    for(int i = 0 ; i < N_nodes ; i++)
    {
        free(Graph[i]);
    }
    free(Graph);
}

/// cree le graph en fonction du nombres de sommet.
T_Node ** create_my_graphe(int N_nodes)
{
    T_Node ** x = (T_Node **)calloc(N_nodes,sizeof(T_Node *));
    for (int i = 0; i < N_nodes ; i++)
    {
        x[i] = (T_Node *)calloc(N_nodes,sizeof(T_Node));
    }
    return x;
}

/// ce demande si la pile est vide (retourn 1 si elle est vide et 0 si elle l'est pas)
int is_openlist_empty(T_DChaines C)
{
    if(C.firstnode == NULL)
    {
        return 1;
    }
    else return 0;
}

/// permet de convertir le nom appellation du node a la bonne mémoir dans l'info list
int find_correct_node(T_Node* info_list,int N_node,int a)
{
    for(int i = 0 ; i < N_node ; i++)
    {
        if(a == info_list[i].Node)
        {
            return i;
        }
    }
    puts("Node don't exist retry");
    return -2;
}

/// crée un node.
T_Node* Create_Node(int id,int id2,int weight)
{
    T_Node* n = (T_Node*)calloc(1,sizeof(T_Node));
    n->nextNode = NULL;
    n->previous_node = NULL;
    n->weight = weight;
    n->Node = id;
    n->Node2 = id2;
    n->prev_node = -1;
    return n;
}

void DFS(T_Node* info_list,T_Node** Graph,int N_nodes,int* status,int node)
{
    int Stop_cheak = 0;
    for (int i = 0; i < N_nodes; ++i) {
        if(Stop_cheak == 0)
        {
            if(Graph[node][i].Link == 1 && Graph[node][i].Analyse_Done == 0 && info_list[i].cheak == -1)
            {
                Graph[node][i].Analyse_Done = 1;
                Graph[i][node].Analyse_Done = 1;
                info_list[i].cheak = node;
                DFS(info_list,Graph,N_nodes,status,i);
            }
            else if(Graph[node][i].Link == 1 && Graph[node][i].Analyse_Done == 0 && info_list[i].cheak != -1)
            {
                status = 1;
                Stop_cheak = 1;
            }
        }
    }
}

int Verif_DFS(T_Node* info_list,T_Node** Graph,int N_nodes,int first_node)
{
    int status = 0;
    for (int i = 0; i < N_nodes; ++i) {
        info_list[i].cheak = -1;
        for(int j = 0 ; j < N_nodes ; j++)
        {
            Graph[i][j].Analyse_Done = 0;
        }
    }
    DFS(info_list,Graph,N_nodes,&status,first_node);
    return status;
}

int main() {
    //////////////////////
    // Open Graphe file //
    //////////////////////

    FILE* Files;
    Files = fopen("Graphe.txt","r");
    if ( Files == NULL ) {
        printf("Cannot open file Graphe.txt\n");
        exit(0);
    }

    /// need the number of nodes
    int N_nodes;
    fscanf(Files,"%i",&N_nodes);
    /// create the Graph and infolist.
    T_Node ** Graph = create_my_graphe(N_nodes);
    T_Node * info_list = (T_Node*)calloc(N_nodes,sizeof(T_Node));
    int holder;
    /// give the name to the correct node.
    for(int i = 0 ; i < N_nodes ; i++)
    {
        fscanf( Files,"%i",&holder);
        info_list[i].Node = holder;
    }
    /// fill the Graph
    T_DChaines Openlist = iniChaine();
    int N_link; int a; int b; int weight;
    fscanf(Files,"%i",&N_link);
    for (int (i) = 0; (i) < N_link; ++(i))
    {
        fscanf(Files,"%i",&a);
        fscanf(Files,"%i",&b);
        fscanf(Files,"%i",&weight);
        a = find_correct_node(info_list,N_nodes,a);
        b = find_correct_node(info_list,N_nodes,b);
        T_Node* n = Create_Node(a,b,weight);
        Openlist = pushlist(Openlist,n);
    }
    fclose(Files);
    Sorter(Openlist);
    int Total_weight = 0;
    int status;


    int Node1 = Openlist.firstnode->Node;
    int first_Node = Node1;
    weight = Openlist.firstnode->weight;
    int Node2 = Openlist.firstnode->Node2;
    Openlist = delet_first_element(Openlist);
    Graph[Node1][Node2].Link = 1;
    Graph[Node2][Node1].Link = 1;
    Graph[Node1][Node2].weight = weight;
    Graph[Node2][Node1].weight = weight;
    Total_weight += weight;
    while(is_openlist_empty(Openlist) == 0)
    {
        Node1 = Openlist.firstnode->Node;
        weight = Openlist.firstnode->weight;
        Node2 = Openlist.firstnode->Node2;
        Openlist = delet_first_element(Openlist);
        Graph[Node1][Node2].Link = 1;
        Graph[Node2][Node1].Link = 1;
        Graph[Node1][Node2].weight = weight;
        Graph[Node2][Node1].weight = weight;
        status = Verif_DFS(info_list,Graph,N_nodes,first_Node);
        Total_weight += weight;
        if(status == 1)
        {
            Graph[Node1][Node2].Link = 0;
            Graph[Node2][Node1].Link = 0;
            Graph[Node1][Node2].weight = -1;
            Graph[Node2][Node1].weight = -1;
            Total_weight -= weight;
        }
        else if(status == 0)
        {
            info_list[Node1].next_node = Node2;
            info_list[Node2].prev_node = Node1;
        }
        else
        {
            puts("weird stat");
        }
    }

    printf("\nVoicie l'arbre cree avec le noeux precedent et suivant (si il existe) : \n\n");
    printf("template : [sommet precedent]---(poids)---[sommet]---(poids)---[sommet suivant]\n\n\n");
    for (int (i) = 0; (i) < N_nodes; ++(i)) {
        if(info_list[i].next_node != -1 && info_list[i].prev_node != -1)
        {
            printf("[%i]---(%i)---[%i]---(%i)---[%i]\n",info_list[i].prev_node,Graph[info_list[i].prev_node][i].weight,i,Graph[i][info_list[i].next_node].weight,info_list[i].next_node);
        }
        else if(info_list[i].next_node == -1 && info_list[i].prev_node != -1)
        {
            printf("[%i]---(%i)---[%i] No Next Node\n",info_list[i].prev_node,Graph[i][info_list[i].prev_node].weight,i);
        }
        else if(info_list[i].next_node != -1 && info_list[i].prev_node == -1)
        {
            printf("No prev Node [%i]---(%i)---[%i]\n",i,Graph[i][info_list[i].next_node].weight,info_list[i].next_node);
        }
        else
        {
            printf("Weird case\n");
        }
    }
    printf("\n\ntotale weight : %i\n\n",Total_weight);

    for (int i = 0; i < N_nodes; ++i) {
        if(info_list[i].prev_node != -1)
        {
            printf("arc : (%i)--(%i) weight : %i\n",info_list[i].prev_node,i,Graph[info_list[i].prev_node][i].weight);
        }
    }

    free_Graph(Graph,N_nodes);
    free(info_list);
}
