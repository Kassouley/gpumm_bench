import matplotlib.pyplot as plt
from matplotlib import ticker
import sys
import datetime as dt

def read_data(file): 
    label=[]
    minimum=[]
    median=[]
    stability=[]
    median_per_it=[]
    line = file.readline()
    while True:
        line = file.readline()
        if not line:
                break
        line = line.replace('\n', '')
        line = line.replace(' ', '')
        line = line.split(sep="|")
        label.append(line[0])
        minimum.append(float(line[1]))
        median.append(float(line[2]))
        median_per_it.append(float(line[3]))
        stability.append(float(line[4]))
    return label, minimum, median, stability, median_per_it


def main() :
    file = open(sys.argv[2])
    label, minimum, median, stability, median_per_it = read_data(file)

    fig, ax1 = plt.subplots(1,1)
    ax2 = ax1.twinx()
    ax1.bar(label, median, label="median value")
    ax1.set_ylabel(f'{sys.argv[4]}')
    ax1.plot(0, color='y', label="(med-min)/min")
    ax2.plot(stability, color='y', label="(med-min)/min")
    ax1.legend(loc=1)
    ax2.set_ylabel('Stability')
    ax2.yaxis.set_major_formatter(ticker.PercentFormatter())
    ax1.set_xticks(label)
    ax1.set_xticklabels(label,rotation=30, ha='right')
    ax1.set_title(f"Kernel Performances for a {sys.argv[1]}x{sys.argv[1]} matrix")
    plt.tight_layout()
    plt.savefig(sys.argv[3])

if __name__ == '__main__':
    main()

