from bs4 import BeautifulSoup
import requests
import json
import enum
import re

class RegexSelectors:
    """
        . matches any character
        * match 0 or more
        ? match as little as possible (find first occurence of ']' after)
    """
    bracketRegex = r"\[.*?]"

    # remove all whitespace chars (excluding space)
    whitespaceRegex = r"[\t\n\r\f\v]"

    # remove instances of >= 2 spaces
    multipleSpaceRegex = r"[ ]{2,}"

    # left/right strip strings
    stripRegex = r"^[ ]+|[ ]+$"

class TokenType(enum.Enum):
    Tag = 0
    Class_ = 1
    Id = 2

def processSelectStr(str, processAttrs = False):
    """
        take in:
        tag[attr1, ...].class1.class2#id

        return format:
        {
            "selectStr": str
            "tag": tag,
            "class": [ class1, ... ],
            "id": id,
            "attrs": [ attr1, ... ]
        }
    """
    
    ret = {
        "tag": "",
        "class": [],
        "id": ""
    }

    obtainMultiple = False
    if str[0] == "*":
        # get multiple of this element
        obtainMultiple = True
        str = str[1:]

    ret["obtainMultiple"] = obtainMultiple

    currentAttr = ""
    currentType = TokenType.Tag
    for c in str:
        if c in ".#":
            # end of attr
            if currentType == TokenType.Tag:
                ret["tag"] = currentAttr
            elif currentType == TokenType.Class_:
                ret["class"].append(currentAttr)
            else:
                ret["id"] = currentAttr

            # reset type
            if c == '.':
                currentType = TokenType.Class_
            else:
                currentType = TokenType.Id

            currentAttr = ""
        else:
            currentAttr += c

    # last type
    # end of attr
    if currentType == TokenType.Tag:
        ret["tag"] = currentAttr
    elif currentType == TokenType.Class_:
        ret["class"].append(currentAttr)
    else:
        ret["id"] = currentAttr

    # get target attributes
    if processAttrs:
        attrsSearch = re.search(RegexSelectors.bracketRegex, ret["tag"])
        if attrsSearch:
            attrs = ret["tag"][attrsSearch.start(0) + 1:attrsSearch.end(0) - 1]
            ret["tag"] = ret["tag"][0:attrsSearch.start(0)] + ret["tag"][attrsSearch.end(0):]
            
            attrs = re.sub("[ ]", '', attrs)
            ret["attrs"] = re.split("[,]", attrs)
            ret["selectStr"] = str[0:attrsSearch.start(0)] + str[attrsSearch.end(0):]
        else:
            ret["attrs"] = False
            ret["selectStr"] = str
    else:
        ret["selectStr"] = str

    return ret

def cleanText(text):
    text = re.sub(RegexSelectors.whitespaceRegex, ' ', text)
    text = re.sub(RegexSelectors.multipleSpaceRegex, ' ', text)
    text = re.sub(RegexSelectors.stripRegex, '', text)
    return text

def processElementData(element, attrs):
    """
        return format:
        {
            "text": text,
            "attr1": attr1,
            ...
        }
    """
    
    ret = {
        "text": cleanText(element.text)
    }

    if attrs:
        for attr in attrs:
            ret[attr] = element.get(attr)

    return ret

