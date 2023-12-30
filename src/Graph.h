/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <cstddef>
#include <vector>
#include <queue>
#include <iostream>
#include <unordered_set>
#include <stack>

using namespace std;

template <class T> class Edge;
template <class T> class Graph;
template <class T> class Vertex;

template <typename T>
bool stackSearch(stack<T> s, T i);


/****************** Provided structures  ********************/

template <class T>
class Vertex {
	T info;                // contents
	vector<Edge<T> > adj;  // list of outgoing edges
	bool visited;          // auxiliary field
    bool processing;       // auxiliary field

    int lowest = 0;
    int visitIndex = 0;
    int auxiliar = 0;

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
    int getLowest() const;
    void setLowest(int low);
    int getVisitIndex() const;
    void setVisitIndex(int visIdx);
    int getAuxiliar() const;
    void setAuxiliar(int aux);
    const vector<Edge<T> > &getAdj() const;
    void setAdj(const vector<Edge<T> > &adj);
    friend class Graph<T>;
};

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

    vector<T> articulationPoints();
    void dfs_art(Vertex<T> *v, stack<T> &s, vector<T> &l, int &i);


};

/****************** Provided constructors and functions ********************/

template <class T>
Vertex<T>::Vertex(T in): info(in) {}

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

template <class T>
int Vertex<T>::getLowest() const{
    return lowest;
}
template <class T>
void Vertex<T>::setLowest(int low){
    Vertex::lowest = low;
}
template <class T>
int Vertex<T>::getVisitIndex() const{
    return visitIndex;
}
template <class T>
void Vertex<T>::setVisitIndex(int visIdx){
    Vertex::visitIndex = visIdx;
}
template <class T>
int Vertex<T>::getAuxiliar() const {
    return auxiliar;
}
template <class T>
void Vertex<T>::setAuxiliar(int aux) {
    Vertex::auxiliar = aux;
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

/*
 * Auxiliary function to find a vertex with a given content.
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


/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template <class T>
Vertex<T>* Graph<T>::addVertex(const T &in) {
    Vertex<T> * vx = new Vertex<T>(in);
	vertexSet.push_back(vx);
	return vx;
}


/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template <class T>
bool Graph<T>::addEdge(Vertex<T> *sourc, Vertex<T> *dest, Airline w) {
	sourc->addEdge(dest,w);
	return true;
}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T>
void Vertex<T>::addEdge(Vertex<T> *d, Airline w) {
	adj.push_back(Edge<T>(d, w));
}


/*
 * Removes an edge from a graph (this).
 * The edge is identified by the source (sourc) and destination (dest) contents.
 * Returns true if successful, and false if such edge does not exist.
 */
template <class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest) {
	auto v1 = findVertex(sourc);
	auto v2 = findVertex(dest);
	if (v1 == NULL || v2 == NULL)
		return false;
	return v1->removeEdgeTo(v2);
}

/*
 * Auxiliary function to remove an outgoing edge (with a given destination (d))
 * from a vertex (this).
 * Returns true if successful, and false if such edge does not exist.
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

/*
 *  Removes a vertex with a given content (in) from a graph (this), and
 *  all outgoing and incoming edges.
 *  Returns true if successful, and false if such vertex does not exist.
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

// ARTICULATION POINTS
template <class T>
vector<T> Graph<T>::articulationPoints() {
    vector<T> res;

    for (auto vx : this->getVertexSet()){
        vx->setLowest(0);
        vx->setVisitIndex(0);
        vx->setVisited(false);
    }

    stack<T> s;
    int idx = 1;
    for (auto v : this->getVertexSet()){
        if (!v->isVisited()){
            this->dfs_art(v, s, res, idx);
        }
    }

    if (this->getVertexSet().front()->getAuxiliar() > 2){
        res.push_back(this->getVertexSet().front()->getInfo());
    }
    else {
        res.pop_back();
    }

    return res;
}

template <class T>
void Graph<T>::dfs_art(Vertex<T> *v, stack<T> &s, vector<T> &l, int &i){
    v->setLowest(i);
    v->setVisitIndex(i);
    v->setAuxiliar(0);
    v->setVisited(true);
    s.push(v->getInfo());
    i++;

    for (auto e : v->getAdj()){
        Vertex<T> *w = e.getDest();
        if (!w->isVisited()){
            v->setAuxiliar(v->getAuxiliar()+1);
            dfs_art(w, s, l, i);
            v->setLowest(min(v->getLowest(), w->getLowest()));
            if (w->getLowest() >= v->getVisitIndex()){
                l.push_back(v->getInfo());
            }
        }
        else if (stackSearch(s, w->getInfo())){
            v->setLowest(min(v->getLowest(), w->getVisitIndex()));
        }
    }
    s.pop();
}

template <typename T>
bool stackSearch(stack<T> s, const T i){
    while (!s.empty()){
        T x = s.top();
        s.pop();
        if (x == i){
            return true;
        }
    }
    return false;
}

#endif /* GRAPH_H_ */
