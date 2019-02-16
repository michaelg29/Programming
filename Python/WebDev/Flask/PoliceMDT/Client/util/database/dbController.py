import pyodbc

def connect(server, database, username, password):
    cnxn = pyodbc.connect('DRIVER={SQL Server};SERVER='+server+';DATABASE='+database+';UID='+username+';PWD='+ password)
    return cnxn

def execute(cursor, query):
    cursor.execute(query)
    return cursor.fetchall()

def close(cnxn):
    cnxn.close()