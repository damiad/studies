**FLAG 1:**

In the `fixture` file:
```json
"fields" : {
     "footer" : "FLAG{ToJestFlagaZeStopki}",
     "user" : 1
},
```
This suggests that one of the flags is hidden in the admin's footer. By sending ourselves a card, we can see the sender's footer. I noticed that `bot.py` runs in an infinite loop, opening cards sent to the admin.

To trigger a message to us, we send a card to the admin using the following `curl` command obtained from inspecting the page:
```bash
curl 'https://web.kazet.cc:42448/create' \
  -H 'Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7' \
  -H 'Accept-Language: en-PL,en;q=0.9,pl-PL;q=0.8,pl;q=0.7,en-GB;q=0.6,en-US;q=0.5' \
  -H 'Cache-Control: max-age=0' \
  -H 'Connection: keep-alive' \
  -H 'Content-Type: application/x-www-form-urlencoded' \
  -H 'Cookie: csrftoken=ZjCekfsH0sXC21OPR74VLzcqYiLmZcik; sessionid=2oqvrfqj6qp00rqd958d51lzamus4iea' \
  -H 'Origin: https://web.kazet.cc:42448' \
  -H 'Referer: https://web.kazet.cc:42448/create' \
  -H 'Sec-Fetch-Dest: document' \
  -H 'Sec-Fetch-Mode: navigate' \
  -H 'Sec-Fetch-Site: same-origin' \
  -H 'Sec-Fetch-User: ?1' \
  -H 'Upgrade-Insecure-Requests: 1' \
  -H 'User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/118.0.0.0 Safari/537.36' \
  -H 'sec-ch-ua: "Chromium";v="118", "Google Chrome";v="118", "Not=A?Brand";v="99"' \
  -H 'sec-ch-ua-mobile: ?0' \
  -H 'sec-ch-ua-platform: "Linux"' \
  --data-raw 'csrfmiddlewaretoken=UaDXNxp4LRENtJMa2hmrZLttyXlmWY6CJj51XCHBB9rflAqPJegcAavJm5WyL0eM&recipient=wulux&content=%3Cp%3E%26lt%3Bdiv%26gt%3Bhh+%26lt%3Bdiv%26gt%3B%3C%2Fp%3E&template=normal' \
  --compressed
```
Modify the content accordingly to trigger the message to us (now without restrictions and formatting). We needed the `csrfmiddlewaretoken` extracted from cookies.

**FLAG 2:**

In the `start.sh` file:
```bash
echo FLAG{JeszczeJednaFlagaZaPunkty} > /flag.txt
```
It indicates that another flag is hidden in the `flag.txt` file. Searching for calls to the `open` function, we find `views.py` invoking a function from `utils.py`, where we see:
```python
def render_card(card):
  recipient = card.recipient
  content = card.content
  footer = card.sender.profile.footer
  with open(
      f"{os.path.dirname(os.path.realpath(__file__))}/card_templates/{card.template}",
      "r",
  ) as f:
      rendered = f.read()
  rendered = rendered.replace("[recipient]", recipient)
  rendered = rendered.replace("[content]", content)
  rendered = rendered.replace("[footer]", footer)

  return rendered
```
Here, `card.template` can be manipulated to provide any path, including `flag.txt`. We use this to retrieve the flag.

**FLAG 3:**

In the `docker-compose.yml` file:
```yaml
zad41-mimuw-finals-2023-super-secret-microservice:
  volumes:
      - ../zad41-mimuw-finals-2023.flag1:/static/index.html
  build: super-secret-microservice/
  restart: always
  networks:
    - network41-ext
```
It seems the flag is in the `index.html` file in the `super-secret-microservice` folder. We need to access it there.

In `urls.py`, we find `GreetingCardDownloadPNGView` used for downloading PNGs:
```python
path(
    "download-png/<str:token>",
    views.GreetingCardDownloadPNGView.as_view(),
    name="download-png",
),
```
In `views.py`, the `GreetingCardDownloadPNGView` is defined to take a screenshot using Chromium:
```python
class GreetingCardDownloadPNGView(LoginRequiredMixin, generic.View):
  model = GreetingCard

  def get(self, request, token):
      file_name = tempfile.mktemp(suffix=".png")
      subprocess.call(
          [
              "chromium",
              "--headless",
              "--disable-gpu",
              "--no-sandbox",
              "--screenshot=" + file_name,
              "http://127.0.0.1:3000/show-for-png/" + token,
          ]
      )
      with open(file_name, "rb") as f:
          return HttpResponse(f.read(), content_type="image/png")
```
We can use an iframe with a URL pointing to `index.html` to capture the flag screenshot.
