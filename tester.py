import os

for map in ["RANDOM", "CITY", "GAME", "SORTATION", "WAREHOUSE"]:
    steps_num = 5000
    if map == "RANDOM":
        steps_num = 1000

    with open("tmp.config", 'w') as file:
        print(
            f"\n\
# OMAPFS Configuration Example\n\
# Map: RANDOM, CITY, GAME, SORTATION, WAREHOUSE\n\
map_type = {map}\n\
\n\
# Planner algorithms: PIBT, PIBT_TF, EPIBT, EPIBT_LNS, PEPIBT_LNS, WPPL\n\
planner_type = EPIBT\n\
\n\
# Scheduler types: CONST, GREEDY\n\
scheduler_type = CONST\n\
\n\
# TestSystem file paths\n\
map_file = tests/{map.lower()}/map.txt\n\
tasks_file = tests/{map.lower()}/tasks.csv\n\
agents_file = tests/{map.lower()}/\n\
steps_num = {steps_num}\n\
output_dir = kek/{map.lower()}/epd=3", file=file
        )
    os.system("./bin/main tmp.config")
