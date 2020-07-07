from htmldataparser import HtmlDataParser
import json

if __name__ == "__main__":
    formats = json.load(open(r"C:\Users\Michael Grieco\Dropbox\format.json"))
    data = HtmlDataParser(formats["url"], formats["data"])

    while True:
        code = input("Search term: ")
        if code == "stop":
            break

        res = data.request("search", params={
            "startYr": 2020,
            "endYr": 2021,
        }, query={
            "term": code
        })
        print(res)