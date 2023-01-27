#!/usr/bin/env python

# Deep Learning Homework 1

import argparse
import random
import os

import numpy as np
import matplotlib.pyplot as plt

import utils


def configure_seed(seed):
    os.environ["PYTHONHASHSEED"] = str(seed)
    random.seed(seed)
    np.random.seed(seed)


class LinearModel(object):
    def __init__(self, n_classes, n_features, **kwargs):
        self.W = np.zeros((n_classes, n_features))

    def update_weight(self, x_i, y_i, **kwargs):
        raise NotImplementedError

    def train_epoch(self, X, y, **kwargs):
        self.K = 0
        for x_i, y_i in zip(X, y):
            self.update_weight(x_i, y_i, **kwargs)
        print("Mistakes: " + str(self.K))

    def predict(self, X):
        """X (n_examples x n_features)"""
        scores = np.dot(self.W, X.T)  # (n_classes x n_examples)
        predicted_labels = scores.argmax(axis=0)  # (n_examples)
        return predicted_labels

    def evaluate(self, X, y):
        """
        X (n_examples x n_features):
        y (n_examples): gold labels
        """
        y_hat = self.predict(X)
        
        n_correct = (y == y_hat).sum()
        n_possible = y.shape[0]
        return n_correct / n_possible


class Perceptron(LinearModel):
    def update_weight(self, x_i, y_i, **kwargs):
        """
        x_i (n_features): a single training example
        y_i (scalar): the gold label for that example
        other arguments are ignored
        """
        y_ii =  self.predict(x_i)
        if y_ii != y_i:
            self.K +=1
            self.W[y_i, :] += x_i
            self.W[y_ii, :]-= x_i

class LogisticRegression(LinearModel):
    
     
    def update_weight(self, x_i, y_i, learning_rate=0.001):
        """
        x_i (n_features): a single training example
        y_i: the gold label for that example
        learning_rate (float): keep it at the default value for your plots
        """
        # Q3.1b
        #calculate the product between x_i and W
        label_scores = self.W.dot(x_i)[:,None] 
        y_one_hot = np.zeros((np.size(self.W, 0), 1))
        y_one_hot[y_i] = 1
        label_probabilities = np.exp(label_scores) / np.sum(np.exp(label_scores))
        self.W += learning_rate * (y_one_hot - label_probabilities) * x_i[None, :]


def reLU(value):
    return np.maximum(0, value)

def ReLU_prime(value):
    value[value <= 0] = 0
    value[value > 0] = 1
    return value

    

