import os

for map in ["BERLIN_1_256",
            "BERLIN_1_256_SMALL",

            "PARIS_1_256",
            "PARIS_1_256_SMALL",

            "RANDOM_256_10",
            "RANDOM_256_10_SMALL",

            "RANDOM_256_20",
            "RANDOM_256_20_SMALL",

            "SORTATION_LARGE",
            "SORTATION_SMALL",

            "WAREHOUSE_LARGE",
            "WAREHOUSE_SMALL"]:
    steps_num = 5000
    if "SMALL" in map:
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
map_file = tests_sillm/{map.lower()}/map.txt\n\
tasks_path = tests_sillm/{map.lower()}/\n\
agents_path = tests_sillm/{map.lower()}/\n\
steps_num = {steps_num}\n\
output_dir = solutions2/{map.lower()}", file=file
        )
    ret = os.system("./bin/main tmp.config")
    assert ret == 0, "invalid return code"
