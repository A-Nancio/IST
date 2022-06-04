# -- coding: utf-8 --
"""
Grupo tg14
Student id #93689
Student id #93711
"""
import numpy as np

def _i(pp, pn):
    if pp == 0 or pn == 0:
        return 0
    else:
        return -pp * np.math.log(pp, 2) - pn * np.math.log(pn, 2)

def _remainder(D, Y, attribute):
    P = Y.count(1)
    N = Y.count(0)
    ret = 0
    for i in (0, 1):
        Pi = Ni = 0
        for example_index in range(len(D)):
            if D[example_index][attribute] == i:
                if (Y[example_index] == 1): Pi += 1
                else: Ni += 1
        if Pi + Ni == 0: continue
        ret += ((Pi+Ni) / (P+N)) * _i(Pi/(Pi + Ni), Ni/(Pi + Ni))

    return ret

def createdecisiontree(D, Y, noise=False):
    explored_attr = ()
    if 1 not in Y: return [0, 0, 0]
    elif 0 not in Y: return [0, 1, 1]
    Tree = DTL_recursion(D.tolist(), Y.tolist(), explored_attr, noise)
    return prune_tree(Tree)

def DTL_recursion(D, Y, explored_attr, noise):
    tree = []
    if 1 not in Y: return 0
    elif 0 not in Y: return 1
    else:
        best, GI = choose_attribute(D, Y, explored_attr)   #choose the attribute with the highest information gain
        if GI < 0.05 and noise:
            if Y.count(1) > Y.count(0): return 1
            else: return 0
        tree += [best]  #add id of the attribute
        for v in (0, 1):
            examples = []
            examples_decisions = []
            for exp_index in range(len(D)): #create the sub-sets sub-tree
                if D[exp_index][best] == v:
                    examples += [D[exp_index]]
                    examples_decisions += [Y[exp_index]]
            subtree = DTL_recursion(examples, examples_decisions, explored_attr + (best,), noise)
            tree += [subtree]

    return tree

def choose_attribute(D, Y, explored_attr):
    num_attributes = len(D[0])
    best_attribute = None
    I = _i(Y.count(1) / len(Y), Y.count(0) / len(Y))
    maxGI = None
    for attribute in range(num_attributes):
        GI = I -_remainder(D, Y, attribute)
        if attribute in explored_attr:
            continue
        elif best_attribute is None:
            best_attribute = attribute
            maxGI = GI
        else:
            GI = I - _remainder(D, Y, attribute)
            if GI > maxGI:
                best_attribute = attribute
                maxGI = GI

    return best_attribute, maxGI

def prune_tree(tree):
    if not isinstance(tree[1], list) and not isinstance(tree[2], list):
        return tree

    elif not isinstance(tree[1], list) and isinstance(tree[2], list):
        tree[2] = prune_tree(tree[2])
        return tree

    elif not isinstance(tree[2], list) and isinstance(tree[1], list):
        tree[1] = prune_tree(tree[1])
        return tree
    else:
        if tree[1] == tree[2]:
            tree = tree[1]
            prune_tree(tree)
        if tree[1][1] == tree[2][1]:
            aux = tree[0]
            tree[0] = tree[1][0]
            tree[2][0] = aux
            tree[2][1] = tree[1][2]
            tree[1] = tree[1][1]
        elif tree[1][2] == tree[2][2]:
            aux = tree[0]
            tree[0] = tree[2][0]
            tree[1][0] = aux
            tree[1][2] = tree[2][1]
            tree[2] = tree[2][2]
        else:
            tree[1] = prune_tree(tree[1])
            tree[2] = prune_tree(tree[2])
        return tree