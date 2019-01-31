import xml.etree.cElementTree as ET

def indent(elem, level=0):
    '''
    copy and paste from http://effbot.org/zone/element-lib.htm#prettyprint
    it basically walks your tree and adds spaces and newlines so the tree is
    printed in a nice way
    '''
    i = "\n" + level*"  "
    if len(elem):
        if not elem.text or not elem.text.strip():
            elem.text = i + "  "
        if not elem.tail or not elem.tail.strip():
            elem.tail = i
        for elem in elem:
            indent(elem, level+1)
        if not elem.tail or not elem.tail.strip():
            elem.tail = i
    else:
        if level and (not elem.tail or not elem.tail.strip()):
            elem.tail = i

tree = ET.ElementTree(file='test.xml')
doc = tree.getroot()

print("%s, %s" % (doc.tag, doc.attrib))

for movie in doc:
    print('*' * 5, movie.attrib['title'], '*' * 5)

    for att in movie:
        print("%s: %s" % (att.tag, att.text))
'''
movie_name = 'The Lego Movie'
movie = {"type":"funny", \
    "format":"DVD", \
    "year":"2003", \
    "rating":"PG", \
    "stars":"10", \
    "description":"lego"}

movie_element = ET.SubElement(doc, "movie", title=movie_name, id=1)
for key, value in movie.items():
    att_element = ET.SubElement(movie_element, key).text = value
'''
for movie in doc:
    if int(movie.attrib['id']) > 0:
        doc.remove(movie)

indent(doc)

tree.write('test.xml', xml_declaration=True, encoding='utf-8', method="xml")