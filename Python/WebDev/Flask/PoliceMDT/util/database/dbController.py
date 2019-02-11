import pyodbc

'''
# connection
    server = 'localhost\sqlexpress' 
    database = 'MundusInvicte' 
    username = 'sa' 
    password = 'SO4--sulfate' 
    cnxn = pyodbc.connect('DRIVER={SQL Server};SERVER='+server+';DATABASE='+database+';UID='+username+';PWD='+ password)
'''

cnxn, cursor = 0

def connect(server, database, username, password):
    cnxn = pyodbc.connect('DRIVER={SQL Server};SERVER='+server+';DATABASE='+database+';UID='+username+';PWD='+ password)
    cusor = cnxn.cursor()

def execute(query):
    pass

def close():
    cnxn.close()