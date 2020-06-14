import json
import re
from htmldataparser import HtmlDataParser, RegexSelectors

if __name__ == "__main__":
    formats = json.load(open(r"C:\Users\Michael Grieco\Dropbox\format.json"))

    data = HtmlDataParser(formats["url"], formats["data"])
    res = data.request("search", {
        "startYr": "2020",
        "endYr": "2021"
    }, {
        "term": "term"
    })
    print(res["no_pages"])