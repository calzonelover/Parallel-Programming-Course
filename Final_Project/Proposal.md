# Sentiment analysis with deep learning

## Objective
To predict the action (like or unlike) from text of comments in movie reviews

## Datasets
IMDB datasets[0] which contains comments and like or unlike action from the same comments

## Methods
There is various approach to deal with this problem. One of the most sexiest way is to use deep learning to feeding training datasets to train model. There are also bunch of well describe model exists in the world, but in this work I would use wordembedding combinging with vanilla RNN, LSTM, Bidirectional-RNN and Attention mechanism which is one of the state of art model that widely use.

## Plan
1. Investigate and preview datasets to getting familiar
2. Map all of the word uniquely one by one from string to number by clean out all of the space and dot for ending sentences
3. Test word embedding technique by constructing space of unique normialized vector which would be use for mapping number to vector. Note that all of those vector has been initialized as truncated normal with gaussian and 2 sigma cutoff width
4. Build vanilla RNN and train with training datasets and test the model with validation sets
5. Create the rest of those model that I mentioned ealier and relatively compare the accuracy of each model
6. (Optional) If I have time, It's interesting to dig down into attention weights to see how contribute of each word in a given prediction

## Tools
* Pandas, Tensorflow

## Reference
* [0] https://www.kaggle.com/oumaimahourrane/imdb-reviews
* [1] Vinyals, Oriol, et al. Show and tell: A neural image caption generator. arXiv:1411.4555 (2014).