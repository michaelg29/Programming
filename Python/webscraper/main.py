import json
from htmldataparser import HtmlDataParser, RegexSelectors

if __name__ == "__main__":
    formats = json.load(open(r"pipformat.json"))
    data = HtmlDataParser(formats["url"], formats["data"])

    # startYr = 2000
    # while True:
    #     start = input("Input starting year: ")
    #     if len(start) == 4:
    #         startYr = int(start)
    #         break
    #     elif len(start) == 2:
    #         startYr = int("20" + start)
    #         break
    #     else:
    #         print("Invalid input")

    # while True:
    #     code = input("Input course subject and code: ")
    #     if code == "stop":
    #         break
    #     code = re.sub(r"[ ]", '-', code)
    
    #     res = data.request("course", {
    #         "startYr": str(startYr),
    #         "endYr": str(startYr + 1),
    #         "code": code
    #     })
    #     print(res["desc"]["text"])

    while True:
        search = input("Search term: ")
        if search == "stop":
            break

        res = data.request("search", query={
            "term": search
        })
        print(res)