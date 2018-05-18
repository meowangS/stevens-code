#include "myamgraph.hh"

/*
 * See comments within main() to explain what the tests do
 * If there are no comments, it is just a basic function call
 */
int main(int argc, char **argv)
{
    /**            **/
    /** Undirected **/
    /**            **/

    int i, j;
    MyAmGraph<string> *G = new MyAmGraph<string>(5, false);

    cout << "----UNDIRECTED----" << endl;

    /* Vertices Test */
    G->add_vertex("A");
    G->add_vertex("B");
    G->add_vertex("C");
    G->add_vertex("D");
    G->add_vertex("E");
    G->add_vertex("F"); //Beyond cap, will not add

    cout << "Vertices: ";
    G->show_vertices(); //Will not print F
    cout << "------------------" << endl;

    G->show_am(); //Prints out the adjacency matrix from 0 to v
    cout << "------------------" << endl;

    /* Edges Test */
    G->add_edge(0, 1, 10);
    G->add_edge(0, 2, 15);
    G->add_edge(0, 3, 8);
    G->add_edge(1, 3, 20);
    G->add_edge(2, 2, 5);  //Self linking
    G->add_edge(2, 3, 10);
    G->add_edge(2, 4, 16);
    G->add_edge(2, 15, 6); //j is beyond v
    G->add_edge(2, 27, 8); //j is beyond cap
    G->add_edge(3, 2, 15); //i > j, function will swap. 15 will overwrite 10
    G->add_edge(3, 4, 8);
    G->add_edge(5, 1, 3);
    G->add_edge(7, 7, 2);  //Self linking, greater than v & cap
    G->add_edge(12, 2, 8); //i is beyond cap
    G->add_edge(18, 3, 6); //i is beyond v;

    G->show_am();
    cout << "------------------" << endl;

    cout << "Edge weights: ";
    G->show_edges(); //Parses am[][] and prints out the weights
    cout << "------------------" << endl << "BFS: ";

    /* Search Tests */
    G->traverse(false);
    cout << "------------------" << endl << "DFS: ";

    G->traverse(true);
    cout << "------------------" << endl;

    /* Neighbors Test */
    for(i = 0; i < G->V(); i++)
    {
        cout << "Neighbors of " << i << ": ";

        for(j = G->firstNeighbor(i); j >= 0; j = G->nextNeighbor(i, j))
            cout << j << " ";

        cout << endl;
    }

    /* Kruscal */
    cout << "Kruskal: " << endl;
    MyGraph<string> *MST = G->kruskal();
    if(MST != NULL)
    {
        MST->show_edges();
        cout << "Edge count: " << MST->E() << endl;
    } else
        cout << "Result is null! Invalid input" << endl;

    delete MST;

    /* Prims */
    cout << "Prims: " << endl;
    MST = G->prims();
    if(MST != NULL)
    {
        MST->show_edges();
        cout << "Edge count: " << MST->E() << endl;
    }
    else
        cout << "Result is null! Invalid input" << endl;

    /* getMST */
    cout << "getMST: " << endl;
    MST = G->getMST();
    if(MST != NULL)
    {
        MST->show_edges();
        cout << "Edge count: " << MST->E() << endl;
    }
    else
        cout << "Result is null! Invalid input" << endl;


    /* Dijkstra with proper value as parameter*/
    cout << "Dijkstra: " << endl;
    int * res = G->dijkstra(0);
    if(res != NULL)
    {
        for(i = 0; i < G->V(); i++)
        {
            if(res[i] != 2147483647) //Can't access INF, protected data member
                cout << res[i] << " ";
            else
                cout << "INF ";
        }
        cout << endl;
    } else
        cout << "Result is null! Invalid input" << endl;

    /* Dijkstra with improper value as parameter*/
    cout << "Dijkstra: " << endl;
    res = G->dijkstra(G->V());
    if(res != NULL)
    {
        for(i = 0; i < G->V(); i++)
        {
            if(res[i] != 2147483647)
                cout << res[i] << " ";
            else
                cout << "INF ";
        }
        cout << endl;
    } else
        cout << "Result is null! Invalid input" << endl;

    /* Dijkstra with NULL (0) as parameter*/
    cout << "Dijkstra: " << endl;
    res = G->dijkstra((unsigned int) NULL);
    if(res != NULL)
    {
        for(i = 0; i < G->V(); i++)
        {
            if(res[i] != 2147483647)
                cout << res[i] << " ";
            else
                cout << "INF ";
        }
        cout << endl;
    } else
        cout << "Result is null! Invalid input" << endl;

    /**          **/
    /** Directed **/
    /**          **/

    cout << endl << "----DIRECTED----" << endl;

    G = new MyAmGraph<string>(5, true);

    /* Vertices Test */
    G->add_vertex("A");
    G->add_vertex("B");
    G->add_vertex("C");
    G->add_vertex("D");
    G->add_vertex("E");
    G->add_vertex("F"); //Beyond cap, will not add

    cout << "Vertices: ";
    G->show_vertices(); //Will not print F
    cout << "------------------" << endl;

    G->show_am(); //Prints out the adjacency matrix from 0 to v
    cout << "------------------" << endl;

    /* Edges Test */
    G->add_edge(0, 1, 10);
    G->add_edge(0, 2, 15);
    G->add_edge(0, 3, 8);
    G->add_edge(1, 3, 20);
    G->add_edge(1, 0, 4);
    G->add_edge(2, 0, 1);
    G->add_edge(2, 2, 5);  //Self linking
    G->add_edge(2, 3, 10);
    G->add_edge(2, 4, 16);
    G->add_edge(2, 15, 6); //j is beyond v
    G->add_edge(2, 27, 8); //j is eyond cap
    G->add_edge(3, 0, 4);
    G->add_edge(3, 1, 8);  //3 and 1 point to each other
    G->add_edge(3, 2, 15); //i > j, function will *not* swap for digraph
    G->add_edge(3, 4, 8);
    G->add_edge(4, 2, 5);
    G->add_edge(4, 3, 9);
    G->add_edge(7, 7, 2);  //Self linking, greater than v & cap
    G->add_edge(12, 2, 8); //i is beyond cap
    G->add_edge(18, 3, 6); //i is beyond v

    G->show_am();
    cout << "------------------" << endl;

    cout << "Edge weights: ";
    G->show_edges(); //Parses am[][] and prints out the weights
    cout << "------------------" << endl << "BFS: ";

    /* Search Tests */
    G->traverse(false);
    cout << "------------------" << endl << "DFS: ";

    G->traverse(true);
    cout << "------------------" << endl;

    /* Neighbors Test */
    for(i = 0; i < G->V(); i++)
    {
        cout << "Neighbors of " << i << ": ";

        for(j = G->firstNeighbor(i); j >= 0; j = G->nextNeighbor(i, j))
            cout << j << " ";

        cout << endl;
    }

    /* Kruscal with digraph (shouldn't run) */
    cout << "Kruskal: " << endl;
    MST = G->kruskal();
    if(MST != NULL)
    {
        MST->show_edges();
    } else
        cout << "Result is null! Invalid input" << endl;

    /* Prims */
    cout << "Prims: " << endl;
    MST = G->prims();
    if(MST != NULL)
    {
        MST->show_edges();
    } else
        cout << "Result is null! Invalid input" << endl;

    /* getMST */
    cout << "getMST: " << endl;
    MST = G->getMST();
    if(MST != NULL)
    {
        MST->show_edges();
        cout << "Edge count: " << MST->E() << endl;
    }
    else
        cout << "Result is null! Invalid input" << endl;

    /* Dijkstra with proper value as parameter*/
    cout << "Dijkstra: " << endl;
    res = G->dijkstra(0);
    if(res != NULL)
    {
        for(i = 0; i < G->V(); i++)
        {
            if(res[i] != 2147483647)
                cout << res[i] << " ";
            else
                cout << "INF ";
        }
        cout << endl;
    } else
        cout << "Result is null! Invalid input" << endl;

    /* Dijkstra with improper value as parameter*/
    cout << "Dijkstra: " << endl;
    res = G->dijkstra(G->V());
    if(res != NULL)
    {
        for(i = 0; i < G->V(); i++)
        {
            if(res[i] != 2147483647)
                cout << res[i] << " ";
            else
                cout << "INF ";
        }
        cout << endl;
    } else
        cout << "Result is null! Invalid input" << endl;

    /* Dijkstra with NULL (0) as parameter*/
    cout << "Dijkstra: " << endl;
    res = G->dijkstra((unsigned int) NULL);
    if(res != NULL)
    {
        for(i = 0; i < G->V(); i++)
        {
            if(res[i] != 2147483647)
                cout << res[i] << " "; //Should return same result as above (not null)
            else
                cout << "INF ";
        }
        cout << endl;
    } else
        cout << "Result is null! Invalid input" << endl;


    //Clean up variables
    delete    G;
    delete [] res;
    delete    MST;

    /**                        **/
    /**       Max Flow         **/
    /**                        **/

    //Test with normal digraph
    MyAmGraph<string> *maxFlow = new MyAmGraph<string>(4, true);

    maxFlow->add_vertex("A");
    maxFlow->add_vertex("B");
    maxFlow->add_vertex("C");
    maxFlow->add_vertex("D");

    maxFlow->add_edge(0, 1, 3);
    maxFlow->add_edge(0, 2, 5);
    maxFlow->add_edge(0, 3, 4);
    maxFlow->add_edge(1, 2, 6);
    maxFlow->add_edge(1, 3, 7);
    maxFlow->add_edge(2, 3, 8);

    cout << endl;

    maxFlow->show_am();

    cout << endl << "Max flow network: " << endl;
    maxFlow->showMaxFlow(maxFlow->fordFulkerson(0, 3));

    //Test with normal digraph, limits out of bounds
    maxFlow = new MyAmGraph<string>(4, true);

    maxFlow->add_vertex("A");
    maxFlow->add_vertex("B");
    maxFlow->add_vertex("C");
    maxFlow->add_vertex("D");

    maxFlow->add_edge(0, 1, 3);
    maxFlow->add_edge(0, 2, 5);
    maxFlow->add_edge(0, 3, 4);
    maxFlow->add_edge(1, 2, 6);
    maxFlow->add_edge(1, 3, 7);
    maxFlow->add_edge(2, 3, 8);

    cout << endl;

    maxFlow->show_am();

    cout << endl << "Max flow network: " << endl;
    if(maxFlow->fordFulkerson(-4, 5) == NULL)
        cout << "Invalid input! Result is null!" << endl; //Should print this
    else
        cout << "Something went wrong.." << endl; //Should never make it here


    //Test with digraph with edges going both ways
    maxFlow = new MyAmGraph<string>(4, true);

    maxFlow->add_vertex("A");
    maxFlow->add_vertex("B");
    maxFlow->add_vertex("C");
    maxFlow->add_vertex("D");

    maxFlow->add_edge(0, 1, 13);
    maxFlow->add_edge(0, 2, 5);
    maxFlow->add_edge(0, 3, 4);
    maxFlow->add_edge(1, 2, 6);
    maxFlow->add_edge(1, 3, 7);
    maxFlow->add_edge(2, 3, 8);
    maxFlow->add_edge(2, 1, 4);

    cout << endl;

    maxFlow->show_am();

    maxFlow->showMaxFlow(maxFlow->fordFulkerson(0, 3));

    delete maxFlow;

    //maxFlow->show_edges();
    //max->show_edges();

    /**                        **/
    /**         Floyd          **/
    /**                        **/
    //Only tested once, not much to test for
    MyAmGraph<string> *floydTest = new MyAmGraph<string>(4, true);

    floydTest->add_vertex("A");
    floydTest->add_vertex("B");
    floydTest->add_vertex("C");
    floydTest->add_vertex("D");

    floydTest->add_edge(0, 1, 3);
    floydTest->add_edge(0, 2, 5);
    floydTest->add_edge(0, 3, 4);
    floydTest->add_edge(1, 2, 6);
    floydTest->add_edge(1, 3, 7);
    floydTest->add_edge(2, 3, 8);

    cout << endl << "Floyd: " << endl;
    int** floudOut = floydTest->floyd();

    for(int i = 0; i < floydTest->V(); i++)
    {
        for(int j = 0; j < floydTest->V(); j++)
        {
            if(floudOut[i][j] != 2147483647)
                cout << floudOut[i][j];
            else
                cout << "INF";

            cout << "\t";
        }
        cout << endl;
    }

    delete floydTest; /**/

    return 0;
}
