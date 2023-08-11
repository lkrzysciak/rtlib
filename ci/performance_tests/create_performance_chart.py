import json
import matplotlib.pyplot as plt
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--json', type=str, help='json file')
parser.add_argument('--output', type=str, help='name of output file')
args = parser.parse_args()

with open(args.json) as f:
    data = json.load(f)
    containers = {}
    for el in data['data']:
        if el['container'] in containers:
            containers[el['container']]['x'].append(int(el['x']))
            containers[el['container']]['y'].append(int(el['y']))
        else:
            containers[el['container']] = dict()
            containers[el['container']]['x'] = [int(el['x'])]
            containers[el['container']]['y'] = [int(el['y'])]

    for k, v in containers.items():
        plt.plot(v['x'], v['y'], label=k)
    plt.xlabel("Samples")
    plt.ylabel("Time")
    plt.legend()
    plt.savefig(args.output)