/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <cstddef>
#include <vector>
#include <queue>
#include <iostream>

using namespace std;

template <class T> class Edge;
template <class T> class Graph;
template <class T> class Vertex;


/****************** Provided structures  ********************/

/**
 * @brief Class that defines a Vertex of a Graph. Each vertex has contents, a list of outgoing edges, and auxiliary fields to calculate DFS and BFS.
 * @tparam T - Type of data to be stored in the graph vertex.
 */

template <class T>
class Vertex {
	T info;                // contents
	vector<Edge<T> > adj;  // list of outgoing edges
	bool visited;          // auxiliary field
    bool processing;       // auxiliary field
    int steps;             // auxiliary field
    vector<pair<Vertex<T>*, Edge<T>>> previous;    // auxiliary field

    void addEdge(Vertex<T> *dest, Airline w);
	bool removeEdgeTo(Vertex<T> *d);
public:
	Vertex(T in);
    T getInfo() const;
    void setInfo(T in);
    bool isVisited() const;
    void setVisited(bool v);
    bool isProcessing() const;
    void setProcessing(bool p);
    vector<pair<Vertex<T>*, Edge<T>>> getPrevious() const;
    void clearPrevious();
    void addPrevious(pair<Vertex<T>*, Edge<T>>);
    int getSteps() const;
    void setSteps(int s);

    const vector<Edge<T> > &getAdj() const;
    void setAdj(const vector<Edge<T> > &adj);
    friend class Graph<T>;
};

/**
 * @brief Class that defines an Edge of the Graph. Each edge contains a pointer to its destination and a weight.
 * In this project, the weight is an Airline, as each edge represents a flight.
 * @tparam T - Type of data to be stored in the graph.
 */
template <class T>
class Edge {
	Vertex<T> * dest;      // destination vertex
	Airline weight;         // edge weight
public:
	Edge(Vertex<T> *d, Airline w);
    Vertex<T> *getDest() const;
    void setDest(Vertex<T> *dest);
    Airline getWeight() const;
    void setWeight(Airline weight);
    friend class Graph<T>;
	friend class Vertex<T>;
};

/**
 * @brief Class that defines a Graph. Each graph has a vector of pointers to vertices.
 * @tparam T - Type of data stored in the Graph. For this project, T is Airport.
 */
template <class T>
class Graph {
	vector<Vertex<T> *> vertexSet;    // vertex set
    void dfsVisit(Vertex<T> *v,  vector<T> & res) const;
    bool dfsIsDAG(Vertex<T> *v) const;
public:
    Vertex<T> *findVertex(const T &in) const;
    int getNumVertex() const;
	Vertex<T>* addVertex(const T &in);
	bool removeVertex(const T &in);
	bool addEdge(Vertex<T> *sourc, Vertex<T> *dest, Airline w);
	bool removeEdge(const T &sourc, const T &dest);
    vector<Vertex<T> * > getVertexSet() const;
	vector<T> dfs() const;
	vector<T> dfs(const T & source) const;
	vector<T> bfs(const T &source) const;
};

/****************** Provided constructors and functions ********************/

/// Default constructor
template <class T>
Vertex<T>::Vertex(T in): info(in) {}

/// All parameter constructor
template <class T>
Edge<T>::Edge(Vertex<T> *d, Airline w): dest(d), weight(w) {}


template <class T>
int Graph<T>::getNumVertex() const {
	return vertexSet.size();
}

template <class T>
vector<Vertex<T> * > Graph<T>::getVertexSet() const {
    return vertexSet;
}

template<class T>
T Vertex<T>::getInfo() const {
    return info;
}

template<class T>
void Vertex<T>::setInfo(T in) {
    Vertex::info = in;
}

template<class T>
bool Vertex<T>::isProcessing() const {
    return processing;
}

template<class T>
void Vertex<T>::setProcessing(bool p) {
    Vertex::processing = p;
}

template<class T>
Vertex<T> *Edge<T>::getDest() const {
    return dest;
}

template<class T>
void Edge<T>::setDest(Vertex<T> *d) {
    Edge::dest = d;
}

template<class T>
Airline Edge<T>::getWeight() const {
    return weight;
}

template<class T>
void Edge<T>::setWeight(Airline weight) {
    Edge::weight = weight;
}

/**
 * @brief Auxiliary function to find a vertex with a given content.
 */
template <class T>
Vertex<T> * Graph<T>::findVertex(const T &in) const {
	for (auto v : vertexSet)
		if (v->info == in)
			return v;
	return NULL;
}

template <class T>
bool Vertex<T>::isVisited() const {
    return visited;
}

template <class T>
void Vertex<T>::setVisited(bool v) {
    Vertex::visited = v;
}

template<class T>
const vector<Edge<T> > &Vertex<T>::getAdj() const {
    return adj;
}

template <class T>
void Vertex<T>::setAdj(const vector<Edge<T> > &adj) {
    Vertex::adj = adj;
}