class HtmlDataParser:
    def __init__(self, urlFormat, dataFormat):
        self.urlFormat = {}
        self.dataFormat = {}

        self.processUrlFormat(urlFormat)
        self.processDataFormat(dataFormat)

    def processUrlFormat(self, urlFormat):
        if type(urlFormat) is str:
            # is a filepath, so read
            self.urlFormat = json.load(open(urlFormat))
        else:
            self.urlFormat = urlFormat

        # now in json format
        for action in self.urlFormat["actions"]:
            if "query" in self.urlFormat["actions"][action]:
                for query in self.urlFormat["actions"][action]["query"]:
                    # give uniform format to specify values/default
                    if "vals" not in self.urlFormat["actions"][action]["query"][query]:
                        self.urlFormat["actions"][action]["query"][query]["vals"] = False
                    if "default" not in self.urlFormat["actions"][action]["query"][query]:
                        self.urlFormat["actions"][action]["query"][query]["default"] = False

            else:
                # uniform format (have query attribute)
                self.urlFormat["actions"][action]["query"] = False

    def processDataFormat(self, dataFormat):
        if type(dataFormat) is str:
            # is a filepath, so read
            self.dataFormat = json.load(open(dataFormat))
        else:
            self.dataFormat = dataFormat

        # now in json format

        # process each action
        for action in self.dataFormat:
            # process select string passed in
            for key in self.dataFormat[action]:
                self.dataFormat[action][key] = re.sub("[ ]", '', self.dataFormat[action][key])

                elements = []
                selectStrs = re.split("[>]", self.dataFormat[action][key])
                for i in range(len(selectStrs)):
                    # only process attributes for target element (final one - where data comes from)
                    elements.insert(0, processSelectStr(selectStrs[i], i == len(selectStrs) - 1))

                previousElement = False
                for element in elements:
                    if previousElement:
                        element["child"] = previousElement

                    previousElement = element

                self.dataFormat[action][key] = previousElement

    def constructUrl(self, action, params, query = False):
        if action not in self.urlFormat["actions"]:
            return False

        """
            construct url
            1) get domain
            2) fill in parameters
            3) add query vals
        """

        # domain
        url = self.urlFormat["domain"]
        
        # parameters
        route = self.urlFormat["actions"][action]["route"]

        for param in self.urlFormat["actions"][action]["params"]:
            paramString = "{" + param + "}"
            route = re.sub(paramString, params[param], route)

        url += route

        # query
        queryString = ""

        if self.urlFormat["actions"][action]["query"]:
            # looking for query
            for queryParam in self.urlFormat["actions"][action]["query"]:
                valid = False
                title = self.urlFormat["actions"][action]["query"][queryParam]["title"]
                if query and queryParam in query:
                    if self.urlFormat["actions"][action]["query"][queryParam]["vals"]:
                        if query[queryParam] in self.urlFormat["actions"][action]["query"][queryParam]["vals"]:
                            # ensure valid value
                            valid = True
                    else:
                        # no specified value
                        valid = True
                else:
                    if not self.urlFormat["actions"][action]["query"][queryParam]["default"]:
                        continue

                if valid:
                    queryString += title + "=" + str(query[queryParam])
                else:
                    # use default value
                    queryString += title + "=" + str(self.urlFormat["actions"][action]["query"][queryParam]["default"])

                queryString += '&'

            if queryString != "":
                url += "?" + queryString[:len(queryString) - 1] # remove last ampersand

        return url

    def processData(self, action, data):
        soup = BeautifulSoup(data, "html.parser")

        """
            return format:
            action: {
                "key1": {
                    "text": text,
                    "attr1": attr1
                },
                "key2_obtainMultiple": [
                    {
                        "text": text,
                        "attr1": attr1
                    }
                ]
            }
        """

        ret = {}

        for key in self.dataFormat[action]:
            """
                for each key
                - traverse down "tree" (using children in format)
                - select element(s) using selectStr
                - determine if want attrs
            """

            select = self.dataFormat[action][key]
            elements = [ soup ]
            attrs = False
            while True:
                newElements = []
                for el in elements:
                    if el:
                        if select["obtainMultiple"]:
                            # get all instances
                            newElements.extend(el.select(select["selectStr"]))
                        else:
                            # get first instance
                            res = el.select(select["selectStr"])
                            if res:
                                newElements.append(res[0])

                # use the elements in next iteration
                elements = newElements

                # determine if need attributes
                if "attrs" in select:
                    attrs = select["attrs"]

                # if child exists, go down tree, else break
                if "child" in select:
                    select = select["child"]
                else:
                    break

            # take data from elements
            if len(elements) == 1:
                # only one element
                ret[key] = processElementData(elements[0], attrs)
            elif len(elements) > 1:
                # return list
                ret[key] = []
                for el in elements:
                    ret[key].append(processElementData(el, attrs))

        return ret

    def request(self, action, params, query = False):
        url = self.constructUrl(action, params, query)
        if url:
            data = requests.get(url)
            return self.processData(action, data.content)
        else:
            return False