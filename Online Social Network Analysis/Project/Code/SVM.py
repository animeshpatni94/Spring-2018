
# coding: utf-8

# In[16]:


import os
import glob
import re
import numpy as np
import pandas as pd
import nltk
import matplotlib.pyplot as plt
get_ipython().magic('matplotlib inline')
from sklearn.grid_search import GridSearchCV


# In[18]:


data = pd.read_csv('tagged1200.csv')


# In[19]:


from sklearn.cross_validation import train_test_split
corpus, test_corpus, y, yt =train_test_split(data.iloc[:,3], data.iloc[:,7],test_size=0.25, random_state=101)



# In[20]:


from sklearn.feature_extraction import text
vectorizer=text.CountVectorizer(ngram_range=(1,1)).fit(corpus)
Tfidf=text.TfidfTransformer()
X=Tfidf.fit_transform(vectorizer.transform(corpus))
Xt=Tfidf.transform(vectorizer.transform(test_corpus))


from sklearn.svm import LinearSVC
from sklearn.grid_search import GridSearchCV
param_grid={'C':[0.01, 0.1, 1.0, 10.0, 100.0]}
clf=GridSearchCV(LinearSVC(loss='hinge', random_state=101), param_grid)
clf=LinearSVC(C=1.0, class_weight=None, dual=True, fit_intercept=True,
     intercept_scaling=1, loss='hinge', max_iter=1000, multi_class='crammer_singer',
     penalty='l2', random_state=101, tol=0.0001, verbose=0)
clf=clf.fit(X,y)
print("Best Parameters: %s" % clf)


coef=clf.coef_.ravel()
pos_coef=clf.coef_[2]
top_pos=np.argsort(coef)[-15:]
top_neg=np.argsort(coef)[:15]

top_coef=np.hstack([top_neg,top_pos])

feature_names=np.array(vectorizer.get_feature_names())
print(data[120:130])



# In[21]:


coef


# In[23]:


from sklearn.metrics import classification_report, accuracy_score, precision_score, recall_score, f1_score
clf=LinearSVC(C=1.0, class_weight=None, dual=True, fit_intercept=True,
     intercept_scaling=1, loss='hinge', max_iter=1000, multi_class='ovr',
     penalty='l2', random_state=101, tol=0.0001, verbose=0)
clf=clf.fit(X,y)
solution=clf.predict(Xt)
target_names=['0','1','-1']
print(classification_report(yt,solution,target_names=target_names))
print("Accuracy:{0:.1f}%".format(accuracy_score(yt, solution)*100 ))
print("Precision:{0:.1f}%".format(precision_score(yt, solution, average='macro')*100 ))
print("Recall:{0:.1f}%".format(recall_score(yt, solution,average='macro')*100 ))
print("F1:{0:.1f}%".format(f1_score(yt, solution,average='macro')*100 ))




# In[24]:


##Testing
docs_new = ['congratulations', 'sorry', 'failed', 'cap', 'thanks']
X_new_tfidf = Tfidf.transform(vectorizer.transform(docs_new))

predicted = clf.predict(X_new_tfidf)
for doc, category in zip(docs_new, predicted):
    print('%r => %s' % (doc, category))


# In[25]:


import itertools
from sklearn.metrics import confusion_matrix

def plot_confusion_matrix(cm, classes,
                          normalize=False,
                          title='Confusion matrix',
                          cmap=plt.cm.Blues):
   
    if normalize:
        cm = cm.astype('float') / cm.sum(axis=1)[:, np.newaxis]
        print("Normalized confusion matrix")
    else:
        print("Confusion matrix, without normalization")

    print(cm)

    plt.imshow(cm, interpolation='nearest', cmap=cmap)
    plt.title(title)
    plt.colorbar()
    tick_marks = np.arange(len(classes))
    plt.xticks(tick_marks, classes, rotation=45)
    plt.yticks(tick_marks, classes)

    fmt = '.2f' if normalize else 'd'
    thresh = cm.max() / 2.
    for i, j in itertools.product(range(cm.shape[0]), range(cm.shape[1])):
        plt.text(j, i, format(cm[i, j], fmt),
                 horizontalalignment="center",
                 color="white" if cm[i, j] > thresh else "black")

    plt.tight_layout()
    plt.ylabel('True label')
    plt.xlabel('Predicted label')
    
class_names=[0,1,-1]
cnf_matrix= confusion_matrix(yt,solution)
# Plot normalized confusion matrix
plt.figure()
plot_confusion_matrix(cnf_matrix, classes=class_names, normalize=True,
                       title='Normalized confusion matrix')




# In[26]:


#Accuracy vs C
pgrid=[0.01, 0.1, 1.0, 10.0, 100.0]
accur=[]
for p in pgrid:
    param_grid['C']=[p]
    clfplot=GridSearchCV(LinearSVC(loss='hinge', random_state=101, penalty='l2'), param_grid)
    clfplot=clfplot.fit(X,y)
    solu=clfplot.predict(Xt)
    acc=accuracy_score(yt,solu)
    accur.append(acc*100)

plt.plot(pgrid, accur)
plt.ylabel('Accuracy (%)')
plt.xlabel('C value')


# In[27]:


#Accuracy vs penalty
lo=['hinge','squared_hinge']
param_grid={'C':[0.01, 0.1, 1.0, 10.0, 100.0]}
accur=[]
for l in lo:
    clfplot2=GridSearchCV(LinearSVC(loss=l, random_state=101, penalty='l2'), param_grid)
    clfplot2=clfplot2.fit(X,y)
    solu=clfplot2.predict(Xt)
    acc=accuracy_score(yt,solu)
    accur.append(acc*100)

x=np.array([0,1])
plt.ylabel('Accuracy (%)')
plt.xticks(x,lo)
plt.xlabel('Loss function')
plt.plot(x, accur)


# In[28]:


#Accuracy vs ngram range
from sklearn.svm import LinearSVC
from sklearn.grid_search import GridSearchCV
accur=[]
grams=range(1,5)
for r in grams:
    vectorizer=text.CountVectorizer(ngram_range=(r,r),stop_words='english').fit(corpus)
    Tfidf=text.TfidfTransformer()
    X=Tfidf.fit_transform(vectorizer.transform(corpus))
    Xt=Tfidf.transform(vectorizer.transform(test_corpus))
    
    param_grid={'C':[0.01, 0.1, 1.0, 10.0, 100.0]}
    clf3=GridSearchCV(LinearSVC(loss='hinge', random_state=101), param_grid)
    clf3=clf3.fit(X,y)
    solu=clf3.predict(Xt)
    acc=accuracy_score(yt,solu)
    accur.append(acc*100)

plt.plot(grams, accur)
plt.ylabel('Accuracy (%)')
plt.xlabel('ngram')

