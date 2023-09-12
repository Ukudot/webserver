import cgi
import os

form = cgi.FieldStorage()

first_name = form.getvalue('name')
last_name = form.getvalue('surname')
for key, value in os.environ.items():
    print(f"{key} = {value}")
print(f"first_name: {first_name}, last_name: {last_name}")
