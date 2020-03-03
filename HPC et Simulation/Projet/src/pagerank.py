import numpy as np


def create_transition_matrix(A):
    """
    Create a transition matrix from a given adjency matrix

    Args:
        filename (str): Path to the file containing the graph

    Returns:
        P: Transition matrix
    """
    size = A.shape[0]
    P = np.zeros((size, size))
    for i in range(0, size):
        if np.sum(A[i]) != 0:
            P[i] = A[i] / np.sum(A[i])
    return P


def pagerank(A,
             alpha=0.85,
             epsilon=0.00001):
    """
    Computes the pagerank algorithm for a given adjency matrix

    Args:
        A: Adjency Matrix
        alpha: Dumping factor
        epsilon: error

    Returns:
        currVect: stationnary distribution (normalized)
    """
    P = create_transition_matrix(A)
    size = P.shape[0]
    currVect = np.ones(size) / size
    prevVect = np.ones(size)
    G = np.ones((size, size))
    while np.sum(np.abs(currVect - prevVect)) > epsilon:
        prevVect = currVect.copy()
        currVect = alpha * currVect.dot(P) + (1 - alpha) * currVect.dot(G) / size

    return np.argsort(-currVect)
