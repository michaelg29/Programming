import pyodbc

'''
# connection
    server = 'localhost\sqlexpress' 
    database = 'MundusInvicte' 
    username = 'sa' 
    password = 'SO4--sulfate' 
    cnxn = pyodbc.connect('DRIVER={SQL Server};SERVER='+server+';DATABASE='+database+';UID='+username+';PWD='+ password)
'''

def connect(server, database, username, password):
    cnxn = pyodbc.connect('DRIVER={SQL Server};SERVER='+server+';DATABASE='+database+';UID='+username+';PWD='+ password)
    return cnxn

def execute(cursor, query):
    cursor.execute(query)
    return cursor.fetchall()

def close(cnxn):
    cnxn.close()