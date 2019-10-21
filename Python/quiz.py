import docx2txt
import copy

text=docx2txt.process("党员应知应会题库.docx")
text1 = text.split('"')
text=copy.deepcopy(text1)
print(text1)
for index in text1:
    if 'A' in index:
        pass
    else:
        text.remove(index)
        pass
print(text)
