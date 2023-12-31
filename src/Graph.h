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

// Auxiliar function declaration (used in the articulation points algorithm)
template <typename T>
bool stackSearch(stack<T> s, T i);


/****************** Provided structures  ********************/

template <class T>
class Vertex {
	T info;                // contents
	vector<Edge<T> > adj;  // list of outgoing edges
	bool visited;          // auxiliary field
    bool processing;       // auxiliary field

    vector<pair<string,string>> path;    // auxiliary field

    int lowest = 0;
    int visitIndex = 0;
    int auxiliar = 0;

    void addEdge(Vertex<T> *dest, double w, string code);
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
    vector<Edge<T> > &getAdj();
    vector<pair<string,string>> getPath() const;
    void clearPath();
    void addPath(pair<string, string>);
    void setPath(vector<pair<string,string>>);

    unordered_set<string> parents;

    const vector<Edge<T> > &getAdj() const;
    void setAdj(const vector<Edge<T> > &adj);
    friend class Graph<T>;
};

template <class T>
class Edge {
	Vertex<T> * dest;      // destination vertex
	double weight;         // edge weight
    string info;
public:
	Edge(Vertex<T> *d, double w, string code);
    Vertex<T> *getDest() const;
    void setDest(Vertex<T> *dest);
    double getWeight() const;
    void setWeight(double weight);
    string getInfo() const;
    void setInfo(string code);
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
	bool addEdge(Vertex<T> *sourc, Vertex<T> *dest, double w, string code);
	bool removeEdge(const T &sourc, const T &dest);
    vector<Vertex<T> * > getVertexSet() const;
	vector<T> dfs() const;
	vector<T> dfs(const T & source) const;
	vector<T> bfs(const T &source) const;

    int diameter();
    vector<pair<Vertex<T>*, vector<Vertex<T> *>>> longestPaths(int &diameter);
    int bfs_diameter(Vertex<T> *v);

    vector<T> articulationPoints();
    void dfs_art(Vertex<T> *v, stack<T> &s, vector<T> &l, int &i);


};

/****************** Provided constructors and functions ********************/

template <class T>
Vertex<T>::Vertex(T in): info(in) {}

template <class T>
Edge<T>::Edge(Vertex<T> *d, double w, string code): dest(d), weight(w), info(code) {}


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
double Edge<T>::getWeight() const {
    return weight;
}

template<class T>
void Edge<T>::setWeight(double weight) {
    Edge::weight = weight;
}

template <class T>
string Edge<T>::getInfo() const {
    return info;
}
template <class T>
void Edge<T>::setInfo(string code) {
    Edge::info = code;
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
vector<Edge<T> > &Vertex<T>::getAdj() {
    return adj;
}

template <class T>
void Vertex<T>::setAdj(const vector<Edge<T> > &adj) {
    Vertex::adj = adj;
}

template<class T>
void Vertex<T>::addPath(pair<string, string> prev) {
    path.push_back(prev);
}

template <class T>
void Vertex<T>::setPath(vector<pair<string,string>> path_){
    Vertex<T>::path = path_;
}

template<class T>
vector<pair<string, string>> Vertex<T>::getPath() const {
    return path;
}

template<class T>
void Vertex<T>::clearPath() {
    path.clear();
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
bool Graph<T>::addEdge(Vertex<T> *sourc, Vertex<T> *dest, double w, string code) {
	sourc->addEdge(dest,w,code);
	return true;
}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T>
void Vertex<T>::addEdge(Vertex<T> *d, double w, string code) {
	adj.push_back(Edge<T>(d, w, code));
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

// DIAMETER BFS
template <class T>
int Graph<T>::diameter(){
    int diameter = 0;
    for (Vertex<T> *w : vertexSet){
        w->setVisited(false);
    }
    for (Vertex<T> *v : vertexSet){
        if (!v->isVisited()){
            v->setVisited(true);
            int result = bfs_diameter(v);
            diameter = max(result, diameter);
        }
    }
    return diameter;
}

template <class T>
int Graph<T>::bfs_diameter(Vertex<T> *v) {
    for (Vertex<T> *w : vertexSet){
        w->setVisited(false);
        w->setVisitIndex(-1);
    }
    int diameter = 0;
    queue<Vertex<T> *> remaining;
    v->setVisited(true);
    v->setVisitIndex(0);
    remaining.push(v);

    while (!remaining.empty()){
        Vertex<T> *current = remaining.front();
        remaining.pop();

        for (const Edge<T> &e : current->getAdj()){
            Vertex<T> *next = e.getDest();
            if (!next->isVisited()){
                remaining.push(next);
                next->setVisited(true);
                next->setVisitIndex(current->getVisitIndex()+1);
                if (next->getVisitIndex() > diameter){
                    diameter = next->getVisitIndex();
                }
            }
        }

    }
    return diameter;
}

// LONGEST PAIRS
template <class T>
vector<pair<Vertex<T>*, vector<Vertex<T> *>>> Graph<T>::longestPaths(int &diameter){
    diameter = 0;
    for (Vertex<T> *w : vertexSet){
        w->setVisited(false);
    }
    vector<pair<Vertex<T>*, vector<Vertex<T> *>>> longest;
    for (Vertex<T> *v : vertexSet){
        int result = bfs_diameter(v);
        if (result > diameter){
            diameter = result;
            longest.clear();
        }
        if (result == diameter){
            vector<Vertex<T> *> ends;
            for (Vertex<T> *w : vertexSet){
                if (w->getVisitIndex() == diameter){
                    ends.push_back(w);
                }
            }
            longest.push_back(make_pair(v, ends));
        }
    }
    return longest;
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
