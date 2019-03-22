
# coding: utf-8

# In[1]:


# coding: utf-8

# # CS579: Assignment 1
#
# In this assignment, we'll implement community detection and link prediction algorithms using Facebook "like" data.
#
# The file `edges.txt.gz` indicates like relationships between facebook users. This was collected using snowball sampling: beginning with the user "Bill Gates", I crawled all the people he "likes", then, for each newly discovered user, I crawled all the people they liked.
#
# We'll cluster the resulting graph into communities, as well as recommend friends for Bill Gates.
#
# Complete the **15** methods below that are indicated by `TODO`. I've provided some sample output to help guide your implementation.


# You should not use any imports not listed here:
from collections import Counter, defaultdict, deque
import copy
from itertools import combinations
import math
import networkx as nx
import urllib.request


## Community Detection

def example_graph():
    """
    Create the example graph from class. Used for testing.
    Do not modify.
    """
    g = nx.Graph()
    g.add_edges_from([('A', 'B'), ('A', 'C'), ('B', 'C'), ('B', 'D'), ('D', 'E'), ('D', 'F'), ('D', 'G'), ('E', 'F'), ('G', 'F')])
    return g

def bfs(graph, root, max_depth):
    unvisited = deque()
    d = deque()
    d.append(root)
    nodedist = defaultdict(int)
    nodedist[root] = 0
    nodepath = defaultdict(lambda:0)
    nodepath[root] = 1
    nodeparent = defaultdict(lambda:0)
    do = 0
    while(len(d)>0):
        present = d.popleft()
        if(nodedist[present]<max_depth):
            for n in graph.neighbors(present):
                if(n not in unvisited and n!=root):
                    nodedist[n] = nodedist[present]+1
                    nodepath[n] = nodepath[n]+1
                    nodeparent[n] = [present]
                    unvisited.append(n)
                    d.append(n)
                if(n!=root and nodedist[n]>=nodedist[present]+1):
                    nodepath[n] = nodepath[n] + 1
                    nodeparent[n].append(present)
    return nodedist,nodepath,nodeparent
    #if
    #nodepath[i]= 
    pass


def complexity_of_bfs(V, E, K):
    return V+E
    pass


def bottom_up(root, node2distances, node2num_paths, node2parents):
    
    ###TODO
    maxdist = max(node2distances.values())
    nodes = [i for i,j in node2distances.items() if(j==maxdist)]
    weight = {i:1 for i in node2distances.keys()}
    e=[]
    best = sorted(node2distances, key=node2distances.get,reverse=True)
    for i in best:
        if i!=root:
            for p in node2parents[i]:
                weight[p] = weight[p] + (weight[i]/len(node2parents[i]))
                if i<=p:
                    e.append((i,p))
                else:
                    e.append((p,i))
    
    result = defaultdict(lambda:0)
    for i in e:
        if(root in i):
            rindex = i.index(root)
            if(rindex<=0):
                result[i] = (weight[i[1]]/len(node2parents[i[1]]))
            else:
                result[i] = (weight[i[0]]/len(node2parents[i[0]]))
        elif i[0] not in node2parents[i[1]]:
            result[i] = (weight[i[0]]/len(node2parents[i[0]]))
        else:
            result[i] = (weight[i[1]]/len(node2parents[i[1]]))
    
    return result   
    pass


def approximate_betweenness(graph, max_depth):
    
    ###TODO
    dict_app = defaultdict(lambda:0)
    for i in graph.nodes():
        nodedist,nodepath,nodeparent = bfs(graph,i,max_depth)
        bup = bottom_up(i,nodedist,nodepath,nodeparent)
        for j in list(bup.keys()):
            dict_app[j] = dict_app[j] + bup[j]
    for k in dict_app:
        dict_app[k] = (dict_app[k]/2)
    return dict_app
    pass


def get_components(graph):
    """
    A helper function you may use below.
    Returns the list of all connected components in the given graph.
    """
    return [c for c in nx.connected_component_subgraphs(graph)]

def partition_girvan_newman(graph, max_depth):
    
    ###TODO
    newg = graph.copy()
    comp = get_components(graph)
    edges = approximate_betweenness(newg, max_depth)
    if(graph.order == 1):
        return [graph.nodes()]
    sortedg = sorted(edges.items(),key = lambda x:(-x[1],x[0][0],x[0][1]))
    for i in sortedg:
        if(len(comp)==1):
            newg.remove_edge(*(i[0]))
            comp = get_components(newg)
    return comp
    pass

def get_subgraph(graph, min_degree):
    
    ###TODO
    newg = []
    for i in graph.nodes():
        if(graph.degree(i)>=min_degree):
            newg.append(i)
    return graph.subgraph(newg)
    pass


""""
Compute the normalized cut for each discovered cluster.
I've broken this down into the three next methods.
"""

def volume(nodes, graph):
    
    ###TODO
    c=0
    for i in graph.edges():
        if(i[0] in nodes or i[1] in nodes):
            c=c+1
    return c
    pass


def cut(S, T, graph):
   
    ###TODO
    cutcount=0
    for i in graph.edges():
        if((i[0] in S and i[1] in T) or (i[0] in T and i[1] in S)):
            cutcount = cutcount + 1
    return cutcount
    pass


