Naive Bayes Classifier
===============
This project came out of a desire to learn C. The usage is as follows

In ```runclassifier.c```, 

```
// create a classifier
struct bclassifier *b = getclassifier();

// train the classifier
train(b, "iris.csv");

// classify some data
int c = classify(b, vector);

// destory classifier
destroyclassifier(b);

```

The training data compiles by a certain format, shown by the Iris flower data set

```
species_group_number, sepal_length, sepal_width, petal_length, petal_width
```

