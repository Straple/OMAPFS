import matplotlib.pyplot as plt
import numpy as np
import sys
import matplotlib.colors
# import seaborn as sns
import pandas as pd
from PIL import Image
from matplotlib.ticker import FixedFormatter
from matplotlib.ticker import MaxNLocator

# plan_algos = ["EPIBT(1)", "EPIBT(2)", "EPIBT(3)"]
# colors = ['lime', 'dodgerblue', 'orange', 'red', 'blueviolet', 'aqua', 'deeppink', 'brown']
# colors = ['green', 'blue', 'orange', 'red', 'blueviolet', 'aqua', 'deeppink', 'brown']
# plan_algos_name = ['EPIBT+LNS+GG', 'EPIBT+LNS', 'EPIBT+GG', 'EPIBT', 'PIBT+GG', 'PIBT', 'WPPL+GG', 'PIBT+traffic flow']
markers = ['o', 'v', 's', 'p', '*', 'x', 'D', 'P']

# color_palette = sns.color_palette("tab10", 8)
# plt.rcParams['axes.prop_cycle'] = plt.cycler(color=color_palette)

data = pd.read_csv('metrics.csv', sep=',')
data['avg step time (ms)'] = data['avg step time (ms)'].astype(int)

maps = set(data.groupby('map type').groups)
print("maps:", maps)

is_first = True

planner_to_marker = dict()
marker_it = 0

def add_map(map_name, map_text, column):
    global marker_it
    global is_first
    if not map_name in maps:
        return
    grouped = data.groupby('map type').get_group(map_name)
    grouped = grouped.groupby('planner type')

    if True:
        if len(maps) == 1:
            ax = axes[0]
        else:
            ax = axes[0][column]
        ax.imshow(np.asarray(Image.open("image/" + map_name.lower() + '.png')))
        ax.title.set_text(map_text)
        ax.set_xticks([])
        ax.set_yticks([])

    for planner_type in grouped.groups:
        df = grouped.get_group(planner_type)

        if not planner_type in planner_to_marker:
            planner_to_marker[planner_type] = markers[marker_it]
            marker_it += 1

        if True:
            if len(maps) == 1:
                ax = axes[1]
            else:
                ax = axes[1][column]
            ax.plot(df['agents num'], df['throughput'], alpha=1, label=planner_type, marker=planner_to_marker[planner_type])
            if is_first:
                ax.set_ylabel('Throughput')
            ax.grid(True)

            ax.yaxis.set_major_locator(MaxNLocator(integer=True))

        if True:
            if len(maps) == 1:
                ax = axes[2]
            else:
                ax = axes[2][column]
            ax.plot(df['agents num'], df['avg step time (ms)'], alpha=1, label=planner_type, marker=planner_to_marker[planner_type])
            ax.set_yscale('log')
            if is_first:
                ax.set_ylabel('Decision Time (ms)')
            ax.grid(True)
            ax.set_xlabel('Number of Agents')
            ax.set_ylim(0.5, 2000)

    if is_first:
        is_first = False


if __name__ == '__main__':
    fig, axes = plt.subplots(3, len(maps), figsize=(14, 10))

    add_map('RANDOM', 'random-32-32-20\nSize: 32x32\n|V|=819', 0)
    add_map('CITY', 'Paris-1-256\nSize: 256x256\n|V|=47240', 1)
    add_map('GAME', 'brc202d\nSize: 481x530\n|V|=43151', 2)
    add_map('SORTATION', 'sortation\nSize: 140x500\n|V|=54320', 3)
    add_map('WAREHOUSE', 'warehouse\nSize: 140x500\n|V|=38586', 4)

    lines_labels = [ax.get_legend_handles_labels() for ax in fig.axes]
    lines, labels = [sum(lol, []) for lol in zip(*lines_labels)]
    # remove not unique lines
    while True:
        kek = labels.copy()
        if len(kek) == 0:
            break
        kek.pop(-1)
        if labels[-1] in kek:
            lines.pop(-1)
            labels.pop(-1)
        else:
            break
    # print(labels)
    fig.legend(lines, labels, loc='lower center', ncol=4)

    plt.savefig("metrics_plot.pdf", format='pdf', dpi=800)
    #plt.show()
