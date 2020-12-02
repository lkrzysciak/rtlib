import json
import matplotlib.pyplot as plt
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--json', type=str, help='json file')
parser.add_argument('--output', type=str, help='name of output file')
args = parser.parse_args()

with open(args.json) as f:
    data = json.load(f)
    dates = [i['container'] for i in data['data']]
    values = [int(i['duration']) for i in data['data']]
    plt.bar(dates, values)
    plt.xticks(rotation=90)
    plt.subplots_adjust(bottom=0.30)
    plt.savefig(args.output)