import os
from win32com import client


def doc2docx(path):
    w = client.Dispatch('Word.Application')
    w.Visible = 0
    w.DisplayAlerts = 0
    doc = w.Documents.Open(path)
    newpath = os.path.splitext(path)[0] + '.docx'
    doc.SaveAs(newpath, 12, False, "", True, "", False, False, False, False)
    doc.Close()
    w.Quit()
    os.remove(path)
    return newpath


Docu_Data = []


def bianLi(rootDir):
    for root, dirs, files in os.walk(rootDir):
        for file in files:
            if file[-4:] == '.doc':
                doc2docx(os.path.join(root, file))
            elif file[-4:] == '.wps':
                os.renames(os.path.join(root, file),
                           os.path.join(root, file + ".doc"))
                doc2docx(os.path.join(root, file + ".doc"))
            for dir in dirs:
                bianLi(dir)


rootDir = "C:\\Users\\luyimeng\\Desktop\\查重材料"
bianLi(rootDir)
print("OK")
