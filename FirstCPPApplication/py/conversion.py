import csv
import json
import os
from pprint import pprint as pp
import xlrd, xlwt


"""
The goal is to have a CSV and a JSON file that are each the same values, just a different way
of showing them. I'll need a way to convert the data from json to CSV, and have it
so that the csv never has its columns out of order.
"""

MONSTERS_JSON_PATH = os.path.join("./", "monsters.json")
MONSTERS_CSV_PATH = os.path.join("./", "monsters.csv")

MONSTERS_COLUMNS = ["name", "xp_value", "champ_chance", "pack_size",
           "preferred_pack_size", "age"]

def monsters_validation(json_dict):
    json_dict.setdefault("errors", { "monsters" : {}, })
    monster_errors = json_dict['errors']['monsters']
    for monster_id in json_dict.get("monsters"):
        missing_cols = [ (col, u'WARN: Column missing') for col in MONSTERS_COLUMNS\
                        if col not in json_dict['monsters'][monster_id].keys() ]
        if missing_cols:
            if monster_id not in monster_errors:
                monster_errors[monster_id] = {}
            monster_errors[monster_id].update(dict(missing_cols))

    return json_dict

def from_json(path, validation=None):
    with open(path) as f:
        json_dict = json.loads(f.read())

    if validation:
        json_dict = validation(json_dict)

    return json_dict

def from_csv(path, validation=None):
    csv_file = csv.reader(open(path))
    monsters = {}
    csv_file.next() #discard first row
    for i, line in enumerate(csv_file):
        print line[1]
        monster_id = line[0]
        monsters.setdefault(monster_id, {})
        for col_i, col in enumerate(line[1:]):
            matched_col = MONSTERS_COLUMNS[col_i]
            monsters[monster_id][matched_col] = col
    return monsters


if __name__ == "__main__":
    # monsters_dict = from_json(MONSTERS_JSON_PATH, monsters_validation)
    monsters_dict = from_csv(MONSTERS_CSV_PATH, monsters_validation)
    pp(monsters_dict)