def norm_cut(S, T, graph):
   
    ###TODO
    val1 = float(cut(S,T,graph)/volume(S,graph))
    val2 = float(cut(S,T,graph)/volume(T,graph))
    normal = val1+val2
    return float(normal)
    pass


def brute_force_norm_cut(graph, max_size):
    
        ###TODO
    #for i in graph.edges():
    #    c=0
    #    while(c<max_size):
    #        if(grapgh.has_edge(i[0],i[1])):
    #            S=graph.neighbors(i[0])
    #            T=graph.neighbors(i[1])
    #            c++;
    
    #edgelist = []
    #out_list = []
    #comp=[]
    #for i in graph.nodes():
    #    edgelist[i]=graph.neighbors(i)
    #out_list=[c for i in range(len(edgelist)) for c in itertools.combinations(edgelist,i+1)]
    #g = nx.Graph()
    #g.add_edges_from(out_list)
    #comp = get_components(g)
    #if(len(comp)<max_size):
    
    u = set(graph.nodes())
    s = len(graph.nodes())
    r = []
    for i in range(1,s//2 +1):
        #subset = set(intertools.combinations(u,i))
        #for i in graph.nodes():
        #edgelist[i]=graph.neighbors(i)
        #out_list=[c for i in range(len(edgelist)) for c in itertools.combinations(edgelist,i+1)]
        subset = set(combinations(u,i))
        for i in subset:
            i = set(i)
            j = u - i
            val = norm_cut(i, j, graph)
            temp = (val,[])
            if(len(temp[1]) <= max_size):   
                for v in i:
                    for w in j:
                        if graph.has_edge(v,w):                            
                            t = (v,w)
                            t = tuple(sorted(t))
                            temp[1].append(t)
                            r.append(temp)
    return r
    pass

def score_max_depths(graph, max_depths):
   
    ###TODO
    score = []
    for i in max_depths:
        comp = partition_girvan_newman(graph, i)
        score.append((i,norm_cut(comp[0].nodes(),comp[1].nodes(),graph)))
    return score
    pass


## Link prediction

# Next, we'll consider the link prediction problem. In particular,
# we will remove 5 of the accounts that Bill Gates likes and
# compute our accuracy at recovering those links.

def make_training_graph(graph, test_node, n):
    
    ###TODO
    newg = graph.copy()
    sortedg=sorted(newg.neighbors(test_node))[:n]
    for i in sortedg:
        newg.remove_edge(test_node,i)
    return newg
    
    pass



def jaccard(graph, node, k):
    
    ###TODO
    score=[]
    newg = graph.copy()
    partner = set(newg.neighbors(node))
    for i in newg.nodes():
        if(i not in newg.neighbors(node) and i!=node):
            n = set(newg.neighbors(i))
            jacard = float(len(partner & n))/(len(partner|n))
            score.append(((node,i),jacard))
    sortedg = sorted(score, key=lambda x: (-x[1],x[0]))[:k]
    #(('Bill Gates', 'Bill Gates'), 1.0) cause this is a self loop so of no use. popping the first value out
    #sortedg.pop(0)
    return sortedg
    pass

def evaluate(predicted_edges, graph):
    
    ###TODO
    c = 0
    for i in predicted_edges:
        if i[1] in graph.neighbors(i[0]):
        #if(graph.has_edge(i[0],i[1])):
            c = c + 1     
    return float(c/len(predicted_edges))
    pass


"""
Next, we'll download a real dataset to see how our algorithm performs.
"""
def download_data():
    """
    Download the data. Done for you.
    """
    urllib.request.urlretrieve('http://cs.iit.edu/~culotta/cs579/a1/edges.txt.gz', 'edges.txt.gz')


def read_graph():
    """ Read 'edges.txt.gz' into a networkx **undirected** graph.
    Done for you.
    Returns:
      A networkx undirected graph.
    """
    return nx.read_edgelist('edges.txt.gz', delimiter='\t')


def main():
    """
    FYI: This takes ~10-15 seconds to run on my laptop.
    """
    download_data()
    graph = read_graph()
    print('graph has %d nodes and %d edges' %
          (graph.order(), graph.number_of_edges()))
    subgraph = get_subgraph(graph, 2)
    print('subgraph has %d nodes and %d edges' %
          (subgraph.order(), subgraph.number_of_edges()))
    print('norm_cut scores by max_depth:')
    print(score_max_depths(subgraph, range(1,5)))
    clusters = partition_girvan_newman(subgraph, 3)
    print('%d clusters' % len(clusters))
    print('first partition: cluster 1 has %d nodes and cluster 2 has %d nodes' %
          (clusters[0].order(), clusters[1].order()))
    print('smaller cluster nodes:')
    print(sorted(clusters, key=lambda x: x.order())[0].nodes())
    test_node = 'Bill Gates'
    train_graph = make_training_graph(subgraph, test_node, 5)
    print('train_graph has %d nodes and %d edges' %
          (train_graph.order(), train_graph.number_of_edges()))


    jaccard_scores = jaccard(train_graph, test_node, 5)
    print('\ntop jaccard scores for Bill Gates:')
    print(jaccard_scores)
    print('jaccard accuracy=%g' %
          evaluate([x[0] for x in jaccard_scores], subgraph))


if __name__ == '__main__':
    main()

