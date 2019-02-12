import pyodbc

# connection
server = 'localhost\sqlexpress' 
database = 'MundusInvicte' 
username = 'client' 
password = 'client123' 
cnxn = pyodbc.connect('DRIVER={SQL Server};SERVER='+server+';DATABASE='+database+';UID='+username+';PWD='+ password)

# cursor object
cursor = cnxn.cursor()

# execute query
try:
    #query = "insert into dbo.Users(LastName, FirstName, Username, Email) \
    #values ('%s', '%s', '%s', '%s')" % \
    #('admin', 'admin', 'admin', 'admin')

    #query = "delete from dbo.Users where ID > 0"

    # execute statement
    #cursor.execute(query)

    # commit changes
    cnxn.commit()
except:
    # rollback with error
    cnxn.rollback()

query = "select * from dbo.users"

try:
    cursor.execute(query)

    # get result
    results = cursor.fetchall()
    for row in results:
        id = row[0]
        lname = row[1]
        fname = row[2]
        username = row[3]
        email = row[4]

        # print
        print("{", \
                "\tid=%d" % id, \
                "\tlast name=%s" % lname, \
                "\tfirst name=%s" % fname, \
                "\tusername=%s" % username, \
                "\temail=%s" % email, 
                "}", \
                sep="\n")
except:
    print("could not fetch data")

# close connection
cnxn.close()