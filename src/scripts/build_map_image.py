import matplotlib.pyplot as plt
import numpy as np
import pandas as pd


def read(filename):
    df = pd.read_csv(filename)
    df = df.drop(columns=df.columns[0])
    print(df)
    return df


def paint_one(data, to_file):
    fig, axes = plt.subplots(1, 1, figsize=(10, 10))
    map = np.array(data)
    ax = axes

    ax.imshow(map)

    if True:
        mask = map == -1
        red_mask = np.zeros((*map.shape, 4))
        red_mask[mask] = [0, 0, 0, 1]
        ax.imshow(red_mask)
    if True:
        mask = map != -1
        red_mask = np.zeros((*map.shape, 4))
        red_mask[mask] = [1, 1, 1, 1]
        ax.imshow(red_mask)

    plt.xticks([])
    plt.yticks([])

    plt.subplots_adjust(left=0, right=1, top=1, bottom=0)
    # for .png: dpi=800
    plt.savefig(to_file, bbox_inches='tight', pad_inches=0)
    plt.show()


if __name__ == '__main__':
    from_file = "../../kek/warehouse/0/heatmap_all.csv"
    to_file_one = "warehouse.pdf"
    to_file_all = ""

    data = read(from_file)
    paint_one(data, to_file_one)