template<class T>
void Vertex<T>::setSteps(int s) {
    steps = s;
}

template<class T>
int Vertex<T>::getSteps() const {
    return steps;
}

template<class T>
void Vertex<T>::addPrevious(pair<Vertex<T>*, Edge<T>> prev) {
    previous.push_back(prev);
}

template<class T>
vector<pair<Vertex<T>*, Edge<T>>> Vertex<T>::getPrevious() const {
    return previous;
}

template<class T>
void Vertex<T>::clearPrevious() {
    previous.clear();
}

/**
 *  @brief Adds a vertex with a given content or info (in) to a graph (this).
 *  @return Returns true if successful, and false if a vertex with that content already exists.
 */
template <class T>
Vertex<T>* Graph<T>::addVertex(const T &in) {
    Vertex<T> * vx = new Vertex<T>(in);
	vertexSet.push_back(vx);
	return vx;
}


/**
 * @brief Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * @return Returns true if successful, and false if the source or destination vertex does not exist.
 */
template <class T>
bool Graph<T>::addEdge(Vertex<T> *sourc, Vertex<T> *dest, Airline w) {
	sourc->addEdge(dest,w);
	return true;
}

/**
 * @brief Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T>
void Vertex<T>::addEdge(Vertex<T> *d, Airline w) {
	adj.push_back(Edge<T>(d, w));
}


/**
 * @brief Removes an edge from a graph (this).
 * The edge is identified by the source (sourc) and destination (dest) contents.
 * @return Returns true if successful, and false if such edge does not exist.
 */
template <class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest) {
	auto v1 = findVertex(sourc);
	auto v2 = findVertex(dest);
	if (v1 == NULL || v2 == NULL)
		return false;
	return v1->removeEdgeTo(v2);
}

/**
 * @brief Auxiliary function to remove an outgoing edge (with a given destination (d))
 * from a vertex (this).
 * @return Returns true if successful, and false if such edge does not exist.
 */
template <class T>
bool Vertex<T>::removeEdgeTo(Vertex<T> *d) {
	for (auto it = adj.begin(); it != adj.end(); it++)
		if (it->dest  == d) {
			adj.erase(it);
			return true;
		}
	return false;
}

/**
 *  @brief Removes a vertex with a given content (in) from a graph (this), and
 *  all outgoing and incoming edges.
 *  @return Returns true if successful, and false if such vertex does not exist.
 */
template <class T>
bool Graph<T>::removeVertex(const T &in) {
	for (auto it = vertexSet.begin(); it != vertexSet.end(); it++)
		if ((*it)->info  == in) {
			auto v = *it;
			vertexSet.erase(it);
			for (auto u : vertexSet)
				u->removeEdgeTo(v);
			delete v;
			return true;
		}
	return false;
}

/**
 * @brief Conducts a depth-first search of the graph.
 * @return Vector of the contents of the vertices of the graph traversed by the DFS.
 */
template <class T>
vector<T> Graph<T>::dfs() const {
	vector<T> res;
	for (auto v : vertexSet)
		v->visited = false;
	for (auto v : vertexSet)
	    if (! v->visited)
	    	dfsVisit(v, res);
	return res;
}

/**
 * @brief Auxiliary function for the DFS.
 * @param v - Source vertex of the search.
 * @param res - Vector to insert the contents of traversed vertices.
 */
template <class T>
void Graph<T>::dfsVisit(Vertex<T> *v, vector<T> & res) const {
    v->visited = true;
    res.push_back(v->info);
    for (auto & e : v->adj) {
        auto w = e.dest;
        if ( ! w->visited)
            dfsVisit(w, res);
    }
}

/**
 * @brief Conducts a depth-first search of the graph from a source vertex.
 * @return Vector of the contents of the vertices of the graph traversed by the DFS.
 */
template <class T>
vector<T> Graph<T>::dfs(const T & source) const {
    vector<T> res;
    auto s = findVertex(source);
    if (s == nullptr)
        return res;

    for (auto v : vertexSet)
        v->visited = false;

    dfsVisit(s, res);
    return res;
}

/**
 * @brief Conducts a breadth-first search of the graph from a source vertex.
 * @return Vector of the contents of the vertices of the graph traversed by the BFS.
 */
template <class T>
vector<T> Graph<T>::bfs(const T & source) const {
	vector<T> res;
	auto s = findVertex(source);
	if (s == NULL)
		return res;
	queue<Vertex<T> *> q;
	for (auto v : vertexSet)
		v->visited = false;
	q.push(s);
	s->visited = true;
	while (!q.empty()) {
		auto v = q.front();
		q.pop();
		res.push_back(v->info);
		for (auto & e : v->adj) {
			auto w = e.dest;
		    if ( ! w->visited ) {
				q.push(w);
				w->visited = true;
		    }
		}
	}
	return res;
}

#endif /* GRAPH_H_ */
