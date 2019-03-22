import networkx as nx

def jaccard_wt(graph, node):
    new  = set(graph.neighbors(node))
    jac = []
    G = graph.nodes()
    for i in G:
        if i not in new:
            if i != node:
                new1 = set(graph.neighbors(i))
                z = sum(1/graph.degree(n) for n in ( new & new1 ))
                num = float(z)
                z1 = float(sum(graph.degree(j) for j in new))
                dem1 = float(1/z1)
                z2 = float(sum(graph.degree(k) for k in new1))
                dem2 = float(1/z2)
                dem = dem1+dem2
                final = num/dem
                jac.append(((node,i),final))
    k = lambda x:(-x[1],x[0])
    #jac = sorted(jac,key = k)
    return jac
    pass