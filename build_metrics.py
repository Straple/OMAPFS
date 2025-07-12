import pandas as pd
import os

INPUT_DIR = "../answers/"

table = {
}

for map in ["random", "city", "game", "sortation", "warehouse"]:
    for ed in range(4):
        for test in range(10):
            filename = INPUT_DIR + map + "/ed=" + str(ed) + "/" + str(test) + "/metrics.csv"
            if os.path.exists(filename):
                df = pd.read_csv(filename)
                df = df.T
                df.columns = df.iloc[0]
                df = df[1:]

                for column_name in df.columns:
                    value = df[column_name][0]
                    if not column_name in table:
                        table[column_name] = []
                    table[column_name].append(value)

table = pd.DataFrame(data=table)
table.to_csv('metrics.csv', index=False)
