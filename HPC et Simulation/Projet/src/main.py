import argparse
import sys
import pyfiglet as pf
from utils import read_graph_from_file, plot_results
from simulation import epidemic_propagation


def setup_cli_parser():
    # Create command line parser with args
    cli_parser = argparse.ArgumentParser(
        description="Epidemic modelisation using improved PageRank algorithm."
    )

    cli_parser.add_argument(
        "-f",
        "--filename",
        dest="datasetFile",
        type=str,
        action="store",
        help="Path of the file containing the data graph",
    )

    cli_parser.add_argument(
        "-i",
        "--init-infected",
        dest="initiallyInfected",
        type=float,
        action="store",
        default=0.05,  # Default to 5%
        help="Percentage of initially infected people",
    )

    cli_parser.add_argument(
        "-v",
        "--init-vaccinated",
        dest="initiallyVaccinated",
        type=float,
        action="store",
        default=0.15,  # Default to 15%
        help="Percentage of initially vaccinated people",
    )

    cli_parser.add_argument(
        "-t",
        "--trasmission-rate",
        dest="contaminationRate",
        type=float,
        action="store",
        default=0.2,  # Default to 20%
        help="Probability to contaminate a neighbor",
    )

    cli_parser.add_argument(
        "-c",
        "--cure-rate",
        dest="cureRate",
        type=float,
        action="store",
        default=0.24,  # Default to 24%
        help="Probability to be cured when infected",
    )

    cli_parser.add_argument(
        "-a",
        "--alpha",
        dest="alpha",
        type=float,
        action="store",
        default=0.85,  # Default to 0.85
        help="Dumping factor",
    )

    cli_parser.add_argument(
        "-e",
        "--epsilon",
        dest="epsilon",
        type=float,
        action="store",
        default=0.00001,  # Default to 0.00001
        help="Error factor",
    )

    return cli_parser


if __name__ == "__main__":

    """
    
    Entry point of the program.
    
    """

    # Print beautiful message :D
    print(pf.figlet_format("Epidemic Simu."))

    # Setup command line parser
    cmdLineParser = setup_cli_parser()

    if len(sys.argv) == 1:
        cmdLineParser.print_help(sys.stderr)
        sys.exit(1)
    args = cmdLineParser.parse_args()

    print("========================================================")
    print("Started new epidemic simulation with these parameters : ")
    print(
        "\nX% of initially infected: ", str(args.initiallyInfected),
        "\nY% of initially vaccinated: ", str(args.initiallyVaccinated),
        "\nNeighbour contamination rate: ", str(args.contaminationRate),
        "\nSelf cure rate: ", str(args.cureRate),
        "\nDumping factor: ", str(args.alpha),
        "\nError factor: ", str(args.epsilon),
        "\nDataset file: ", str(args.datasetFile)
    )
    print("========================================================")

    # Read graph from file
    print("\n\nReading graph from file: ", str(args.datasetFile))
    adjencyMatrix, graphInfo = read_graph_from_file(args.datasetFile)
    print("========================================================")
    print("Successfully loaded graph. Important informations : \n", str(graphInfo))
    print("========================================================")

    print("\nExecuting Scenario 1 : Epidemic simulation without vaccination")
    resNoVaccination = epidemic_propagation(
        adjencyMatrix,
        "noVaccination",
        args.initiallyInfected,
        0,
        args.contaminationRate,
        args.cureRate,
        args.alpha,
        args.epsilon)

    print("\nExecuting Scenario 2 : Epidemic simulation with random vaccination")
    resRandVaccination = epidemic_propagation(
        adjencyMatrix,
        "randomVaccination",
        args.initiallyInfected,
        args.initiallyVaccinated,
        args.contaminationRate,
        args.cureRate,
        args.alpha,
        args.epsilon)

    print("\nExecuting Scenario 3 : Epidemic simulation with pagerank vaccination")
    resPRVaccination = epidemic_propagation(
        adjencyMatrix,
        "PRVaccination",
        args.initiallyInfected,
        args.initiallyVaccinated,
        args.contaminationRate,
        args.cureRate,
        args.alpha,
        args.epsilon)

    # Plot the results
    plot_results(args.datasetFile, resNoVaccination, resRandVaccination, resPRVaccination)


