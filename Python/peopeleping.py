import requests
from bs4 import BeautifulSoup
import time
n = 1
for p in range(1,4):
    list_url = 'http://opinion.people.com.cn/GB/8213/353915/353916/index{0}.html'.format(p)
    list_wbdata = requests.get(list_url).content
    list_soup = BeautifulSoup(list_wbdata, 'html5lib')
    list_link = list_soup.select("td.t11 a")
    for l in list_link:
        page_href = l.get('href')
        page_title = l.get_text()
        #print(page_title,page_href)
        page_url = 'http://opinion.people.com.cn'+ page_href
        page_wbdata = requests.get(page_url).content
        page_soup = BeautifulSoup(page_wbdata,'html5lib')
        content = page_soup.select_one("div.box_con")
        paper_num = page_soup.select_one('#paper_num')
        paper_num = paper_num.get_text().split(' ')[3]
        author = page_soup.select_one('.author')
        author = author.get_text().replace(" ", "")
        page_title=page_title.split('：')[1]
        #print(author)
        with open('{0}{1}-{2}.txt'.format(paper_num,page_title,author), 'a+', encoding='utf-8', newline='') as files:
            files.write(page_title+'\n')
            files.write(author+'\n')
            #files.writelines(paper_num.get_text)
            files.write('　　')
            files.write(content.get_text().strip().replace(" ", "").replace("\t", ""))
            print("写入完成！",n)
        n += 1
        time.sleep(5)