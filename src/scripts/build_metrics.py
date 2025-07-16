import pandas as pd
import os
from pathlib import Path

INPUT_DIRS = ["solutions"]
OUTPUT_FILENAME = 'metrics.csv'

table = {
}


def process_dir(path):
    filename = path / "metrics.csv"
    if os.path.exists(filename):
        print("process:", filename)
        df = pd.read_csv(filename)
        df = df.T
        df.columns = df.iloc[0]
        df = df[1:]

        for column_name in df.columns:
            value = df[column_name].iloc[0]
            if not column_name in table:
                table[column_name] = []
            table[column_name].append(value)


def scan_directory(path):
    path = Path(path)
    for item in path.rglob('*'):
        if item.is_dir():
            process_dir(item)


for dir in INPUT_DIRS:
    if len(dir) != 0 and dir[-1] != '/' and dir[-1] != '\\':
        dir += '/'
    scan_directory(dir)

table = pd.DataFrame(data=table)
table['agents num'] = table['agents num'].astype(int)
table['steps num'] = table['steps num'].astype(int)
table['finished tasks'] = table['finished tasks'].astype(int)
table = table.sort_values(by=['map type', 'agents num', 'finished tasks'], ascending=[True, True, True])
table.to_csv(OUTPUT_FILENAME, index=False)
