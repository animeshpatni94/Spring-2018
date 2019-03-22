## Overview

To perform predictive analysis of future tweet traffic for a specified set of users based on sentiment analysis of that user's historical tweets.



This problem requires metrics and categorization of historical tweets to be determined based on sentiment analysis to allow for a baseline comparison of tweets before the predictive analysis is performed.



The uniqueness of this project lies in the ability to perform predictive analysis by examining classification of a user's tweets.

## Data

We plan on using TwitterAPI to gather data from a user's timeline. This API will provide us with the most recent 3,200 tweets from a specific user. We anticipate that we will run into problems when accommodating users of different follower sizes. Additionally, we must be careful that the machine training is not biased by the data that we gather. Sentiment analysis detecting negation and sarcasm may pose additional problems when determining the metrics of a tweet.

## Method

We plan on using sentiment analysis, similarity computations, part of speech from natural language processing to help with detection of negation, and other python libraries such as networkx, numpy, and TwitterAPI to facilitate gathering and computation of data. As of now, we do not plan on modifying any of the libraries that are mentioned however we might modify a library if a specific use case presents itself.

## Related Work

http://ieeexplore.ieee.org/abstract/document/5590452/

http://www.tandfonline.com/doi/abs/10.2753/MIS0742-1222290408

https://dl.acm.org/citation.cfm?id=2457094

http://onlinelibrary.wiley.com/doi/10.1002/asi.22768/full

http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.402.2031&rep=rep1&type=pdf

http://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=6423033

## Evaluation

We plan to evaluate our results based on the accuracy and efficiency of our sentiment analysis. We can do this by testing our predictive analysis on historical tweets and measuring our estimated value versus the actual number of re-tweets. We will manually tag a specific number of tweets to ensure that a baseline evaluation is accurate. Once this baseline is gathered, we can perform tuning on the parameters to further increase accuracy and precision. We can ensure further accuracy by ensuring our data set is large enough to eliminate bias. We will produce a confusion matrix, as well as a table of accuracy, precision, recall, F1 score, and n grams for our key plots. We can measure our performance based on the fields mentioned above. Additionally, we are planning to normalize  the number of re-tweets based on each user's profile metrics (ie: number of followers).

