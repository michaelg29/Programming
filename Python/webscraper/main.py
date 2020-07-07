from htmldataparser import HtmlDataParser
import json

if __name__ == "__main__":
    formats = json.load(open(r"C:\Users\Michael Grieco\Dropbox\format.json"))
    data = HtmlDataParser(formats["url"], formats["data"])

    while True:
        code = input("Search course code: ")
        if code == "stop":
            break

        res = data.request("course", params={
            "startYr": 2020,
            "endYr": 2021,
            "code": code
        })
        print(res)
        print()