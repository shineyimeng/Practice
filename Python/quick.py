import os
import jieba
import docx2txt
import xlwt
import xlrd
from xlutils.copy import copy
from gensim import corpora, models, similarities

Docu_Data = []
Doc_list_spek = []


def bianLi(rootDir):
    index_t = 0
    for root, dirs, files in os.walk(rootDir):
        for file in files:
            text = docx2txt.process(os.path.join(root, file))
            Docu_Data.append(text)
            Doc_list_spek.append(os.path.join(root, file))
            index_t = index_t + 1
            for dir in dirs:
                bianLi(dir)
    return Docu_Data


rootDir = "C:\\Users\\luyimeng\\Desktop\\查重材料\\2018年组织生活会"
bianLi(rootDir)
f = xlrd.open_workbook("a.xls")
wb = copy(f)
sheet1 = wb.get_sheet(1)
h = len(Doc_list_spek)
for i in range(h):
    sheet1.write(i, 1, str(Doc_list_spek[i]))
wb.save('a.xls')
all_doc = Docu_Data
Sim_Data = []

all_doc_list = []
for doc in all_doc:

    doc_list = [word for word in jieba.cut(doc)]
    all_doc_list.append(doc_list)


def bianLiTwo(rootdir):
    index_te = 0
    for root, dirs, files in os.walk(rootDir):
        for file in files:
            doc_test = docx2txt.process(os.path.join(root, file))
            doc_test_list = [word for word in jieba.cut(doc_test)]
            dictionary = corpora.Dictionary(all_doc_list)
            dictionary.keys()
            dictionary.token2id
            corpus = [dictionary.doc2bow(doc) for doc in all_doc_list]
            doc_test_vec = dictionary.doc2bow(doc_test_list)
            tfidf = models.TfidfModel(corpus)
            tfidf[doc_test_vec]
            index = similarities.SparseMatrixSimilarity(
                tfidf[corpus], num_features=len(dictionary.keys()))
            sim = index[tfidf[doc_test_vec]]
            f = xlrd.open_workbook("a.xls")
            wb = copy(f)

            sheet1 = wb.get_sheet(0)
            for i in range(index_te+1):
                sheet1.write(i, index_te, str(sim[i]))
            wb.save('a.xls')
            index_te = index_te + 1
            for dir in dirs:
                bianLiTwo(dir)


bianLiTwo(rootDir)
print("完成")
