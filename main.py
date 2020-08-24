import subprocess
import praw
import os
import urllib.request
import json
import datetime
import requests

def downloadfile(name,url):
    name = name+".mp4"
    r = requests.get(url)
    f = open(name,'wb')
    for chunk in r.iter_content(chunk_size=255):
        if chunk:  # filter out keep-alive new chunks
            f.write(chunk)
    f.close()


dt = datetime.datetime.now()
utc_timestamp = dt.timestamp()
reddit = praw.Reddit(client_id ='-EZS61L9Sk1F2w',
                     client_secret = 'eOC9bx1DZ2GUgv7f8qrXTXuhO60',
                     username = 'panobot',
                     password = 'haoyang302',
                     user_agent = 'panobot by /u/TheQinDynasty')

subreddit = reddit.subreddit("408DH", "Videos")

keyphrase = '!panobot'
submissionURL = ''

for comment in subreddit.stream.comments():
    utc_timestamp = dt.timestamp()
    if comment.created_utc > utc_timestamp-10:
        print(datetime.datetime.fromtimestamp(comment.created_utc))
        print('\n')
        print(utc_timestamp)
        if keyphrase in comment.body:
            try:
                submissionURL = comment.submission.permalink
                url = ''.join(('https://reddit.com', submissionURL, '.json'))
                with urllib.request.urlopen(url) as url:
                    data = json.loads(url.read().decode())
                    file = data[0]['data']['children'][0]['data']['secure_media']['reddit_video']['fallback_url']

                    urllib.request.urlretrieve(file, 'test.mp4')
                print('converting...')
                subprocess.call(["./ImageStitch", "test.mp4"])
                print('conversion complete, uploading...')
                replymsg = os.popen("imgur-uploader test.jpg").read()
                comment.reply(replymsg)

            except:
                 comment.reply("Conversion failed")


