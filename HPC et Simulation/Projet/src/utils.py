import networkx as nx
import matplotlib.pyplot as plt
from os.path import basename, splitext


def read_graph_from_file(filename):
    """ Read graph from a file.

    Args:
        filename (str): Path to the file containing the graph

    Returns:
        float: Density
        array: M
        array: Adjency Matrix
    """
    # Try to read graph of format X Y
    try:
        graph = nx.read_edgelist(filename, create_using=nx.DiGraph(), nodetype=int)
    except nx.NetworkXError:
        # Try to read graph of format X,Y
        try:
            graph = nx.read_edgelist(filename, delimiter=",", create_using=nx.DiGraph(), nodetype=int)
        except nx.NetworkXError:
            try:
                # Try to read graph of format X    Y
                graph = nx.read_edgelist(filename, delimiter="\t", create_using=nx.DiGraph(), nodetype=int)
            except nx.NetworkXError:
                print("Error, unable to parse graph from file: ", str(filename))
                exit(1)

    # Create adjency Matrix from parsed graph
    adjencyMatrix = nx.adjacency_matrix(graph).todense()

    # Calculate density of the graph
    density = graph.size() / (len(graph.nodes()) * (len(graph.nodes()) - 1) * 0.5)

    return adjencyMatrix, nx.info(graph).__add__("\nDensity: " + str(density))


def plot_results(filename, resNoVaccination, resRandVaccination, resPRVaccination):
    """ Plot experiments results

    Args:
        filename: Name of the file containing the graph
        resNoVaccination: Result of the first scenario
        resRandVaccination: Result of the second scenario
        resPRVaccination: Result of the third scenario

    Returns:

    """
    # Extract basename
    bn = splitext(basename(filename))[0]

    # Plot differents results
    plt.plot(*resNoVaccination, label="Without vaccination")
    plt.plot(*resRandVaccination, label="With random vaccination")
    plt.plot(*resPRVaccination, label="With Pagerank vaccination")

    # Title
    plt.title("Simulation of an epidemic propagation using the pagerank approach - " + bn)

    # Legends
    plt.xlabel("Iterations")
    plt.ylabel("Infected people")
    plt.legend()

    # Save file
    plt.savefig("../results/" + bn + ".png", bbox_inches="tight")
