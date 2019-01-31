import json

data = json.load(open('test.json'))

# read people
for person in data['people']:
    print('*' * 15)
    for key in person:
        print('%s: %s' % (key, person[key]))

#data['people'] = [{'name':'Michael','money':502222},{'name':'Ben','money':222}]

with open('test.json', 'w') as f:
    json.dump(data, f, ensure_ascii=False, indent=4, separators=(", ", " : "))