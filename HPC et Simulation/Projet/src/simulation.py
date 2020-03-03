import numpy as np
from pagerank import pagerank


def epidemic_propagation(
        adjencyMatrix,
        scenario="noVaccination",
        initContamination=0.05,
        initVaccination=0.12,
        contaminationRate=0.2,
        cureRate=0.26,
        alpha=0.85,
        epsilon=0.00001):
    """
    Computes the propagation of an epidemy

    Args:
        adjencyMatrix: The adjency matrix of the graph
        scenario: The scenario to execute (noVaccination, randomVaccination, specificVaccination)
        initContamination: X% of the initially infected people
        initVaccination: Y% of the initially vaccinated people
        contaminationRate: % of chance to contamine a neighbour
        cureRate: Self cure rate
        alpha: Dumping factor

    Returns:

    """
    size = adjencyMatrix.shape[0]

    # Vaccinate people
    if scenario == "PRVaccination":  # PageRank vaccination (Most important nodes are vaccinate)
        vaccinated_nodes = pagerank(adjencyMatrix, alpha, epsilon)[: int(size * initVaccination)]
    elif scenario == "randomVaccination":  # Random vaccination
        vaccinated_nodes = np.random.choice(size, int(size * initVaccination), replace=False)
    else:  # No vaccination
        vaccinated_nodes = []

    adjencyMatrix = np.delete(adjencyMatrix, vaccinated_nodes, 0)
    adjencyMatrix = np.delete(adjencyMatrix, vaccinated_nodes, 1)

    # Select randomly nodes that are initially infected
    size = adjencyMatrix.shape[0]
    contam_vector = np.random.choice(size, int(size * initContamination))

    # Infection ratio is the parameter given
    res = [contaminationRate]

    # Iterations
    for i in range(200):
        # Get the neighbours of infected nodes
        neighbours = adjencyMatrix[contam_vector, :].nonzero()[1]
        # Infect neighbour (contaminationRate probability)
        contam_vector = np.unique(
            np.concatenate(
                (
                    contam_vector,
                    np.extract(np.random.rand(neighbours.shape[0]) < contaminationRate, neighbours),
                )
            )
        )

        # Infect non-neighbour (1-alpha probability)
        non_neighbours = np.array(list(set(np.arange(0, size)) - set(contam_vector) - set(neighbours)))
        if True and len(non_neighbours) > 0:
            contam_vector = np.unique(
                 np.concatenate(
                    (
                        contam_vector,
                        np.extract(
                            np.random.rand(non_neighbours.shape[0]) < (1 - alpha) / non_neighbours.shape[0], non_neighbours
                        ),
                    )
                )
            )

        # Cure
        contam_vector = np.delete(
            contam_vector, np.where(np.random.rand(contam_vector.shape[0]) < cureRate)[0]
        )

        # Get the number of infected individuals
        res.append(float(len(contam_vector)))

    print("Results : ", list(enumerate(res)))
    return zip(*list(enumerate(res)))
