# coding: utf-8

# # Assignment 3:  Recommendation systems
#
# Here we'll implement a content-based recommendation algorithm.
# It will use the list of genres for a movie as the content.
# The data come from the MovieLens project: http://grouplens.org/datasets/movielens/
# Note that I have not provided many doctests for this one. I strongly
# recommend that you write your own for each function to ensure your
# implementation is correct.

# Please only use these imports.
from collections import Counter, defaultdict
import math
import numpy as np
import os
import pandas as pd
import re
from scipy.sparse import csr_matrix
import urllib.request
import zipfile

def download_data():
    """ DONE. Download and unzip data.
    """
    url = 'https://www.dropbox.com/s/h9ubx22ftdkyvd5/ml-latest-small.zip?dl=1'
    urllib.request.urlretrieve(url, 'ml-latest-small.zip')
    zfile = zipfile.ZipFile('ml-latest-small.zip')
    zfile.extractall()
    zfile.close()


def tokenize_string(my_string):
    """ DONE. You should use this in your tokenize function.
    """
    return re.findall('[\w\-]+', my_string.lower())

def tokenize(movies):
    t=[]
    for i in movies["genres"]:
        z = tokenize_string(i)
        t.append(z)
    #movies["tokens"] = t
    s = np.array(t)
    #print(s)
    s1 = pd.Series(s,movies.index)
    #print(s1)
    movies["tokens"] = s1
    return movies

def featurize(movies):
    feats = defaultdict(lambda: 0)
    f_list = list(defaultdict(lambda: 0))
    ind = movies.index
    shape = movies.shape[0]
    mat=[]
    mat_list = []
    doc = []
    r = []
    c = []
    d = []
    mo = movies['tokens']
    for i in mo:
        w = 0
        for j in set(i):
            feats[j]=feats[j]+1
            w=w+1
    vocab = dict((i, sorted(feats).index(i)) for i in sorted(feats))
    
    new_dict = {}
    for i in mo:
        new_dict = defaultdict(lambda: 0)
        for k in i:
            new_dict[k] = new_dict[k]+1
        f_list.append(new_dict)
    
    i=0
    while i<shape:
        r=[]
        d=[]
        c=[]
        t=True
        doc = f_list[i]
        max_k = doc[max(doc,key=doc.get)]
        for j in doc:
            if j in vocab:
                c.append(vocab[j])
                w1 = doc[j]/max_k
                w = math.log((shape / feats[j]),10)
                tf_idf = w1 * w
                d.append(tf_idf)
        r = [0]*len(c)
        l1 = len(vocab)
        mat = csr_matrix((d,(r,c)),shape = (1, l1))
        mat_list.append(mat)
        i=i+1
    f =pd.DataFrame(mat_list,ind)
    movies['features'] = f
    return movies,vocab

            
def train_test_split(ratings):
    """DONE.
    Returns a random split of the ratings matrix into a training and testing set.
    """
    test = set(range(len(ratings))[::1000])
    train = sorted(set(range(len(ratings))) - test)
    test = sorted(test)
    return ratings.iloc[train], ratings.iloc[test]


def cosine_sim(a, b):
    A = a.dot(b.T).toarray()[0][0]
    B = (np.linalg.norm(a.toarray())*np.linalg.norm(b.toarray()))
    return float(A/B)


def make_predictions(movies, ratings_train, ratings_test):
    r=[]
    iterr = ratings_test.iterrows()
    for i,j in iterr:
        mid = j['movieId']
        feat = movies.loc[movies.movieId == mid].squeeze()['features']
        wr = 0.
        ss = 0.
        a = True
        uid = j['userId']
        user = ratings_train.loc[ratings_train.userId == uid]
        u_iterr = user.iterrows()
        for k,l in u_iterr:
            co_sine = cosine_sim(feat,movies.loc[movies.movieId==l['movieId']].squeeze()['features'])
            #if co_sine < 0:
            if co_sine > 0:
                ss = ss + co_sine
                wr = wr + co_sine * l['rating']
                a = False
        if a != True:
            r.append(wr/ss)
        elif a == True:
            r.append(user['rating'].mean())
    
    z = np.array(r)
    return z

def mean_absolute_error(predictions, ratings_test):
    """DONE.
    Return the mean absolute error of the predictions.
    """
    return np.abs(predictions - np.array(ratings_test.rating)).mean()

def main():
    download_data()
    path = 'ml-latest-small'
    ratings = pd.read_csv(path + os.path.sep + 'ratings.csv')
    movies = pd.read_csv(path + os.path.sep + 'movies.csv')
    movies = tokenize(movies)
    movies, vocab = featurize(movies)
    print('vocab:')
    print(sorted(vocab.items())[:10])
    ratings_train, ratings_test = train_test_split(ratings)
    print('%d training ratings; %d testing ratings' % (len(ratings_train), len(ratings_test)))
    predictions = make_predictions(movies, ratings_train, ratings_test)
    print('error=%f' % mean_absolute_error(predictions, ratings_test))
    print(predictions[:10])


if __name__ == '__main__':
    main()