class MLP(object):
    # Q3.2b. This MLP skeleton code allows the MLP to be used in place of the
    # linear models with no changes to the training loop or evaluation code
    # in main().
    def __init__(self, n_classes, n_features, hidden_size, h):
        # Initialize an MLP with a single hidden layer.
        self.num_layers = 2
        self.w1 = np.random.normal(0.1, 0.1, size=(hidden_size, n_features))
        self.w2 =  np.random.normal(0.1, 0.1, size=(n_classes, hidden_size))
        self.b1 = np.zeros(hidden_size)
        self.b2 = np.zeros(n_classes)
        
    def softmax_grad(self, s):
        jacobian_m = np.diag(s)
        for i in range(len(jacobian_m)):
            for j in range(len(jacobian_m)):
                if i == j:
                    jacobian_m[i][j] = s[i] * (1-s[i])
                else: 
                    jacobian_m[i][j] = -s[i]*s[j]
        return jacobian_m
        
    def reLU(self, value):
        return np.maximum(0, value)

    def ReLU_prime(self, value):
        return np.where(value <= 0, 0, 1)
    
    def forward(self, x):
        z1 = self.w1.dot(x) + self.b1
        a1 = self.reLU(z1)

        z2 = self.w2.dot(a1) + self.b2
        a2 = self.reLU(z2)
        return a1, z1, a2, z2
    
    def softmax(self, output):
        output -= output.max()
        probs = np.exp(output) / np.sum(np.exp(output))
        return probs
    
    def compute_loss(self, output, y):
        probs = self.softmax(output)
        loss = np.multiply(-y, np.log(probs))
        return loss
    
    def backward(self, x, y, a1, z1, a2, z2):
        probs = self.softmax_grad(z2)
        grad_z2 = probs - y  # Grad of loss wrt last z.
        print(grad_z2.shape)
        print(a1.shape)
        grad_w2 = grad_z2[:, None].dot(a1[:, None].T)
        grad_b2 = grad_z2

        grad_a1 = self.w2.T.dot(grad_z2)
        
        grad_z1 = grad_a1 * self.ReLU_prime(z1)
        grad_w1 = grad_z1[:, None].dot(x[:, None].T)
        grad_b1 = grad_z1
        return grad_w1, grad_b1, grad_w2, grad_b2
    
    def update_parameters(self, grad_w1, grad_b1, grad_w2, grad_b2, learning_rate=0.001):
        self.w1 -= learning_rate * grad_w1
        self.w2 -= learning_rate * grad_w2
        self.b1 -= learning_rate * grad_b1
        self.b2 -= learning_rate * grad_b2

    def predict_label(self,output):
        y_hat = np.zeros_like(output)# The most probable label is also the label with the largest logit.
        y_hat[np.argmax(output)] = 1
        return y_hat
    
    def predict(self, X):
        predicted_labels = []
        for x in X:
            output= self.forward(x)[3]
            y_hat = self.predict_label(output)
            predicted_labels.append(y_hat)
        predicted_labels = np.array(predicted_labels)
        return predicted_labels

    def evaluate(self, X, y):
        """
        X (n_examples x n_features)
        y (n_examples): gold labels
        """
        # Identical to LinearModel.evaluate()
        n_correct = 0
        y_hat = self.predict(X)
        if np.array_equal(y, y_hat):
            n_correct += 1
        n_possible = y.shape[0]
        return n_correct / n_possible

    def train_epoch(self, X, y, learning_rate=0.001):
        total_loss = 0
        for x_i, y_i in zip(X, y):
            a1, z1, a2, z2 = self.forward(x_i)
            loss = self.compute_loss(z2, y_i)
            total_loss += loss
            grad_w1, grad_b1, grad_w2, grad_b2 = self.backward(x_i, y_i, a1, z1, a2, z2)
            self.update_parameters(grad_w1, grad_b1, grad_w2, grad_b2)
        print(total_loss)

def plot(epochs, valid_accs, test_accs):
    plt.xlabel('Epoch')
    plt.ylabel('Accuracy')
    plt.xticks(epochs)
    plt.plot(epochs, valid_accs, label='validation')
    plt.plot(epochs, test_accs, label='test')
    plt.legend()
    plt.show()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('model',
                        choices=['perceptron', 'logistic_regression', 'mlp'],
                        help="Which model should the script run?")
    parser.add_argument('-epochs', default=20, type=int,
                        help="""Number of epochs to train for. You should not
                        need to change this value for your plots.""")
    parser.add_argument('-hidden_size', type=int, default=200,
                        help="""Number of units in hidden layers (needed only
                        for MLP, not perceptron or logistic regression)""")
    parser.add_argument('-layers', type=int, default=1,
                        help="""Number of hidden layers (needed only for MLP,
                        not perceptron or logistic regression)""")
    parser.add_argument('-learning_rate', type=float, default=0.001,
                        help="""Learning rate for parameter updates (needed for
                        logistic regression and MLP, but not perceptron)""")
    opt = parser.parse_args()

    utils.configure_seed(seed=42)

    add_bias = opt.model != "mlp"
    data = utils.load_classification_data(bias=add_bias)
    train_X, train_y = data["train"]
    dev_X, dev_y = data["dev"]
    test_X, test_y = data["test"]

    n_classes = np.unique(train_y).size  # 10
    n_feats = train_X.shape[1]

    # initialize the model
    if opt.model == 'perceptron':
        model = Perceptron(n_classes, n_feats)
    elif opt.model == 'logistic_regression':
        model = LogisticRegression(n_classes, n_feats)
    else:
        model = MLP(n_classes, n_feats, opt.hidden_size, opt.layers)
    epochs = np.arange(1, opt.epochs)
    valid_accs = []
    test_accs = []
    for i in epochs:
        print('Training epoch {}'.format(i))
        train_order = np.random.permutation(train_X.shape[0])
        train_X = train_X[train_order]
        train_y = train_y[train_order]
        model.train_epoch(
            train_X,
            train_y,
            learning_rate=opt.learning_rate
        )
        valid_accs.append(model.evaluate(dev_X, dev_y))
        test_accs.append(model.evaluate(test_X, test_y))

    # plot
    plot(epochs, valid_accs, test_accs)


if __name__ == '__main__':
    main()